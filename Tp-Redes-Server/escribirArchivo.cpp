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
bool crearArchivoButacas(string nombreArchivo,string tituloArchivo){
    bool archivoCreado = false;
    if(verificarSiExisteArchivo(nombreArchivo)==false){//solo es paracrear un archivo
        char butacas[TAMANIO_I][TAMANIO_J];
        iniciarButacas(butacas);
        darFormato_y_GuardarButacasEnArchivo(nombreArchivo,tituloArchivo,butacas);
        archivoCreado = true;
    }
    return archivoCreado; ///para identificar si el servicio ya esta registrado e informarle al cliente
}
/***********************************************************************/


/***********************************************************************/
void registrarViajesEnArchivo(string nombreArchivo){
   vector <string> vectorButacas = leerArchivoGuardarEnVectorString(nombreArchivo);
   string destinoFechaTurno = vectorButacas[1];
   string butacas = traerSoloButacas(vectorButacas);
   string butacasReservadas="";
   for(int i=0;i<60;i++){
        if(i<20&&butacas[i]=='X'){
          butacasReservadas = butacasReservadas+"A"+to_string(i+1)+"_";
        }else if(i>=20&&i<40&&butacas[i]=='X'){
          butacasReservadas = butacasReservadas+"B"+to_string(i-19)+"_";
        }else if(i>=40&&butacas[i]=='X'){
          butacasReservadas = butacasReservadas+"C"+to_string(i-39)+"_";
        }
   }//for i
   butacasReservadas.pop_back();//saco el último guion que le queda (no se puede igualar directamente a un string)
   guardarEnArchivoBinario(destinoFechaTurno+" "+butacasReservadas,"info_servicios");
}
/***********************************************************************/

/***********************************************************************/
void darFormato_y_GuardarButacasEnArchivo(string nombreArchivo, string titulo,char butacas[TAMANIO_I][TAMANIO_J]){
    string lineaAGuardar;
    guardarEnArchivoConFormato(titulo,nombreArchivo);
    for (int i =0; i<TAMANIO_I;i++){
        if(i==2){guardarEnArchivoConFormato("-------------------------------------------",nombreArchivo);}
        if(i==4){guardarEnArchivoConFormato("===========================================",nombreArchivo);}
        for (int j=0; j<TAMANIO_J;j++){
           if(i==0&&j<1){lineaAGuardar=lineaAGuardar+"  "+butacas[i][j]+" ";
           }else if(j<1){lineaAGuardar=lineaAGuardar+" "+butacas[i][j]+" |";
           }else{lineaAGuardar=lineaAGuardar+butacas[i][j]+" ";}
        }//cierro el for de columna
        guardarEnArchivoConFormato(lineaAGuardar,nombreArchivo);
        lineaAGuardar="";

    }//Cierro el for de renglon

}
/***********************************************************************/

/***********************************************************************/
void guardarEnArchivoConFormato(string lineaAGuardar, string nombreArchivo){
      bool NO_EsPrimeraCarga = verificarSiExisteArchivo(nombreArchivo);// Si no existe el archivo quiere decir que va a ser la primera cargaga de datos (ya que cuando lo creo cargo datos)
   nombreArchivo= nombreArchivo+".txt";
   ofstream archivo(nombreArchivo.c_str(),ios::out | ios::app);
        if(NO_EsPrimeraCarga==false){//si es la primera carga
          archivo<<"\n"<<lineaAGuardar<<"\n\n"; //pongo título y salto de linea al final para dejar un reglón vacio
        }else{//si NO es la primera carga, pongo salto de linea al comienzo
          archivo<<"\n"<<lineaAGuardar;
        }/*sin este if me generaba una linea demás, al comienzo del archivo, por el salto de linea. Si pusiese el salto de linea al final genería la linea demás al final del
         archivo y no tengo forma manejarlo cuando... en ambos casos al mostrar los registros del archivo me mostraría uno demás en blanco arriba o abajo*/
   archivo.close();
}
/***********************************************************************/

