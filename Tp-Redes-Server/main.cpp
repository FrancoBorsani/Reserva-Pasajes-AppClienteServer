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

vector<string> archivos_servicios; //en este vector guardo los 6 servicios del dia, mediante su nombreArchivo para poder identificarlos


void registrarServerLog(string evento, string aRegistrar);
void verificarArchivoServerLog();

class Server{
public:
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[1024];
    Server()
    {
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5555);

        bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(server, 0);
        verificarArchivoServerLog();
        registrarServerLog("Inicia servidor", "Socket creado. Puerto de escucha:4747");

        cout << "Escuchando para conexiones entrantes." << endl;

        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            cout << "Cliente conectado!" << endl;
        }
    }

    string Recibir()
    {
        fd_set fds ;
        struct timeval tv ;

        tv.tv_sec = 120 ;
        tv.tv_usec = 0 ;

        FD_ZERO(&fds) ;
        FD_SET(client, &fds) ;

        int n = select (client, &fds, NULL, NULL, &tv) ;

        if (n == 0){
          printf("Timeout..\n");
          // Cerrar sesion
        }
        recv(client, buffer, sizeof(buffer), 0);

        string buff = buffer;

        memset(buffer, 0, sizeof(buffer));
      return buff;
    }

    void Enviar(string respuesta)
    {
        for(int i=0;i<respuesta.length();i++){
            this->buffer[i]= respuesta[i];
        }

        send(client, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    void CerrarSocket()
    {
        closesocket(client);
        WSACleanup();
        verificarArchivoServerLog();
        registrarServerLog("Socket cerrado, cliente desconectado", "");
        cout << "Socket cerrado, cliente desconectado." << endl;
    }
};


void crearServicio(string userName , Server*& servidor);
void registrarServicio_en_archivoRespaldoServicios(string nombreArchivo);//para poder cargar los servicios en el vector
void cargarServiciosEnVector();


bool crearArchivoButacas(string nombreArchivo,string tituloArchivo);
void gestionarAsiento(string nombreArchivo,Server *&Servidor, string userName, bool reservar);
void liberar(Server *&Servidor, string userName);
string verificarSolicitud_Y_Responder(Server *&Servidor,vector <string> vectorButacas, string userName, bool reservar);
void marcarButacaComoOcupada(vector <string> vectorButacas, int pos_I, int pos_J, string userName);
void marcarButacaComoLiberada(vector <string> vectorButacas, int pos_I, int pos_J, string userName);
string butacaAString(int pos_I, int pos_J);
vector<string> separarPalabrasPuntoYComa(string str);
string checkUser(Server *&Servidor);
void registrarServerLog(string evento, string aRegistrar);
void registrarUserLog(string evento, string aRegistrar);
void crearArchivoUserLog(string usuario);


void iniciarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
void mostrarButacas(vector <string> vectorButacas);
int asignarValorPosI_A_Letra(char letra);

void darFormato_y_GuardarButacasEnArchivo(string nombreArchivo, string titulo,char butacas[TAMANIO_I][TAMANIO_J]);
void guardarEnArchivo(string lineaAGuardar, string nombreArchivo);
bool verificarSiExisteArchivo(string nombreArchivo);
vector <string> leerArchivoGuardarEnVectorString(string nombreArchivo);
void guardarEnArchivoYaFormateada(string lineaAGuardar, string nombreArchivo);
void marcarEnArchivoReservaRealizada(vector <string> vectorButacas);
string traerSoloButacas(vector <string> vectorButacas);

int numeroDeSentencias(string file);
void manejarPeticion(string nombreArchivo,string userName, Server *&Servidor);
void mostrarRegistro(string userName, Server *&Servidor);



/************************************
         MAIN
***********************************/
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
/************************************
        FIN  MAIN
***********************************/



/***********************************************************************/
void registrarServicio_en_archivoRespaldoServicios(string nombreArchivo){
    ofstream archivoHistorialServicios;
    archivoHistorialServicios.open("Respaldo_Servicios.txt" , ios::out | ios::app);

    if(!archivoHistorialServicios.is_open()){
        cout<<"Error, no se pudo abrir el archivo: Registro_Historico_Servicios.txt"<<endl;
        exit(1);
    }
    archivoHistorialServicios<<nombreArchivo<<endl;///guardo el nombre del servicio creado, con el fin de tener un respaldo de los servicios  para poder identificarlos a la hora de cargarlos en el vector
    archivoHistorialServicios.close();
}
/***********************************************************************/


/**********************************************************************/
void cargarServiciosEnVector(){
    if(verificarSiExisteArchivo("Respaldo_Servicios")){
            string servicio = "";
            ifstream archivoServiciosRespaldo;
            archivoServiciosRespaldo.open("Respaldo_Servicios.txt" , ios::in);

            if( archivoServiciosRespaldo.fail())
            {
                cout << "Error, no existe el archivo: Respaldo_Servicios.txt"<< endl;
                exit(1);
            }
            while(!archivoServiciosRespaldo.eof()){ ///el archivo  puede contener hasta 6 lineas , cada linea hace refencia a un servicio

                getline(archivoServiciosRespaldo , servicio);
                if(servicio != ""){
                    archivos_servicios.push_back(servicio);//guardo el  servicio en el vector
                }

                //limpiamos la cadena para la prox iteracion
                servicio.clear();
            }
            archivoServiciosRespaldo.close();
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
    string msg = "";
    string nombreArchivo = "";
    string tituloArchivo = "";
    if(origenRecibido=="Mar del Plata"){nombreArchivo=fechaRecibida+";Mar_Del_Plata;"+turnoRecibido; tituloArchivo=fechaRecibida+" "+"Mar_Del_Plata"+" "+turnoRecibido;}
    else if(origenRecibido=="Buenos Aires"){nombreArchivo=fechaRecibida+";Buenos_Aires;"+turnoRecibido; tituloArchivo=fechaRecibida+" "+"Buenos_Aires"+" "+turnoRecibido;}


        if(archivos_servicios.empty()){///si el vector esta vacio
                crearArchivoButacas(nombreArchivo , tituloArchivo); ///crea su registro con sus correspondientes datos
                registrarUserLog("Crea el servicio con los datos ("+tituloArchivo+")" , userName);///registro la accion del usuario en su archivo
                registrarServicio_en_archivoRespaldoServicios(nombreArchivo); ///registro el servicio creado en un archivo de respaldo

                msg = "El Servicio ("+tituloArchivo+") fue creado correctamente";
                archivos_servicios.push_back(nombreArchivo); ///agrego el nombre del archivo en el vector
        }
        else{
            if(archivos_servicios.size() < 6){ ///verifico que ya no existan los 6 servicios del dia
                int pos = 0;
                string fechaRegistrada="";
                ///guardo solo la fecha en la variable fechaRegistrada
                while(archivos_servicios[0][pos] != ';'){
                    fechaRegistrada += archivos_servicios[0][pos];
                    pos++;
                }

                if(fechaRecibida == fechaRegistrada){

                            if(crearArchivoButacas(nombreArchivo , tituloArchivo)){ ///si el servicio no existe crea su registro con sus correspondientes datos
                                registrarUserLog("Crea el servicio con los datos ("+tituloArchivo+")" , userName);///registro la accion del usuario en su archivo
                                registrarServicio_en_archivoRespaldoServicios(nombreArchivo); ///registro el servicio creado en un archivo de respaldo

                                msg = "El Servicio ("+tituloArchivo+") fue creado correctamente";
                                archivos_servicios.push_back(nombreArchivo); ///agrego el nombre del archivo en el vector
                            }
                            else{
                                    msg = "El Servicio ("+tituloArchivo+") que quiere crear ya existe";
                            }

                }
                else{msg = "La fecha("+fechaRecibida+") no coincide con la fecha registrada("+fechaRegistrada+")";}
            }
            else{msg="No se puede crear otro servicio debido a que ya fueron creados los 6 servicios del dia";}
        }

        servidor->Enviar(msg); ///le informa al usuario el resultado de la operacion
}
/***********************************************************************/



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
   guardarEnArchivoYaFormateada(destinoFechaTurno+" "+butacasReservadas,"Registro_historico_de_viajes");
}
/***********************************************************************/


/***********************************************************************/
void gestionarAsiento(string nombreArchivo,Server *&Servidor, string userName, bool reservar){

    vector <string> vectorButacas = leerArchivoGuardarEnVectorString(nombreArchivo);

    Servidor->Enviar(traerSoloButacas(vectorButacas));

    string salir = "false";

    bool salirWhile = false;

    while(!salirWhile){

        vectorButacas = leerArchivoGuardarEnVectorString(nombreArchivo);
        salir = verificarSolicitud_Y_Responder(Servidor,vectorButacas, userName, reservar);

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
void liberar(Server *&Servidor, string userName){

}

/***********************************************************************/

/***********************************************************************/
void manejarPeticion(string nombreArchivo,string userName, Server *&Servidor){
    string peticion="";
    bool salir = false;
    string opcionesPosibles=" ";
    while(!salir){
      salir = true;
        peticion = Servidor->Recibir();
 //       cout<<peticion<<endl;
        //Servidor->Enviar("_");//Envio cualquier cosa para que no dé error

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
            string opcion = Servidor->Recibir();
            if(opcion=="ReservarAsiento"){
                gestionarAsiento(nombreArchivo,Servidor, userName,true);
                opcion = "";     salir = false;
            }
            else if(opcion=="LiberarAsiento"){
                gestionarAsiento(nombreArchivo,Servidor, userName, false);
                opcion = "";     salir = false;
            }
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
void darFormato_y_GuardarButacasEnArchivo(string nombreArchivo, string titulo,char butacas[TAMANIO_I][TAMANIO_J]){
    string lineaAGuardar;
    guardarEnArchivo(titulo,nombreArchivo);
    for (int i =0; i<TAMANIO_I;i++){
        if(i==2){guardarEnArchivo("-------------------------------------------",nombreArchivo);}
        if(i==4){guardarEnArchivo("===========================================",nombreArchivo);}
        for (int j=0; j<TAMANIO_J;j++){
           if(i==0&&j<1){lineaAGuardar=lineaAGuardar+"  "+butacas[i][j]+" ";
           }else if(j<1){lineaAGuardar=lineaAGuardar+" "+butacas[i][j]+" |";
           }else{lineaAGuardar=lineaAGuardar+butacas[i][j]+" ";}
        }//cierro el for de columna
        guardarEnArchivo(lineaAGuardar,nombreArchivo);
        lineaAGuardar="";

    }//Cierro el for de renglon

}
/***********************************************************************/

/***********************************************************************/
void guardarEnArchivo(string lineaAGuardar, string nombreArchivo){
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

/***********************************************************************/
void guardarEnArchivoYaFormateada(string lineaAGuardar, string nombreArchivo){
   bool NO_EsPrimeraCarga = verificarSiExisteArchivo(nombreArchivo);// Si no existe el archivo quiere decir que va a ser la primera cargaga de datos (ya que cuando lo creo cargo datos)
   nombreArchivo= nombreArchivo+".txt";
   ofstream archivo(nombreArchivo.c_str(),ios::out | ios::app);
        if(NO_EsPrimeraCarga==false){//si es la primera carga
          archivo<<lineaAGuardar; //solo pongo la linea
        }else{//si NO es la primera carga, pongo salto de linea al comienzo
          archivo<<"\n"<<lineaAGuardar;
        }/*sin este if me generaba una linea demás, al comienzo del archivo, por el salto de linea. Si pusiese el salto de linea al final genería la linea demás al final del
         archivo y no tengo forma manejarlo cuando... en ambos casos al mostrar los registros del archivo me mostraría uno demás en blanco arriba o abajo*/
   archivo.close();
}
/***********************************************************************/

/***********************************************************************/
string verificarSolicitud_Y_Responder(Server *&Servidor,vector <string> vectorButacas, string userName, bool reservar){
    string mensajePeticion = "";
    string mensajeDelCli="";
    char letra = '\0';
    int pos_J = -1;
    int pos_I = -1;
    bool posicionDisponible = false;

    mensajeDelCli = Servidor->Recibir(); //Se recibe la butaca. EJ: B8

    if(mensajeDelCli!="0"){
        letra = mensajeDelCli[0];
        mensajeDelCli.erase(0,1);//Saco la letra que guerdé
        pos_J =atoi(const_cast< char *>(mensajeDelCli.c_str()));
        pos_J = pos_J*2+2; //es por la diferencia que hay entre la posicion de vista en consola y la del archivo
        pos_I = asignarValorPosI_A_Letra(letra);


        if(vectorButacas[pos_I][pos_J]=='O' && reservar){
           Servidor->Enviar("Disponible");//está disponible

           marcarButacaComoOcupada(vectorButacas, pos_I, pos_J, userName);
           mensajePeticion = Servidor->Recibir();
        }
        else if(vectorButacas[pos_I][pos_J]=='X' && !reservar){
            Servidor->Enviar("Disponible");//está disponible

            marcarButacaComoLiberada(vectorButacas, pos_I, pos_J, userName);
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
void marcarEnArchivoReservaRealizada(vector <string> vectorButacas){
    ofstream archivoAuxiliar;
    archivoAuxiliar.open("auxiliar.txt",ios::out);
    if(archivoAuxiliar.is_open()){
       for(int i=0;i<11;i++){
         archivoAuxiliar<<vectorButacas[i]<<"\n";
       }//Fin for
    }else{
        cout<<"No se pudo abrir el archivo o aun no ha sido creado"<<endl;
    }
    archivoAuxiliar.close();
    remove("Registro_de_butacas.txt");
    rename("auxiliar.txt","Registro_de_butacas.txt");
}
/**********************************************************************/

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

/***********************************************************************/
void marcarButacaComoOcupada(vector <string> vectorButacas, int pos_I, int pos_J, string userName){

        vectorButacas[pos_I][pos_J] = 'X';
        marcarEnArchivoReservaRealizada(vectorButacas);

        string butaca = butacaAString(pos_I, pos_J);

        string reserva = "Reserva_";

        reserva+=butaca;

        registrarUserLog(reserva, userName);
        system("cls");
        mostrarButacas(vectorButacas);
        cout<<"************************************"<<endl;
        cout<<"** Butaca reservada exitosamente. **"<<endl;
        cout<<"************************************"<<endl;
 }

 void marcarButacaComoLiberada(vector <string> vectorButacas, int pos_I, int pos_J, string userName){

        vectorButacas[pos_I][pos_J] = 'O';
        marcarEnArchivoReservaRealizada(vectorButacas);

        string butaca = butacaAString(pos_I, pos_J);

        string libera = "Libera_";
        libera+=butaca;

        registrarUserLog(libera, userName);
        system("cls");
        mostrarButacas(vectorButacas);
        cout<<"************************************"<<endl;
        cout<<"** Butaca liberada exitosamente. **"<<endl;
        cout<<"************************************"<<endl;
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

string checkUser(Server *&Servidor)
{
    string usuarioEncontrado = "false";
    char delimitador = ';';
    vector<string> resultados;
    vector<string> userAndPass;
    int contador = 0;
    string loggedUser = "";


    while(contador<3 && usuarioEncontrado == "false"){

        string linea;
        fstream file;

        file.open("users.dat", ios::in);

        userAndPass = separarPalabrasPuntoYComa(Servidor->Recibir());

        if(file.is_open())
        {
            while(!file.eof()){

                getline(file, linea);

                resultados = separarPalabrasPuntoYComa(linea);

                if(resultados[0] == userAndPass[0] && resultados[1] == userAndPass[1]){
                        usuarioEncontrado = "true";
                        verificarArchivoServerLog();
                        registrarServerLog("Usuario autenticado", resultados[0]);
                        crearArchivoUserLog(resultados[0]);
                        registrarUserLog("Inicia sesion", resultados[0]);
                        loggedUser = resultados[0];
                }

            }
        }

        file.close();

        usuarioEncontrado == "true" ? cout<<"Usuario Encontrado"<<endl<<endl : cout<<"Crendenciales invalidas..."<<endl<<endl<<"Por favor ingrese sus datos nuevamente (Le quedan " << " intentos)"<<endl<<endl;

        contador++;

        if(usuarioEncontrado == "true") {
            contador = 4;
        }

        Servidor->Enviar(usuarioEncontrado+";"+to_string(contador));

    }
  return loggedUser;
}



void registrarServerLog(string evento, string aRegistrar){
    std::ofstream serverLog("server.log", std::ios::ate | std::ios::in);
    if(serverLog.fail()){ //Si el archivo no se encuentra o no esta disponible o presenta errores
            cout<<"No se pudo abrir el archivo server log"; //Muestra el error
                        }
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    serverLog<<buf;
    serverLog<<": "<<evento<<" - "<<aRegistrar<<endl;
    serverLog<<": ==================================="<<endl;
    serverLog.close();
}


void crearArchivoUserLog(string usuario){
    string nombreArchivo = usuario + ".log";
  std::ifstream userLog( nombreArchivo );
  if(userLog.fail()){
    //EL ARCHIVO NO EXISTE
    std::ofstream userLogCrear( nombreArchivo );
  }

  userLog.close();
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




void registrarUserLog(string evento, string aRegistrar){
    string nombreArchivo = aRegistrar + ".log";
    std::ofstream userLog( nombreArchivo , std::ios::ate | std::ios::in);
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    userLog<<buf;
    userLog<<": "<<evento<<" - "<<aRegistrar<<endl;
    userLog<<": ==================================="<<endl;
    userLog.close();


}
