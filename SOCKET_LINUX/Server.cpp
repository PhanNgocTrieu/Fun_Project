#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <dirent.h>
#include <filesystem>
#include <vector>
#include <iostream>

#define PREAMBLE_LS 1000
#define PREAMBLE_RM 2000
#define PREAMBLE_DL 3000

using namespace std;

string readFiles(const string &dirPath)
{
    string res = "";
    DIR *dir;
    struct dirent *ent;
    dir = opendir(dirPath.c_str());
    if (dir)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            res += ent->d_name;
            res += " ";
        }
        cout << endl;
    }
    else
    {
        res = "Could not find the path!";
    }

    return res;
}

string removeDirOrFile(const string &path)
{
    int status;
    status = remove(path.c_str());
    if (status == 0)
    {
        return string("File deleted successfully\n");
    }
    return string("Error deleting!\n");
}

char *substr(char *arr, int begin, int len)
{
    char *res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

vector<string> split(char *_string)
{
    vector<string> res;
    string s = "";
    char *pStr = _string;
    int limit = strlen(_string);
    while (pStr != nullptr && limit > 0)
    {
        if (*pStr == ' ')
        {
            res.push_back(s);
            s = "";
        }
        else
        {
            s += *pStr;
        }
        pStr++;
        limit--;
    }
    if (s != "")
    {
        res.push_back(s);
    }
    return res;
}

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

string getString(char *const &_String, int size)
{
    string s = "";
    for (int idex = 0; idex < size; idex++)
    {
        s += _String[idex];
    }
    return s;
}

void downloadFile(int sockfd, message_header msg, char *buff)
{
    if (sockfd < 0)
    {
        return;
    }

    while (1)
    {
        char *fileName;
        memcpy(fileName, buff + sizeof(message_header), msg.length_of_data);
        if (fileName[0] == '.')
        {
            break;
        }
        FILE *fp = fopen(fileName, "r");
        fseek(fp, 0, SEEK_END);
        long FileSize = ftell(fp);
        char GotFileSize[1024];

        send(sockfd, GotFileSize, 1024, 0);
        rewind(fp);
        long SizeCheck = 0;
        char *mfcc;
        if (FileSize > 1499)
        {
            mfcc = (char *)malloc(1500);
            while (SizeCheck < FileSize)
            {
                int Read = fread(mfcc, 1500, sizeof(char), fp);
                int Sent = send(sockfd, mfcc, Read, 0);
                SizeCheck += Sent;
            }
        }
        else
        {
            mfcc = (char *)malloc(FileSize + 1);
            fread(mfcc, FileSize, sizeof(char), fp);
            send(sockfd, mfcc, FileSize, 0);
        }
        fclose(fp);
        sleep(500);
        free(mfcc);
    }
}

int main(int argc, char const *argv[])
{

    int server_fd, new_socket, read_Val;
    struct sockaddr_in address; // address for connecting
    int opt = 1;
    int addrlen = sizeof(address);
    int PORT;

    // Communicating from Server and Client variables;
    
    vector<string> token;

    if (argc < 2)
    {
        perror("Error: Port not provided");
        exit(EXIT_FAILURE);
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 1234
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    PORT = atoi(argv[1]);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 1234
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("\nServer is listening....");

    char *data_to_send;
    message_header recmsg;
    char *buff = new char[1024];
    char msgFromClient[100];
    e_msg_type typeOfMessage;

    read_Val = read(new_socket, buff, 1024);
    if (read_Val <= 0)
    {
        perror("Error: could not read fom client!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("\nServer read %d bytes\n", read_Val);
    }

    
    memcpy(&typeOfMessage, buff + sizeof(recmsg.preamble) + sizeof(recmsg.msg_class), sizeof(recmsg.msg_type));

    if (typeOfMessage == e_msg_type_ls)
    {
        // *********** RECEIVING REQUEST FROM CLIENT ************
        int index = 0;
        memcpy(&recmsg.preamble, buff + index, sizeof(recmsg.preamble));
        index += sizeof(recmsg.preamble);
        memcpy(&recmsg.msg_class, buff + index, sizeof(recmsg.msg_class));
        index += sizeof(recmsg.msg_class);
        memcpy(&recmsg.msg_type, buff + index, sizeof(recmsg.msg_type));
        index += sizeof(recmsg.msg_type); // xem phan address
        memcpy(&recmsg.timestamp, buff + index, sizeof(recmsg.timestamp));
        index += sizeof(recmsg.timestamp);
        memcpy(&recmsg.length_of_data, buff + index, sizeof(recmsg.length_of_data));
        memcpy(&msgFromClient, buff + sizeof(struct message_header), recmsg.length_of_data);
        // msgFromClient[recmsg.length_of_data + 1] = '\n';

        // **** CONVERT TO STRING ****
        string stringMessage = getString(msgFromClient, recmsg.length_of_data);

        // **** CHECKING DATA FROM CLIENT
        cout << "\tCommand: " << stringMessage << endl;
        cout << "\tlen of path: " << stringMessage.length() << endl;
        // cout << "\tMessage: " << filePath << endl;

        // ************* SENDING RESPONSE MESSAGE TO CLIENT *********
        message_header msgForClient;

        // **** HANDLING WITH LISTING ALL OF FILES ****
        string filePath = readFiles(stringMessage);

        // **** SETUP MEMORY FOR SENDING ****
        msgForClient.preamble = PREAMBLE_LS;
        msgForClient.msg_class = e_msg_class_response;
        msgForClient.msg_type = e_msg_type_ls;
        msgForClient.timestamp = 1231212;
        msgForClient.length_of_data = filePath.length();

        // **** CHECKING MESSAGE BEFORE SENDING
        cout << "Files: " << filePath;

        // **** SETUP MEMORY SPACE FOR SENDING ****
        data_to_send = (char *)malloc(sizeof(message_header) + msgForClient.length_of_data);
        int idex = 0;
        memcpy(data_to_send + idex, &msgForClient.preamble, sizeof(msgForClient.preamble));
        idex += sizeof(msgForClient.preamble);
        memcpy(data_to_send + idex, &msgForClient.msg_class, sizeof(msgForClient.msg_class));
        idex += sizeof(msgForClient.msg_class);
        memcpy(data_to_send + idex, &msgForClient.msg_type, sizeof(msgForClient.msg_type));
        idex += sizeof(msgForClient.msg_type);
        memcpy(data_to_send + idex, &msgForClient.timestamp, sizeof(msgForClient.timestamp));
        idex += sizeof(msgForClient.timestamp);
        memcpy(data_to_send + idex, &msgForClient.length_of_data, sizeof(msgForClient.length_of_data));
        memcpy(data_to_send + sizeof(message_header), (char *)filePath.c_str(), msgForClient.length_of_data);

        // **** SENDING RESPOSE MESSAGE ****
        int statusSend = send(new_socket, data_to_send, sizeof(message_header) + msgForClient.length_of_data, 0);
        if (statusSend < 0)
        {
            perror("Error: Sending failed");
        }
        else
        {
            //cout << "Already send!" << endl;
            printf("\nClient send %d bytes\n", statusSend);
        }

        // **** DEALLOCATED MEMORY THAT WAS ALLOCATED ****
        free(data_to_send);
        data_to_send = nullptr;
    }

    if (typeOfMessage == e_msg_type_rm)
    {
        // **** RECEIVING FROM CLIENT *****
        int index = 0;
        memcpy(&recmsg.preamble, buff + index, sizeof(recmsg.preamble));
        index += sizeof(recmsg.preamble);
        memcpy(&recmsg.msg_class, buff + index, sizeof(recmsg.msg_class));
        index += sizeof(recmsg.msg_class);
        memcpy(&recmsg.msg_type, buff + index, sizeof(recmsg.msg_type));
        index += sizeof(recmsg.msg_type); // xem phan address
        memcpy(&recmsg.timestamp, buff + index, sizeof(recmsg.timestamp));
        index += sizeof(recmsg.timestamp);
        memcpy(&recmsg.length_of_data, buff + index, sizeof(recmsg.length_of_data));
        memcpy(&msgFromClient, buff + sizeof(struct message_header), recmsg.length_of_data);

        // **** CONVERT TO STRING  ****
        string stringMessage = getString(msgFromClient, recmsg.length_of_data);

        // **** HANDLING REMOVING FILE ****
        string message = removeDirOrFile(stringMessage);

        // **** CHECKING MESSAGE BEFORE SENDING ****
        cout << "Is it successful? -- " << message << endl;

        // **** FULLFILLING DATA TO HEADER
        message_header sendmsg;
        sendmsg.preamble = PREAMBLE_RM;
        sendmsg.msg_class = e_msg_class_response;
        sendmsg.msg_type = e_msg_type_rm;
        sendmsg.timestamp = 1231212;
        sendmsg.length_of_data = message.length();

        // **** SETUP MEMORY FOR SENDING ****
        data_to_send = (char *)malloc(sizeof(message_header) + sendmsg.length_of_data);
        index = 0;
        memcpy(data_to_send + index, &sendmsg.preamble, sizeof(sendmsg.preamble));
        index += sizeof(sendmsg.preamble);
        memcpy(data_to_send + index, &sendmsg.msg_class, sizeof(sendmsg.msg_class));
        index += sizeof(sendmsg.msg_class);
        memcpy(data_to_send + index, &sendmsg.msg_type, sizeof(sendmsg.msg_type));
        index += sizeof(sendmsg.msg_type);
        memcpy(data_to_send + index, &sendmsg.timestamp, sizeof(sendmsg.timestamp));
        index += sizeof(sendmsg.timestamp);
        memcpy(data_to_send + index, &sendmsg.length_of_data, sizeof(sendmsg.length_of_data));
        memcpy(data_to_send + sizeof(message_header), (char *)message.c_str(), sendmsg.length_of_data);

        // **** SENDING RESPONSE TO CLIENT ****
        int statusSend = send(new_socket, data_to_send, sizeof(message_header) + sendmsg.length_of_data , 0);
        if (statusSend < 0)
        {
            perror("Error: Sending failed");
        }
        else
        {
            //cout << "Already send!" << endl;
            printf("\nClient send %d bytes\n", statusSend);
        }



        // **** DEALLOCATED MEMORY WE ALLOCATED
        free(data_to_send);
        data_to_send = nullptr;
    }

    if (typeOfMessage == e_msg_type_get)
    {
        int index = 0;
        memcpy(&recmsg.preamble, buff + index, sizeof(recmsg.preamble));
        index += sizeof(recmsg.preamble);
        memcpy(&recmsg.msg_class, buff + index, sizeof(recmsg.msg_class));
        index += sizeof(recmsg.msg_class);
        memcpy(&recmsg.msg_type, buff + index, sizeof(recmsg.msg_type));
        index += sizeof(recmsg.msg_type); // xem phan address
        memcpy(&recmsg.timestamp, buff + index, sizeof(recmsg.timestamp));
        index += sizeof(recmsg.timestamp);
        memcpy(&recmsg.length_of_data, buff + index, sizeof(recmsg.length_of_data));
        memcpy(&msgFromClient, buff + sizeof(struct message_header), recmsg.length_of_data);
        msgFromClient[recmsg.length_of_data + 1] = '\n';

        string stringMessage = getString(msgFromClient, recmsg.length_of_data);
        downloadFile(new_socket, recmsg, buff);
    }




    close(new_socket);
    close(server_fd);

    return 0;
}
