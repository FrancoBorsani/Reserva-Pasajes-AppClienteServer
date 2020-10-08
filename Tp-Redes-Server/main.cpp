#include <iostream>
#include <winsock2.h>
#include <string>
#include <conio.h>

using namespace std;

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

        cout << "Escuchando para conexiones entrantes." << endl;
        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            cout << "Cliente conectado!" << endl;
        }
    }

    string Recibir()
    {
      recv(client, buffer, sizeof(buffer), 0);
      cout << "El cliente dice: " << buffer << endl;
      memset(buffer, 0, sizeof(buffer));
    }
    void Enviar()
    {
        cout<<"Escribe el mensaje a enviar: ";
        cin>>this->buffer;
        send(client, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "Mensaje enviado!" << endl;
    }
    void CerrarSocket()
    {
        closesocket(client);
        cout << "Socket cerrado, cliente desconectado." << endl;
    }
};


/***********************************************************************/
std::string imprimirPasillo(){
    return "  | =======================================";
}
/***********************************************************************/

/***********************************************************************/
void imprimirAutobus(char asientos[3][20]){
    cout<<"  |                  ";
    for(int i = 0; i < 11; i++){
        if(i<10){
            cout<<" 1";
        }
        else{
            cout<<" 2"<<endl;
        }
    }
    cout<<"  |";
    int numeroAsiento = 1;

    for(int i = 0 ; i < 20 ; i++){
        if(numeroAsiento<10){
            cout<<" "<<numeroAsiento;
            numeroAsiento++;
        }
        else{
            numeroAsiento=0;
        }
    }
    cout<<" 0"<<endl;
    cout<<"-------------------------------------------"<<endl;
    for(int i = 0; i < 3; i++){
        switch(i){
            case 0: cout<<"A |";break;
            case 1: cout<<"B |";break;
            case 2: cout<<imprimirPasillo()<<endl;cout<<"C |";break;
        }
        for(int j = 0 ; j < 20 ; j++){
            cout<<" "<<asientos[i][j];
        }
        cout<<""<<endl;
    }

    cout<<""<<endl;
}
/***********************************************************************/


/***********************************************************************/
//En realidad, el que acepta la conexion es el Servidor. La ultima parte de la funcion deberia ser en esa clase. Esto no deberia tener argumentos y se deberian enviar los input alla.
bool verificarIpYPuerto(std::string ipReal, int puertoReal){

    std::string ipInput="";
    int puertoInput = 0;

    while(ipInput==""){
        cout<<"Ingrese la direccion IP"<<endl;
        cin>>ipInput;
        system("CLS");
    }
    while(puertoInput==0){
        cout<<"Ingrese el puerto"<<endl;
        cin>>puertoInput;
        system("CLS");
    }

    int ipCmp = ipInput.compare(ipReal);
    int puertoCmp = (puertoInput==puertoReal);

    return (ipCmp==0 && puertoCmp==1)?true:false;
}
/***********************************************************************/


/***********************************************************************/
void menuCliente(){

    char matriz[3][20] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'}};

    bool respuesta = verificarIpYPuerto("192.178.0.0",4747);

    while(!respuesta){
       cout<<"Ip o puerto incorrecto. Vuelva a intentarlo."<<endl;
       _getch();
       system("CLS");
       respuesta = verificarIpYPuerto("192.178.0.0",4747);
    }


    int servicioElegido = 0;

    while(servicioElegido!=4){

        cout<<"1-Alta servicio"<<endl;
        cout<<"2-Gestionar pasajes"<<endl;
        cout<<"3-Ver registro de actividades"<<endl;
        cout<<"4-Cerrar sesion"<<endl;
        cin>>servicioElegido;

        switch(servicioElegido){
            case 1: system("CLS");
                    cout<<servicioElegido<<endl;
                    _getch();
                    system("CLS");
                    break;
            case 2: system("CLS");
                    imprimirAutobus(matriz);
                    _getch();
                    system("CLS");
                    break;
            case 3: system("CLS");
                    cout<<servicioElegido<<endl;
                    _getch();
                    system("CLS");
                    break;
            case 4: system("CLS");
                    cout<<"Sesion cerrada correctamente"<<endl;
                    _getch();
                    system("CLS");
                    break;
            default: system("CLS");
                    cout<<"Ingreso una opcion incorrecta."<<
                    endl;
                    _getch();
                    system("CLS");
                    break;
        }
    }

}
/***********************************************************************/


int main()
{
    menuCliente();
    Server *Servidor = new Server();
    while(true)
    {
        Servidor->Recibir();
        Servidor->Enviar();
    }
}