/***********************************************************************/
void guardarEnArchivoSinFormato(string lineaAGuardar, string nombreArchivo){
    nombreArchivo= nombreArchivo+".txt";
    ofstream archivo(nombreArchivo.c_str(),ios::out | ios::app);

    archivo<<lineaAGuardar<<"\n"; //solo pongo la linea

    archivo.close();
}
/***********************************************************************/


/***********************************************************************/
void guardarEnArchivoBinario(string lineaAGuardar, string nombreArchivo){

    nombreArchivo=nombreArchivo+".bin";
    ofstream archivoBin (nombreArchivo.c_str(), std::ios::out | std::ios::app | ios :: binary);

    archivoBin<<lineaAGuardar<<"\n";

    archivoBin.close();
}
/***********************************************************************/


/***********************************************************************/
bool verificarSiExisteArchivo(string nombreArchivo){
    nombreArchivo= nombreArchivo+".txt";
    bool yaExisteArchivo = true;
    ifstream archivo;//ifstream(tipo de variable para abrir un archivo)...  archivo (nombre de la variable)
    archivo.open(nombreArchivo.c_str(),ios::in);// con archivo.open le digo que quiero abrir un archivo y con ios::in le digo que abro para leerlo
    if(archivo.fail())//si hay un error y no se abre el arvhivo
    {
        yaExisteArchivo = false;
    }
    archivo.close();//cerramos archivo
 return yaExisteArchivo;
}
/***********************************************************************/



/***********************************************************************/
bool verificarSiExisteArchivoBinario(string nombreArchivo){
    nombreArchivo= nombreArchivo+".bin";
    bool yaExisteArchivo = true;
    ifstream archivo;//ifstream(tipo de variable para abrir un archivo)...  archivo (nombre de la variable)
    archivo.open(nombreArchivo.c_str(),std::ios::in | ios :: binary);// con archivo.open le digo que quiero abrir un archivo y con ios::in le digo que abro para leerlo
    if(archivo.fail())//si hay un error y no se abre el arvhivo
    {
        yaExisteArchivo = false;
    }
    archivo.close();//cerramos archivo
 return yaExisteArchivo;
}
/***********************************************************************/



/***********************************************************************/
int asignarValorPosI_A_Letra(char letra){
    int pos_I = -1;

    if(letra=='A' || letra == 'a'){
        pos_I=7;
    }
    else if(letra=='B' || letra == 'b'){
        pos_I=8;
    }
    else {
        pos_I=10;
    }

    return pos_I;
}
/***********************************************************************/


/***********************************************************************/
void iniciarButacas(char butacas[TAMANIO_I][TAMANIO_J]){
    char numeros[20] = {'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0'};
    char letras[5] = {' ',' ','A','B','C'};
    for (int i =0; i<TAMANIO_I;i++){
        for (int j=1; j<TAMANIO_J;j++){
           if(j<=9){butacas[0][j] = letras[0];}else if(j<=19){butacas[0][j] = '1';}else{butacas[0][j] = '2';}
           butacas[1][j] = numeros[j-1];
           butacas[i][0] = letras[i];
           butacas[i][j] = 'O';
        }
    }
}
/***********************************************************************/


/***********************************************************************/
void mostrarButacas(vector <string> vectorButacas){
    for(int i=0;i<11;i++){
        if(i>3){
         cout<<vectorButacas[i]<<endl;
        }
    }
}
/***********************************************************************/


/**********************************************************************/
string butacaAString(int pos_I, int pos_J){
    char letra = '\0';

    pos_J = pos_J/2-2;
    pos_J++;
    std::string posJ_str = std::to_string(pos_J);

    if(pos_I == 7) letra = 'A';
    else if(pos_I == 8) letra = 'B';
    else letra = 'C';

    string butaca = letra+posJ_str;

    return butaca;
}
/**********************************************************************/

