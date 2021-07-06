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
        cout << "\ns : " << s << endl;
        cout << "len: " << s.length() << endl;
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

void DownloadFile(int sockfd, char *data_to_send, message_header msg, vector<string> splitString)
{
    char *fileName;
    fileName = (char *)stringforFile[1].c_str();
    char *localPath;
    localPath = (char *)stringforFile[2].c_str();

    msg.preamble = 12343;
    msg.msg_class = e_msg_class_request;
    msg.msg_type = e_msg_type_get;
    msg.timestamp = 112324;

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
        msg.length_of_data = strlen(fileName);

        data_to_send = (char *)malloc(sizeof(struct message_header) + msg.length_of_data);

        // setup for sending fileName;
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
        memcpy(data_to_send + sizeof(message_header), fileName, msg.length_of_data);

        printf("\nTotal data to send: %ld bytes", sizeof(struct message_header) + msg.length_of_data);
        send(sockfd, fileName, sizeof(fileName), 0);
        char GotFileSize[1024];
        recv(sockfd, GotFileSize, 1024, 0);

        // taking size of File in integer
        long FileSize = atoi(GotFileSize);
        long SizeCheck = 0;

        // open local file that we will store;
        FILE *fp = fopen(localPath.c_str(), "w");
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

int main(int argc, char *argv[])
{
    int sockfd, portno, readVal;
    int portNum;
    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi to
    message_header msg;

    char msgToServer[100];
    char *data_to_send;
    char *buffer;

    FILE *fp;
    //Client nhan tham so hostname va port tu dong lenh
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        return -1;
    }
    portno = atoi(argv[2]); //Chuyen cong dich vu thanh so nguyen

    //Tao socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "ERROR opening socket");
        return -1;
    }

    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;     //Mac dinh
    serv_addr.sin_port = htons(portno); //Cong dich vu
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    //Goi ham connect de thuc hien mot ket noi den server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return 1;
    }

    cout << "Message/Command: ";
    fgets(msgToServer, 100, stdin);

    vector<string> splitString = split(msgToServer);
    if (splitString.size() != 0)
    {
        if (splitString[0] == "ls")
        {
            msg.length_of_data = splitString[1].length();
            cout << "Message to send:\n";
            cout << "\tMessage: " << splitString[1] << endl;
            cout << "\tLen: " << splitString[1].length() << endl;

            msg.preamble = 1111;
            msg.msg_type = e_msg_type_ls;
            msg.timestamp = 123456789;

            data_to_send = (char *)malloc(sizeof(struct message_header) + msg.length_of_data);
            printf("\nTotal data to send: %ld bytes", sizeof(struct message_header) + msg.length_of_data);

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
            memcpy(data_to_send + sizeof(struct message_header), (char *)splitString[1].c_str(), msg.length_of_data);

            int statusSend = send(sockfd, data_to_send, sizeof(struct message_header) + msg.length_of_data, 0);
            if (statusSend < 0)
            {
                perror("Error: Sending failed");
            }
            else
            {
                //cout << "Already send!" << endl;
                printf("\nClient send %d bytes\n", statusSend);
            }
        }

        if (splitString[0] == "rm")
        {
            msg.length_of_data = splitString[1].length();
            cout << "Message to send:\n";
            cout << "\tMessage: " << splitString[1] << endl;
            cout << "\tLen: " << splitString[1].length() << endl;

            msg.preamble = 1111;
            msg.msg_type = e_msg_type_rm;
            msg.timestamp = 123456789;

            data_to_send = (char *)malloc(sizeof(struct message_header) + msg.length_of_data);
            printf("\nTotal data to send: %ld bytes", sizeof(struct message_header) + msg.length_of_data);

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
            memcpy(data_to_send + sizeof(struct message_header), (char *)splitString[1].c_str(), msg.length_of_data);

            int statusSend = send(sockfd, data_to_send, sizeof(struct message_header) + msg.length_of_data, 0);
            if (statusSend < 0)
            {
                perror("Error: Sending failed");
            }
            else
            {
                //cout << "Already send!" << endl;
                printf("\nClient send %d bytes\n", statusSend);
            }
        }

        if (splitString[0] == "download")
        {

            DownloadFile(sockfd, data_to_send, msg, splitString);
        }

        string list;
        buffer = new char[1024];
        readVal = read(sockfd, buffer, 1024);
        if (readVal <= 0)
        {
            perror("Error: could not read fom client!\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("\nServer read %d bytes\n", readVal);
        }

        char msgFromServer[1024];

        printf("\nMessage from client: \n%s\n", buffer);

        delete buffer;
        buffer = nullptr;
    }
    close(sockfd); //Dong socket
    return 0;
}
