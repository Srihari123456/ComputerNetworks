#include<stdio.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd){
        char buff[MAX];
        int n;

        while(1){
                bzero(buff,sizeof(buff));
                printf(">>> ");
                n =0;
                while((buff[n++] = getchar() ) != '\n');
                write(sockfd,buff,sizeof(buff));

                bzero(buff,sizeof(buff));
                read(sockfd,buff,sizeof(buff));

                printf("Server: %s\n",buff);

                if((strncmp(buff,"exit",4)) == 0){
                        printf("~Client Exits~\n");
                        break;
                }


        }
}

int main(){
        int sockfd,connfd,len;
        struct sockaddr_in servaddr,cli;

        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0){
                printf("Socket creation failed!");
                exit(0);
        }
        printf("Creation Success!");
        bzero(&servaddr,sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);

        if( connect(sockfd,(SA*)&servaddr, sizeof(servaddr)) != 0){
                        printf("Server connection failed!");
                        exit(0);
        }
        printf("Connection success!\n");
        chat(sockfd);
        close(sockfd);
}
