#include "Servicio.h"

void crearServicio(Servicio& servicio){
    servicio.fecha="";
    servicio.origen = "";
    servicio.destino = "";
    servicio.turno = "";
}

void eliminarServicio(Servicio& servicio){
}


string getFecha(Servicio& servicio){
    return servicio.fecha;
}
void setFecha(Servicio& servicio , string fecha){
    servicio.fecha = fecha;
}

string  getOrigen(Servicio& servicio){
    return servicio.origen;
}
void setOrigen(Servicio& servicio,string origen){
    servicio.origen = origen;
}


string  getDestino(Servicio& servicio){
    return servicio.destino;
}
void setDestino(Servicio& servicio,string destino){
    servicio.destino = destino;
}

string getTurno(Servicio& servicio){
    return servicio.turno;
}

void setTurno(Servicio& servicio , string turno){
    servicio.turno = turno;
}

/*
char** getOmnibus(Servicio& servicio){
    char ** matriz;

    matriz = new  char*[TAMANIO_I];
    for(int i=0; i<TAMANIO_I ; i++){
        matriz[i] = new char[TAMANIO_J];
    }
    for(int i=0;i<TAMANIO_I;i++){
        for(int j=0;j<TAMANIO_J; j++){
            matriz[i][j] = servicio.omnibus[i][j];
        }
    }
    return matriz;
}
*/
void setOmnibus(Servicio& servicio, char omnibus[TAMANIO_I][TAMANIO_J] ){
       for (int i =0; i<TAMANIO_I;i++){
        for (int j=0; j<TAMANIO_J;j++){
                servicio.omnibus[i][j] = omnibus[i][j];
        }
    }
}
