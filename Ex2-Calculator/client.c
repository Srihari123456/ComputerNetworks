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
int a;
int b;
};
void print_menu()
{
printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
printf("Welcome To Client Side Calculator!\n");
printf("Operations:\n");
printf("1) Addition\n");
printf("2) Subtraction\n");
printf("3) Multiplication\n");
printf("4) Division\n");
printf("5) Exit\n");
}
void calc_client(int sockfd)
{
print_menu();
int op, a, b;
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
write(sockfd, &input, sizeof(input));
printf("\n\nExiting Calculator...\n\n");
break;
}
printf("Enter integer a: ");
scanf("%d",&a);
printf("Enter integer b: ");
scanf("%d",&b);
input.op = htonl(op);
input.a = htonl(a);
input.b = htonl(b);
printf("Sending values to server...\n");
write(sockfd, &input, sizeof(input));
int result;
read(sockfd, &result, sizeof(result));
printf("\n>>>Result from Server: %d\n",result);
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
}
printf("Connection Success!\n");
calc_client(sockfd);
close(sockfd);
}