#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include<math.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
struct calc_var
{
        int op;
        float a;
        float b;
        float c;
};
int calc_server(int connfd)
{
        printf("\n~~~~~~~~~~~~~~~~~~~~\n");
        printf("Welcome to Server Side Calculator!\n");
        while(1)
        {
                struct calc_var output;
                printf("\nReading Client input...\n");
                if ( read(connfd, &output, sizeof(output)) )
                {
                        float a = ntohl(output.a);
                        float b = ntohl(output.b);
                        float c = ntohl(output.c);
                        int op = ntohl(output.op);
                        if (op == 5)
                        {
                                printf("\nExiting Calculator...\n");
                                break;
                        }
                        float ans;
                        switch (op)
                        {
                                case 1:
                                        printf("Operation Multiplication: ");
                                        printf("%f * %f",a,b);
                                        ans = a * b;
                                        break;
                                case 2:
                                        printf("Operation Division: ");
                                        printf("%f / %f",a,b);
                                        ans = (float)a /(float) b;
                                        break;
                                case 3:
                                        printf("Operation Average of three numbers: ");
                                        printf("(%f + %f + %f)/3",a,b,c);
                                        ans = a+b+c;
                                        ans = ans/3;
                                        break;
                                case 4:
                                        printf("Operation: Square root of ");
                                        printf("%f",a);
                                        ans = pow(a,0.5);
                                        break;
                        }
                        printf(" = %.2f\n",ans);
                        write(connfd, &ans, sizeof(ans));
                }
        }
        return 0;
}
int main()
{
        int sockfd, connfd, len;
        struct sockaddr_in servaddr, cli;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
                printf("Socket creation failed.\n");
                exit(0);
        }
        printf("Creation Success!\n");
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);
        if (( bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
        {
                printf("Socket binding failed.\n");
                exit(0);
        }
        printf("Binding Success!\n");
        if (( listen(sockfd, 5)) != 0)
        {
                printf("Listen Failed \n");
                exit(0);
        }
        printf("Listen Success!\n");
        len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0)
        {
                printf("Server accept failed.\n");
                exit(0);
        }
        else
        {
                printf("Accept Success!\n");
        }
        calc_server(connfd);
        close(sockfd);
}
