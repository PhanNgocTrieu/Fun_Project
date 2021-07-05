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
struct message_ls
{
    // struct message_header header;
    // // The data here
    string msgToServer;
    int lenOfMessage;
};
struct message_dwn
{
    struct message_header header;
    // The data here
    char *fileSave;
    char *location;
};

int main(int argc, char *argv[])
{
    int sockfd, portno, readVal;
    int portNum;
    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi to

    message_ls *msgls = new message_ls;
    string readFromSever;
    string command;

    char *data_to_send;
    char *buffer = new char[1024];
    memset(buffer, 0, 1024);

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
    getline(cin, msgls->msgToServer);

    msgls->lenOfMessage = msgls->msgToServer.length();
    cout << "Message to send:\n";
    cout << "\tMessage: " << msgls->msgToServer << endl;
    cout << "\tLen: " << msgls->lenOfMessage << endl;
    int statusSend = send(sockfd, (char *)msgls, sizeof(e_msg_type_ls) + msgls->lenOfMessage, 0);
    if (statusSend < 0)
    {
        perror("Error: Sending failed");
    }
    else
        cout << "Already send!" << endl;
    //read(sockfd,buffer,sizeof(buffer));

    recv(sockfd, buffer, 1024, 0);
    cout << buffer << endl;

    close(sockfd); //Dong socket
    return 0;
}
