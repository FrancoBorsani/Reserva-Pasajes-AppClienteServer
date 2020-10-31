#include "serverClass.h"
#include "leerArchivo.h"
#include "escribirArchivo.h"
#include <iostream>
#include <winsock2.h>
#include <string>
#include <conio.h>
#include <clocale>//es para usar ñ y acento
#include <fstream> //Lib. para trabajar con archivos
#include <ctime> //Lib. para trabajar con fechas / tiempos
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <windns.h>

#define TAMANIO_I  5
#define TAMANIO_J  21

using namespace std;




int main()
{
    setlocale(LC_CTYPE,"Spanish");// Spanish (de la librería locale.h) es para usar ñ y acento

    Server *Servidor = new Server();

    renovacionDeMicrosDisponibles();//Pasa a un archivo general y borra archivos los individuales de los servicios de fechas anteriores a la actual y carga el nombre del archivo de los servicios ACTIVOS en el vector por si el sistema se cerro de forma inesperada

    string respuesta = checkData(Servidor);

    Servidor->Enviar(respuesta);

    while(respuesta!="true"){
        respuesta = checkData(Servidor);
        Servidor->Enviar(respuesta);
    }


    string userName = checkUser(Servidor);

    manejarPeticion(userName, Servidor);

    Servidor->CerrarSocket();

    system("cls");

    main();
}
