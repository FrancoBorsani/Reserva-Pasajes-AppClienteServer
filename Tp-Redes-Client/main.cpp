#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include <string>


#define TAMANIO_I  5
#define TAMANIO_J  21

using namespace std;

void iniciarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
void mostrarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
bool verificarPosicion(char butacas[TAMANIO_I][TAMANIO_J],int pos_I, int pos_J);
bool verificarCoordenadas(int numero,char letra);
int asignarValorPosI_A_Letra(char letra);
void determinarAccion_A_Seguir(char butacas[TAMANIO_I][TAMANIO_J],int posicionDisponible,int pos_I,int pos_J);
void elegirButaca(char butacas[TAMANIO_I][TAMANIO_J]);


class Client{
public:
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];
    Client()
    {
        cout<<"Conectando al servidor..."<<endl<<endl;
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_addr.s_addr = inet_addr("192.168.0.1");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5555);
        connect(server, (SOCKADDR *)&addr, sizeof(addr));
        cout << "Conectado al Servidor!" << endl;
    }
    void Enviar()
    {
        cout<<"Escribe el mensaje a enviar: ";
        cin>>this->buffer;
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "Mensaje enviado!" << endl;
    }
    void Recibir()
    {
        recv(server, buffer, sizeof(buffer), 0);
        cout << "El servidor dice: " << buffer << endl;
        memset(buffer, 0, sizeof(buffer));
    }
    void CerrarSocket()
    {
       closesocket(server);
       WSACleanup();
       cout << "Socket cerrado." << endl << endl;
    }
};


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
void determinarAccion_A_Seguir(char butacas[TAMANIO_I][TAMANIO_J], bool posicionDisponible, int pos_I, int pos_J){

    if (posicionDisponible){
        butacas[pos_I][pos_J] = 'X';
        cout<<"Butaca reservada exitosamente."<<endl;
    }
    else{
        cout<<"ERROR: Butaca ya reservada."<<endl;
    }
}
/***********************************************************************/


/***********************************************************************/
bool verificarPosicion(char butacas[TAMANIO_I][TAMANIO_J],int pos_I, int pos_J){
    return (butacas[pos_I][pos_J] == 'O')?true:false;
}
/***********************************************************************/


/***********************************************************************/
bool verificarCoordenadas(int numero,char letra){

    return ((numero>0 && numero<21) && ( letra=='A'||letra=='a'||letra=='B'||letra=='b'||letra=='C'||letra=='c'))?true:false;
}
/***********************************************************************/


/***********************************************************************/
void elegirButaca(char butacas[TAMANIO_I][TAMANIO_J]){

    int pos_I = -1;
    int numero = -1;
    char letra = ' ';
    bool flag = false;
    std::string respuesta;

    while(!flag){

        cout<<"--- Elija la butaca a reservar (Ejemplo: A12). Para volver al menu anterior ingrese un 0.---"<<endl;
        cin>>respuesta;

        if(respuesta.length()==2){
            letra = respuesta[0];
            numero = (int)respuesta[1] - 48;
        }
        else if(respuesta.length()==3){
            letra = respuesta[0];
            numero = std::stoi(respuesta.substr(1,2));
        }
        else if(respuesta=="0"){
            flag = true;
        }

        if(!flag){
            if(verificarCoordenadas(numero, letra)){
                flag = true;
                pos_I = asignarValorPosI_A_Letra(letra);
            }
            else{
                cout<<"ERROR: Ingreso una ubicacion no valida."<<endl;
                _getch();
                system("cls");
                iniciarButacas(butacas);
                mostrarButacas(butacas);
            }
        }

    }

    if(respuesta!="0"){
        bool posicionDisponible = verificarPosicion(butacas, pos_I, numero);
        determinarAccion_A_Seguir(butacas, posicionDisponible, pos_I, numero);
    }

}
/***********************************************************************/


/***********************************************************************/
void reservarAsiento(char butacas[TAMANIO_I][TAMANIO_J]){
    iniciarButacas(butacas);
    mostrarButacas(butacas);
    elegirButaca(butacas);
}
/***********************************************************************/


/***********************************************************************/
void gestionarPasajes(char butacas[TAMANIO_I][TAMANIO_J]){

    int opcionElegida = 0;

    while(opcionElegida!=4){

        cout<<"1-Reservar un asiento"<<endl;
        cout<<"2-Liberar un asiento"<<endl;
        cout<<"3-Elegir otro servicio"<<endl;
        cout<<"4-Volver al menu anterior"<<endl;
        cin>>opcionElegida;

        switch(opcionElegida){
            case 1: system("CLS");
                    reservarAsiento(butacas);
                    _getch();
                    system("CLS");
                    break;
            case 2: system("CLS");
                    //Liberar asiento
                    _getch();
                    system("CLS");
                    break;
            case 3: system("CLS");
                    //Elegir otro servicio
                    _getch();
                    system("CLS");
                    break;
            case 4: system("CLS");
                    cout<<"Pulse enter para continuar."<<endl;
                    _getch();
                    system("CLS");
                    break;
            default: system("CLS");
                    cout<<"Ingreso una opcion incorrecta."<<endl;
                    _getch();
                    system("CLS");
                    break;
        }
    }
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

    //deberia ser global o pasarse por referencia.
    char matriz[TAMANIO_I][TAMANIO_J] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
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
                    gestionarPasajes(matriz);
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
    Client *Cliente = new Client();
    while(true)
    {
        Cliente->Enviar();
        Cliente->Recibir();
    }
}
