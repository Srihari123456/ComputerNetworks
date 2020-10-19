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

struct request
{
	char choice;
	char request[MAX];
};


int response_send(int sockfd)
{
	
	char buff[MAX];
	int n;
	char choice;

	struct request input;
		
	while(1)
	{	
		printf("Do you wish to send an HTTP Request?: ");
		choice = getchar();
		getchar();
		
		input.choice = choice;
		
		if (choice != '1')
		{
			strcpy(input.request,"None");
			printf("Exiting...\n");
			write(sockfd, &input, sizeof(input));
			return 0;
		}

		bzero(buff, sizeof(buff));
		printf("Enter HTTP Request: ");
		n = 0;
		while (( buff[n++] = getchar()) != '\n');
	
		strcpy(input.request,buff);	
		write(sockfd, &input, sizeof(input));

		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
	
		printf("\nServer Response: \n~\n%s~\n",buff);

		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
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

	if (( connect(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket connection failed.\n");
		exit(0);
	}
	printf("Connection Success!\n\n");

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	response_send(sockfd);
	close(sockfd);
	
	printf("\n\nConnection Closed.\n");
}
