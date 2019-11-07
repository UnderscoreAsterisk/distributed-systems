#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>

#define SERVER_PORT 4200
#define CLIENT_PORT 4300

#define CHECK_SUCCESS(val, expected, msg) if(val<expected) {perror(msg); return -1;}
#define REQ_STRING "time?"

int main() {
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_SUCCESS(sd, 0, "Failed to create socket.\n")

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = SERVER_PORT;

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = CLIENT_PORT;

    int rv = bind(sd, (struct sockaddr*)(&client_address), sizeof(client_address));
    CHECK_SUCCESS(rv, 0, "Failed to bind address to the socket.\n")

    char request[] = REQ_STRING;
    size_t req_len = strlen(REQ_STRING)+1;
    size_t bytes = 0;

    int bytes_sent = sendto(sd, &request, req_len, 0, (struct sockaddr*)(&server_address), sizeof(server_address));
    CHECK_SUCCESS(bytes_sent, req_len, "Failed to send request.\n");

    char response[50] = {0};
    int bytes_recv = recv(sd, response, 50, MSG_WAITALL);
    CHECK_SUCCESS(bytes_recv, 1, "Bad read.\n");

    printf("Server response: %s", response);

}