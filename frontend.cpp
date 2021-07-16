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
    
    sockfd_server = socket (AF_INET, SOCK_STREAM, 0); // Creacion del socket.

    my_addr_server.sin_family = AF_INET; //Protocolo TCP
    my_addr_server.sin_port = htons(3000); //Puerto
    my_addr_server.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa
    
    if (-1 == bind (sockfd_server, (struct sockaddr *) &my_addr_server, sizeof(struct sockaddr) ))  // Asignando IP y puerto al socket
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    if (-1 == listen(sockfd_server, 50))    // Se habilita el socket para poder recibir conexiones
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    addrlen_server = sizeof (struct sockaddr);  // Se llama a accept() y el servidor queda en espera de conexiones. accept() es bloqueante
    socket_aceptado_server = accept(sockfd_server, (struct sockaddr *)&remote_addr_server, (socklen_t *) &addrlen_server);  //el servidor queda en espera de conexiones

    int sockfd, socket_aceptado, num;
    struct sockaddr_in remote_addres;
    int addrlen;

    sockfd = socket (AF_INET, SOCK_STREAM, 0);  // Creacion del socket

    remote_addres.sin_family = AF_INET; //Protocolo TCP
    remote_addres.sin_port = htons(3001); //Puerto
    remote_addres.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa
    
    connect (sockfd, (struct sockaddr *) &remote_addres, sizeof(struct sockaddr));

    int cantidad;
    vector<pair<string,string>> cache;

    while (true)
    {
        char buffer [1024];
        char buffer_recv [1024];
        recv(socket_aceptado_server, buffer, sizeof(char)*1024, 0); //Se recibe la consulta desde el cliente
        
        int indice;
        if ((indice = esta(buffer, cache)) >= 0)    //Se comprueba si la palabra esta en el caché
        {
            cache = reemplazo(indice, cache); //Se cambia la palabra de posicion dentro del cache
            send(socket_aceptado_server, cache[cache.size()-1].second.c_str(), sizeof(char)*1024 , 0); //se envia la respuesta al cliente desde el cache
        }
        else
        {
            send(sockfd, buffer, sizeof(char)*1024 , 0);    //Se envia la consulta al indice invertido
            recv(sockfd, buffer_recv, sizeof(char)*1024 , 0);   //se recibe la respuesta desde el indice invertido
            cache = insertar(buffer, buffer_recv, cache);   //se agrega la consulta con su respuesta al cache
            send(socket_aceptado_server, buffer_recv, sizeof(char)*1024 , 0);   //se envia la respuesta al cliente
        }
    }

    // ciere de los sockets
    close(socket_aceptado_server);
    close(sockfd_server);
    close(sockfd);

    return 0;
}

