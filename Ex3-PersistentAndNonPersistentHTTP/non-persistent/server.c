#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#define MAX 500
#define PORT 8080
#define SA struct sockaddr
struct sockaddr_in servaddr, cli;

int request_handler(int sockfd){
        char buff[MAX];
        char object[MAX]="";
        int n,i,j,connfd;
        char choice;
        while(1){
                if (( listen(sockfd, 5)) != 0){
                        printf("Listen Failed \n");
                        exit(0);
                }
                printf("Listen Success!\n");
                int len = sizeof(cli);
                connfd = accept(sockfd, (SA*)&cli, &len);
                if (connfd < 0){
                        printf("Server accept failed.\n");
                        exit(0);
                }
                printf("Accept Success!\n\n");

                printf("************************************\n\n");
                bzero(buff, sizeof(buff));
                if ( read(connfd, &buff, sizeof(buff))){
                        printf("Request: %s\n",buff);
                        int len = strlen(buff);
                        for(i=4,j=0; i<(len-1); i++,j++){
                                object[j] = buff[i];
                        }

                        FILE *fd;
                        char filename[MAX]="../Objects/";
                        char message[MAX], content[MAX]="";
                        strcat(content, "REQUEST TIME: ");
                        time_t rawtime;
                        struct tm * timeinfo;
                        time (&rawtime);
                        timeinfo = localtime(&rawtime);
                        strcat(content, asctime(timeinfo));
                        strcat(content, "Content: \n");
                        strcat(filename,object);
                        fd = fopen(filename,"r+");

                        if (fd == NULL){
                                printf("Invalid object\n");
                        return 1;
                        }


                while ( fgets(message,MAX,fd) != NULL ){
                        strcat(content,message);
                }
                fclose(fd);
                printf("Requested Object: \n\n%s\n",content);
                printf("***********************************\n\n");
                bzero(buff,sizeof(buff));
                write(connfd, content, sizeof(content));
        }
        }
        return 0;
}

int main(){
        int sockfd, connfd, len;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
                printf("Error in socket creating.");
                exit(0);
        }
        printf("Successfully created\n");
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);
        if (( bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0){
                printf("Socket binding failed.\n");
                exit(0);
        }
        printf("Binding Success!\n");
        request_handler(sockfd);
        close(sockfd);
        printf("\nConnection Closed.\n");
}

