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

struct request
{
	char choice;
	char request[MAX];
};


int request_handler(int connfd)
{
	char buff[MAX];
	char object[MAX]="";
	int n,i,j;
	char choice;	
	struct request input;
	
	while(1)
	{
		if ( read(connfd, &input, sizeof(input)))
		{
			choice = input.choice;
			strcpy(buff, input.request);			

			printf("Request: %s\n",buff);

			if ( choice != '1')
			{
				printf("Exiting...\n");
				break;
			}
			
			int len = strlen(buff);
			for(i=4,j=0; i<(len-1); i++,j++)
			{
					object[j] = buff[i];	
			}
	
			FILE *fd;
	
			char filename[MAX]="";
			char message[MAX], content[MAX]="";
		
			strcat(content, "REQUEST TIME: ");
			time_t rawtime;
			struct tm * timeinfo;
	
			time (&rawtime);
			timeinfo = localtime(&rawtime);
	
			strcat(content, asctime(timeinfo));
			strcat(content, "Content: \n");

			strcat(filename,object);
			fd = fopen(filename,"r+");

			if (fd == NULL)
			{
				printf("Object does not exist\n");
				return 1;
			}
		
			while ( fgets(message,MAX,fd) != NULL )
			{
				strcat(content,message);
			}		
		
			fclose(fd);
	
			printf("Requested Object: \n~\n%s~\n",content);
	
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
			write(connfd, content, sizeof(content));
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
		printf("Accept Success!\n\n");
	}

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	request_handler(connfd);

	close(sockfd);
	printf("\n\nConnection Closed.\n");
	
}
