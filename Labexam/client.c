#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
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
void print_menu()
{
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("Welcome To Client Side Calculator!\n");
        printf("Operations:\n");
        printf("1) Multiplication\n");
        printf("2) Division\n");
        printf("3) Average\n");
        printf("4) Square root\n");
        printf("5) Exit\n");
}
void calc_client(int sockfd)
{
        print_menu();
        int op;
        float a, b,c;
        struct calc_var input;
        while(1)
        {
                printf("\nEnter Operator: ");
                scanf("%d",&op);
                if (op == 5)
                {
                        input.op = htonl(op);
                        input.a = htonl(0);
                        input.b = htonl(0);
                        input.c = htonl(0);
                        write(sockfd, &input, sizeof(input));
                        printf("\n\nExiting Calculator...\n\n");
                        break;
                }
                else if((op == 1) || (op == 2)){
                        printf("Enter number a: ");
                        scanf("%f",&a);
                        printf("Enter number b: ");
                        scanf("%f",&b);
                        if((op == 2) && (b==0)){
                                printf("Division by zero is invalid! Try again");
                                continue;
                        }
                        input.op = htonl(op);
                        input.a = htonl(a);
                        input.b = htonl(b);
                        input.c = htonl(0);
                }
                else if(op == 4){
                        printf("Enter number a: ");                                                                                                                                                                                         scanf("%f",&a);
                        if(a<0){
                                printf("Square root of negative numbers are imaginary! Try again");
                                continue;
                        }
                        input.op = htonl(op);
                        input.a = htonl(a);
                        input.b = htonl(0);
                        input.c = htonl(0);
                }
                else {
                        printf("Enter number a: ");
                        scanf("%f",&a);
                        printf("Enter number b: ");
                        scanf("%f",&b);
                        printf("Enter number c:");
                        scanf("%f",&c);
                        input.op = htonl(op);
                        input.a = htonl(a);
                        input.b = htonl(b);
                        input.c = htonl(c);
                }
                printf("Sending values to server...\n");
                write(sockfd, &input, sizeof(input));
                float result;
                read(sockfd, &result, sizeof(result));
                printf("\n>>>Result from Server: %.2f\n",result);
        }
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
        if (( connect(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
        {
                printf("Socket connection failed.\n");
                exit(0);
        }                                                                                                                                                                                                                    printf("Connection Success!\n");
        calc_client(sockfd);
        close(sockfd);
}
