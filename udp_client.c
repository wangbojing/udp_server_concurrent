

#include <unistd.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>

void createClient(int id,int myPort,int peerPort){
  
    int reuse = 1;
      
    int socketFd;
    struct sockaddr_in peer_Addr;
    peer_Addr.sin_family = PF_INET;
    peer_Addr.sin_port = htons(peerPort);
    peer_Addr.sin_addr.s_addr = inet_addr("192.168.233.128");

    struct sockaddr_in self_Addr;
    self_Addr.sin_family = PF_INET;
    self_Addr.sin_port = htons(myPort);
    self_Addr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
    
    if ((socketFd = socket(PF_INET, SOCK_DGRAM| SOCK_CLOEXEC, 0)) == -1) {
        perror("child socket");
        exit(1);
    } 

    int opt=fcntl(socketFd,F_GETFL);
    fcntl(socketFd,F_SETFL,opt|O_NONBLOCK);

    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse))){
            exit(1);
    }
    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse))){
        exit(1);
    }
    
    if (bind(socketFd, (struct sockaddr *) &self_Addr, sizeof(struct sockaddr))){
        perror("chid bind");
        exit(1);
    } else {

    }
   
    if (connect(socketFd, (struct sockaddr *) &peer_Addr, sizeof(struct sockaddr)) == -1) {
        perror("chid connect");
        exit(1);
    }
   
    
    usleep(1); // --> key

    char buffer[1024] = {0};
    memset(buffer, 0, 1024);
    sprintf(buffer, "hello %d", id);
    sendto(socketFd, buffer, strlen(buffer), 0, (struct sockaddr *) &peer_Addr, sizeof(struct sockaddr_in));

}

void serial(int clinetNum){
    for(int i=1;i<=clinetNum;i++){
        createClient(i,2025+i,1234);
    }
}
int main(int argc, char * argv[])
{

	serial(1024);
	
    printf("serial success\n");
    return 0;
}
