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
        int server_socket;
        struct sockaddr_in server_addr;
        struct sockaddr_storage clientStorage;
        socklen_t client_addr_size;
        server_socket = socket(PF_INET, SOCK_DGRAM, 0);
        if (server_socket < 0){
                printf("SOCKET ERROR");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(9000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
                printf("BIND ERROR\n");
        }
        client_addr_size = sizeof clientStorage;
        char from_client[512], send_tld[512], from_tld[512], send_client[512];
        char domain[10], domain_without_newline[512];
        FILE *fptr;
        char f_buffer[512];
        bool flag;
        while (1){
                bzero(from_client, 512);
                bzero(send_client, 512);
                bzero(domain_without_newline, 512);
                bzero(domain, 10);
                bzero(f_buffer, 512);
                bzero(from_tld, 512);
                bzero(send_tld, 512);
                int index = 0;
                fptr = NULL;
                flag = false;
                recvfrom(server_socket, from_client, 512, 0, (struct sockaddr *)&clientStorage, &client_addr_size);
                printf("\nURL received from Client: %s\n", from_client);
                if (strcmp(from_client, "exit") == 0)
                {
                        printf("Quitting Root Server\n");
                        close(server_socket);
                        exit(0);
                }
                index = 0;
                int i = 0;
                for (i = strlen(from_client); from_client[i] != '.'; i--);
                i++;
                for (; from_client[i] != '\0'; i++){
                        domain[index++] = from_client[i];
                }
                strcpy(domain_without_newline, domain);
                strcat(domain_without_newline, "\0");
                domain[index++] = '\n';
                domain[index] = '\0';
                fptr = fopen("domains.txt", "r");
                while (fgets(f_buffer, 512, fptr) != NULL)
                {
                        if (strcmp(domain, f_buffer) == 0)
                        {
                                flag = true;
                                strcat(send_client, "found");
                                strcat(send_client, "$");
                                strcat(send_client, domain_without_newline);
                        }
                }
                if (flag == false){
                        strcat(send_client, "notfound$404 Page Not Found");
                }
                printf("RESPONSE SENT TO CLIENT: %s\n", send_client);
                sendto(server_socket, send_client, strlen(send_client), 0, (struct sockaddr *)&clientStorage, client_addr_size);
                printf("\n***********************\n");
        }
        close(server_socket);
        return 0;
}
