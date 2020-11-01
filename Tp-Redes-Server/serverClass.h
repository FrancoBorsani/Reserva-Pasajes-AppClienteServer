#ifndef SERVERCLASS_H_INCLUDED
#define SERVERCLASS_H_INCLUDED
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
#define GLOBAL_IP  "192.168.88.9"
#define PUERTO_GLOBAL 5000


using namespace std;



void registrarServerLog(string evento);
void registrarUserLog(string evento, string aRegistrar);
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

        serverAddr.sin_addr.s_addr = inet_addr(GLOBAL_IP);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PUERTO_GLOBAL);

        bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(server, 0);

        cout << "Escuchando para conexiones entrantes." << endl;

        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            cout << "Cliente conectado!" << endl;
            verificarArchivoServerLog();
            registrarServerLog("Cliente conectado");
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
            cout<<"Timeout: cliente desconectado"<<endl;
            Enviar("Timeout");
        }
        recv(client, buffer, sizeof(buffer), 0);

        string buff = buffer;

        memset(buffer, 0, sizeof(buffer));
        return buff;
    }

    void Enviar(string respuesta)
    {
        for(int i=0;i<(int)respuesta.length();i++){
            this->buffer[i]= respuesta[i];
        }

        send(client, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    void CerrarSocket(string username)
    {
        closesocket(client);
        WSACleanup();
        verificarArchivoServerLog();
        registrarServerLog("Cliente desconectado");
        cout << "Socket cerrado, cliente desconectado." << endl;
        registrarUserLog("Cierra sesion", username);

    }
};


void crearServicio(string userName , Server*& servidor);
void renovacionDeMicrosDisponibles();

string checkData(Server *&Servidor);
void mostrarRegistro(string userName, Server *&Servidor);
string checkUser(Server *&Servidor);
void gestionarAsiento(string nombreArchivo,Server *&Servidor, string userName, bool reservar);
void liberar(Server *&Servidor, string userName);
string verificarSolicitud_Y_Responder(Server *&Servidor,vector <string> vectorButacas, string userName, bool reservar, string nombreArchivo);
int numeroDeSentencias(string file);
void manejarPeticion(string userName, Server *&Servidor);

#endif // SERVERCLASS_H_INCLUDED
