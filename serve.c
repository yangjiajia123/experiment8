#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <linux/input.h>  
#include <fcntl.h>  
#include <sys/time.h>   
#include <sys/stat.h>  
#include <unistd.h> 

#define PORT 5000
#define BACKLOG 10
#define LENGTH 512

int main(int argc,char **argv)
{
 int sockfd,nsockfd;
 int num;
 int sin_size;
 
 int fd, retval;  
 char buf[6];  
 fd_set readfds;  
 struct timeval tv;  
 fd = open( "/dev/input/mice", O_RDONLY );    
    
 char sdbuf[LENGTH];
 struct sockaddr_in addr_local;
 struct sockaddr_in addr_remote;
 char sendstr[16]={"123456789abcde"};
 
 if(fd<0) 
  {  
      printf("Failed to open \"/dev/input/mice\".\n");  
      exit(1);  
  } 
 else
  {  
      printf("open \"/dev/input/mice\" successfuly.\n");  
    }   

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
 
   while(1) 
     {  
         tv.tv_sec = 5;  
         tv.tv_usec = 0;  
         FD_ZERO( &readfds );  
         FD_SET( fd, &readfds );  
         retval = select( fd+1, &readfds, NULL, NULL, &tv );  
         if(retval==0) 
         {  
             printf( "Time out!\n" );  
         }  
         if(FD_ISSET(fd,&readfds)) 
             {   
                if(read(fd, buf, 6) <= 0) 
                {  
                   continue;  
                }  
             
                sprintf(sdbuf,"Button type = %d, X = %d, Y = %d, Z = %d\n", (buf[0] & 0x07), buf[1], buf[2],   buf[3]);  
  
                if(num=send(nsockfd,sdbuf,strlen(sdbuf),0)==-1)
                {
                     printf("ERROR:Failed to send string.\n");
                     close(sockfd);
                     exit(1);
                }
             }  
      }  
     close(fd);  
    }
   close (nsockfd);
   while(waitpid(-1,NULL,WNOHANG)>0);
   }
  }
