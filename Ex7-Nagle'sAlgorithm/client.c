#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>


#define MAX 500
#define PORT 8080
#define SA struct sockaddr

struct request
{
	char choice;
	char request[MAX];
};


int send_message(int sockfd)
{
	int rtt, udelay;
	char message[MAX],buffer[MAX], ack[MAX];
	printf("Enter required RTT: ");
	scanf("%d",&rtt);

	printf("Enter required uniform delay: ");
	scanf("%d",&udelay);

	printf("Enter message: ");
	scanf("%s",message);
	
	//printf("%d\t%d\t%s\n",rtt,udelay,message);

	int msglen = strlen(message);
	printf("msglen = %d\n",msglen);
	
	int i = 0, bufflen;
	int j,k,rtt_count=0;
	int wcount = 0;	
	int scount = 0;
	int totaltime = 0;
	while (1)
	{
		rtt_count++;
		printf("\nRTT #%d: \n",rtt_count);
		if (i == 0)	
		{	
			//wcount = 1;
			//strcat(buffer, message[0]);
			buffer[0] = message[0];
			k = 1;
			scount = 1;
			wcount = 1;	
		}
		else
		{
			wcount += floor(rtt/udelay);
			if (wcount <= scount)
			{
				printf("Wait for send buffer to fill: %ds\n",udelay-rtt);
				totaltime += (udelay - rtt);
				wcount++;
			}
			int slen = wcount - scount;
			//printf("hello\n");
			//bufflen = floor(rtt/udelay);
			for(j=i,k=0;k<slen && j<msglen ;j++,k++)
			{	
				//printf("%c %d",message[j], j);
				buffer[k] = message[j];
			}
			scount+=k;
			//printf("\n");
		}
		
		int ret = send(sockfd, &buffer, sizeof(buffer), 0);
		
		printf("Buffer sent: %s\n",buffer);
		
		recv(sockfd, &ack, sizeof(ack), 0);
		
		printf("Ack received: %s\n",ack);
		
		printf("~~~~~~~~~~~~~~~\n");
		
		bzero(buffer, sizeof(buffer));
		bzero(ack, sizeof(ack));
	
		i+=k;
		totaltime += rtt;
		if (i >= msglen)
		{
			break;	
		}
	}		

	printf("Total time: %d(time taken to write first packet) + %d = %ds\n",udelay,totaltime,totaltime+udelay);
	
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

//	response_send(sockfd);
	
	send_message(sockfd);

	close(sockfd);

	printf("\n\nConnection Closed.\n");
}
