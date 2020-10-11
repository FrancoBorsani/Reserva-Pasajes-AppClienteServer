#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <clocale>//es para usar ñ y acento

#define TAMANIO_I  5
#define TAMANIO_J  21
#define GLOBAL_IP  "192.168.88.9"
#define PUERTO_GLOBAL 4747
using namespace std;

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
        addr.sin_addr.s_addr = inet_addr(GLOBAL_IP);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5555);
        connect(server, (SOCKADDR *)&addr, sizeof(addr));
        cout << "Conectado al Servidor!" << endl;
    }
    void Enviar(string respuesta)
    {
         for(int i=0;i<respuesta.length();i++){
           this->buffer[i]= respuesta[i];
         }

        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "Mensaje enviado!" << endl;
    }

     string Recibir()
    {
      recv(server, buffer, sizeof(buffer), 0);
   //   cout << "El servidor dice: " << buffer << endl;
      string buff = buffer;
      memset(buffer, 0, sizeof(buffer));
      return buff;
    }

    void CerrarSocket()
    {
       closesocket(server);
       WSACleanup();
       cout << "Socket cerrado." << endl << endl;
    }
};

void iniciarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
void mostrarButacas(char butacas[TAMANIO_I][TAMANIO_J]);
bool verificarPosicion(char butacas[TAMANIO_I][TAMANIO_J],int pos_I, int pos_J);
bool verificarCoordenadas(int numero,char letra);
int asignarValorPosI_A_Letra(char letra);
void determinarAccion_A_Seguir(char butacas[TAMANIO_I][TAMANIO_J],int posicionDisponible,int pos_I,int pos_J);
string elegirButaca(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J]);
void menuCliente(Client *&Cliente);
bool verificarIpYPuerto(std::string ipReal, int puertoReal);
void gestionarPasajes(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J]);
void reservarAsiento(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J]);

int main()
{
   setlocale(LC_CTYPE,"Spanish");// Spanish (de la librería locale.h) es para usar ñ y acento

   Client *Cliente = new Client();
   menuCliente(Cliente);

   while(true)
    {
//      Cliente->Enviar(respuesta);
        Cliente->Recibir();

    }
}



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
void determinarAccion_A_Seguir(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J], bool posicionDisponible, int pos_I, int pos_J){

    if (posicionDisponible){
        butacas[pos_I][pos_J] = 'X';
        system("cls");
        mostrarButacas(butacas);
        cout<<"************************************"<<endl;
        cout<<"** Butaca reservada exitosamente. **"<<endl;
        cout<<"************************************"<<endl;
    }
    else{
        cout<<"ERROR: Butaca ya reservada."<<endl;
        elegirButaca(Cliente,butacas);
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
string elegirButaca(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J]){

    int pos_I = -1;
    int numero = -1;
    char letra = ' ';
    bool noContinuar = false;
    std::string respuesta;
    std::string respuestaAux;
    bool entradaCorrecta = false;
    bool errorTipeo=false;
    while(!noContinuar){//Mientras noContinuar sea fals

        cout<<"--- Elija la butaca a reservar (Ejemplo: A12). Para volver al menu anterior ingrese un 0.---"<<endl;
        cin>>respuesta;
        respuestaAux = respuesta;
        if(respuesta=="0"){
            noContinuar = true;
        }//Cierra el else if respuesta == "0"
        else{
            letra = respuesta[0];//1) guarda "B" en un supesto ingreso de "B7"
            respuesta.erase(0,1);//2)borro la que ya guardé, en un supesto ingreso de "B7" borra "B"
            numero=atoi(const_cast< char *>(respuesta.c_str()));//3) si no convierte da "0" (por ejemplo: si encuetra "A4" no convierte, si hay solo numero/s convierte), pero si encuentra "12a" (número/s y letra/s) tambien convierte el/los números y descarta letra/s
            if(respuesta==to_string(numero)){//me aseguro de que despues del número/s no se hallan ingresado mas letras. Supuesto: se ingre sa "B12hj" o "B2gh", B se guarda en
               entradaCorrecta=true;             // el paso 1, numero en el paso 2 y en if se rechaza por la/las letras que siguen al número
            }else{
              bool errorTipeo=true;
              cout<<"*********** ERROR: ha ingresado datos incorrectos, vuelva a intentarlo!!!"<<endl<<"Pulse cualquier tecla para continuar."<<endl;
              _getch();
              system("cls");
              mostrarButacas(butacas);
            }//Fin if respuesta==to_string(numero)

        }//Cierra el else del if respuesta == "0"

        if(noContinuar==false && entradaCorrecta==true){
            if(verificarCoordenadas(numero, letra)){
                pos_I = asignarValorPosI_A_Letra(letra);
                noContinuar = true;
            }
            else if(errorTipeo==false){
                cout<<"ERROR: Ingreso una ubicacion no valida."<<endl;
                _getch();
                system("cls");
                mostrarButacas(butacas);
            }//Cierra el if verificarCoordenadas
        }//Cierra el if !noContinuar

    }//cierra el while

    if(respuesta!="0"){
        Cliente->Enviar(respuestaAux);
        bool posicionDisponible = false;
        if(Cliente->Recibir()=="true"){
                posicionDisponible = true;
        }
        determinarAccion_A_Seguir(Cliente,butacas, posicionDisponible, pos_I, numero);
    }
 return respuestaAux;
}
/***********************************************************************/



/***********************************************************************/
void reservarAsiento(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J]){
     string respuesta = "";
    iniciarButacas(butacas);
    mostrarButacas(butacas);
    respuesta = elegirButaca(Cliente,butacas);

}
/***********************************************************************/


/***********************************************************************/
void gestionarPasajes(Client *&Cliente,char butacas[TAMANIO_I][TAMANIO_J]){
    int opcionElegida = 0;

    while(opcionElegida!=4){

        cout<<"1-Reservar un asiento"<<endl;
        cout<<"2-Liberar un asiento"<<endl;
        cout<<"3-Elegir otro servicio"<<endl;
        cout<<"4-Volver al menu anterior"<<endl;
        cin>>opcionElegida;

        switch(opcionElegida){
            case 1: system("CLS");
                   reservarAsiento(Cliente,butacas);
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
        //cout<<"Ingrese la direccion IP"<<endl;
        ipInput=GLOBAL_IP;
        system("CLS");
    }
    while(puertoInput==0){
        //cout<<"Ingrese el puerto"<<endl;
        puertoInput=PUERTO_GLOBAL;
        system("CLS");
    }

    int ipCmp = ipInput.compare(ipReal);
    int puertoCmp = (puertoInput==puertoReal);

    return (ipCmp==0 && puertoCmp==1)?true:false;
}
/***********************************************************************/


/***********************************************************************/
void menuCliente(Client *&Cliente){

    //deberia ser global o pasarse por referencia.
    char matriz[TAMANIO_I][TAMANIO_J] = {{'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O'}};
    string respuestaCli = "";
    bool respuesta = verificarIpYPuerto(GLOBAL_IP,PUERTO_GLOBAL);

    while(!respuesta){
       cout<<"Ip o puerto incorrecto. Vuelva a intentarlo."<<endl;
       _getch();
       system("CLS");
       respuesta = verificarIpYPuerto(GLOBAL_IP,PUERTO_GLOBAL);
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
                   gestionarPasajes(Cliente,matriz);
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
