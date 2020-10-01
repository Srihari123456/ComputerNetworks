#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define MAX 500
#define PORT 8080
#define SA struct sockaddr

int receive_message(int connfd)
{
	char buff[MAX];
	char ack[MAX] = "ACK";	
	
	while(1)
	{
		if ( recv(connfd, &buff, sizeof(buff), 0) )
		{
			printf("Message from Client: %s\n",buff);

			int ret = send(connfd, &ack, sizeof(ack), 0);
			printf("Ack sent\n\n\n");
		}
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
		printf("Accept Success!\n\n");
	}

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	receive_message(connfd);
	

	close(sockfd);
	printf("\n\nConnection Closed.\n");
	
}
