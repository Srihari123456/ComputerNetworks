#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
int main(){
        int root_socket, nBytes;
        struct sockaddr_in server_addr;
        socklen_t addr_size;
        root_socket = socket(PF_INET, SOCK_DGRAM, 0);
        if (root_socket < 0){
                printf("ERROR SOCKET CREATION");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(9000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
        addr_size = sizeof server_addr;
        int tld_socket;
        struct sockaddr_in tld_addr;
        struct sockaddr_storage serverStorage;
        socklen_t tld_addr_size;
        tld_socket = socket(PF_INET, SOCK_DGRAM, 0);
        if (tld_socket < 0){
                printf("SOCKET ERROR");
        }
        tld_addr.sin_family = AF_INET;
        tld_addr.sin_port = htons(7000);
        tld_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        tld_addr_size = sizeof tld_addr;
        char url[512];
        char buffer[1024];
        char from_root[512], status_from_root[100], response_from_root[512], from_tld[512];
        int i, index;
        while(1){
                bzero(url, 512);
                bzero(buffer, 1024);
                bzero(from_root, 512);
                bzero(response_from_root, 512);
                bzero(status_from_root, 100);
                bzero(from_tld, 512);
                i = 0;
                index = 0;
                printf("\nEnter url: (exit to quit)\n");
                scanf("%s", url);
                if (strcmp(url, "exit") == 0){
                        printf("\nQuitting Program\n");
                        sendto(root_socket, "exit", sizeof("exit"), 0, (struct sockaddr *)&server_addr, addr_size);
                        close(root_socket);
                        sendto(tld_socket, "exit", sizeof("exit"), 0, (struct sockaddr *)&tld_addr, tld_addr_size);
                        close(tld_socket);
                        exit(0);
                }
                printf("\nSending URL %s to Root Server\n", url);
                if (sendto(root_socket, url, sizeof(url), 0, (struct sockaddr *)&server_addr, addr_size) < 0){
                        printf("ERROR\n");
                }
                if (recvfrom(root_socket, from_root, 512, 0, NULL, NULL) < 0){
                        printf("ERROR\n");
                }
                printf("Response Received from Root Server: %s\n", from_root);
                index = 0;
                for (i = 0; from_root[i] != '$'; i++){
                        status_from_root[index++] = from_root[i];
                }
                status_from_root[index] = '\0';
                i++;
                index = 0;
                for (; from_root[i] != '\0'; i++){
                        response_from_root[index++] = from_root[i];
                }
                response_from_root[index] = '\0';
                if (strcmp(status_from_root, "found") == 0){
                        strcat(response_from_root, "$");
                        strcat(response_from_root, url);
                        printf("\nSending domain file name and URL %s to TLD Server\n", response_from_root);
                        if (sendto(tld_socket, response_from_root, sizeof(response_from_root), 0, (struct sockaddr *)&tld_addr, tld_addr_size) < 0){
                                printf("ERROR IN SENDING\n");
                        }
                        if (recvfrom(tld_socket, from_tld, 512, 0, NULL, NULL) < 0){
                                printf("ERROR IN RECEIVING\n");
                        }
                        printf("Response Received from TLD Server: %s\n", from_tld);
                }
                printf("\n***********************\n");
        }
        close(root_socket);
        return 0;
}
