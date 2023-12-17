#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4
#define PORT 12345

void send_frame(int frame_number, int sockfd) {
    // Simulated function to send a frame
    printf("Sending frame: %d\n", frame_number);
    write(sockfd, &frame_number, sizeof(frame_number));
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    int frame_number = 0;
    int base = 0;
    int next_seq_num = 0;

    while (1) {
        // Send frames within the window
        while (next_seq_num < base + WINDOW_SIZE && next_seq_num < 10) {
            send_frame(next_seq_num, sockfd);
            next_seq_num++;
        }

        // Simulated delay (e.g., waiting for acknowledgments)
        sleep(2);

        // Receive acknowledgments
        int ack_frame;
        while (read(sockfd, &ack_frame, sizeof(ack_frame)) != -1) {
            printf("Received ACK for frame: %d\n", ack_frame);

            if (ack_frame >= base) {
                base = ack_frame + 1;
                printf("Updated base to: %d\n", base);
            }
        }
    }

    close(sockfd);
    return 0;
}