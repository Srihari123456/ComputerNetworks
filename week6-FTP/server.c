#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
int Search(char *fname, char *str){
        FILE *fp;
        int find_result = 0;
        char ch;
        char temp[512],message[2048];
        if((fp = fopen(fname, "r")) == NULL){
                printf("Error in search\n");
                return(-1);
        }
        strcat(str,"\n");
        strcpy(message, "\0");
        while(fgets(message,1000,fp)!= NULL){
                int x = strcmp(message,str);
                if(x == 0){
                        find_result++;
                        return 1;
                }
        }
        fclose(fp);
                return 0;
}
int main()
{
        int listenfd = 0, connfd = 0;
        struct sockaddr_in serv_addr;
        char username[2048],password[2048];
        char readBuff[2048];
        if((listenfd = socket(AF_INET, SOCK_STREAM, 0))< 0){
                printf("\n Socket not created\n");
                return 1;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(8002);
        if((bind(listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)))>0){
                printf("\n Not binded \n");
                return 1;
        }
        if(listen(listenfd, 10) == -1){
                printf("Failed to listen\n");
                return -1;
        }
        connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
        int listenfd1 = 0, connfd1 = 0;
        struct sockaddr_in serv_addr1;
        if((listenfd1 = socket(AF_INET, SOCK_STREAM, 0))< 0){
                printf("\n Socket not created\n");
                return 1;
        }
        serv_addr1.sin_family = AF_INET;
        serv_addr1.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr1.sin_port = htons(8004);
        if((bind(listenfd1, (struct sockaddr*)&serv_addr1,sizeof(serv_addr1)))>0){
                printf("\n Not binded \n");
                return 1;
        }
        if(listen(listenfd1, 10) == -1){
                printf("Failed to listen\n");
                return -1;
        }
        while(1){
                bzero(readBuff,sizeof(readBuff));
                read(connfd,readBuff,sizeof(readBuff));
                puts("Read from client\n");
                int i = strncmp(readBuff,"exit",4);
                if(i==0){
                        close(listenfd);
                        break;
                }
                int res = Search("database.txt",readBuff);
                if(res==1){
                        printf("Authenticated\n");
                        char message[1000],sent[10000],fname[1000];
                        write(connfd,"True",sizeof("True"));
                        FILE *fp;
                        char o[10];
                        connfd1 = accept(listenfd1,(struct sockaddr*)NULL,NULL);
                        while(1){
                                read(connfd1,fname,sizeof(fname));
                                if(strcmp(fname,"quit")==0) break;
                                if(fp = fopen(fname,"r")){
                                        write(connfd1,"yes",sizeof("yes"));
                                        bzero(sent,sizeof(sent));
                                        bzero(o,sizeof(o));
                                        read(connfd1,o,sizeof(o));
                                        if(strcmp(o,"Y")==0){
                                                while(fgets(message,1000,fp)!=NULL)
                                                        strcat(sent,message);
                                                puts(sent);
                                                write(connfd1,sent,sizeof(sent));
                                        }
                                }
                                else{
                                        write(connfd1,"no",sizeof("no"));
                                }
                        }
                }
                else{
                        printf("Authentication Failed \n");
                        write(connfd,"False",sizeof("False"));
                }
        }
}