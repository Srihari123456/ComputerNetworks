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
                bzero(buff,MAX);
                read(sockfd,buff,sizeof(buff));
                printf("\nClient: %s>>> ",buff);
                bzero(buff,MAX);
                n = 0;

                while( (buff[n++] = getchar()) != '\n');
                write(sockfd,buff,sizeof(buff));

//              bzero(buff,sizeof(buff));
//              read(sockfd,buff,sizeof(buff));

//              printf("Server: %s\n",buff);
                if( strncmp("exit",buff,4) == 0){
                        printf("~~~Server exits~~~\n");
                        break;
                }
        }
}
int main(){
        int sockfd,connfd,len;
        struct sockaddr_in servaddr, cli;

        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0){
                printf("Socket creation failed.\n");
                exit(0);
        }

        printf("Creation Success!");
        bzero(&servaddr,sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);

        if( bind(sockfd,(SA*)&servaddr, sizeof(servaddr)) != 0){
                printf("Socket binding failed!\n");
                exit(0);
        }
        printf("Binding success!\n");

        if( listen(sockfd,5) != 0){
                printf("Listen failed!");
                exit(0);
        }
        printf("Listen success!\n");
        len = sizeof(cli);

        connfd = accept(sockfd, (SA*)&cli, &len);
        if(connfd <0){
                printf("Server Accept failed\n");
                exit(0);
        }
        else{
                printf("Access Success!\n");
        }
        chat(connfd);
        close(sockfd);
}

