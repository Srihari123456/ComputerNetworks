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
int a = ntohl(output.a);
int b = ntohl(output.b);
int op = ntohl(output.op);
if (op == 5)
{
printf("\nExiting Calculator...\n");
break;
}
int ans;
switch (op)
{
case 1:
printf("Operation Addition: ");
printf("%d +",a);
ans = a + b;
break;
case 2:
printf("Operation Subtraction: ");
printf("%d -",a);
ans = a - b;
break;
case 3:
printf("Operation Multiplication: ");
printf("%d *",a);
ans = a * b;
break;
case 4:
printf("Operation: Division: ");
printf("%d /",a);
ans = a / b;
break;
}
printf(" %d = %d\n",b,ans);
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