



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

    // struct iphdr buffer;
    unsigned char buffer[65535] = {0};

    // struct ipv4_header buffer;

    while (1) {
        ssize_t msg_size = recvfrom(accepted_fd, 
                                &buffer, sizeof(buffer), 
                                0, // flags
                                (struct sockaddr*)&client_addr, 
                                &client_addrlen);

		printf("[server] received packet from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        printf("%s\n", buffer);

        char *http_header = strtok(buffer, "\r\n");

        printf("http header: %s\n", http_header);
    
        // struct iphdr header = {0};

        // struct ipv4_header* header = (struct ipv4_header*)&buffer; 
        // printhex(buffer, 128);     


        // printf("[server] packet length: %d\n", test->version);
        
    }
    
    

    // int received = recv()


    return 0;
}