#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
int main()
{
        struct sockaddr_in serveraddr;
        int listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(listensockfd < 0){
                printf("Error in creating socket.");
                return 0;
        }
        printf("\nCreation Success\n");
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(4000);
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);


        if(bind(listensockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0){
                printf("Error in binding.");
                return 0;
        }
        printf("\nBinding success\n");
        int server_listen = listen(listensockfd,10);
        if(server_listen == -1){
                printf("Error in listening.");
                return 0;
        }
        printf("\nListen Success\n");
        int connection = accept(listensockfd, (struct sockaddr*)NULL, NULL);
        if(connection < 0){
                printf("\nServer accept failed.\n");
                return 0;
        }
        printf("\nAccess success \n");
        while(1){
                char fn[1000],readmsg[1000],headermsg[1000],res[1000];
                read(connection,fn,sizeof(fn));
                read(connection,headermsg,sizeof(headermsg));

                printf("HTTP Request: ");
                printf("%s",headermsg,sizeof(headermsg));
                printf("\n");
                FILE *fp;
                fp = fopen(fn,"r");

                fread(readmsg,1000,1,fp);
                char timedate[1000];
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                strcpy(timedate,asctime(tm));
                printf("Current time: %s \n", timedate);
                printf("Content of %s: %s",fn,readmsg);

                strcpy(res,"");
                strcat(res,"HTTP RESPONSE MESSAGE: ");
                strcat(res,"\nTimestamp: ");
                strcat(res,timedate);
                strcat(res,"Message: ");
                strcat(res,readmsg);

                write(connection,res,sizeof(res));
        }
}
