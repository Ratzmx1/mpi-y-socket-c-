//Enzo Pigatti Salinas - 19.751.752-2
//Luis Varela Gutierrez - 18.779.441-2
# include <unistd.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]){

    map<string, vector<string>> archivos;
    map<string, set<string>> palabras;
    map<string, int> nodos;
    map<int, int> cantidades;
    map<int, int> cantidadQueryPorNodo;

    vector<string> vectorPalabras;
    vector<string> consultas;
    
    char buffer[150];

    MPI_Status status;

    string nombreArchivo, palabra, consulta;

    int cantidadArchivos, cantidadPalabras, largo, id, cNodos, cantidad, cantidadQuery, largoConsulta;
    int tagCantArchivos = 100,
        tagLargoArchivo = 101,
        tagNombreArchivo = 102,
        tagCantidadPalabras = 103,
        tagLargoPalabra = 104,
        tagPalabra = 105,
        tagCantidadPalabrasMapeo = 106,
        tagLargoPalMap = 107,
        tagPalMap = 108,
        tagPalabraConsulta = 109,
        tagEnvioDeCantidad = 110,
        tagEnvioConsulta = 111,
        tagCantidadConsultas = 112,
        tagNodoResponde = 113,
        tagLargoEnvioRespuesta = 114,
        tagEnvioRespuesta = 115,
        tagEnvioLargoConsulta = 116;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cNodos);
    
    if(id == 0){
        set<string> palabrasIndividuales;
        cin >> cantidadArchivos;
        for (int i = 1; i < cNodos; i++){
            MPI_Send(&cantidadArchivos, 1, MPI_INT, i, tagCantArchivos, MPI_COMM_WORLD);
        }
        for (int i = 0; i < cantidadArchivos; i++){
            vectorPalabras.clear();

            cin >> nombreArchivo;
            largo = nombreArchivo.size();
            for (int i = 1; i < cNodos; i++){
                MPI_Send(&largo, 1, MPI_INT, i, tagLargoArchivo, MPI_COMM_WORLD); // ENVIO LARGO NOMBRE ARCHIVO
                MPI_Send(nombreArchivo.c_str(), largo + 1, MPI_CHAR, i, tagNombreArchivo, MPI_COMM_WORLD); // ENVIO Nombre archivo
            }
            
            cin >> cantidadPalabras;
            for (int i = 1; i < cNodos; i++){
                MPI_Send(&cantidadPalabras, 1, MPI_INT, i, tagCantidadPalabras, MPI_COMM_WORLD);
            }
            for (int j = 0; j < cantidadPalabras; j++){
                cin >> palabra;
                largo = palabra.size();
                for (int i = 1; i < cNodos; i++){
                    MPI_Send(&largo, 1, MPI_INT, i, tagLargoPalabra, MPI_COMM_WORLD);
                    MPI_Send(palabra.c_str(), largo + 1, MPI_CHAR, i, tagPalabra, MPI_COMM_WORLD); // ENVIO Palabra
                }
                vectorPalabras.push_back(palabra);
            }
            archivos[nombreArchivo] = vectorPalabras;
        }

        for(auto archivo : archivos){                        //Separacion de las palabras para dejarlas sin repeticion
            for(auto picota: archivo.second){
                palabrasIndividuales.insert(picota);
            }
        }

                                      
        for (int i = 0; i < palabrasIndividuales.size(); i++){      //Calcula la cantidad de palabras que iran en cada nodo
            int nodo = i % cNodos;
            set<string>::iterator it = palabrasIndividuales.begin();
            advance(it, i);
            string pal = *it;
            nodos[pal] = nodo+1;
            if ( nodo != 0 ){
                cantidades[nodo] ++;
            }
        }

        for (int i = 1; i < cNodos; i++){
            MPI_Send(&cantidades[i], 1, MPI_INT, i, tagCantidadPalabrasMapeo, MPI_COMM_WORLD);
        }
        
        for (int i = 0; i < palabrasIndividuales.size(); i++){  //Ingresa las palabras a los nodos
            int nodo = i % cNodos;
            set<string>::iterator it = palabrasIndividuales.begin();
            advance(it, i);
            string pal = *it;
            if ( nodo == 0 ){
                set<string> nuevaPalabra;
                nuevaPalabra.clear();
                for(auto archivo: archivos){
                    for(auto picota: archivo.second){
                        if(picota == pal){
                            nuevaPalabra.insert(archivo.first);
                        }
                    }
                    palabras[pal] = nuevaPalabra;
                }
            }else{
                largo = pal.size();
                MPI_Send(&largo, 1, MPI_INT, nodos[pal] - 1, tagLargoPalMap, MPI_COMM_WORLD);
                MPI_Send(pal.c_str(), largo + 1, MPI_CHAR, nodos[pal] - 1, tagPalMap, MPI_COMM_WORLD);
            }
        }

        

        // for (int i = 0; i < cantidad; i++){     //Se leen las  desded el archivoconsultas
        //     cin >> consulta;
        //     if(nodos[consulta] - 1 == 0){
        //         cout << "Resultados para \"" <<  consulta << "\": ";
        //         for (auto i : palabras[consulta]){
        //             cout << i << " ";
        //         }
        //         cout << endl;
        //     }else if(nodos[consulta] - 1 > 0 ){
        //         cantidadQueryPorNodo[nodos[consulta] - 1] += 1;
        //         consultas.push_back(consulta);
        //     }else{
        //         cout << "Resultados para \"" <<  consulta << "\": No hay resultados." << endl;
        //     }
        // }

       
        // for (int i = 1; i < cNodos ; i++){       // SE MANDAN LAS CANTIDADES A LOS NODOS QUE FALTAN
        //     MPI_Send(&cantidadQueryPorNodo[i], 1, MPI_INT, i, tagEnvioDeCantidad, MPI_COMM_WORLD);
        // }






        cin >> cantidad;
        for (int j = 1; j < cNodos; j++){
            MPI_Send(&cantidad, 1, MPI_INT, j , tagCantidadConsultas, MPI_COMM_WORLD);
        }
        string query;
        string respuesta;
        for (int i = 0; i < cantidad; i++)
        {   
            cin >> query;
            int nodoRespuesta = nodos[query] - 1;

            for (int j = 1; j < cNodos; j++){
                MPI_Send(&nodoRespuesta, 1, MPI_INT, j , tagNodoResponde, MPI_COMM_WORLD);
            }

            if (nodoRespuesta == 0){
                respuesta = "";
                for(auto x: palabras[query]){
                    respuesta += x + " ";
                }
            }else if(nodoRespuesta < 0){
                respuesta = "no ta";
            }else{
                largo = query.size();
                MPI_Send(&largo, 1, MPI_INT, nodoRespuesta , tagEnvioLargoConsulta, MPI_COMM_WORLD);

                MPI_Send(query.c_str(), largo + 1, MPI_CHAR, nodoRespuesta , tagEnvioConsulta, MPI_COMM_WORLD);

                MPI_Recv(&largo, 1, MPI_INT, nodoRespuesta, tagLargoEnvioRespuesta, MPI_COMM_WORLD, &status);
                MPI_Recv(buffer, largo + 1, MPI_CHAR, nodoRespuesta, tagEnvioRespuesta, MPI_COMM_WORLD, &status);
                respuesta = buffer;
            }
            cout << respuesta << endl;
        }
        

        
        // for (int i = 0; i < consultas.size(); i++){     //Envio del largo y las consultas
        //     int largo = consultas[i].length();
        //     MPI_Send(&largo, 1, MPI_INT, nodos[consultas[i]] - 1, tagCantidadConsultas, MPI_COMM_WORLD);
        //     MPI_Send(consultas[i].c_str(), largo + 1, MPI_CHAR, nodos[consultas[i]] - 1, tagEnvioConsulta, MPI_COMM_WORLD);

        //     MPI_Recv(&largo, 1, MPI_INT, 0, tagCantidadConsultas, MPI_COMM_WORLD, &status);
        //     MPI_Recv(buffer, largo + 1, MPI_CHAR, 0, tagEnvioConsulta, MPI_COMM_WORLD, &status);

        //     cout << buffer << endl;
        // }
        
    }else {                   
        MPI_Recv(&cantidadArchivos, 1, MPI_INT, 0, tagCantArchivos, MPI_COMM_WORLD, &status);       //Se recibe la cantidad de archivos correspondiente a cada nodo
        for (int i = 0; i < cantidadArchivos; i++){             //Se recibe el nombre del archivo y la cantidad de palabras junto con las palabras
            vectorPalabras.clear();

            MPI_Recv(&largo, 1, MPI_INT, 0, tagLargoArchivo, MPI_COMM_WORLD, &status);
            MPI_Recv(buffer, largo + 1, MPI_CHAR, 0, tagNombreArchivo, MPI_COMM_WORLD, &status);
            nombreArchivo = buffer;
            MPI_Recv(&cantidadPalabras,  1, MPI_INT, 0, tagCantidadPalabras, MPI_COMM_WORLD, &status);

            for (int j = 0; j < cantidadPalabras; j++){
                MPI_Recv(&largo, 1, MPI_INT, 0, tagLargoPalabra, MPI_COMM_WORLD, &status);
                MPI_Recv(buffer, largo + 1, MPI_CHAR, 0, tagPalabra, MPI_COMM_WORLD, &status);
                palabra = buffer;
                vectorPalabras.push_back(palabra);
            }
            archivos[nombreArchivo] = vectorPalabras;   
        }

        MPI_Recv(&cantidadPalabras,  1, MPI_INT, 0, tagCantidadPalabrasMapeo, MPI_COMM_WORLD, &status);     //se recive la cantidad de palabras, el largo y las palabras para el mapeo
        for (int i = 0; i < cantidadPalabras; i++){
            MPI_Recv(&largo, 1, MPI_INT, 0, tagLargoPalMap, MPI_COMM_WORLD, &status);
            MPI_Recv(buffer, largo + 1, MPI_CHAR, 0, tagPalMap, MPI_COMM_WORLD, &status);
            string pal = buffer;
            set<string> palabrasMapeo;
            palabrasMapeo.clear();
            for(auto archivo: archivos){
                for(auto picota: archivo.second){
                    if(picota == pal){
                        palabrasMapeo.insert(archivo.first);
                    }
                }
               palabras[pal] = palabrasMapeo;
            }
        }

        


        int cantConsultas;

        MPI_Recv(&cantConsultas, 1, MPI_INT, 0, tagCantidadConsultas, MPI_COMM_WORLD, &status);
        // cout << cantConsultas << endl;
        for (int i = 0; i < cantConsultas; i++)
        {
            int nodoResponde;
            MPI_Recv(&nodoResponde, 1, MPI_INT, 0, tagNodoResponde, MPI_COMM_WORLD, &status);
            string query;

            if (nodoResponde == id)
            {
                MPI_Recv(&largo, 1, MPI_INT, 0, tagEnvioLargoConsulta, MPI_COMM_WORLD, &status);
                
                MPI_Recv(buffer, largo + 1, MPI_CHAR, 0, tagEnvioConsulta, MPI_COMM_WORLD, &status);
                query = buffer;
                string respuesta = "";

                for(auto x: palabras[query]){
                    respuesta += x + " ";
                }
                largo = respuesta.size();
                MPI_Send(&largo, 1, MPI_INT, 0, tagLargoEnvioRespuesta, MPI_COMM_WORLD);
                MPI_Send(respuesta.c_str(), largo + 1, MPI_CHAR, 0, tagEnvioRespuesta, MPI_COMM_WORLD);
            }
        }
        
        
        

        // MPI_Recv(&cantidadQuery, 1, MPI_INT, 0, tagEnvioDeCantidad, MPI_COMM_WORLD, &status);           // Se recibe cantidad de consultas
        
        // for (int i = 0; i < cantidadQuery; i++){            //Se reciben el largo de las consultas y las consultas 
        //     MPI_Recv(&largoConsulta, 1, MPI_INT, 0, tagCantidadConsultas, MPI_COMM_WORLD, &status);
        //     MPI_Recv(buffer, largoConsulta + 1, MPI_CHAR, 0, tagEnvioConsulta, MPI_COMM_WORLD, &status);
        //     string consultaFinal = buffer;
            
        //     cout << "Resultados para \"" <<  consultaFinal << "\": ";
        //     for (auto i : palabras[consultaFinal]){
        //         cout << i << " ";
        //     }
        //     cout << endl;
        // }
    }
    MPI_Finalize();
    return 0;
}