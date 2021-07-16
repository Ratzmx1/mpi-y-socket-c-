#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int esta(string palabra, vector<pair<string, string>> cache) //Funcion para comprobar que la consulta este dentro del cache
{
  for (int i = 0; i < cache.size(); i++)
  {
    if (palabra == cache[i].first)
    {
      return i;
    }
  }
  return -1;  
}

vector<pair<string, string>> reemplazo(int indice, vector<pair<string, string>> cache) //Funcion para cambiar la consulta de posicion dentro del cache
{
  pair<string, string> palabras = cache[indice];
  cache.erase(cache.begin()+indice);
  cache.push_back(palabras);
  return cache;
}

vector<pair<string, string>> insertar(string palabra, string respuesta, vector<pair<string, string>> cache) //Funcion para agregar una consulta al cache
{
  cache.push_back({palabra, respuesta});
  if (cache.size() > 3)
  {
    cache.erase(cache.begin());
  }
  return cache;
}