#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

void receive_frame(int sockfd) {
    int frame_number;

    // Receive frame
    read(sockfd, &frame_number, sizeof(frame_number));
    printf("Received frame: %d\n", frame_number);

    // Simulated acknowledgment (ACK)
    printf("Sending ACK for frame: %d\n", frame_number);
    write(sockfd, &frame_number, sizeof(frame_number));
}

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    new_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_sock == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Receive frames
        receive_frame(new_sock);
    }

    close(sockfd);
    close(new_sock);
    return 0;
}