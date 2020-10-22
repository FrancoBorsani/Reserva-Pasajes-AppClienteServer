#ifndef _SERVICIO_H_
#define _SERVICIO_H_

#define TAMANIO_I  5
#define TAMANIO_J  21
#include<string>

using namespace std;

/**
    AXIOMAS
    id  > 0
    31 >= dia >= 1
    12 >= mes >= 1
    omnibus se asume que empieza vacio
*/

typedef struct{
    int id;
    int dia,mes,anio;
    string origen;
    string destino;
    char omnibus[TAMANIO_I][TAMANIO_J] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'}}; //el omnibus se inicializa vacio agregandole 'O´
}Servicio;

void crearServicio(Servicio& servicio);

void eliminarServicio(Servicio& servicio);


int getId(Servicio& servicio);

void setId(Servicio& servicio,int id);

string getFecha(Servicio& servicio);

void setFecha(Servicio& servicio  , int dia , int mes , int anio);

string  getOrigen(Servicio& servicio);

void setOrigen(Servicio& servicio,string origen);

string  getDestino(Servicio& servicio);

void setDestino(Servicio& servicio,string destino);


char** getOmnibus(Servicio& servicio);//retorna un puntero que apunta a la primer posicion de  memoria en donde se encuentra la matriz del servicio indicado

void setOmnibus(Servicio& servicio , char omnibus[TAMANIO_I][TAMANIO_J]);

#endif // _SERVICIO_H_
