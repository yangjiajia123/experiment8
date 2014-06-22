#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#define PORT 5000
#define LENGTH 256

int main(int argc,char *argv[])
{
 int sockfd;
 int num;
 char revbuf[LENGTH];
 struct sockaddr_in addr_remote;
 
 if(argc!=2)
 {
  printf("Mr Jiajia Usage:talker HOST IP(ex:/talker 192.168.1.92).\n");
  return 0;
 }
 
 if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)
 {
  printf("ERROR:Failed to obtain Socket Descriptor!\n");
  return 0;
 }

 addr_remote.sin_family=AF_INET;
 addr_remote.sin_port=htons(PORT);
 inet_pton(AF_INET,argv[1],&addr_remote.sin_addr);
 bzero(&(addr_remote.sin_zero),8);

 if(connect(sockfd,(struct sockaddr *)&addr_remote,sizeof(struct sockaddr))==-1) {
  printf("ERROR:Failed to connect to the hoat!\n");
  return 0;
 }

else
 printf("OK:Have connected to the %s!\n",argv[1]);

 while(strcmp(revbuf,"exit")!=0)
 {
  

 bzero(revbuf,LENGTH);
 num=recv(sockfd,revbuf,LENGTH,0);

 switch(num)
 {
 case -1:
 printf("ERROR:Failed to send you data!\n");
 return 0;
 case 0:
 close(sockfd);
 return 0;
 default :
 printf("Mr Jiajia:reviece total %d bytes!\n",num);
 break;
 }
 revbuf[num]='\0';
 printf("Mr Jiajia:Revieced is string is : %s\n",revbuf);
 }

close(sockfd);
return 0;

 
}

