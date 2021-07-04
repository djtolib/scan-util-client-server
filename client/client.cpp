#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in addr;

    char buf[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        cout<<"Unable to create socket."<<endl;
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        cout<<"Connection error."<<endl;
        return 2;
    }

    strcpy(buf,argv[1]);

    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, 1024 , 0);

    cout<<buf<<endl;

    close(sock);

    return 0;
}