#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>

#define PREAMBLE_LS 1000
#define PREAMBLE_RM 2000
#define PREAMBLE_DL 3000

#define BUFSIZE 65536

using namespace std;

enum e_msg_class
{
    e_msg_class_request,
    e_msg_class_response
};
enum e_msg_type
{
    e_msg_type_ls = 0,
    e_msg_type_rm,
    e_msg_type_get
};
struct message_header
{
    int preamble; //=0xAA55AA55
    e_msg_class msg_class;
    enum e_msg_type msg_type;
    long int timestamp; // epoch time in milisecond
    int length_of_data;
};

vector<string> split(char *_string)
{
    vector<string> res;
    string s = "";
    char *pchar = _string;
    int limit = strlen(_string) - 1;
    while (pchar != nullptr && limit > 0)
    {
        if (*pchar != ' ')
        {
            s += *pchar;
        }
        else
        {
            res.push_back(s);
            s = "";
        }
        pchar++;
        limit--;
    }
    if (s.size() != 0)
    {
        res.push_back(s);
    }

    return res;
}
// void check()
// {
//     printf("\nSize of header          = %ld bytes", sizeof(struct message_header));
//     printf("\nSize of preamble        = %ld", sizeof(msg.preamble));
//     printf("\nSize of msg_class       = %ld", sizeof(msg.msg_class));
//     printf("\nSize of msg_type        = %ld", sizeof(msg.msg_type));
//     printf("\nSize of timestamp       = %ld", sizeof(msg.timestamp));
//     printf("\nSize of length_of_data  = %ld", sizeof(msg.length_of_data));

//     printf("\n\nValue of preamble       	= %d", msg.preamble);
//     printf("\nValue of msg_class        = %d", msg.msg_class);
//     printf("\nValue of msg_type         = %d", msg.msg_type);
//     printf("\nValue of timestamp        = %ld", msg.timestamp);
//     printf("\nValue of length_of_data   = %d", msg.length_of_data);

//     printf("\n\nAddress of preamble      	= %p", &msg.preamble);
//     printf("\nAddress of msg_class       = %p", &msg.msg_class);
//     printf("\nAddress of msg_type        = %p", &msg.msg_type);
//     printf("\nAddress of timestamp       = %p", &msg.timestamp);
//     printf("\nAddress of length_of_data  = %p", &msg.length_of_data);
// }

void DownloadFile(int sockfd)
{

    char *fileName;

    char *localPath;

    if (sockfd < 0)
    {
        return;
    }
    while (1)
    {
        if (fileName[0] == '.')
        {
            send(sockfd, fileName, sizeof(fileName), 0);
            break;
        }
        if (localPath[0] == '.')
        {
            send(sockfd, localPath, sizeof(localPath), 0);
            break;
        }
        char GotFileSize[1024];
        recv(sockfd, GotFileSize, 1024, 0);

        // taking size of File in integer
        long FileSize = atoi(GotFileSize);
        long SizeCheck = 0;

        // open local file that we will store;
        FILE *fp = fopen(localPath, "w");
        char *mfcc;
        if (FileSize > 1499)
        {
            // maximun is 1500 bytes
            mfcc = (char *)malloc(1500);
            while (SizeCheck < FileSize)
            {
                int Received = recv(sockfd, mfcc, 1500, 0);
                SizeCheck += Received;
                fwrite(mfcc, 1, Received, fp);
                fflush(fp);
                printf("Filesize: %ld\nSizecheck: %ld\nReceived: %d\n\n", FileSize, SizeCheck, Received);
            }
        }
        else
        {
            mfcc = (char *)malloc(FileSize + 1);
            int Received = recv(sockfd, mfcc, FileSize, 0);
            fwrite(mfcc, 1, Received, fp);
            fflush(fp);
        }
        fclose(fp);
        sleep(500);
        free(mfcc);
    }
}

string getString(char *const &_String, int size)
{
    string s = "";
    for (int idex = 0; idex < size; idex++)
    {
        s += _String[idex];
    }
    return s;
}

