#include "Servicio.h"

void crearServicio(Servicio& servicio){
    servicio.id = 1;
    servicio.dia = 1;
    servicio.mes = 1;
    servicio.anio = 1;
    servicio.origen = "";
    servicio.destino = "";
}

void eliminarServicio(Servicio& servicio){
}


int getId(Servicio& servicio){
    return servicio.id;
}
void setId(Servicio& servicio,int id){
    servicio.id = id;
}

string getFecha(Servicio& servicio){
    return std::to_string(servicio.dia) + "-"+ std::to_string(servicio.mes) + "-"+  std::to_string(servicio.anio);
}
void setFecha(Servicio& servicio , int dia , int mes , int anio){
    servicio.dia = dia;
    servicio.mes = mes;
    servicio.anio = anio;
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
void setOmnibus(Servicio& servicio, char omnibus[TAMANIO_I][TAMANIO_J] ){
       for (int i =0; i<TAMANIO_I;i++){
        for (int j=0; j<TAMANIO_J;j++){
                servicio.omnibus[i][j] = omnibus[i][j];
        }
    }
}
