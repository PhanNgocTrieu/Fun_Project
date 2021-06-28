#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <vector>

using namespace std;
// #define PORT 8080
#define MAX 1024
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
    // The data here
};
struct message_del
{
    struct message_header header;
    // The data here
};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/*
Socket:

socket -> bind -> listen -> accept -> received/send -> close

*/

char *listFilesInDir(char *const fileName)
{
    return fileName;
}

int main(int argc, char const *argv[])
{
    int server_fd, client_fd, readFromClient;
    struct sockaddr_in address;
    int PORT;
    int addrlen = sizeof(address);
    char readClientMessage[MAX] = {0};
    const char *Return = "exit";
    char *sendingToClient;

    if (argc < 2)
    {
        error("Error: No port was provided!");
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // take Port is second input
    PORT = atoi(argv[1]);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Error: bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Error: listen failed");
        exit(EXIT_FAILURE);
    }
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Error: accept failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        memset(readClientMessage, 0, sizeof(readClientMessage));
        readFromClient = read(client_fd, readClientMessage, 1024);
        printf("Message from Client: %s\n", readClientMessage);

        if (strncmp(readClientMessage, "exit", 4) == 0)
        {
            send(client_fd, Return, strlen(Return), 0);
            printf("Server exit....\n");
            break;
        }

        if (strncmp(readClientMessage, "ls", 2) == 0)
        {
            printf("Starting list all files in dir!\n");
            sendingToClient = new char[MAX];
            DIR *direct;
            struct dirent *end;
            if (direct)
            {
                direct = opendir(".");
                while ((end = readdir(direct)) != nullptr)
                {
                    strcat(sendingToClient, end->d_name);
                    // strcat(fromServertoClient, end->d_name);
                    strcat(sendingToClient, "\n");
                }
                closedir(direct);
            }
            printf("%s\n", sendingToClient);
            send(client_fd, sendingToClient, strlen(sendingToClient), 0);
            continue;
        }

        memset(sendingToClient, 0, sizeof(sendingToClient));
        sendingToClient = (char *)"Server responds\n";
        send(client_fd, sendingToClient, strlen(sendingToClient), 0);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}