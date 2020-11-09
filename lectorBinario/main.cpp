#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

int main()
{
    //LEER DEL ARCHIVO BINARIO
    string mensaje;
    ifstream leerArchivo ("../Tp-Redes-Server/info_servicios.bin", ios::in | ios::out | ios::binary);

    while (true) {
        getline(leerArchivo,mensaje);
        cout<<mensaje<<endl;
        if( leerArchivo.eof() ) break;
    }


    //OTRA FORMA DE LECTURA
  //  leerArchivo.seekg (0); //POSICIÓN DESDE LA QUE EMPIEZA A LEER
  //  leerArchivo.read (mensaje, 4000);

    leerArchivo.close();


    return 0;
}
