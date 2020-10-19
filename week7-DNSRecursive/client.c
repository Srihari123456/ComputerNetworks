CLIENT.C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define size 1024
int main() {
        int sockfd,n,len;
        char buffer[size];
        char *hello = "hello from the client";
        struct sockaddr_in servaddr;
        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        if (sockfd<0)
        {
                printf("Socket creation failed\n");
                return 1;
        }
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(9000);
        servaddr.sin_addr.s_addr = INADDR_ANY;
        while(1){
                printf("\nEnter host name: ");
                scanf("%s", buffer);
                if(strcmp(buffer,"exit") == 0)break;
                sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                n=recvfrom(sockfd,buffer,size,0,(struct sockaddr*)&servaddr,&len);
                buffer[n] = '\0';
                printf("IP address: %s\n",buffer);
        }
        close(sockfd);
        return 0;
}

