#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <clocale>//es para usar ñ y acento
#include <cstdlib>
#include <vector>

#define TAMANIO_I  5
#define TAMANIO_J  21
#define GLOBAL_IP  "192.168.88.9"
#define PUERTO_GLOBAL 4747

using namespace std;

string LINEA_A_GLOBAL = "";
string LINEA_B_GLOBAL = "";
string LINEA_C_GLOBAL = "";

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
        //cout << "Mensaje enviado!" << endl;
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



void mostrarButacasCliente();
vector <string> recibirButacas_Y_separar(string butacas);
bool verificarPosicion(int pos_I, int pos_J);
bool verificarCoordenadas(int numero,char letra);
int asignarValorPosI_A_Letra(char letra);
void determinarAccion_A_Seguir(Client *&Cliente,bool posicionDisponible,int pos_I,int pos_J, bool reservar);

string elegirButaca(Client *&Cliente, bool reservar);
void menuCliente(Client *&Cliente);
void pedirRegistroDeActividades(Client *&Cliente);
bool verificarIpYPuerto(std::string ipReal, int puertoReal);
void gestionarPasajes(Client *&Cliente);
void reservarAsiento(Client *&Cliente, bool reservar);
string login();
vector<string> separarPalabras(string str);
bool autenticacion(Client *&Cliente);


/***********************************************************************/
/********************************MAIN***********************************/
/***********************************************************************/
int main()
{
   setlocale(LC_CTYPE,"Spanish");// Spanish (de la librería locale.h) es para usar ñ y acento

   Client *Cliente = new Client();

   bool ingresar = autenticacion(Cliente);

   if (ingresar){
   		menuCliente(Cliente);
   }else{
   	   Cliente->CerrarSocket();
   }


}
/***********************************************************************/
/*******************************FIN MAIN********************************/
/***********************************************************************/



/***********************************************************************/
void mostrarButacasCliente(){
    string lineaA = ""; string lineaB= ""; string lineaC = "";
    for(int i=0;i<20;i++){
      lineaA=lineaA+LINEA_A_GLOBAL[i]+" ";
      lineaB=lineaB+LINEA_B_GLOBAL[i]+" ";
      lineaC=lineaC+LINEA_C_GLOBAL[i]+" ";
   }//for i
   //--------------------------------------------------
   cout<<"                     1 1 1 1 1 1 1 1 1 1 2"<<
   endl<<"  |1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0"<<
   endl<<"------------------------------------------"<<
   endl<<" A|"<<lineaA<<endl<<" B|"<<lineaB<<endl
   <<"=========================================="<<
   endl<<" C|"<<lineaC<<endl;
   //--------------------------------------------------
}
/***********************************************************************/


/***********************************************************************/
vector <string> recibirButacas_Y_separar(string butacas){
   string lineaA = "";
   string lineaB = "";
   string lineaC = "";

   vector <string> vectorLineas;
   //--------------------------------------------------
   for(int i=0;i<60;i++){
        if(i<20){
          lineaA+=butacas[i];
        }else if(i>=20&&i<40){
          lineaB+=butacas[i];
        }else{
          lineaC+=butacas[i];
        }
   }//for i
   vectorLineas.push_back(lineaA);
   vectorLineas.push_back(lineaB);
   vectorLineas.push_back(lineaC);

   //--------------------------------------------------
  return vectorLineas;
}
/***********************************************************************/


/***********************************************************************/
bool verificarPosicion(int pos_I, int pos_J){
    bool estaDisponible=false;
    if(pos_I==0&&LINEA_A_GLOBAL[pos_J] == 'O'){
         estaDisponible=true;
    }else if (pos_I==1&&LINEA_B_GLOBAL[pos_J] == 'O'){
         estaDisponible=true;
    }else if(LINEA_C_GLOBAL[pos_J] == 'O'){
         estaDisponible=true;
    }
    return estaDisponible;
}
/***********************************************************************/


/***********************************************************************/
bool verificarCoordenadas(int numero,char letra){

    return ((numero>0 && numero<21) && ( letra=='A'||letra=='a'||letra=='B'||letra=='b'||letra=='C'||letra=='c'))?true:false;
}
/***********************************************************************/


