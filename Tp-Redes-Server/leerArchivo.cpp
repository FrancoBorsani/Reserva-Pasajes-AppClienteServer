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
    string soloButacas;
cout<<endl;
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
vector <string> leerArchivoGuardarEnVectorString(string nombreArchivo)
{
    vector <string> butacasEnSting;
    nombreArchivo = nombreArchivo+".txt";
    ifstream archivo;//ifstream(tipo de variable para abrir un archivo)...  archivo (nombre de la variable)
    string texto;// variable string

    archivo.open(nombreArchivo.c_str(),ios::in);// con archivo.open le digo que quiero abrir un archivo y con ios::in le digo que abro para leerlo

    if(archivo.fail())//si hay un error y no se abre el arvhivo
    {
        cout<<"No se ha podido abrir archivo"<<endl;//se muestra
        exit(1);//sale
    }
    int i=0;
    //si se abre
    while(!archivo.eof())//mientras no sea el final del archivo
    {
           getline(archivo,texto);//Tomo lo que va encontrando en "archivo" y lo copio en "texto"
           butacasEnSting.push_back(texto);//guardo en una posición del vector la lines obtenidad del archivo
           i++;
    }
   archivo.close();//cerramos archivo
   return butacasEnSting;
}
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
