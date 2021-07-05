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
            cout << "file: " << ent->d_name << " ";
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
        return string("File deleted successfully\n");
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

struct message_ls
{
    // struct message_header header;
    // // The data here
    string msgToServer;
    int lenOfMessage;
};

int main(int argc, char const *argv[])
{

    int server_fd, new_socket, read_Val;
    struct sockaddr_in address; // address for connecting
    int opt = 1;
    int addrlen = sizeof(address);
    int PORT;
    message_ls *msgFromClient;

    // Communicating from Server and Client variables;
    char *commu;
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
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening....\n");
    char *buff = new char[1024];
    string buffer = "";
    read_Val = read(new_socket, buff, 1024);

    //read_Val = read(new_socket, buff, 1024);
    if (read_Val <= 0)
    {
        perror("Error: could not read fom client!\n");
        exit(EXIT_FAILURE);
    }
    msgFromClient = new message_ls[sizeof(buff)];
    memcpy(msgFromClient, (message_ls *)buff, sizeof(buff));
    cout << "From Client:\n";
    cout << "\tMessage: " << msgFromClient->msgToServer << endl;
    cout << "\tlengthOfData: " << msgFromClient->lenOfMessage << endl;
    cout << "\tAdrress buff " << &buff << endl;
    cout << "\tValue buff: " << buff << endl;
    const char *receive = "Already Received!";
    send(new_socket, receive, strlen(receive), 0);

  

    close(new_socket);
    close(server_fd);

    return 0;
}