/***********************************************************************/
int asignarValorPosI_A_Letra(char letra){
    int pos_I = -1;

    if(letra=='A' || letra == 'a'){
        pos_I=0;
    }
    else if(letra=='B' || letra == 'b'){
        pos_I=1;}
    else {
        pos_I=2;
    }

    return pos_I;
}
/***********************************************************************/


/***********************************************************************/
void determinarAccion_A_Seguir(Client *&Cliente,bool posicionDisponible, int pos_I, int pos_J, bool reservar){

    char estadoButaca = 'X';
    string peticionButaca = "reservada";
    string errorButaca = "reservada";

    if(!reservar){
      estadoButaca = 'O';
      peticionButaca = "liberada";
      errorButaca = "libre";
    }

    if (posicionDisponible){
          if(pos_I==0){
            LINEA_A_GLOBAL[pos_J] = estadoButaca;
          }else if (pos_I==1){
            LINEA_B_GLOBAL[pos_J] = estadoButaca;
          }else{
            LINEA_C_GLOBAL[pos_J] = estadoButaca;
          }
          system("cls");
          Cliente->Enviar("true");
          mostrarButacasCliente();
          cout<<"************************************"<<endl;
          cout<<"** Butaca "<<peticionButaca<<" exitosamente. **"<<endl;
          cout<<"************************************"<<endl<<endl<<"Precione cualquier tecla para continuar..."<<endl;
          if(estadoButaca=='O'){_getch();}
    }
    else{
        cout<<"*** ERROR: Butaca ya "<<errorButaca<<". ***"<<endl<<endl;
        Cliente->Enviar("false");
        elegirButaca(Cliente,reservar);
    }
}
/***********************************************************************/


/***********************************************************************/
string elegirButaca(Client *&Cliente, bool reservar){

    int pos_I = -1;
    int numero = -1;
    char letra = '\0'; //Vacio
    bool salir = false;
    string respuesta = "";
    string respuestaAux = "";

    string tipoPeticion = "reservar";
    if(!reservar) tipoPeticion = "liberar";


    while(!salir){//Mientras salir sea false

        cout<<"--- Elija la butaca a "<<tipoPeticion<<" (Ejemplo: A12). Para volver al menu anterior ingrese un 0.---"<<endl;
        cin>>respuesta;
        respuestaAux = respuesta;

        if(respuesta=="0"){
            salir = true;
        }
        else{
            letra = respuesta[0];//1) guarda "B" en un supuesto ingreso de "B7"
            respuesta.erase(0,1);//2)borro la que ya guardé, en un supuesto ingreso de "B7" borra "B"
            numero = atoi(const_cast< char *>(respuesta.c_str())); // si no convierte da "0" (por ejemplo: si encuetra "A4" no convierte, si hay solo numero/s convierte), pero si encuentra "12a" (número/s y letra/s) tambien convierte el/los números y descarta letra/s

            if(respuesta==to_string(numero)){//me aseguro de que despues del número/s no se hallan ingresado mas letras. Supuesto: se ingre sa "B12hj" o "B2gh".
                if(verificarCoordenadas(numero, letra)){
                    pos_I = asignarValorPosI_A_Letra(letra);
                    salir = true;
                }
            }
            else{
                cout<<"***ERROR: Ubicacion no valida.***"<<endl<<"Pulse cualquier tecla para continuar."<<endl;
                _getch();
                system("cls");
                mostrarButacasCliente();
            }

        }
    }//cierra el while

    Cliente->Enviar(respuestaAux);

    if(respuestaAux!="0"){

        bool posicionDisponible = false;

        string posicion = Cliente->Recibir();
//        cout<<posicion<<endl;
        if(posicion=="Disponible"){
            posicionDisponible = true;
        }
        numero=numero-1; //es porque el vector arranca en pos 0
        determinarAccion_A_Seguir(Cliente, posicionDisponible, pos_I, numero, reservar);
    }
    else{
        cout<<"Precione cualquier tecla para continuar...";
        Cliente->Enviar("true");
    }

    return respuestaAux;
}
/***********************************************************************/


