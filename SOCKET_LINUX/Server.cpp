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
#include <chrono>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

#define PREAMBLE_LS 1000
#define PREAMBLE_RM 2000
#define PREAMBLE_DL 3000

using namespace std;

string readFiles(string dirPath)
{
    if (dirPath[dirPath.length() - 1] != '/')
    {
        char c = '/';
        dirPath.push_back(c);
    }
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

struct message_ls
{
    struct message_header header;
    // Data
    std::string fileList;
};

struct message_rm
{
    struct message_header header;
    // Data
    std::string announce;
};

struct message_get
{
    struct message_header header;
    // Data
    std::string fileName;
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

int main(int argc, char const *argv[])
{
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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

    while (1)
    {
        printf("\n\nServer is listening....\n");

        char *data_to_send;
        int size_of_header_not_padding = sizeof(message_header::preamble) + sizeof(message_header::msg_class) +
                                         sizeof(message_header::msg_type) + sizeof(message_header::timestamp) + sizeof(message_header::length_of_data);
        char *buff = (char *)malloc(1024);
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

        memcpy(&typeOfMessage, buff + sizeof(message_header::preamble) + sizeof(message_header::msg_class), sizeof(message_header::msg_type));
        cout << "\nTYPE OF MESSAGE: " << typeOfMessage << endl;

        if (typeOfMessage == e_msg_type_ls)
        {
            message_ls recmsg;
            char msgFromClient[100];
            // *********** RECEIVING REQUEST FROM CLIENT ************
            int index = 0;
            memcpy(&recmsg.header.preamble, buff + index, sizeof(recmsg.header.preamble));
            index += sizeof(recmsg.header.preamble);
            memcpy(&recmsg.header.msg_class, buff + index, sizeof(recmsg.header.msg_class));
            index += sizeof(recmsg.header.msg_class);
            memcpy(&recmsg.header.msg_type, buff + index, sizeof(recmsg.header.msg_type));
            index += sizeof(recmsg.header.msg_type); // xem phan address
            memcpy(&recmsg.header.timestamp, buff + index, sizeof(recmsg.header.timestamp));
            index += sizeof(recmsg.header.timestamp);
            memcpy(&recmsg.header.length_of_data, buff + index, sizeof(recmsg.header.length_of_data));
            index += sizeof(recmsg.header.length_of_data);
            memcpy(&msgFromClient, buff + index, recmsg.header.length_of_data);
            recmsg.fileList = getString(msgFromClient, recmsg.header.length_of_data);

            // **** CONVERT TO STRING ****
            string listString = getString(msgFromClient, recmsg.header.length_of_data);


            // ************* SENDING RESPONSE MESSAGE TO CLIENT *********
            message_ls msgForClient;

            // **** HANDLING WITH LISTING ALL OF FILES ****
            std::string fileList = readFiles(recmsg.fileList);

            // **** SETUP MEMORY FOR SENDING ****
            msgForClient.header.preamble = PREAMBLE_LS;
            msgForClient.header.msg_class = e_msg_class_response;
            msgForClient.header.msg_type = e_msg_type_ls;
            msgForClient.header.timestamp = millisec_since_epoch;
            msgForClient.header.length_of_data = fileList.length();
            msgForClient.fileList = fileList;


            // **** SETUP MEMORY SPACE FOR SENDING ****

            data_to_send = (char *)malloc(size_of_header_not_padding + msgForClient.header.length_of_data);
            int idex = 0;
            memcpy(data_to_send + idex, &msgForClient.header.preamble, sizeof(msgForClient.header.preamble));
            idex += sizeof(msgForClient.header.preamble);
            memcpy(data_to_send + idex, &msgForClient.header.msg_class, sizeof(msgForClient.header.msg_class));
            idex += sizeof(msgForClient.header.msg_class);
            memcpy(data_to_send + idex, &msgForClient.header.msg_type, sizeof(msgForClient.header.msg_type));
            idex += sizeof(msgForClient.header.msg_type);
            memcpy(data_to_send + idex, &msgForClient.header.timestamp, sizeof(msgForClient.header.timestamp));
            idex += sizeof(msgForClient.header.timestamp);
            memcpy(data_to_send + idex, &msgForClient.header.length_of_data, sizeof(msgForClient.header.length_of_data));
            idex += sizeof(msgForClient.header.length_of_data);
            memcpy(data_to_send + idex, (char *)msgForClient.fileList.c_str(), msgForClient.header.length_of_data);

            // ****** ALL OF BYTES THAT WE HAVE TO SEND ****
            printf("\nTotal data to send: %d bytes", size_of_header_not_padding + msgForClient.header.length_of_data);

            // **** SENDING RESPOSE MESSAGE ****
            int statusSend = send(new_socket, data_to_send, size_of_header_not_padding + msgForClient.header.length_of_data, 0);
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


        // **** Handling with rm type
        if (typeOfMessage == e_msg_type_rm)
        {
            message_rm recmsg;
            char msgFromClient[1024];
            // *********** RECEIVING REQUEST FROM CLIENT ************
            int index = 0;
            memcpy(&recmsg.header.preamble, buff + index, sizeof(recmsg.header.preamble));
            index += sizeof(recmsg.header.preamble);
            memcpy(&recmsg.header.msg_class, buff + index, sizeof(recmsg.header.msg_class));
            index += sizeof(recmsg.header.msg_class);
            memcpy(&recmsg.header.msg_type, buff + index, sizeof(recmsg.header.msg_type));
            index += sizeof(recmsg.header.msg_type); // xem phan address
            memcpy(&recmsg.header.timestamp, buff + index, sizeof(recmsg.header.timestamp));
            index += sizeof(recmsg.header.timestamp);
            memcpy(&recmsg.header.length_of_data, buff + index, sizeof(recmsg.header.length_of_data));
            index += sizeof(recmsg.header.length_of_data);
            memcpy(&msgFromClient, buff + index, recmsg.header.length_of_data);
            recmsg.announce = getString(msgFromClient, recmsg.header.length_of_data);


            // **** CONVERT TO STRING ****
            string filePath = getString(msgFromClient, recmsg.header.length_of_data);

            // **** HANDLING REMOVING FILE ****
            string message = removeDirOrFile(filePath);

            // **** CHECKING MESSAGE BEFORE SENDING ****
            cout << "\nIs it successful? --> " << message << endl
                 << endl;

            // **** FULLFILLING DATA TO HEADER
            message_rm sendmsg;
            sendmsg.header.preamble = PREAMBLE_RM;
            sendmsg.header.msg_class = e_msg_class_response;
            sendmsg.header.msg_type = e_msg_type_rm;
            sendmsg.header.timestamp = millisec_since_epoch;
            sendmsg.header.length_of_data = message.length();
            sendmsg.announce = message;


            // **** SETUP MEMORY FOR SENDING ****
            data_to_send = (char *)malloc(size_of_header_not_padding + sendmsg.header.length_of_data);
            int idex = 0;
            memcpy(data_to_send + idex, &sendmsg.header.preamble, sizeof(sendmsg.header.preamble));
            idex += sizeof(sendmsg.header.preamble);
            memcpy(data_to_send + idex, &sendmsg.header.msg_class, sizeof(sendmsg.header.msg_class));
            idex += sizeof(sendmsg.header.msg_class);
            memcpy(data_to_send + idex, &sendmsg.header.msg_type, sizeof(sendmsg.header.msg_type));
            idex += sizeof(sendmsg.header.msg_type);
            memcpy(data_to_send + idex, &sendmsg.header.timestamp, sizeof(sendmsg.header.timestamp));
            idex += sizeof(sendmsg.header.timestamp);
            memcpy(data_to_send + idex, &sendmsg.header.length_of_data, sizeof(sendmsg.header.length_of_data));
            idex += sizeof(sendmsg.header.length_of_data);
            memcpy(data_to_send + idex, (char *)sendmsg.announce.c_str(), sendmsg.header.length_of_data);



            // ****** ALL OF BYTES THAT WE HAVE TO SEND ****
            printf("\nTotal data to send: %d bytes", size_of_header_not_padding + sendmsg.header.length_of_data);

            // **** SENDING RESPOSE MESSAGE ****
            int statusSend = send(new_socket, data_to_send, size_of_header_not_padding + sendmsg.header.length_of_data, 0);
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

        // // **** HANDLING WITH DOWNLOAD FILE
        if (typeOfMessage == e_msg_type_get)
        {
            message_get recmsg;
            char msgFromClient[100];
            // **** RECEIVING FILENAME FROM CLIENT
            int index = 0;
            memcpy(&recmsg.header.preamble, buff + index, sizeof(recmsg.header.preamble));
            index += sizeof(recmsg.header.preamble);
            memcpy(&recmsg.header.msg_class, buff + index, sizeof(recmsg.header.msg_class));
            index += sizeof(recmsg.header.msg_class);
            memcpy(&recmsg.header.msg_type, buff + index, sizeof(recmsg.header.msg_type));
            index += sizeof(recmsg.header.msg_type); // xem phan address
            memcpy(&recmsg.header.timestamp, buff + index, sizeof(recmsg.header.timestamp));
            index += sizeof(recmsg.header.timestamp);
            memcpy(&recmsg.header.length_of_data, buff + index, sizeof(recmsg.header.length_of_data));
            index += sizeof(recmsg.header.length_of_data);
            memcpy(&msgFromClient, buff + index, recmsg.header.length_of_data);
            recmsg.fileName = getString(msgFromClient, recmsg.header.length_of_data);

            // ****** SENDING RESPONSE TO CLIENT *****
            char *filepath = (char *)malloc(recmsg.header.length_of_data);
            strcpy(filepath, msgFromClient);

            printf("\nFilepath       = %s", filepath);

            FILE *fp = fopen(filepath, "r");
            if (fp != NULL)
            {
                printf("\nFile exits");
                fseek(fp, 0, SEEK_END);
                long fileSize = ftell(fp);
                printf("\nFilesize          = %ld bytes", fileSize);
                fseek(fp, 0, SEEK_SET); // reset the pointer t	o the start of the file
                long totalSend = 0;
                char *pBuf = (char *)malloc(fileSize);
                int Read = fread(pBuf, 1, fileSize, fp);
                printf("\nCopy %d bytes to buffer\n", Read);
                totalSend = send(new_socket, pBuf, fileSize, 0);

                printf("\nServer send total %ld bytes\n", totalSend);
                fflush(fp);
                fclose(fp);
            }
            else
            {
                printf("\nFile does not exit!");
                const char *receive = "File does not exist!";
                send(new_socket, receive, strlen(receive), 0);
            }
        }
        free(buff);
        buff = nullptr;
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
