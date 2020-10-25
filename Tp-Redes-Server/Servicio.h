#ifndef _SERVICIO_H_
#define _SERVICIO_H_

#define TAMANIO_I  5
#define TAMANIO_J  21
#include<string>

using namespace std;

/**
    AXIOMAS
    omnibus se asume que empieza vacio
*/

typedef struct{
    string fecha;
    string origen;
    string destino;
    string turno;
    char omnibus[TAMANIO_I][TAMANIO_J] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'}}; //el omnibus se inicializa vacio agregandole 'O´
}Servicio;

void crearServicio(Servicio& servicio);

void eliminarServicio(Servicio& servicio);


string getFecha(Servicio& servicio);

void setFecha(Servicio& servicio  , string fecha);

string  getOrigen(Servicio& servicio);

void setOrigen(Servicio& servicio,string origen);

string  getDestino(Servicio& servicio);

void setDestino(Servicio& servicio,string destino);

string getTurno(Servicio& servicio);

void setTurno(Servicio& servicio , string turno);


//char** getOmnibus(Servicio& servicio);//retorna un puntero que apunta a la primer posicion de  memoria en donde se encuentra la matriz del servicio indicado

void setOmnibus(Servicio& servicio , char omnibus[TAMANIO_I][TAMANIO_J]);

#endif // _SERVICIO_H_
