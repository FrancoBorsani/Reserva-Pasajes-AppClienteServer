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
    verificarArchivoServerLog();
    registrarServerLog("Inicia servidor");

    string puertoEscucha = "Socket creado. Puerto de escucha: " + std::to_string(PUERTO_GLOBAL);
    registrarServerLog(puertoEscucha);

    while(true){
        setlocale(LC_CTYPE,"Spanish");// Spanish (de la librería locale.h) es para usar ñ y acento

        Server *Servidor = new Server();

        string userName = checkUser(Servidor);

        manejarPeticion(userName, Servidor);

        Servidor->CerrarSocket(userName);

        system("cls");

        renovacionDeMicrosDisponibles();
    }

   // main();
}

