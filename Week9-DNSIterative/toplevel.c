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
struct counter
{
        int edu;
        int in;
        int com;
}counter;
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
        server_addr.sin_port = htons(7000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
                printf("BIND ERROR\n");
        }
        client_addr_size = sizeof clientStorage;
        char from_root[512], send_root[512];
        char domain[10];
        char filename[512], url[512], full_dir[512];
        char domain_filename[512], fbuffer[512], url_token[512], ip_token[512];
        char *pure_domain;
        FILE *fptr, *fptr2;
        bool flag;
        counter.com = 0;
        counter.in = 0;
        counter.edu = 0;
        while (1){
                bzero(from_root, 512);
                bzero(send_root, 512);
                bzero(filename, 512);
                bzero(fbuffer, 512);
                bzero(full_dir, 512);
                bzero(domain, 10);
                bzero(domain_filename, 512);
                bzero(url_token, 512);
                bzero(ip_token, 512);
                pure_domain = NULL;
                bzero(url, 512);
                int index = 0;
                int i = 0;
                fptr = NULL;
                fptr2 = NULL;
                flag = false;
                int specific_counter;
                if (recvfrom(server_socket, from_root, 512, 0, (struct sockaddr *)&clientStorage, &client_addr_size) < 0){
                        printf("ERROR\n");
                }
                if (strcmp(from_root, "exit") == 0){
                        printf("Quitting from TLD Server\n");
                        close(server_socket);
                        return (0);
                }
                index = 0;
                for (i = 0; from_root[i] != '$'; i++){
                        domain[index++] = from_root[i];
                }
                domain[index] = '\0';
                i++;
                index = 0;
                for (; from_root[i] != '\0'; i++){
                        url[index++] = from_root[i];
                }
                url[index] = '\0';
                printf("Request received from Client\nDomain: %s\nURL: %s\n", domain, url);
                strcat(filename, domain);
                strcat(filename, ".txt");
                fptr = fopen(filename, "r");
                if (fptr == NULL){
                        printf("\nResponse sent to Client: Page Not Found\n");
                        sendto(server_socket, "Page Not Found", strlen("Page Not Found"), 0, (struct sockaddr *)&clientStorage, sizeof(clientStorage));
                }
                else{
                        if (strcmp(domain, "edu") == 0){
                                specific_counter = (counter.edu++) % 2;
                        }
                        else if (strcmp(domain, "com") == 0){
                                specific_counter = (counter.com++) % 2;
                        }
                        else if (strcmp(domain, "in") == 0){
                                specific_counter = (counter.in++) % 2;
                        }
                        int count = 0;
                        while (fgets(domain_filename, 512, fptr) != NULL){
                                if (specific_counter == count){
                                        break;
                                }
                        }
                        printf("Parsing contents from File: %s\n", domain_filename);
                        for (i = 0; domain_filename[i] != '\n'; i++);
                        domain_filename[i] = '\0';
                        strcat(full_dir, domain);
                        strcat(full_dir, "/");
                        strcat(full_dir, domain_filename);
                        fptr2 = fopen(full_dir, "r");
                        if (fptr2 == NULL){
                                printf("Response sent to Client: Page Not Found\n");
                                sendto(server_socket, "Page Not Found", strlen("Page Not Found"), 0, (struct sockaddr *)&clientStorage, sizeof(clientStorage));
                        }
                        while (fgets(fbuffer, 512, fptr2) != NULL){
                                index = 0;
                                for (i = 0; fbuffer[i] != '$'; i++){
                                        url_token[index++] = fbuffer[i];
                                }
                                url_token[index] = '\0';
                                index = 0;
                                i++;
                                for (; fbuffer[i] != '\0'; i++){
                                        ip_token[index++] = fbuffer[i];
                                }
                                ip_token[index] = '\0';
                                if(strcmp(url_token, url) == 0){
                                        flag = true;
                                        printf("Response sent to Client\nURL: %s\nIP: %s\n", url_token, ip_token);
                                        sendto(server_socket, ip_token, strlen(ip_token), 0, (struct sockaddr *)&clientStorage, sizeof(clientStorage));
                                }
                        }
                        if(flag == false){
                                printf("Response sent to Client: Page Not Found\n");
                                sendto(server_socket, "Page Not Found", strlen("Page Not Found"), 0, (struct sockaddr *)&clientStorage, sizeof(clientStorage));
                        }
                }
                printf("\n***********************\n");
        }
        close(server_socket);
        fclose(fptr);
        fclose(fptr2);
        return 0;
}
