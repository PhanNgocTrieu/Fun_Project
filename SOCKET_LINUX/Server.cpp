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

// void checking()
// {
//     printf("\n\nAddress of preamble       = %p", &recmsg.preamble);
//     printf("\nAddress of msg_class        = %p", &recmsg.msg_class);
//     printf("\nAddress of msg_type         = %p", &recmsg.msg_type);
//     printf("\nAddress of timestamp        = %p", &recmsg.timestamp);
//     printf("\nAddress of length_of_data   = %p", &recmsg.length_of_data);

//     printf("\n\nValue of preamble           = %d", recmsg.preamble);
//     printf("\nValue of msg_class          = %d", recmsg.msg_class);
//     printf("\nValue of msg_type           = %d", recmsg.msg_type);
//     printf("\nValue of timestamp          = %ld", recmsg.timestamp);
//     printf("\nValue of length_of_data     = %d", recmsg.length_of_data);
//     printf("\nMessage from client         = %s\n", msgFromClient);
// }

string getString(char *const &_String, int size)
{
    string s = "";
    for (int idex = 0; idex < size; idex++)
    {
        s += _String[idex];
    }
    return s;
}

void downloadFile(int sockfd, message_header msg, char * buff)
{
    if (sockfd < 0)
    {
        return;
    }

    while (1)
    {
        char * fileName;
        memcpy(fileName, buff + sizeof(message_header), msg.length_of_data);
        recv(sockfd, fileName, sizeof(fileName),0);
        if (fileName[0] == '.')
        {
            break;
        }
        FILE* fp = fopen(fileName, "r");
		fseek(fp, 0, SEEK_END);
		long FileSize = ftell(fp);
		char GotFileSize[1024];
		snprintf(GotFileSize, 10, "%ld", FileSize);
		send(sockfd, GotFileSize, 1024, 0);
		rewind(fp);
		long SizeCheck = 0;
		char* mfcc;
		if(FileSize > 1499){
			mfcc = (char*)malloc(1500);
			while(SizeCheck < FileSize){
				int Read = fread(mfcc, 1500, sizeof(char), fp);
				int Sent = send(sockfd, mfcc, Read, 0);
				SizeCheck += Sent;
			}
		}
		else{
			mfcc = (char*)malloc(FileSize + 1);
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
    char *data_to_send;
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
    char *buff = new char[1024];
    char msgFromClient[100];
    e_msg_type typeOfMessage;
    string stringMessage;
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
    message_header recmsg;
    memcpy(&typeOfMessage, buff + sizeof(recmsg.preamble) + sizeof(recmsg.msg_class), sizeof(recmsg.msg_type));

    if (typeOfMessage == e_msg_type_ls)
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

        stringMessage = getString(msgFromClient, recmsg.length_of_data);

        string filePath = readFiles(stringMessage);
        cout << "\tCommand: " << stringMessage << endl;
        cout << "\tlen of path: " << stringMessage.length() << endl;
        cout << "\tMessage: " << filePath << endl;
        int statusSend = send(new_socket, (char *)filePath.c_str(), filePath.length(), 0);
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

    if (typeOfMessage == e_msg_type_rm)
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

        stringMessage = getString(msgFromClient, recmsg.length_of_data);

        string message = removeDirOrFile(stringMessage);
        cout << "\tCommand: " << stringMessage << endl;
        cout << "\tlen of path: " << message.length() << endl;
        cout << "\tMessage: " << message << endl;
        int statusSend = send(new_socket, (char *)message.c_str(), message.length(), 0);
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

    if (typeOfMessage == e_msg_type_get)
    {
        downloadFile(new_socket, recmsg, buff);
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
