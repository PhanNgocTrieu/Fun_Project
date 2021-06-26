/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 80

void func(int sockfd)
{
    char buffer[MAX];
    int len = 0;

    while (1)
    {
        bzero(buffer, MAX);
        read(sockfd, buffer, sizeof(buffer));
        printf("From Client: %s\t To client: ", buffer);
        while ((buffer[len++] = getchar()) != '\n')
            ;

        write(sockfd, buffer, sizeof(buffer));

        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serverSocketChecking;
    int PORT;
    int opt = 1;
    int clientAddr;
    struct sockaddr_in serverAddr;
    socklen_t clientAddrLength;
    char *buffer;
    // argc[1] is port
    if (argc < 2)
    {
        fprintf(stderr, "Error, no port provided\n");
    }
    serverSocketChecking = socket(AF_INET, SOCK_STREAM, 0);
    if ( serverSocketChecking == -1)
    {
        error("Error: Opening Socket!");
    }
    else
    {
        printf("Socket successfully created....\n");
    }

    bzero((sockaddr *)&serverAddr, sizeof(serverAddr));
    // convert string to int
    PORT = atoi(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htons(PORT);
    serverAddr.sin_port = INADDR_ANY;

    // bind socket
    int bindCheck = bind(serverSocketChecking, (sockaddr *)&serverAddr, sizeof(serverAddr));
    
    if ( bindCheck != 0)
    {
        error("Error: Binding Socket");
    }
    else
    {
        printf("Binding successful!\n");
    }

    // Set up Listen to Client
    if (listen(serverSocketChecking, 10) != 0)
    {
        error("Listen  failed\n");
    }
    else
    {
        printf("Server listening...\n");
    }

    clientAddrLength = sizeof(clientAddr);
    int newsockfd = accept(serverSocketChecking, (struct sockaddr *)&clientAddr, &clientAddrLength);
    if (newsockfd < 0)
    {
        error("Server accept failed...\n");
    }
    else
    {
        printf("Server accept the client...\n");
    }

    bzero(buffer, 256);
    int checkRead = read(   newsockfd, buffer, 255);
    if (checkRead < 0)
        error("ERROR reading from socket");
    printf("Here is the message: %s\n", buffer);

    int checkWrite = write(newsockfd, "I got your message", 18);
    if (checkRead < 0)
        error("ERROR writing to socket");

    func(newsockfd);

    close(newsockfd);
    close(serverSocketChecking);
    return 0;
}
