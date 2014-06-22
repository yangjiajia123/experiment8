#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define PORT 5000
#define BACKLOG 10
#define LENGTH 512

int main(void)
{
 int sockfd,nsockfd;
 int num;
 int sin_size;
 char sdbuf[LENGTH];
 struct sockaddr_in addr_local;
 struct sockaddr_in addr_remote;
 char sendstr[16]={"123456789abcde"};

 if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
 {
  printf("ERROR:Failed to obtain Socket Descriptor!\n");
  return 0;
 }

 else
 printf("Mr Jiajia :Obtain Socket Descriptor seccussfully!\n");

 addr_local.sin_family=AF_INET;
 addr_local.sin_port=htons(PORT);
 addr_local.sin_addr.s_addr=INADDR_ANY;
 bzero(&(addr_local.sin_zero),8);

 if(bind(sockfd,(struct sockaddr *)&addr_local,sizeof(struct sockaddr))==-1)
 {
  printf("ERROR:Failed to bind Port %d\n",PORT);
  return 0;
 }

 if(listen(sockfd,BACKLOG)==-1)
 {
  printf("ERROR:Failed to listen Port %d.\n",PORT);
  return 0;
 }
 else
 printf("Mr Jiajia :listen to the Port %d successfully!\n",PORT);
 while(1)
 {
  sin_size=sizeof(struct sockaddr);
 if((nsockfd=accept(sockfd,(struct sockaddr *)&addr_remote,&sin_size))==-1)
  { 
   printf("ERROR!\n");
   continue;
  }
 else
  printf("OK:%s!\n",inet_ntoa(addr_remote.sin_addr));
 
 if(!fork())
 {
  printf("Mr Jiajia You can enter string,and press 'exit' to end the connect.\n");
 while(strcmp(sdbuf,"exit")!=0)
  {
  scanf("%s",sdbuf);
  if(num=send(nsockfd,sdbuf,strlen(sdbuf),0)==-1)
   {
  printf("ERROR:Failed to send string.\n");
  close(sockfd);
  exit(1);
   }

 printf("Mr Yangjiajia:Send %d bytes successfully,please enter gain!\n",num);
  }
 }
 close (nsockfd);
 while(waitpid(-1,NULL,WNOHANG)>0);
 }
}


