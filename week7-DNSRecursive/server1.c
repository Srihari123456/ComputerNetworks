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
        int sockfd,sockfd2,n,len;
        char send_buff[size],recv_buff[size];
        char *hello = "Hello from server";
        struct sockaddr_in servaddr,servaddr2,cliaddr;
        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        if (sockfd<0){
                printf("Socket Creation failed\n");
                return 1;
        }
        sockfd2 = socket(AF_INET,SOCK_DGRAM,0);
        if (sockfd2<0)
        {
                printf("Socket 2 Creation failed\n");
                return 1;
        }
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(9000);
        servaddr2.sin_family = AF_INET;
        servaddr2.sin_addr.s_addr = INADDR_ANY;
        servaddr2.sin_port = htons(9500);
        if( bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
                printf("Binding error\n");
                return 1;
        }
        len = sizeof(cliaddr);
        while(1){
                n = recvfrom(sockfd,recv_buff,size,0,(struct sockaddr*)&cliaddr,&len);
                recv_buff[n] = '\0';
                printf("\nClient: %s\n", recv_buff);
                char filename[20];
                int j=0;
                for(int i=strlen(recv_buff)-3;i<strlen(recv_buff);i++,j++)
                        filename[j] = recv_buff[i];
                filename[j] = '\0';
                strcat(filename,"_domain.txt");
                bzero(send_buff,size);
                FILE* fp = fopen(filename,"r");
                while(fgets(send_buff+strlen(send_buff),size,fp));
                fclose(fp);
                sendto(sockfd2,recv_buff,strlen(recv_buff),0,(struct sockaddr*)&servaddr2,sizeof(servaddr2));
                sendto(sockfd2,send_buff,strlen(send_buff),0,(struct sockaddr*)&servaddr2,sizeof(servaddr2));
                n=recvfrom(sockfd2,recv_buff,size,0,(struct sockaddr*)&servaddr2,&len);
                recv_buff[n] = '\0';
                printf("Server 2: %s\n",recv_buff);
                sendto(sockfd,recv_buff,strlen(recv_buff),0,(struct sockaddr*)&cliaddr,len);
        }
        return 0;
}
