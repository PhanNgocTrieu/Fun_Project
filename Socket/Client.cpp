// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAX 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    int PORT;
    struct sockaddr_in serv_addr;
    //const char *sendingServer = "sendingServer from client";
    char *sendingToServer;
    char *readFromServer;

    if (argc < 3)
    {
        error("No port to link!");
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    PORT = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // handling send and receiv
    while (1)
    {
        int n = 0;
        sendingToServer = new char[MAX];
        bzero(sendingToServer, sizeof(sendingToServer));

        //Enter string
        printf(">: ");
        while (((sendingToServer[n++] = getchar()) != '\n'));

        send(sock, sendingToServer, strlen(sendingToServer), 0);

        readFromServer = new char[MAX];
        valread = read(sock, readFromServer, MAX);
        if (strncmp(readFromServer, "exit", 4) == 0)
        {
            printf("Client exit....\n");
            break;
        }

        if (strncmp(sendingToServer, "ls", 2) == 0)
        {
            char *pch = strtok(readFromServer, "\n");
            while (pch != nullptr)
            {
                printf("%s\n", pch);
                pch = strtok(nullptr, "\n");
            }
            delete pch;
            pch = nullptr;
            continue;
        }

        valread = read(sock, readFromServer, MAX);
        printf("%s\n", readFromServer);
    }

    if (readFromServer)
    {
        delete readFromServer;
        readFromServer = nullptr;
    }
    if (sendingToServer)
    {
        delete sendingToServer;
        sendingToServer = nullptr;
    }
    return 0;
}