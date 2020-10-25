#ifndef LEERARCHIVO_H_INCLUDED
#define LEERARCHIVO_H_INCLUDED
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

string traerSoloButacas(vector <string> vectorButacas);
vector <string> leerArchivoGuardarEnVectorString(string nombreArchivo);
vector<string> separarPalabrasPuntoYComa(string str);

#endif // LEERARCHIVO_H_INCLUDED
