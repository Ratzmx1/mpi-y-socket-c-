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

    sockfd = socket (AF_INET, SOCK_STREAM, 0); // Creacion del socket

    remote_addres.sin_family = AF_INET; // Protocolo TCP
    remote_addres.sin_port = htons(3000); // Puerto
    remote_addres.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa
    
    connect (sockfd, (struct sockaddr *) &remote_addres, sizeof(struct sockaddr)); // coneccion al socket

    char buffer[1024];

    cout << "Para finalizar el programa ingrese -1" << endl;

    while (true)
    {
        cin >> buffer;  //Ingreso por teclado de la palabra a consultar
        if (string(buffer) == "-1") //condicion para terminar el programa
        {
            break;
        }
        send(sockfd, buffer, sizeof(char)*1024 , 0);    //se envia la palabra hacia el frontend
        recv(sockfd, buffer, sizeof(char)*1024, 0);     //se recibe la respuesta desde el frontend
        cout << buffer << endl;
    }


    close(sockfd);  //cierre del socket
    return 0;
}

