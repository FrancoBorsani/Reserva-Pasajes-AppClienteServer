#ifndef ESCRIBIRARCHIVO_H_INCLUDED
#define ESCRIBIRARCHIVO_H_INCLUDED
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

bool crearArchivoButacas(string nombreArchivo,string tituloArchivo);
void registrarViajesEnArchivo(string nombreArchivo);
bool verificarSiDuplicaDato(string destinoFechaTurno ,string butacasReservadas);
void darFormato_y_GuardarButacasEnArchivo(string nombreArchivo, string titulo,char butacas[TAMANIO_I][TAMANIO_J]);
void guardarEnArchivoConFormato(string lineaAGuardar, string nombreArchivo);
void guardarEnArchivoSinFormato(string lineaAGuardar, string nombreArchivo);
void guardarEnArchivoBinario(string lineaAGuardar, string nombreArchivo);
bool verificarSiExisteArchivo(string nombreArchivo);
bool verificarSiExisteArchivoBinario(string nombreArchivo);
void actualizarCambiosEnArchivo(vector <string> vecString,string nombreArchivo);
int asignarValorPosI_A_Letra(char letra);
void iniciarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
void mostrarButacas(vector <string> vectorButacas);
string butacaAString(int pos_I, int pos_J);
string getIdServicio(string nombreArchivo);
void marcarButacaComoOcupada(vector <string> vectorButacas, int pos_I, int pos_J, string userName, string nombreArchivo);
void marcarButacaComoLiberada(vector <string> vectorButacas, int pos_I, int pos_J, string userName, string NombreArchivo);
void registrarUserLog(string evento, string aRegistrar);
void crearArchivoUserLog(string usuario);


#endif // ESCRIBIRARCHIVO_H_INCLUDED
