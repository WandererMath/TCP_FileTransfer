#include<fstream>
#include<iostream>
#include<string.h>
#include <stdlib.h>


#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 




#define CHUNK_SIZE 64000
#define PORT 3456

using namespace std;

void recvFile(int cSpec, string filename);

int main(){
    int sockfd, cSpec, len;
	struct sockaddr_in servaddr, cli;
    string fileName="received.pdf";

    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    len = sizeof(cli);

    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 1);

    cSpec = accept(sockfd, (struct sockaddr*)&cli, (socklen_t*)&len);
    recvFile(cSpec, fileName);
    close(sockfd);

}

void recvFile(int cSpec, string filename){
    char buffer[CHUNK_SIZE];
    int fileSize;
    char end_sp[]="ED";
    fstream fw(filename, ios::out);

    bzero(buffer, CHUNK_SIZE);
    //get file size
    if (recv(cSpec, &fileSize, sizeof(fileSize), 0)<0){
        cout<<"ERROR"<<endl;
    }
    cout<<fileSize<<endl;

    //receive file chunk by chunk
    int n_bytes=fileSize;
    int count;
    while(n_bytes>0){
        count=recv(cSpec, buffer, CHUNK_SIZE, 0);
        if (count>0){
            fw.write(buffer, count);
            n_bytes-=count;
            bzero(buffer, CHUNK_SIZE);
            
        }
    }
    fw.close();
    send(cSpec, end_sp, 2, 0);

  
}