int main(int argc, char *argv[])
{
    // **** SETUP VARIABLES FOR CLIENT SIDE
    int sockfd, portno, readVal;
    int portNum;
    struct sockaddr_in serv_addr;

    //  client receives argurments form consoles
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        return -1;
    }
    portno = atoi(argv[2]); //Chuyen cong dich vu thanh so nguyen

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "ERROR opening socket");
        return -1;
    }

    // Setup address of server for connecting
    serv_addr.sin_family = AF_INET;     //Mac dinh
    serv_addr.sin_port = htons(portno); //Cong dich vu
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Call connect for connecting to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return 1;
    }

    while (1)
    {
        // COMMON MESSAGE
        message_header msg;
        char msgToServer[1024];
        char *data_to_send;
        FILE *fp;

        cout << "\nMessage/Command: ";
        fgets(msgToServer, 1024, stdin);

        // **** HANDLING INPUT FROM CONSOLE ****

        vector<string> splitString = split(msgToServer);

        if (splitString.size() != 0)
        {
            // **** HANDLING WITH LIST FUNCTIONALITY
            if (splitString[0] == "ls")
            {
                // ****** CHECKING FOR FILEPATH THAT WE WANT TO LIST *******
                // * If we enter: ls  => list of files at the same directory
                // * otherwise: ls <path> => List of files at <path>-required director

                string filePath;
                filePath = splitString.size() == 1 ? "./" : splitString[1];

                // ***** SETUP FOR HEADER *****
                msg.preamble = PREAMBLE_LS;
                msg.length_of_data = e_msg_class_request;
                msg.msg_type = e_msg_type_ls;
                msg.timestamp = 123456789;
                msg.length_of_data = filePath.length();

                // ***** CHECKING FOR WHAT WE ALREADY ENTER *****
                cout << "Message to send:\n";
                cout << "\tMessage: " << filePath << endl;
                cout << "\tLen: " << filePath.length() << endl;

                // ***** SET-UP MEMORY FOR SENDING ******
                data_to_send = (char *)malloc(sizeof(struct message_header) + msg.length_of_data);
                int idex = 0;
                memcpy(data_to_send + idex, &msg.preamble, sizeof(msg.preamble));
                idex += sizeof(msg.preamble);
                memcpy(data_to_send + idex, &msg.msg_class, sizeof(msg.msg_class));
                idex += sizeof(msg.msg_class);
                memcpy(data_to_send + idex, &msg.msg_type, sizeof(msg.msg_type));
                idex += sizeof(msg.msg_type);
                memcpy(data_to_send + idex, &msg.timestamp, sizeof(msg.timestamp));
                idex += sizeof(msg.timestamp);
                memcpy(data_to_send + idex, &msg.length_of_data, sizeof(msg.length_of_data));
                memcpy(data_to_send + sizeof(struct message_header), (char *)filePath.c_str(), msg.length_of_data);

                // ****** ALL OF BYTES THAT WE HAVE TO SEND ****
                printf("\nTotal data to send: %ld bytes", sizeof(struct message_header) + msg.length_of_data);

                // ***** SENDING DATA TO SERVER *****
                int statusSend = send(sockfd, data_to_send, sizeof(struct message_header) + msg.length_of_data, 0);
                if (statusSend < 0)
                {
                    perror("Error: Sending failed");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    //cout << "Already send!" << endl;
                    printf("\nClient send %d bytes\n", statusSend);
                }

                // ************ RECEIVING RESPONSE FROM SERVER **************

                // **** SET-UP VARIABLES *****
                message_header recmsg;
                char *recev_data = (char *)malloc(1024);
                char messgFromServer[1024];

                // **** READING RESPONSE FROM SERVER
                readVal = recv(sockfd, recev_data, 1024, 0);
                if (readVal < 0)
                {
                    cout << "\nReceived failed!\n";
                    exit(EXIT_FAILURE);
                }
                else
                {
                    printf("\nClient received %d bytes\n", readVal);
                }

                // **** FULFILL MEMORY SPACES
                int index = 0;
                memcpy(&recmsg.preamble, recev_data + index, sizeof(recmsg.preamble));
                index += sizeof(recmsg.preamble);
                memcpy(&recmsg.msg_class, recev_data + index, sizeof(recmsg.msg_class));
                index += sizeof(recmsg.msg_class);
                memcpy(&recmsg.msg_type, recev_data + index, sizeof(recmsg.msg_type));
                index += sizeof(recmsg.msg_type);
                memcpy(&recmsg.timestamp, recev_data + index, sizeof(recmsg.timestamp));
                index += sizeof(recmsg.timestamp);
                memcpy(&recmsg.length_of_data, recev_data + index, sizeof(recmsg.length_of_data));
                memcpy(messgFromServer, recev_data + sizeof(message_header), recmsg.length_of_data);

                // **** CONVERT MESSAGE TO STRING
                string msgOfClient = getString(messgFromServer, recmsg.length_of_data);

                // **** DISPLAY DATA THAT WE RECEIVED
                printf("Message from Server:\n");
                printf("\t  Preamble:       = %d    \n", recmsg.preamble);
                printf("\t  Class:          = %d    \n", recmsg.msg_class);
                printf("\t  Type:           = %d    \n", recmsg.msg_type);
                printf("\t  timestamps:     = %ld    \n", recmsg.timestamp);
                printf("\t  Len:            = %d    \n", recmsg.length_of_data);
                cout << "Message of client: " << msgOfClient << endl
                     << endl;

                // ****** DEALLOCATED ALL MEMORY WHICH ARE ALLOCATED *****
                free(recev_data);
                recev_data = nullptr;
                free(data_to_send);
                data_to_send = nullptr;
            }

            // **** HANDLING WITH REMOVE FUNCTIONALITY
            if (splitString[0] == "rm")
            {
                // ********* SENDING DATA TO SERVER *******

                // **** CHECK PATH FOR REMOVING ****
                if (splitString.size() == 1)
                {
                    //break;
                }

                // **** get path value ****
                string filePath = splitString[1];

                // **** SETUP FOR HEADER ****
                msg.preamble = PREAMBLE_RM;
                msg.msg_class = e_msg_class_request;
                msg.msg_type = e_msg_type_rm;
                msg.timestamp = 123456789;
                msg.length_of_data = filePath.length();

                // **** DISPLAY DATA FOR CHECKING ****
                cout << "Message to send:\n";
                cout << "\tMessage: " << filePath << endl;
                cout << "\tLen: " << filePath.length() << endl;

                // **** SETUP MEMORY SPACE FOR SENDING ****
                data_to_send = (char *)malloc(sizeof(struct message_header) + msg.length_of_data);
                int idex = 0;
                memcpy(data_to_send + idex, &msg.preamble, sizeof(msg.preamble));
                idex += sizeof(msg.preamble);
                memcpy(data_to_send + idex, &msg.msg_class, sizeof(msg.msg_class));
                idex += sizeof(msg.msg_class);
                memcpy(data_to_send + idex, &msg.msg_type, sizeof(msg.msg_type));
                idex += sizeof(msg.msg_type);
                memcpy(data_to_send + idex, &msg.timestamp, sizeof(msg.timestamp));
                idex += sizeof(msg.timestamp);
                memcpy(data_to_send + idex, &msg.length_of_data, sizeof(msg.length_of_data));
                memcpy(data_to_send + sizeof(struct message_header), (char *)filePath.c_str(), msg.length_of_data);

                // **** TOTAL BYTES WE SEND *****
                printf("\nTotal data to send: %ld bytes\n", sizeof(struct message_header) + msg.length_of_data);

                // **** SENDING DATA TO SERVER ****
                int statusSend = send(sockfd, data_to_send, sizeof(struct message_header) + msg.length_of_data, 0);
                if (statusSend < 0)
                {
                    perror("\nError: Sending failed\n");
                }
                else
                {
                    //cout << "Already send!" << endl;
                    printf("\nClient send %d bytes\n\n", statusSend);
                }

                // ********* RECEIVING RESPONSE FROM SERVER *******

                // **** Setup variables for communicating
                message_header recmsg;
                char *data_recv = (char *)malloc(1024);
                char message[1024];
                // **** RECEIVING MESSAGE FROM SERVER
                readVal = recv(sockfd, data_recv, 1024, 0);
                if (readVal < 0)
                {
                    cout << "Reading Failed!\n";
                    //break;
                }
                else
                {
                    printf("\nClient received %d bytes\n", readVal);
                }

                // **** FULLFILL VALUES
                idex = 0;
                memcpy(&recmsg.preamble, data_recv + idex, sizeof(recmsg.preamble));
                idex += sizeof(recmsg.preamble);
                memcpy(&recmsg.msg_class, data_recv + idex, sizeof(recmsg.msg_class));
                idex += sizeof(recmsg.msg_class);
                memcpy(&recmsg.msg_type, data_recv + idex, sizeof(recmsg.msg_type));
                idex += sizeof(recmsg.msg_type);
                memcpy(&recmsg.timestamp, data_recv + idex, sizeof(recmsg.timestamp));
                idex += sizeof(recmsg.timestamp);
                memcpy(&recmsg.length_of_data, data_recv + idex, sizeof(recmsg.length_of_data));
                memcpy(&message, data_recv + sizeof(message_header), recmsg.length_of_data);

                // **** CONVERT MESSAGE TO STRING
                string msgOfClient = getString(message, recmsg.length_of_data);

                // **** DISPLAY DATA THAT WE RECEIVED
                printf("\nDATA from Server:\n");
                printf("\t  Preamble:       = %d    \n", recmsg.preamble);
                printf("\t  Class:          = %d    \n", recmsg.msg_class);
                printf("\t  Type:           = %d    \n", recmsg.msg_type);
                printf("\t  timestamps:     = %ld    \n", recmsg.timestamp);
                printf("\t  Len:            = %d    \n", recmsg.length_of_data);
                cout << "Message from Server: " << msgOfClient << endl;

                // ****** DEALLOCATED ALL MEMORY WHICH ARE ALLOCATED *****
                free(data_recv);
                data_recv = nullptr;
                free(data_to_send);
                data_to_send = nullptr;
            }

            if (splitString[0] == "download")
            {
                // **** SETIUP DATA WE NEEDS
                string fileName = splitString[1];
                string localPath = splitString[2];

                // **** DISPLAYING DATA
                cout << "\n\nFILENAME: " << fileName << endl;
                cout << "localPath: " << localPath << endl;

                // **** SENDING FILENAME TO SERVER
                msg.preamble = PREAMBLE_DL;
                msg.msg_class = e_msg_class_request;
                msg.msg_type = e_msg_type_get;
                msg.timestamp = 112324;
                msg.length_of_data = fileName.length();
                cout << "\n\nfileName: " << fileName << " -- len: " << fileName.length() << endl;
                data_to_send = (char *)malloc(sizeof(struct message_header) + msg.length_of_data);

                // **** SETUP FOR SENDING FILENAME;
                int idex = 0;
                memcpy(data_to_send + idex, &msg.preamble, sizeof(msg.preamble));
                idex += sizeof(msg.preamble);
                memcpy(data_to_send + idex, &msg.msg_class, sizeof(msg.msg_class));
                idex += sizeof(msg.msg_class);
                memcpy(data_to_send + idex, &msg.msg_type, sizeof(msg.msg_type));
                idex += sizeof(msg.msg_type);
                memcpy(data_to_send + idex, &msg.timestamp, sizeof(msg.timestamp));
                idex += sizeof(msg.timestamp);
                memcpy(data_to_send + idex, &msg.length_of_data, sizeof(msg.length_of_data));
                memcpy(data_to_send + sizeof(message_header), (char *)fileName.c_str(), msg.length_of_data);

                printf("\nTotal data to send: %ld bytes", sizeof(struct message_header) + msg.length_of_data);

                // **** SENDING FILENAME TO SERVer
                int sendVal = send(sockfd, data_to_send, sizeof(struct message_header) + msg.length_of_data, 0);
                if (sendVal < 0)
                {
                    cout << "Error: send failed\n";
                    exit(EXIT_FAILURE);
                }
                else
                {
                    cout << "\nClient send " << sendVal << " bytes!\n"
                         << endl;
                }

                // **** RECEIVING RESPONSE FROM SERVER
                int totalRec = 0;
                char *data_recv = new char[BUFSIZE];
                memset(data_recv, 0, BUFSIZE);
                printf("Line: %d \n", __LINE__);

                while (true)
                {
                    int rec = recv(sockfd, data_recv, BUFSIZE, 0);
                    totalRec += rec;
                    if (rec == 0)
                    {
                        break;
                    }
                }
                printf("Line: %d \n", __LINE__);
                printf("\nClien received total %d bytes\n", totalRec);

                // **** CREATE & SAVE A LOCAL FILE
                char *localfile = (char *)malloc(1 + strlen(localPath.c_str()) + strlen("local_") + strlen(msgToServer));
                strcpy(localfile, localPath.c_str());
                if (localPath[localPath.length() - 1] == '/')
                {
                    strcat(localfile, "local_");
                }
                strcat(localfile, fileName.c_str());

                printf("\nClient save file to %s\n", localfile);
                FILE *fp = fopen(localfile, "w");
                // **** WRITING DATA TO LOCAL FILE
                fwrite(data_recv, 1, totalRec, fp);
            }
        }
        if (data_to_send)
        {
            free(data_to_send);
            data_to_send = nullptr;
        }
    }
    close(sockfd); //Dong socket
    return 0;
}
