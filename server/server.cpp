#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

const int PORT = 54000;
const int BUFFER_SIZE = 1024;

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Failed to create socket!" << endl;
        return EXIT_FAILURE;
    }
    cout << "Server socket created!" << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cerr << "Failed to bind socket to address!" << endl;
        close(serverSocket);
        return EXIT_FAILURE;
    }
    cout << "Socket has been bound to port " << PORT << endl;

    if (listen(serverSocket, SOMAXCONN) < 0)
    {
        cerr << "Failed to start listening!" << endl;
        close(serverSocket);
        return EXIT_FAILURE;
    }
    cout << "Server is listening for incoming connections..." << endl;

    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);

    while (true)
    {
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket < 0)
        {
            cerr << "Failed to establish a connection with client!" << endl;
            continue;
        }

        cout << "Server is connected with client at IP " << inet_ntoa(clientAddress.sin_addr)
        << " and port " << ntohs(clientAddress.sin_port) << endl;

        char buffer[BUFFER_SIZE] = {0};
        const char* helloMsg = "Привет! Как тебя зовут?";
        send(clientSocket, helloMsg, strlen(helloMsg), 0);
        recv(clientSocket, buffer, BUFFER_SIZE, 0);

        const char* response = "";
        if(strcmp(buffer, "Birdswift\n") == 0)
        {
            response = "классно, как и моего создателя";
        }
        else
        {
            response = "приятно познакомиться";
        }
        send(clientSocket, response, strlen(response), 0);
        close(clientSocket);
    }

    return EXIT_SUCCESS;
}
