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

    cargarServiciosEnVector();//carga el nombre del archivo de los servicios creados en el vector por si el sistema se cerro de forma inesperada

    string userName = checkUser(Servidor);
    string nombreArchivo = "Registro_de_butacas";
    string tituloArchivo = ">>> REGISTRO DE BUTACAS VACIAS Y OCUPADA <<<";
    crearArchivoButacas(nombreArchivo,tituloArchivo);

    manejarPeticion(nombreArchivo,userName, Servidor);

    Servidor->CerrarSocket();

    system("cls");

    main();
}
