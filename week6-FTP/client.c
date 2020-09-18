#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>
int main(){
        int sockfd,n;
        struct sockaddr_in serv_addr;
        char username[2048],password[2048];
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0){
                printf("\n Error opening socket");
                return 0;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(8002);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
                printf("\n Error in connecting");
                return 0;
        }
        char result[2048];
        char sendBuff[2048];
        while(1){
                bzero(sendBuff,sizeof(sendBuff));
                printf("Enter the username(exit to leave):");
                scanf("%s",username);
                username[strlen(username)] ='\0';
                int i = strncmp(username,"exit",4);
                if(i==0){
                        write(sockfd,username,sizeof(username));
                        break;
                }
                printf("Enter the password:");
                scanf("%s",password);
                password[strlen(password)] ='\0';
                strcat(sendBuff,username);
                strcat(sendBuff,":");
                strcat(sendBuff,password);
                strcat(sendBuff,";");
                puts(sendBuff);
                write(sockfd, sendBuff, sizeof(sendBuff));
                read(sockfd,result,sizeof(result));
                char o[10];
                if(strcmp(result,"True")==0){
                        int sockfd1, n;
                        struct sockaddr_in serv_addr1;
                        sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
                        if(sockfd1 < 0){
                                printf("\n Error opening socket");
                                return 0;
                        }
                        char fname[100],message[10000];
                        serv_addr1.sin_family = AF_INET;
                        serv_addr1.sin_port = htons(8004);
                        serv_addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
                        if(connect(sockfd1,(struct sockaddr*)&serv_addr1,sizeof(serv_addr1)) < 0){
                                printf("\n Error in connecting\n");
                                return 0;
                        }

                        char resp[100];
                        while(1){
                                bzero(fname,sizeof(fname));
                                bzero(message,sizeof(message));
                                printf("Enter the filename(quit to stop) :");
                                scanf("%s",fname);
                                write(sockfd1,fname,sizeof(fname));
                                if(strcmp(fname,"quit")==0) break;
                                printf("The file requested is %s\n",fname);
                                bzero(resp,sizeof(resp));
                                read(sockfd1,resp,sizeof(resp));

                        if(strcmp(resp,"yes")==0){
                                        write(sockfd1,"Y",sizeof("Y"));
                                        read(sockfd1,message,sizeof(message));
                                        printf("Contents of the file are :\n %s",message);

                        }
                        if(strcmp(resp,"no")==0){
                                printf("File Not found \n");
                        }
                }
                close(sockfd1);
        }
        else if(strcmp(result,"False")==0){
                printf("Password incorrect \n");
        }
        else{
                printf("Error\n");
        }
}
        close(sockfd);
        return 0;
}