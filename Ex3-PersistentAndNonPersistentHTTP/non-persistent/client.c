#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 500
#define PORT 8080
#define SA struct sockaddr



int connects();
int response(int sockfd){
        char buff[MAX];
        int n;
        char choice;
        bzero(buff, sizeof(buff));
        printf("Enter HTTP Request: ");
        n = 0;
        while (( buff[n++] = getchar()) != '\n');
        write(sockfd, &buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("\nServer Response: \n\n%s\n",buff);
        printf("**********************************\n\n");
        return 0;
}

int main(){
        int choice;
        while(1){
                printf("Want to send an HTTP Request?:Then enter 1");
                choice = getchar();
                getchar();

                if ( choice != '1' ){
                printf("Exiting.\n");
                break;
                }
                connects();
        }
        return 0;
}

int connects(){
        int sockfd, connfd, len;
        struct sockaddr_in servaddr, cli;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
                printf("Socket creation failed.\n");
                exit(0);
        }
        printf("Creation Success!\n");
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);
        if (( connect(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0){
                printf("Socket connection failed.\n");
                exit(0);
        }
        printf("Connection Success!\n\n");
        printf("***************************************\n\n");
        response(sockfd);
        close(sockfd);
        printf("Bye Connection Closed.\n\n*****************\n\n");
}
