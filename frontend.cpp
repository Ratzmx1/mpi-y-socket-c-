#include <vector>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include "vector.cpp"
using namespace std;

#define MAX_ELEM_RECIBIDOS 1024

int main()
{
    int i, sockfd_server, socket_aceptado_server;
    struct sockaddr_in my_addr_server;
    struct sockaddr_in remote_addr_server;
    int addrlen_server;

    // Creacion del socket.
    sockfd_server = socket (AF_INET, SOCK_STREAM, 0); 

    my_addr_server.sin_family = AF_INET; //Protocolo TCP
    my_addr_server.sin_port = htons(3000); //Puerto
    my_addr_server.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa
    //my_addr_server.sin_addr.s_addr = inet_addr ("192.168.101.68"); // IP por donde recibira paquetes el programa

    // Asignando IP y puerto al socket
    if (-1 == bind (sockfd_server, (struct sockaddr *) &my_addr_server, sizeof(struct sockaddr) ))
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Se habilita el socket para poder recibir conexiones.
    if (-1 == listen(sockfd_server, 50))
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Se llama a accept() y el servidor queda en espera de conexiones. accept() es bloqueante.
    addrlen_server = sizeof (struct sockaddr);
    socket_aceptado_server = accept(sockfd_server, (struct sockaddr *)&remote_addr_server, (socklen_t *) &addrlen_server);

    int sockfd, socket_aceptado, num;
    struct sockaddr_in remote_addres;
    int addrlen;


    // Creacion del socket.
    sockfd = socket (AF_INET, SOCK_STREAM, 0); 

    remote_addres.sin_family = AF_INET; //Protocolo TCP
    remote_addres.sin_port = htons(3001); //Puerto
    remote_addres.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa

    //int connect ( int sockfd, struct sockaddr *serv_addr, int addrlen )
    
    connect (sockfd, (struct sockaddr *) &remote_addres, sizeof(struct sockaddr));


    
    int cantidad;
    vector<pair<string,string>> cache;

    // recv(socket_aceptado_server, &cantidad, sizeof(int), 0);
    // send(sockfd, &cantidad, sizeof(int), 0);

    while (true)
    {
        //Recibiendo datos. recv() es bloqueante
        char buffer [1024];
        char buffer_recv [1024];
        recv(socket_aceptado_server, buffer, sizeof(char)*1024, 0);
        
        int indice;
        if ((indice = esta(buffer, cache)) >= 0)
        {
            cache = reemplazo(indice, cache);
            send(socket_aceptado_server, cache[cache.size()-1].second.c_str(), sizeof(char)*1024 , 0);
        }
        else
        {
            send(sockfd, buffer, sizeof(char)*1024 , 0);
            recv(sockfd, buffer_recv, sizeof(char)*1024 , 0);
            cache = insertar(buffer, buffer_recv, cache);
            send(socket_aceptado_server, buffer_recv, sizeof(char)*1024 , 0);
        }
    }
    close(socket_aceptado_server);
    close(sockfd_server);
    close(sockfd);

    return 0;
}

