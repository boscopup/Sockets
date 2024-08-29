#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <cstring>

using namespace std;

int main() {
    // Create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket
    if (bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
        cout << "Error binding socket: " << strerror(errno) << endl;
        shutdown(serverSocket, SHUT_RDWR);
        return 0;
    } else {
        cout << "Bind sucessful!" << endl;
    }

    // Listen for connections
    if (listen(serverSocket, 1) == -1) {
        cout << "Error listening on socket: " << strerror(errno) << endl;
    } else {
        cout << "Listening successful!" << endl;
    }

    // Accept client connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        cout << "Error accepting socket: " << strerror(errno) << endl;
    }

    // Receive data from client
    char buffer[1024] = {0};
    int byteCount = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (byteCount < 0) {
        cout << "Server error receiving data: " << strerror(errno) << endl;
        return 0;
    } else {
        cout << "Message from client: " << buffer << endl;
    }

    memset(buffer, 0, sizeof(buffer));

    printf("Enter your message: ");
    cin.getline(buffer, 1024);
    byteCount = send(clientSocket, buffer, 1024, 0);
    if (byteCount == -1) {
        cout << "Server error sending message: " << strerror(errno) << endl;
    } else {
        cout << "Server: Successfully sent " << byteCount << " bytes of data." << endl;
    }


    
    shutdown(serverSocket, SHUT_RDWR);
    return 0;
}