



#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <math.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "http/request.h"

#define PORT_NO 8080
#define HTTP_SIZE 65535
#define URL_SIZE 1024

void print_ip_address(uint32_t addr) {
    printf("%d.%d.%d.%d", (addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF);
}

void printhex(unsigned char *buf, size_t len) {
    // unsigned char *ch = (unsigned char *)&buf;

	int num_hex_digits = ceil((log(len) / log(2)) / 4.0);
    char hex_addr_format[32] = {0};
	sprintf(hex_addr_format, "0x%%0%dX: ", num_hex_digits);
	

    int line_idx = 0;
    for (int i=0; i<len; i++) {
        if (line_idx == 0) {
            printf(hex_addr_format, i);
        }

        printf("%02X ", buf[i]);
        line_idx++;

        if (line_idx == 16) {
            line_idx = 0;
            printf("\n");
        }
    }
    printf("\n");
}

char two_hex_chars_to_char(char h1, char h0) {
    char out = 0;

    // h1 is alpha
    if (h1 > '9') {
        if (h1 > 'F') {

        }
        out += (h1 - 55) * 16;
    }

    // h1 is numeric
    else {
        out += (h1 - 48) * 16;
    }

    // h0 is alpha
    if (h0 > '9') {
        out += (h0 - 55);
    }

    // h0 is numeric
    else {
        out += (h0 - 48);
    }

    return out;
}

void url_path(char *buf, char *str, int len) {
    // char buf[URL_SIZE] = {0};
    char pchar = 0;

    int idx = 0;

    for (int i=0; i<len; i++) {

        // path delimiter
        if (str[i] == '/' && pchar != '/') {
            buf[idx++] = str[i];
            pchar = str[i];
        }

        // 0-9 
        else if (str[i] >= '0' && str[i] <= '9') {
            buf[idx++] = str[i];
            pchar = str[i];
        }

        // A-Z
        else if (str[i] >= 'A' && str[i] <= 'Z') {
            buf[idx++] = str[i];
            pchar = str[i];
        }

        // a-z
        else if (str[i] >= 'a' && str[i] <= 'z') {
            buf[idx++] = str[i];
            pchar = str[i];
        }

        // _ - .
        else if (str[i] == '_' || str[i] == '-' || str[i] == '.') {
            buf[idx++] = str[i];
            pchar = str[i];
        } 

        else if (str[i] == '%' && len-i > 2) {
            printf("found control character: %%%c%c\n", str[i+1], str[i+2]);

            char ctrl = two_hex_chars_to_char(str[i+1], str[i+2]);

            buf[idx++] = ctrl;
            pchar = ctrl;
            i += 2;
        }
        
        else if (str[i] == '?') {
            break;
        }
    }

    printf("url: %s\n", buf);
}

char response_test[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nServer: Hello\r\nContent-Length: 28\r\n\r\n<html><h1>Hello!</h1></html>";


int main(void) {
    // printhex((unsigned char*) "hello", 5);
    // return 0;

    // create TCP socket
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;         // AF_INET: sets address scheme to IPv4
	server_addr.sin_port = htons(PORT_NO);    // htons(): converts byte order of port number
	server_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY: binds to address of all available network interfaces
	memset(server_addr.sin_zero, 0, 8);       // using memset() to set sin_zero field to zero

    // https://stackoverflow.com/questions/1592055/bind-address-already-in-use
    int opt = 1;
    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int sock_bind;
	sock_bind = bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); 
	
    if (sock_bind < 0) {
        perror("error: bind():");
        exit(1);
    }

    int lstn = listen(server_socket_fd, 1);
    if (lstn != 0) {
        perror("error: listen():");
        exit(1);
    }

    // define client address structure (place to store received packet's source address)
	struct sockaddr_in client_addr = {0};
	unsigned int client_addrlen = sizeof(client_addr);

    int accepted_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addrlen); 
    if (accepted_fd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    }

    char buffer[HTTP_SIZE] = {0};

    while (1) {
        int msg_size = recvfrom(accepted_fd, 
                                &buffer, sizeof(buffer), 
                                0, // flags
                                (struct sockaddr*)&client_addr, 
                                &client_addrlen);

        printf("[server] received %d B packet from %s:%d\n", msg_size, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));


        if (msg_size > 0) {

            printf("%s\n", buffer);

            char *http_header = strtok(buffer, "\r\n");

            char *request_type = strtok(http_header, " \t");
            char *request_path = strtok(NULL, " \t");
            char *request_http = strtok(NULL, " \t");

            char *relative_path = request_path+1;

            printf("Request type: %s\n", request_type);
            printf("Request path: %s\n", request_path);
            printf("Request ver: %s\n", request_http);

            printf("Requested representation of file %s\n", request_path);

            // sanitize url
            char buf[URL_SIZE] = {0};
            url_path(buf, request_path, strlen(request_path));

            char buffer2[65535];


            printf("%s\n", response_test);

            sendto(accepted_fd, &response_test, sizeof(response_test), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        }
        else {
            sendto(accepted_fd, NULL, 0, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

        }

		

        
    }

    return 0;
}