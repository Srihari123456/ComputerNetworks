#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
int main(){
        struct sockaddr_in cli;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0){
                printf("Error in creating socket.");
                return 0;
        }
        printf("\nCreation Success\n");
        cli.sin_family = AF_INET;
        cli.sin_port = htons(4000);
        cli.sin_addr.s_addr = inet_addr("127.0.0.1");

        int cli_connect = connect(sockfd, (struct sockaddr *)&cli, sizeof(cli));
        if(cli_connect<0){
                printf("\nServern connection failed\n.");
                return 0;
        }
        printf("\nConnection Success\n");

        while(1){
                char name[40],readmsg[1000],get_header[1000];
                printf("Enter filename: ");
                scanf("%s",name);
                printf("\n");
                strcpy(get_header,"");
                strcat(get_header,"GET: ");
                strcat(get_header,name);
                printf("%s",get_header);
                printf("\n");
                write(sockfd,name,sizeof(name));
                write(sockfd,get_header,sizeof(get_header));
                read(sockfd,readmsg,sizeof(readmsg));
                printf("%s",readmsg);
        }
}
