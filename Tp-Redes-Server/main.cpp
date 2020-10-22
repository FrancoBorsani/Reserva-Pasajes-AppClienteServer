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
#include <windows.h>

#define TAMANIO_I  5
#define TAMANIO_J  21
using namespace std;

void registrarServerLog(string evento, string aRegistrar);


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

        tv.tv_sec = 10 ;
        tv.tv_usec = 0 ;
        // Set up the file descriptor set.
        FD_ZERO(&fds) ;
        FD_SET(client, &fds) ;

        int n = select (client, &fds, NULL, NULL, &tv) ;

        if ( n == 0){
          printf("Timeout..\n");
          closesocket(client);
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
        registrarServerLog("Socket cerrado, cliente desconectado", "");
        cout << "Socket cerrado, cliente desconectado." << endl;
    }
};

void verificarSolicitud_Y_Responder(Server *&Servidor,char butacas[TAMANIO_I][TAMANIO_J]);
bool verificarPosicion(char butacas[TAMANIO_I][TAMANIO_J],int pos_I, int pos_J);
int asignarValorPosI_A_Letra(char letra);
void marcarButacaComoOcupada(char butacas[TAMANIO_I][TAMANIO_J], int pos_I, int pos_J);
void mostrarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
void iniciarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
int numeroDeSentencias(string file);
void manejarPeticion(string peticion, string userName, Server *&Servidor);
void mostrarRegistro(string userName, Server *&Servidor);
vector<string> getUsernameAndPassword(string str);
string checkUser(Server *&Servidor);
void registrarServerLog(string evento, string aRegistrar);
void registrarUserLog(string evento, string aRegistrar);
void crearArchivoUserLog(string usuario);

/************************************
         MAIN
***********************************/
int main()
{
    setlocale(LC_CTYPE,"Spanish");// Spanish (de la librería locale.h) es para usar ñ y acento
/*
     char matriz[TAMANIO_I][TAMANIO_J] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'}};
*/
    Server *Servidor = new Server();
    //iniciarButacas(matriz);
    //mostrarButacas(matriz);
    //verificarSolicitud_Y_Responder(Servidor,matriz);

    //cout<< resultado[0] <<endl << resultado[1] <<endl;

    string userName = checkUser(Servidor);


    while(true){
        string peticion = Servidor->Recibir();
        manejarPeticion(peticion, userName, Servidor);
    }

    return 0;
}
/************************************
        FIN  MAIN
***********************************/

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
void manejarPeticion(string peticion, string userName, Server *&Servidor){

char matriz[TAMANIO_I][TAMANIO_J] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'}};

    if(peticion=="Registro"){
        mostrarRegistro(userName,Servidor);
    }
    else if(peticion=="Reservar"){
        verificarSolicitud_Y_Responder(Servidor,matriz);
    }

}

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
void verificarSolicitud_Y_Responder(Server *&Servidor,char butacas[TAMANIO_I][TAMANIO_J]){

    string mensajeDelCli="";
    char letra;
    int pos_J;
    int pos_I;
    bool posicionDisponible = false;

    mensajeDelCli = Servidor->Recibir();//en este mensaje solo puede llegar letra-numero o letra-numero-numero (sin guiones)
    letra = mensajeDelCli[0];
    mensajeDelCli.erase(0,1);//Saco la letra que guerdé
    pos_J =atoi(const_cast< char *>(mensajeDelCli.c_str()));
    pos_I = asignarValorPosI_A_Letra(letra);
    posicionDisponible = verificarPosicion(butacas, pos_I, pos_J);
    if(posicionDisponible){
       Servidor->Enviar("true");
       marcarButacaComoOcupada(butacas, pos_I, pos_J);
    }else{
        Servidor->Enviar("false");
        cout<<"No se asino una butaca "<<endl<<endl;
        mostrarButacas(butacas);
    }
}
/***********************************************************************/


/***********************************************************************/
int asignarValorPosI_A_Letra(char letra){
    int pos_I = -1;

    if(letra=='A' || letra == 'a'){
        pos_I=2;
    }
    else if(letra=='B' || letra == 'b'){
        pos_I=3;}
    else {
        pos_I=4;
    }

    return pos_I;
}
/***********************************************************************/


/***********************************************************************/
bool verificarPosicion(char butacas[TAMANIO_I][TAMANIO_J],int pos_I, int pos_J){
    return (butacas[pos_I][pos_J] == 'O')?true:false;
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
void mostrarButacas(char butacas[TAMANIO_I][TAMANIO_J]){
    cout<<endl;
    for (int i =0; i<TAMANIO_I;i++){
        if(i==2){cout <<"---------------------------------------------------------------"<< endl;}
        if(i==4){cout <<"==============================================================="<< endl;}
        for (int j=0; j<TAMANIO_J;j++){
           if(i==0&&j<1){cout <<"  "<<butacas[i][j]<<" ";
           }else if(j<1){cout <<" "<<butacas[i][j]<<" |";
           }else{cout <<" "<<butacas[i][j]<<" ";}
        }
        printf("\n");
    }
}
/***********************************************************************/

/***********************************************************************/
void marcarButacaComoOcupada(char butacas[TAMANIO_I][TAMANIO_J], int pos_I, int pos_J){

        butacas[pos_I][pos_J] = 'X';
        system("cls");
        mostrarButacas(butacas);
        cout<<"************************************"<<endl;
        cout<<"** Butaca reservada exitosamente. **"<<endl;
        cout<<"************************************"<<endl;
 }
/***********************************************************************/

vector<string> getUsernameAndPassword(string str) {

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

            userAndPass = getUsernameAndPassword(Servidor->Recibir());

            if(file.is_open())
            {
                while(!file.eof()){

                    getline(file, linea);

                    resultados = getUsernameAndPassword(linea);

                    if(resultados[0] == userAndPass[0] && resultados[1] == userAndPass[1]){
                            usuarioEncontrado = "true";
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
            std::ofstream serverLog("server.txt", std::ios::ate | std::ios::in);
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


