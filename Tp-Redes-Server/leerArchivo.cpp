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



/***********************************************************************/
string traerSoloButacas(vector <string> vectorButacas){
    string soloButacas = "";
    for(int i=0;i<11;i++){
        if(i>6&&i!=9){
          for(int j=0;j<43;j++){
              if(j>3 && j%2==0 && j<42){
                 soloButacas=soloButacas+vectorButacas[i][j];
              }else if(j==42){
                 soloButacas=soloButacas+vectorButacas[i][j];
              }//if interno
          }//for j
         }//if i>3
    }//for i
    return soloButacas;
}
/***********************************************************************/


/***********************************************************************/
vector <string> leerArchivoBinarioGuardarEnVectorString(string nombreArchivo)
{
    vector <string> vecString;

    nombreArchivo=nombreArchivo+".bin";

    string texto = "";

    ifstream archivo;//ifstream(tipo de variable para abrir un archivo)...  archivo (nombre de la variable)
    archivo.open(nombreArchivo.c_str(),std::ios::in | ios :: binary);
    if(archivo.is_open()){
        while(!archivo.eof()){
           getline(archivo,texto);//Tomo lo que va encontrando en "archivo" y lo copio en "texto"
           vecString.push_back(texto);//guardo en una posición del vector la linea obtenida del archivo
        }
        archivo.close();
    }

    return vecString;
}
/***********************************************************************/


/***********************************************************************/
vector<string> separarPalabrasPuntoYComa(string str) {

        int posInit = 0;
        int posFound = 0;
        string splitted;
        char pattern = ';';
        vector<string> resultados;

        while(posFound >= 0){
            posFound = str.find(pattern, posInit);
            splitted = str.substr(posInit, posFound - posInit);
            posInit = posFound + 1;
            resultados.push_back(splitted);
        }

        return resultados;
}
/***********************************************************************/


/***********************************************************************/
bool siFechaActualEsMayor(string str){
     bool fechaEsActualMayor = false;
    int *fechaActual = traerFechaActual();
    vector<string> strFecha = separarPalabrasPuntoYComa(str);

    int * fecha = separarFechaPasarAInt(strFecha[0]);

    if(compararDosFechas(fechaActual, fecha)>0){
        fechaEsActualMayor = true;
    }
   return fechaEsActualMayor;
}
/***********************************************************************/


/***********************************************************************/
int *traerFechaActual(){//LIBRERIA <ctime>
    int static fecha[3];
    time_t now = time(0);/* fecha/hora actual basado en el sistema actual */
    tm * time = localtime(&now);/* Objeto de una estructura tm con fecha/hora local */
    int year = 1900 + time->tm_year;//año correcto    (en tm_year;  year arranca desde 1900)
    fecha[0] = time->tm_mday;
    fecha[1] = time->tm_mon+1;//le sumo uno porque arranca en 0 (Enero=0)
    fecha[2] = year;
    return fecha;
}
/***********************************************************************/


/***********************************************************************/
int compararDosFechas(int fecha[3], int fecha2[3])//retorna; int x=0 si las fechas son iguales, int x>0 si la primera fecha es mayor, o int x<0 si la segunda fecha es mayor (el máximo positivo o negativo que puede retornar es 371)
{   int resultado;
    int resultado2;
     if(fecha[2]==fecha2[2]){//si los años son igules
        resultado= fecha[0] + fecha[1]*31;  //hago dia + mes*31 lo guardo en resultado (31 es por el máximo dias que puede tener un mes)
        resultado2=fecha2[0] + fecha2[1]*31;//hago lo mismo que en la enterior (si tengo mes/dia = 31/1   mes2/dia2 = 1/2 y no multiplico por 31 daría que la primera es mayor)
        resultado=resultado - resultado2;//resultado=0 las fechas son iguales, resultado>0 la primera fecha es mayor y resultado<0 la segunda fecha es mayor
     }
     else if(fecha[2]<fecha2[2]){ resultado=-1;}
     else if(fecha[2]>fecha2[2]){ resultado=1; }

 return resultado;
}
/***********************************************************************/


/***********************************************************************/
int *separarFechaPasarAInt(string input_fecha)
{

  char *str=const_cast< char *>(input_fecha.c_str());//castea string a char arrays
	char * ptr;
	ptr = strtok(str,"-");
	int static fechaAux[3];//debe ser static para que perdure cuando termine esta función
	int i =0;
	while (ptr != NULL)
	{
		fechaAux[i] = atoi(ptr);//

	    ptr = strtok (NULL, "-");
	    i++;
	}

 return fechaAux;//lo que retorna debe ser recibido por un un int puntero (int *variable)
}
/***********************************************************************/
