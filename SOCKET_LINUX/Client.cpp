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
#include <chrono>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

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

struct message_ls
{
    struct message_header header;
    // Data
    string file_path;
};

struct message_rm
{
    struct message_header header;
    // Data
    string file_path;
};

struct message_get
{
    struct message_header header;
    // Data
    string file_path;
    string local_path;
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
    // TIME IN MILLISECOND
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

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

        int size_of_header_not_padding = (sizeof(message_header::preamble) + sizeof(message_header::msg_class) + sizeof(message_header::msg_type) + sizeof(message_header::timestamp) + sizeof(message_header::length_of_data));

        char msgToServer[1024];
        char *data_to_send = nullptr;

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
                message_ls msg;
                string filePath;

                // ***** SETUP FOR HEADER *****
                msg.file_path = splitString.size() == 1 ? "./" : splitString[1];
                msg.header.preamble = PREAMBLE_LS;
                msg.header.msg_class = e_msg_class_request;
                msg.header.msg_type = e_msg_type_ls;
                msg.header.timestamp = millisec_since_epoch;
                msg.header.length_of_data = msg.file_path.length();

                // ***** SET-UP MEMORY FOR SENDING ******
                data_to_send = (char *)malloc(size_of_header_not_padding + msg.header.length_of_data);
                int idex = 0;
                memcpy(data_to_send + idex, &msg.header.preamble, sizeof(msg.header.preamble));
                idex += sizeof(msg.header.preamble);
                memcpy(data_to_send + idex, &msg.header.msg_class, sizeof(msg.header.msg_class));
                idex += sizeof(msg.header.msg_class);
                memcpy(data_to_send + idex, &msg.header.msg_type, sizeof(msg.header.msg_type));
                idex += sizeof(msg.header.msg_type);
                memcpy(data_to_send + idex, &msg.header.timestamp, sizeof(msg.header.timestamp));
                idex += sizeof(msg.header.timestamp);
                memcpy(data_to_send + idex, &msg.header.length_of_data, sizeof(msg.header.length_of_data));
                idex += sizeof(msg.header.length_of_data);
                memcpy(data_to_send + idex, (const char *)msg.file_path.c_str(), msg.header.length_of_data);

                // ****** ALL OF BYTES THAT WE HAVE TO SEND ****
                printf("\nTotal data to send: %d bytes", size_of_header_not_padding + msg.header.length_of_data);

                // ***** SENDING DATA TO SERVER *****
                int statusSend = send(sockfd, data_to_send, size_of_header_not_padding + msg.header.length_of_data, 0);
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
                message_ls recmsg;
                char *recev_data = (char *)malloc(1024);
                // char messgFromServer[1024];

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
                memcpy(&recmsg.header.preamble, recev_data + index, sizeof(recmsg.header.preamble));
                index += sizeof(recmsg.header.preamble);
                memcpy(&recmsg.header.msg_class, recev_data + index, sizeof(recmsg.header.msg_class));
                index += sizeof(recmsg.header.msg_class);
                memcpy(&recmsg.header.msg_type, recev_data + index, sizeof(recmsg.header.msg_type));
                index += sizeof(recmsg.header.msg_type);
                memcpy(&recmsg.header.timestamp, recev_data + index, sizeof(recmsg.header.timestamp));
                index += sizeof(recmsg.header.timestamp);
                memcpy(&recmsg.header.length_of_data, recev_data + index, sizeof(recmsg.header.length_of_data));
                index += sizeof(recmsg.header.length_of_data);
                memcpy(msgToServer, recev_data + index, recmsg.header.length_of_data);
                recmsg.file_path = getString(msgToServer, recmsg.header.length_of_data);

                cout << "Message of client: " << recmsg.file_path << endl
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
                message_rm msg;
                // **** CHECK PATH FOR REMOVING ****
                if (splitString.size() == 1)
                {
                    break;
                }
                else
                {
                }
                // **** get path value ****
                string filePath = splitString[1];

                // **** SETUP FOR HEADER ****
                msg.header.preamble = PREAMBLE_RM;
                msg.header.msg_class = e_msg_class_request;
                msg.header.msg_type = e_msg_type_rm;
                msg.header.timestamp = millisec_since_epoch;
                msg.header.length_of_data = filePath.length();
                msg.file_path = filePath;

                // **** SETUP MEMORY SPACE FOR SENDING ****
                data_to_send = (char *)malloc(size_of_header_not_padding + msg.header.length_of_data);
                int idex = 0;
                memcpy(data_to_send + idex, &msg.header.preamble, sizeof(msg.header.preamble));
                idex += sizeof(msg.header.preamble);
                memcpy(data_to_send + idex, &msg.header.msg_class, sizeof(msg.header.msg_class));
                idex += sizeof(msg.header.msg_class);
                memcpy(data_to_send + idex, &msg.header.msg_type, sizeof(msg.header.msg_type));
                idex += sizeof(msg.header.msg_type);
                memcpy(data_to_send + idex, &msg.header.timestamp, sizeof(msg.header.timestamp));
                idex += sizeof(msg.header.timestamp);
                memcpy(data_to_send + idex, &msg.header.length_of_data, sizeof(msg.header.length_of_data));
                idex += sizeof(msg.header.length_of_data);
                memcpy(data_to_send + idex, (char *)msg.file_path.c_str(), msg.header.length_of_data);

                // **** TOTAL BYTES WE SEND *****
                printf("\nTotal data to send: %d bytes\n", msg.header.length_of_data);

