#include "serverClass.h"
#include "leerArchivo.h"
#include "escribirArchivo.h"
#include <iostream>
#include <winsock2.h>
#include <algorithm>
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

vector<string> archivos_servicios;


/**********************************************************************/
void renovacionDeMicrosDisponibles(){
    vector <string> vecStringAux;
    if(verificarSiExisteArchivo("Archivos_activos")){
            string servicio = "";
            ifstream archivoServiciosRespaldo;
            archivoServiciosRespaldo.open("Archivos_activos.txt" ,ios::in);
            while(!archivoServiciosRespaldo.eof()){ ///el archivo  puede contener hasta 6 lineas , cada linea hace refencia a un servicio
                getline(archivoServiciosRespaldo , servicio);
                if(servicio != ""){
                   if(siFechaActualEsMayor(servicio)==false){//si los servicios tienen fechas que son igual o mayor a la fecha actual
                      archivos_servicios.push_back(servicio);//guardo el  servicio en el vector GLOBAL
                      vecStringAux.push_back(servicio);//guardo el  servicio en el vector que uso en el if (vecStringAux.size()>0)
                   }else{//si la fecha actual es mayor
                     registrarViajesEnArchivo(servicio);//los paso a un archivo general (servicio realizado)
                     servicio=servicio+".txt";
                     remove(servicio.c_str());//borro el archivo individual
                   }
                }

                //limpiamos la cadena para la prox iteracion
                servicio.clear();
            }
            archivoServiciosRespaldo.close();

             if(vecStringAux.size()>0){//si quedó para dejar en el archivo "Archivos_activos"
                   actualizarCambiosEnArchivo(vecStringAux,"Archivos_activos");//saco lo que pasé al archivo definitivo
            }else if(vecStringAux.size()==0){ remove("Archivos_activos.txt"); }//Si no quedó ni un registro con fecha igual o superior a la actual BORRO EL ARCHIVO y se generará cuado sea necesario

    }else{
       cout << "Error, no existe el archivo: Archivos_activos.txt"<< endl;
       exit(1);
    }
}
/************************************************************************/


/***********************************************************************/
void crearServicio(string userName , Server*& servidor){
    /***Datos recibidos del  usuario****/
    string fechaRecibida = servidor->Recibir();
    string origenRecibido = servidor->Recibir();
    string turnoRecibido  = servidor->Recibir();
    /*****************************/
    turnoRecibido[0] = toupper(turnoRecibido[0]);

    string msg = "";
    string nombreArchivo = "";
    string tituloArchivo = "";
    if(origenRecibido=="mar del plata"){nombreArchivo=fechaRecibida+";Mar_Del_Plata;"+turnoRecibido; tituloArchivo=fechaRecibida+" "+"Mar_Del_Plata"+" "+turnoRecibido;}
    else if(origenRecibido=="buenos aires"){nombreArchivo=fechaRecibida+";Buenos_Aires;"+turnoRecibido; tituloArchivo=fechaRecibida+" "+"Buenos_Aires"+" "+turnoRecibido;}

    if(crearArchivoButacas(nombreArchivo , tituloArchivo)){ ///si el servicio no existe crea su registro con sus correspondientes datos
        registrarUserLog("Crea el servicio con los datos ("+tituloArchivo+")" , userName);///registro la accion del usuario en su archivo
        guardarEnArchivoSinFormato(nombreArchivo, "Archivos_activos"); ///registro el servicio creado en un archivo de respaldo

        msg = "El Servicio ("+tituloArchivo+") fue creado correctamente";
        archivos_servicios.push_back(nombreArchivo); ///agrego el nombre del archivo en el vector
    }
    else{
            msg = "El Servicio ("+tituloArchivo+") que quiere crear ya existe";
    }

        servidor->Enviar(msg); ///le informa al usuario el resultado de la operacion
}
/***********************************************************************/



void registrarServerLog(string evento){
    std::ofstream serverLog("server.log", std::ios::ate | std::ios::in);
    if(serverLog.fail()){ //Si el archivo no se encuentra o no esta disponible o presenta errores
            cout<<"No se pudo abrir el archivo server log"; //Muestra el error
                        }
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    if(evento == "Inicia servidor"){
        serverLog<<endl<<buf<<": ==================================="<<endl;
        serverLog<<buf<<": "<<evento<<endl;
        serverLog<<buf<<": ==================================="<<endl;
    }else{
    serverLog<<buf;
    serverLog<<": "<<evento<<endl;
    }

    serverLog.close();
}


void verificarArchivoServerLog(){
    string nombreArchivo = "server.log";
  std::ifstream serverLog( nombreArchivo );
  if(serverLog.fail()){
    //EL ARCHIVO NO EXISTE
    std::ofstream serverLogCrear( nombreArchivo );
  }

  serverLog.close();
}
/***********************************************************************/


