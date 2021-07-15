#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>

#define MAX_ELEM_RECIBIDOS 1000
using namespace std;

int main()
{
    int sockfd, socket_aceptado, num;
    struct sockaddr_in remote_addres;
    int addrlen;


    // Creacion del socket.
    sockfd = socket (AF_INET, SOCK_STREAM, 0); 

    remote_addres.sin_family = AF_INET; //Protocolo TCP
    remote_addres.sin_port = htons(3000); //Puerto
    remote_addres.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa

    //int connect ( int sockfd, struct sockaddr *serv_addr, int addrlen )
    
    connect (sockfd, (struct sockaddr *) &remote_addres, sizeof(struct sockaddr));



    int cantidad;

    cin >> cantidad;
    send(sockfd, &cantidad, sizeof(int) , 0);

    char buffer[1024];

    for (size_t i = 0; i < cantidad; i++)
    {
        cin >> buffer;
        send(sockfd, buffer, sizeof(char)*1024 , 0);

        recv(sockfd, buffer, sizeof(char)*1024, 0);
        cout << buffer << endl;
    }


    close(sockfd);
    return 0;
}

