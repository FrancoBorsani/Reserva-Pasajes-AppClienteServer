#ifndef LEERARCHIVO_H_INCLUDED
#define LEERARCHIVO_H_INCLUDED
#include <iostream>
#include <winsock2.h>
#include <string>
#include <conio.h>
#include <clocale>//es para usar � y acento
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
vector <string> leerArchivoBinarioGuardarEnVectorString(string nombreArchivo);
vector<string> separarPalabrasPuntoYComa(string str);
bool siFechaActualEsMayor(string str);
int *traerFechaActual();
int compararDosFechas(int fecha[3], int fecha2[3]);
int *separarFechaPasarAInt(string input_fecha);


#endif // LEERARCHIVO_H_INCLUDED