/***********************************************************************/
void mostrarRegistro(string userName, Server *&Servidor){

    std::string userFile = userName+".log";
    std::string numero = std::to_string(numeroDeSentencias(userFile));
    Servidor->Enviar(numero);
    fstream file;
    file.open(userFile);

    if(file.is_open()){
        for(int i = 0 ; i < stoi(numero); i++){
            string linea = "";
            getline(file, linea);
            Servidor->Enviar(linea);
        }
    }
    file.close();

}
/***********************************************************************/


string checkUser(Server *&Servidor)
{
    string usuarioEncontrado = "false";
    vector<string> resultados;
    vector<string> userAndPass;
    int contador = 0;
    string loggedUser = "";


    while(contador<3 && usuarioEncontrado == "false"){

        string linea;
        fstream file;

        file.open("users.dat", ios::in);

        userAndPass = separarPalabrasPuntoYComa(Servidor->Recibir());

        if(file.is_open()){
            while(!file.eof()){

                getline(file, linea);

                resultados = separarPalabrasPuntoYComa(linea);

                if(resultados[0] == userAndPass[0] && resultados[1] == userAndPass[1]){
                        usuarioEncontrado = "true";
                        verificarArchivoServerLog();
                        registrarServerLog("Usuario autenticado" + resultados[0]);
                        crearArchivoUserLog(resultados[0]);
                        registrarUserLog("Inicia sesion", resultados[0]);
                        loggedUser = resultados[0];
                }


            }
        }

        file.close();

     //   usuarioEncontrado == "true" ? cout<<"Usuario Encontrado"<<endl<<endl : cout<<"Crendenciales invalidas..."<<endl<<endl<<"Por favor ingrese sus datos nuevamente (Le quedan " << " intentos)"<<endl<<endl;
        if(usuarioEncontrado == "true"){
            cout<<"Usuario Encontrado"<<endl<<endl;

        }else{
        cout<<"Crendenciales invalidas..."<<endl<<endl<<"Por favor ingrese sus datos nuevamente (Le quedan " << " intentos)"<<endl<<endl;
        verificarArchivoServerLog();
        registrarServerLog("Intento de ingreso - Credenciales invalidas");
        }

        contador++;

        if(usuarioEncontrado == "true") {
            contador = 4;
        }

        Servidor->Enviar(usuarioEncontrado+";"+to_string(contador));

    }
  return loggedUser;
}


/***********************************************************************/
void gestionarAsiento(string nombreArchivo,Server *&Servidor, string userName, bool reservar){

    vector <string> vectorButacas = leerArchivoGuardarEnVectorString(nombreArchivo);

    Servidor->Enviar(traerSoloButacas(vectorButacas));

    string salir = "false";

    bool salirWhile = false;

    while(!salirWhile){

        vectorButacas = leerArchivoGuardarEnVectorString(nombreArchivo);
        salir = verificarSolicitud_Y_Responder(Servidor,vectorButacas, userName, reservar, nombreArchivo);

        if(salir=="true"){
            salirWhile=true;
        }
        else if(salir!="true" && salir!="false"){//es porque cuando se desconecta el cliente  cerrando la ventana llegaba vacio
            salir ="true";
            salirWhile = true;
            system("cls");
        }
    }

}
/***********************************************************************/


/***********************************************************************/
string verificarSolicitud_Y_Responder(Server *&Servidor,vector <string> vectorButacas, string userName, bool reservar, string nombreArchivo){
    string mensajePeticion = "";
    string mensajeDelCli="";
    char letra = '\0';
    int pos_J = -1;
    int pos_I = -1;

    mensajeDelCli = Servidor->Recibir(); //Se recibe la butaca. EJ: B8

    if(mensajeDelCli!="0"){
        letra = mensajeDelCli[0];
        mensajeDelCli.erase(0,1);//Saco la letra que guerdé
        pos_J =atoi(const_cast< char *>(mensajeDelCli.c_str()));
        pos_J = pos_J*2+2; //es por la diferencia que hay entre la posicion de vista en consola y la del archivo
        pos_I = asignarValorPosI_A_Letra(letra);


        if(vectorButacas[pos_I][pos_J]=='O' && reservar){
           Servidor->Enviar("Disponible");//está disponible

           marcarButacaComoOcupada(vectorButacas, pos_I, pos_J, userName, nombreArchivo);
           mensajePeticion = Servidor->Recibir();
        }
        else if(vectorButacas[pos_I][pos_J]=='X' && !reservar){
            Servidor->Enviar("Disponible");//está disponible

            marcarButacaComoLiberada(vectorButacas, pos_I, pos_J, userName, nombreArchivo);
            mensajePeticion = Servidor->Recibir();
        }
        else{
            Servidor->Enviar("NoDisponible");//está disponible
            //mostrarButacas(vectorButacas);
            mensajePeticion = Servidor->Recibir();
        }
    }
    else{
        mensajePeticion = Servidor->Recibir();
    }

    return mensajePeticion;
}
/***********************************************************************/

vector<string> separarDatos(string datosAutobus){
    vector<string> datos;

    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;

    //Separo los datos
    while ((pos = datosAutobus.find(delimiter)) != std::string::npos) {
        token = datosAutobus.substr(0, pos);
        datos.push_back(token);
        datosAutobus.erase(0, pos + delimiter.length());
    }

    datos.push_back(datosAutobus);

    return datos;
}

