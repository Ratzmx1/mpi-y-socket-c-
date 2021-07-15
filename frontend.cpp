#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
using namespace std;

#define MAX_ELEM_RECIBIDOS 1024

int main()
{
    int i, sockfd, socket_aceptado;
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;
    int addrlen;


    // Creacion del socket.
    sockfd = socket (AF_INET, SOCK_STREAM, 0); 

    my_addr.sin_family = AF_INET; //Protocolo TCP
    my_addr.sin_port = htons(3000); //Puerto
    my_addr.sin_addr.s_addr = inet_addr ("127.0.0.1"); // IP por donde recibira paquetes el programa
    //my_addr.sin_addr.s_addr = inet_addr ("192.168.101.68"); // IP por donde recibira paquetes el programa

    // Asignando IP y puerto al socket
    if (-1 == bind (sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr) ))
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Se habilita el socket para poder recibir conexiones.
    if (-1 == listen(sockfd, 50))
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Se llama a accept() y el servidor queda en espera de conexiones. accept() es bloqueante.
    addrlen = sizeof (struct sockaddr);
    socket_aceptado = accept(sockfd, (struct sockaddr *)&remote_addr, (socklen_t *) &addrlen);
    char buffer [1024];

    int cantidad;

    recv(socket_aceptado, &cantidad, sizeof(int), 0);
    
    printf("Cantidad: %d\n",cantidad);

    for (size_t i = 0; i < cantidad; i++)
    {
        //Recibiendo datos. recv() es bloqueante
        recv(socket_aceptado, buffer, sizeof(char)*1024, 0);
        string wea = buffer;
        wea+=" xd";
        send(socket_aceptado, wea.c_str(), sizeof(char)*1024 , 0);
        
    }
    close(socket_aceptado);
    close(sockfd);

    return 0;
}

