#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SOCK_PORT 4200

#define CHECK_SUCCESS(val, expected, msg) if(val<expected) {perror(msg); return -1;}
#define REQ_STRING "time?"

int main() {
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_SUCCESS(sd, 0, "Failed to create socket.\n")

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = SOCK_PORT;

    int rv = bind(sd, (struct sockaddr*)(&server_address), sizeof(server_address));
    CHECK_SUCCESS(rv, 0, "Failed to bind address to the socket.\n")

    struct sockaddr client_address;
    socklen_t client_len;
    char request[6];
    size_t req_len = strlen(REQ_STRING)+1;
    size_t bytes = 0;

    // block till a request arrives
    while(bytes = recvfrom(sd, request, req_len, MSG_WAITALL, &client_address, &client_len)) {
        CHECK_SUCCESS(bytes, req_len, "Bad read.\n");
        if(strncmp(request, REQ_STRING, req_len) == 0) {

            time_t current_time = time(NULL);
            char* response = ctime(&current_time);
            size_t response_len = strlen(response)+1;
            printf("Request received! responding with: %s\n", response);
            size_t bytes_sent = sendto(sd, (void*)response, response_len, 0, &client_address, client_len);
            CHECK_SUCCESS(bytes_sent, response_len, "Failed to send response.\n");

        } else perror("Bad request.\n");
    }

    return 0;
}