bool checkIfMultipleBus(string datosAutobus){

    vector<string> datos = separarDatos(datosAutobus);

    bool fechaVacia = false;
    bool origenVacio = false;
    bool turnoVacio = false;

    if(datos[0]=="0-0-0"){
        fechaVacia = true;
    }

    if(datos[1]=="0"){
        origenVacio = true;
    }

    if(datos[2]=="0"){
        turnoVacio = true;
    }


    return (fechaVacia || origenVacio ||turnoVacio);
}

/***********************************************************************/
vector<string> leerArchivoGeneral(){
    string archivoGeneral = "Archivos_activos.txt";

    string texto = "";

    fstream archivo;

    archivo.open(archivoGeneral);


    vector<string> archivosActivos;

    if(archivo.is_open()){
        while(!archivo.eof()){
           getline(archivo,texto);//Tomo lo que va encontrando en "archivo" y lo copio en "texto"
           archivosActivos.push_back(texto);//guardo en una posición del vector la linea obtenida del archivo
        }
        archivo.close();
    }

    return archivosActivos;
}


vector<string> elegirAutobus(string datosAutobus){

    vector<string> datos = separarDatos(datosAutobus);

    vector<string> archivosActivos = leerArchivoGeneral();

    vector<string> archivosADevolver;

    for(int i = 0 ; i < archivosActivos.size(); i++){
        vector<string> currentBusData = separarDatos(archivosActivos[i]);

        if( (datos[0]=="0-0-0" || currentBusData[0]==datos[0]) &&
            (datos[1]=="0" || currentBusData[1]==datos[1]) &&
            (datos[2]=="0" || currentBusData[2]==datos[2])) {

            archivosADevolver.push_back(archivosActivos[i]);
        }
    }


    return archivosADevolver;
}
/***********************************************************************/
string changeNameIfMultipleBus(string nombreArchivoAutobus, Server *&Servidor){

    if(checkIfMultipleBus(nombreArchivoAutobus)){

        vector<string> autobusesDisponibles = elegirAutobus(nombreArchivoAutobus);
        Servidor->Enviar( std::to_string(autobusesDisponibles.size()) );
        for(int i = 0 ; i < autobusesDisponibles.size(); i++){
            Servidor->Enviar(autobusesDisponibles[i]);
        }
        string autobusElegido = Servidor->Recibir();

        vector<string> archivosActivos = leerArchivoGeneral();

        nombreArchivoAutobus = autobusElegido;
    }


    return nombreArchivoAutobus;
}

/***********************************************************************/
void manejarPeticion(string userName, Server *&Servidor){
    string peticion="";
    bool salir = false;
    string opcionesPosibles=" ";

    while(!salir){

        salir = true;
        peticion = Servidor->Recibir();

        if(peticion=="Registro"){
            mostrarRegistro(userName,Servidor);
            salir = false;
        }
        else if(peticion=="AltaServicio"){
            crearServicio(userName, Servidor);
            salir = false;
        }
        else if(peticion=="Gestionar"){
            salir = false;

            string nombreArchivoAutobus = Servidor->Recibir();

            nombreArchivoAutobus = changeNameIfMultipleBus(nombreArchivoAutobus, Servidor);

            vector <string> butacasAutobus = leerArchivoGuardarEnVectorString(nombreArchivoAutobus);

            if(!butacasAutobus.empty()){

                Servidor->Enviar(traerSoloButacas(butacasAutobus));

                string opcion = Servidor->Recibir();

                if(opcion=="ReservarAsiento"){
                    gestionarAsiento(nombreArchivoAutobus,Servidor, userName,true);
                    opcion = "";     salir = false;
                }
                else if(opcion=="LiberarAsiento"){
                    gestionarAsiento(nombreArchivoAutobus,Servidor, userName, false);
                    opcion = "";     salir = false;
                }
                else if(opcion=="ElegirOtroServicio"){

                    string nombreArchivoAutobusAux = Servidor->Recibir();

                    nombreArchivoAutobusAux = changeNameIfMultipleBus(nombreArchivoAutobusAux, Servidor);

                    vector <string> butacasAutobusAux = leerArchivoGuardarEnVectorString(nombreArchivoAutobusAux);

                    if(!butacasAutobusAux.empty()){
                         Servidor->Enviar(traerSoloButacas(butacasAutobusAux));
                    }
                    else Servidor->Enviar("ServicioInexistente");

                    opcion = "";     salir = false;
                }
            }
            else Servidor->Enviar("ServicioInexistente");
        }

        peticion="";


    }
}
/***********************************************************************/


/***********************************************************************/
int numeroDeSentencias(std::string archivo){

        int contador = 0;

        fstream file;

        file.open(archivo);

        if(file.is_open()){
            while(!file.eof()){
                string linea = "";
                getline(file, linea);
                contador++;
            }
        }
        file.close();
    return contador;
};
/***********************************************************************/