/***********************************************************************/
void menuCliente(Client *&Cliente){

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
                    Cliente->Enviar("AltaServicio");
                    _getch();
                    system("CLS");
                    break;
            case 2: system("CLS");
                    gestionarPasajes(Cliente);
                    break;
            case 3: system("CLS");
                    pedirRegistroDeActividades(Cliente);
                    _getch();
                    system("CLS");
                    break;
            case 4: system("CLS");
                    Cliente->CerrarSocket();
                    cout<<"Sesion cerrada correctamente"<<endl;
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
void pedirRegistroDeActividades(Client *&Cliente){

    Cliente->Enviar("Registro");

    string numero = Cliente->Recibir();

    for(int i = 0 ; i < stoi(numero); i++){
        string respuesta = "";
        respuesta = Cliente->Recibir();
        cout<<respuesta<<endl;
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
void gestionarPasajes(Client *&Cliente){
    int opcionElegida = 0;

    while(opcionElegida!=4){

        cout<<"1-Reservar un asiento"<<endl;
        cout<<"2-Liberar un asiento"<<endl;
        cout<<"3-Elegir otro servicio"<<endl;
        cout<<"4-Volver al menu anterior"<<endl;
        cin>>opcionElegida;
        Cliente->Enviar("Gestionar");
        switch(opcionElegida){
            case 1: system("CLS");
                    Cliente->Enviar("ReservarAsiento");
                    reservarAsiento(Cliente, true);
                    _getch();
                    system("CLS");
                    break;
            case 2: system("CLS");
                    Cliente->Enviar("LiberarAsiento");
                    reservarAsiento(Cliente, false);
                    system("CLS");
                    break;
            case 3: system("CLS");
                    Cliente->Enviar("false");
                    //Elegir otro servicio
                    system("CLS");
                    break;
            case 4: Cliente->Enviar("false");system("CLS");
                    break;
            default: Cliente->Enviar("false");
                    cout<<"Ingreso una opcion incorrecta."<<endl<<endl<<"Pulse cualqier tecla para continuar..."<<endl;
                     _getch();  system("CLS");  gestionarPasajes(Cliente);
                     break;
        }
    }
}
/***********************************************************************/


/***********************************************************************/
void reservarAsiento(Client *&Cliente, bool reservar){

    //Cliente->Enviar("--->>> Cliente "+NAME_CLIENTE+" conectado<<<---");
    vector <string> vectorButacas = recibirButacas_Y_separar(Cliente->Recibir());

    LINEA_A_GLOBAL=vectorButacas[0]; LINEA_B_GLOBAL=vectorButacas[1];  LINEA_C_GLOBAL=vectorButacas[2];

    mostrarButacasCliente();
    elegirButaca(Cliente, reservar);

}
/***********************************************************************/


/**********************************************************************/

string login()
{
    string usernameTemp;
    string passwordTemp;
    string credenciales;
    vector<string> responseServer;
    int intentos=0;

        cout << "Bienvenido al Sistema de reserva de pasajes"<<endl<<endl;

        cout << "Ingrese su nombre de usuario: ";

        cin >> usernameTemp;

        cout << endl << "Ingrese su contraseña: ";

        cin >> passwordTemp;

        cout << endl;

        credenciales = usernameTemp + ";" + passwordTemp;

    return credenciales;
}

vector<string> separarPalabras(string str) {

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

bool autenticacion(Client *&Cliente){
   int intentos = 0;
   bool usuarioEncontrado = false;

   vector<string> responseServer;

            while(intentos<3){

            	Cliente->Enviar(login());

	            responseServer = separarPalabras(Cliente->Recibir());

	            intentos = atoi(const_cast< char *>(responseServer[1].c_str()));

	            if(responseServer[0] == "false"){
	            	if(intentos<3){
	            		cout<<"Por favor ingrese sus datos nuevamente (Le quedan " << 3-intentos << " intentos)."<<endl;
						system("pause");
		            	system("cls");
	            	}else {
	            		cout << endl << "Se agotaron la cantidad de intentos de ingreso. Vuelva a intentarlo en otro momento." << endl;
   	   					system("pause");
	   					system("cls");
	            	}

				} else {
					usuarioEncontrado = true;
				}
            }

   return usuarioEncontrado;
}