                // **** SENDING DATA TO SERVER ****
                int statusSend = send(sockfd, data_to_send, size_of_header_not_padding + msg.header.length_of_data, 0);
                if (statusSend < 0)
                {
                    perror("\nError: Sending failed\n");
                }
                else
                {
                    //cout << "Already send!" << endl;
                    printf("\nClient send %d bytes\n\n", statusSend);
                }

                // ************** RECEIVING RESPONSE FROM SERVER **********
                // **** Setup variables for communicating
                message_rm recmsg;
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
                memcpy(&recmsg.header.preamble, data_recv + idex, sizeof(recmsg.header.preamble));
                idex += sizeof(recmsg.header.preamble);
                memcpy(&recmsg.header.msg_class, data_recv + idex, sizeof(recmsg.header.msg_class));
                idex += sizeof(recmsg.header.msg_class);
                memcpy(&recmsg.header.msg_type, data_recv + idex, sizeof(recmsg.header.msg_type));
                idex += sizeof(recmsg.header.msg_type);
                memcpy(&recmsg.header.timestamp, data_recv + idex, sizeof(recmsg.header.timestamp));
                idex += sizeof(recmsg.header.timestamp);
                memcpy(&recmsg.header.length_of_data, data_recv + idex, sizeof(recmsg.header.length_of_data));
                idex += sizeof(recmsg.header.length_of_data);
                memcpy(&msgToServer, data_recv + idex, recmsg.header.length_of_data);

                // **** CONVERT MESSAGE TO STRING
                recmsg.file_path = getString(msgToServer, recmsg.header.length_of_data);


                // ****** DEALLOCATED ALL MEMORY WHICH ARE ALLOCATED *****
                free(data_recv);
                data_recv = nullptr;
                free(data_to_send);
                data_to_send = nullptr;
            }

            // **** HANDLING WITH DOWN-LOAD
            if (splitString[0] == "download")
            {
                message_get msg;
                // **** SETIUP DATA WE NEEDS
                msg.file_path = splitString[1];
                msg.local_path = splitString[2];

                // **** SENDING FILENAME TO SERVER
                msg.header.preamble = PREAMBLE_DL;
                msg.header.msg_class = e_msg_class_request;
                msg.header.msg_type = e_msg_type_get;
                msg.header.timestamp = millisec_since_epoch;
                msg.header.length_of_data = msg.file_path.length();

                data_to_send = (char *)malloc(size_of_header_not_padding + msg.header.length_of_data);

                // **** SETUP FOR SENDING FILENAME;
                int idex = 0;
                memcpy(data_to_send + idex, &msg.header.preamble, sizeof(msg.header.preamble));
                idex += sizeof(msg.header.preamble);
                memcpy(data_to_send + idex, &msg.header.msg_class, sizeof(msg.header.msg_class));
                idex += sizeof(msg.header.msg_class);
                memcpy(data_to_send + idex, &msg.header.msg_type, sizeof(msg.header.msg_type));
                idex += sizeof(msg.header.msg_type);
                memcpy(data_to_send + idex, &msg.header.timestamp, sizeof(msg.header.timestamp));
                idex += sizeof(msg.header.timestamp);
                memcpy(data_to_send + idex, &msg.header.length_of_data, sizeof(msg.header.length_of_data));
                idex += sizeof(msg.header.length_of_data);
                memcpy(data_to_send + idex, (char *)msg.file_path.c_str(), msg.header.length_of_data);

                printf("\nTotal data to send: %d bytes", size_of_header_not_padding + msg.header.length_of_data);

                // **** SENDING FILENAME TO SERVER
                int sendVal = send(sockfd, data_to_send, size_of_header_not_padding + msg.header.length_of_data, 0);
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
                char buffer[BUFSIZE];
                memset(&buffer, 0, BUFSIZE);

                int rec = recv(sockfd, buffer, BUFSIZE, 0);
                totalRec += rec;

                if (strcmp(buffer,"File does not exist!") == 0)
                {
                    cout << "\nCould not find that file!\n\n";
                    continue;
                }

                //recv(sockfd, buffer, BUFSIZE, 0);
                //cout << buffer << endl;
                size_t found = msg.file_path.find_last_of("/\\");
                string fileName;
                if (found != string::npos)
                {
                    fileName = msg.file_path.find_last_of(found + 1);
                }
                else
                {
                    fileName = msg.file_path;
                }
                printf("\nClien received total %d bytes\n", totalRec);

                cout << "fileName: " << fileName << endl;
                char *localfile = (char *)malloc(1 + strlen(msg.local_path.c_str()) + strlen("local_") + strlen(fileName.c_str()));

                strcpy(localfile, msg.local_path.c_str());

                if (msg.local_path[msg.local_path.length() - 1] == '/')
                {
                    strcat(localfile, "local_");
                }
                else
                {
                    strcat(localfile, "/local_");
                }
                strcat(localfile, fileName.c_str());

                printf("%s -- %d\n", __FUNCTION__, __LINE__);
                printf("\nClient save file to %s\n", localfile);

                FILE *fp = fopen(localfile, "w");
                fwrite(buffer, 1, totalRec, fp);

                printf("%s -- %d\n", __FUNCTION__, __LINE__);
                fflush(fp);
                fclose(fp);
            }
        }
        if (data_to_send != nullptr)
        {
            free(data_to_send);
            data_to_send = nullptr;
        }
        printf("%s -- %d\n", __FUNCTION__, __LINE__);
    }
    close(sockfd); //Dong socket
    return 0;
}