string getIdServicio(string nombreArchivo){

    vector<string> datos;

    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;

    //Separo los datos
    while ((pos = nombreArchivo.find(delimiter)) != std::string::npos) {
        token = nombreArchivo.substr(0, pos);
        datos.push_back(token);
        nombreArchivo.erase(0, pos + delimiter.length());
    }

    string fecha = datos[0];

    for(int i = 0 ; i < fecha.length(); i++){
        if(fecha[i]=='-'){
            fecha.erase(fecha.begin()+i);
        }
    }

    string origen = datos[1];
    string turno = nombreArchivo;

   return ""+fecha+origen[0]+turno[0];
}


/***********************************************************************/
void marcarButacaComoOcupada(vector <string> vectorButacas, int pos_I, int pos_J, string userName, string nombreArchivo){
        vectorButacas[pos_I][pos_J] = 'X';
        actualizarCambiosEnArchivo(vectorButacas, nombreArchivo);

        string idServicio = getIdServicio(nombreArchivo);

        string butaca = butacaAString(pos_I, pos_J);
        string reserva = idServicio+" - Reserva_";
        reserva+=butaca;
        registrarUserLog(reserva, userName);
        system("cls");
        mostrarButacas(vectorButacas);
        cout<<"************************************"<<endl;
        cout<<"** Butaca reservada exitosamente. **"<<endl;
        cout<<"************************************"<<endl;
 }
/**********************************************************************/


/**********************************************************************/
 void actualizarCambiosEnArchivo(vector <string> vecString,string nombreArchivo){
    nombreArchivo= nombreArchivo+".txt";
    ofstream archivoAuxiliar;
    archivoAuxiliar.open("auxiliar.txt",ios::out);
    if(archivoAuxiliar.is_open()){
       for(int i=0;i<(int)vecString.size();i++){
           if(i==0){
             archivoAuxiliar<<vecString[i];
           }else{
             archivoAuxiliar<<"\n"<<vecString[i];
           }
       }//Fin for
    }else{
        cout<<"No se pudo abrir el archivo o aun no ha sido creado"<<endl;
    }
    archivoAuxiliar.close();
    remove(nombreArchivo.c_str());
    rename("auxiliar.txt",nombreArchivo.c_str());
}
 /**********************************************************************/


/**********************************************************************/
 void marcarButacaComoLiberada(vector <string> vectorButacas, int pos_I, int pos_J, string userName, string nombreArchivo){
        vectorButacas[pos_I][pos_J] = 'O';
        actualizarCambiosEnArchivo(vectorButacas, nombreArchivo);

        string idServicio = getIdServicio(nombreArchivo);

        string butaca = butacaAString(pos_I, pos_J);
        string libera = idServicio+" - Libera_";
        libera+=butaca;
        registrarUserLog(libera, userName);
        system("cls");
        mostrarButacas(vectorButacas);
        cout<<"************************************"<<endl;
        cout<<"** Butaca liberada exitosamente. **"<<endl;
        cout<<"************************************"<<endl;
 }
/***********************************************************************/


void crearArchivoUserLog(string usuario){
    string nombreArchivo = usuario + ".log";
  std::ifstream userLog( nombreArchivo );
  if(userLog.fail()){
    //EL ARCHIVO NO EXISTE
    std::ofstream userLogCrear( nombreArchivo );
  }
  userLog.close();
}



void registrarUserLog(string evento, string aRegistrar){
    string nombreArchivo = aRegistrar + ".log";
    std::ofstream userLog( nombreArchivo , std::ios::ate | std::ios::in);
    if(userLog.fail()){ //Si el archivo no se encuentra o no esta disponible o presenta errores
            cout<<"No se pudo abrir el archivo user log"; //Muestra el error
                        }
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    if(evento == "Inicia sesion"){
        userLog<<endl<<buf<<": ==================================="<<endl;;
        userLog<<buf<<": "<<evento<<endl;
        userLog<<buf<<": ==================================="<<endl;
    }else{
    userLog<<buf;
    userLog<<": "<<evento<<endl;
         }
    userLog.close();

    }
