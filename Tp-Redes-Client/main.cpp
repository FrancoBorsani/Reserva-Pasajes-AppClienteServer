#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <algorithm>
#include <clocale>//es para usar ñ y acento
#include <ctime>//para comparar la fecha con la fecha actual
#include <cstdlib>
#include <vector>

#define TAMANIO_I  5
#define TAMANIO_J  21


using namespace std;

string LINEA_A_GLOBAL = "";
string LINEA_B_GLOBAL = "";
string LINEA_C_GLOBAL = "";

vector<string> verificarIpYPuerto();

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

        vector<string> datos = verificarIpYPuerto();

        string s = datos[0];
        int n = s.length();
        char ip[n + 1];
        strcpy(ip, s.c_str());

        addr.sin_addr.s_addr = inet_addr(ip);

        addr.sin_family = AF_INET;

        addr.sin_port = htons(stol(datos[1]));

        int crespuesta = connect(server, (SOCKADDR *)&addr, sizeof(addr));
        if(crespuesta==0){
            cout << "Conectado al Servidor!" << endl;
        }
        else{
            cout<<"Error al conectarse. Vuelva a intentarlo mas tarde."<<endl;
            CerrarSocket();
            system("pause");
            exit(1);
        }

    }
    void Enviar(string respuesta)
    {
        for(int i=0;i<respuesta.length();i++){
            this->buffer[i]= respuesta[i];
        }

        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));

    }

    string Recibir()
    {
        recv(server, buffer, sizeof(buffer), 0);

        string buff = buffer;

        if(buff=="Timeout"){
            cout<<"Sesion expirada"<<endl;
            CerrarSocket();
        }
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

void altaServicio(Client*& cliente);
bool compararFecha_con_fechaActual(int a ,int m , int d);
bool verificarFecha(int a , int m , int d);
vector<string> elegirServicio(bool crear);
void mostrarButacasCliente();
vector <string> recibirButacas_Y_separar(string butacas);
bool verificarPosicion(int pos_I, int pos_J);
bool verificarCoordenadas(int numero,char letra);
int asignarValorPosI_A_Letra(char letra);
void determinarAccion_A_Seguir(Client *&Cliente,bool posicionDisponible,int pos_I,int pos_J, bool reservar);
string autobusAUsar();

string elegirButaca(Client *&Cliente, bool reservar);
void menuCliente(Client *&Cliente);
void pedirRegistroDeActividades(Client *&Cliente);
void gestionarPasajes(Client *&Cliente);
void reservarAsiento(Client *&Cliente, bool reservar);
string login();
vector<string> separarPalabras(string str);
bool autenticacion(Client *&Cliente);
void inputDatosServer(Client *&Cliente);


/***********************************************************************/
/********************************MAIN***********************************/
/***********************************************************************/
int main()
{
    setlocale(LC_CTYPE,"Spanish");// Spanish (de la librería locale.h) es para usar ñ y acento

    Client *Cliente = new Client();

    inputDatosServer(Cliente);

    return 0;
}
/***********************************************************************/
/*******************************FIN MAIN********************************/
/***********************************************************************/

void inputDatosServer(Client *&Cliente){

    system("CLS");

    bool ingresar = autenticacion(Cliente);

    if (ingresar){
        system("CLS");
        menuCliente(Cliente);
    }else{
        Cliente->CerrarSocket();
    }

}

bool compararFecha_con_fechaActual(int a ,int m , int d){
    //comprobar que la fecha no sea menor a la actual
    bool valido;
    time_t t = time(NULL);
	tm* timePtr = localtime(&t);
    int diaActual, mesActual , anioActual;
    anioActual=((int)timePtr->tm_year+1900);
    mesActual=((int)timePtr->tm_mon+1);
    diaActual=((int)timePtr->tm_mday);

    if( a > anioActual){valido=true;}
    else if( a == anioActual){
        if(m > mesActual){valido = true;}
        else if(m == mesActual){
            if(d > diaActual){valido = true;}
            else if(d == diaActual){valido=true;}
            else{valido = false;}
        }
        else{valido = false;}
    }
    else{valido=false;}

    return valido;
}
/***********************************************************************/

bool verificarFecha(int a , int m , int d){
    //Array que almacenara los dias que tiene cada mes (si el ano es bisiesto, sumaremos +1 al febrero)
    int dias_mes[] = {31, 28, 31, 30,31, 30, 31, 31, 30, 31, 30, 31};

    //Comprobar si el ano es bisiesto y anadir dia en febrero en caso afirmativo
    if((a%4 == 0 and a%100 != 0) or a%400 == 0)
        dias_mes[1]++;

    //Comprobar que el mes sea valido
    if(m < 1 or m > 12)
        return false;

    int auxM = m - 1;
    //Comprobar que el dia sea valido
    if(d <= 0 or d > dias_mes[auxM])
        return false;

    //compruebo si la fecha que me paso el cliente es mayor o igual a la fecha actual
    if(compararFecha_con_fechaActual(a, m , d)){return true;}
    else return false;
}
/***********************************************************************/

vector<string> elegirServicio(bool crear){

    vector<string> servicioOutput;

    int dia,mes,anio;
    bool fechaValida = false;
    while(!fechaValida){

        if(!crear) cout<<"Si la fecha no le interesa, ingrese 0 en todos los campos."<<endl<<endl;
        cout<<"Ingrese el dia: ";
        cin>>dia;
        cout<<"Ingrese el mes: ";
        cin>>mes;
        cout<<"Ingrese el año: ";
        cin>>anio;

        if(verificarFecha(anio,mes,dia) || (!crear && (dia==0 && mes==0 && anio==0) ) ){
            fechaValida = true;
            servicioOutput.push_back(std::to_string(dia));
            servicioOutput.push_back(std::to_string(mes));
            servicioOutput.push_back(std::to_string(anio));
        }
        else{
            cout<<"La fecha ingresada no es valida"<<endl;
            system("pause"); system("cls");
        }
    }

    string origen,turno;
    bool datosValidos =false;

    while(!datosValidos){
        system("cls");
        if(!crear) cout<<"Ingrese un 0 en el campo que no le interese."<<endl<<endl;
        cin.ignore(); //limpio el buffer
        cout<<"Ingrese el origen(Mar del Plata | Buenos Aires): ";
        getline(cin , origen);
        cout<<"Ingrese el turno(Maniana | Tarde | Noche): ";
        getline(cin , turno);

        std::for_each(origen.begin(), origen.end(), [](char & c) {
            c = ::tolower(c);
        });

        std::for_each(turno.begin(), turno.end(), [](char & c) {
            c = ::tolower(c);
        });

        if( (origen=="mar del plata" || origen=="buenos aires" || (!crear && origen=="0") ) && (turno=="maniana" || turno=="tarde" || turno=="noche" || (!crear && turno=="0") ) ){
                            servicioOutput.push_back(origen);
                            servicioOutput.push_back(turno);
                            datosValidos=true;
        }
        else{cout<<"Datos erroneos"<<endl; system("pause"); system("cls");}
    }

    return servicioOutput;
}



/***********************************************************************/
void altaServicio(Client*& cliente){

    vector<string> datos = elegirServicio(true);

    //Se le envian los datos al server
    cliente->Enviar(datos[0]+"-"+datos[1]+"-"+datos[2]) ; //Fecha
    cliente->Enviar(datos[3]);//Origen
    cliente->Enviar(datos[4]);//Turno

    //El cliente recibe el resultado de la operacion
    system("cls");
    cout<<"===================================================================================="<<endl;
    cout<<"\t"<<cliente->Recibir()<<endl;
    cout<<"===================================================================================="<<endl;

}
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
          cout<<"************************************"<<endl<<endl<<"Pulse cualquier tecla para continuar..."<<endl;
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


    while(!salir){

        cout<<"--- Elija la butaca a "<<tipoPeticion<<" (Ejemplo: A12). Para volver al menu anterior ingrese un 0.---"<<endl;
        cin>>respuesta;
        respuestaAux = respuesta;

        if(respuesta=="0"){
            salir = true;
        }
        else{
            letra = respuesta[0]; //1) guarda "B" en un supuesto ingreso de "B7"
            respuesta.erase(0,1); //2)borro la que ya guardé, en un supuesto ingreso de "B7" borra "B"
            numero = atoi(const_cast< char *>(respuesta.c_str())); // si no convierte da "0" (por ejemplo: si encuetra "A4" no convierte, si hay solo numero/s convierte), pero si encuentra "12a" (número/s y letra/s) tambien convierte el/los números y descarta letra/s

            if(respuesta==to_string(numero)){ //me aseguro de que despues del número/s no se hallan ingresado mas letras. Supuesto: se ingre sa "B12hj" o "B2gh".
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
    }

    Cliente->Enviar(respuestaAux);

    if(respuestaAux!="0"){

        bool posicionDisponible = false;

        string posicion = Cliente->Recibir();

        if(posicion=="Disponible"){
            posicionDisponible = true;
        }
        numero=numero-1; //el vector arranca en pos 0
        determinarAccion_A_Seguir(Cliente, posicionDisponible, pos_I, numero, reservar);
    }
    else{
        cout<<"Pulse cualquier tecla para continuar...";
        Cliente->Enviar("true");
    }

    return respuestaAux;
}
/***********************************************************************/


/***********************************************************************/
void menuCliente(Client *&Cliente){

    string respuestaCli = "";

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
                    altaServicio(Cliente);
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
vector<string> verificarIpYPuerto(){

    std::string ipInput="";
    std::string puertoInput = "";
    vector<string> datos;

    while(ipInput==""){
        cout<<"Ingrese la direccion IP: ";
        cin>>ipInput;
        system("CLS");
    }

    while(puertoInput==""){
        cout<<"Ingrese el puerto: ";
        cin>>puertoInput;
        system("CLS");
    }


    datos.push_back(ipInput);
    datos.push_back(puertoInput);

    return datos;
}
/***********************************************************************/

string autobusAUsar(bool crear){

    vector<string> datosServicio = elegirServicio(crear);

    string origen = datosServicio[3];
    string turno = datosServicio[4];

    if(origen=="buenos aires"){
        origen = "Buenos_Aires";
    }
    else{
        origen = "Mar_Del_Plata";
    }

    turno[0] = toupper(turno[0]);

    return datosServicio[0]+"-"+datosServicio[1]+"-"+datosServicio[2]+";"+origen+";"+turno;
}

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

bool checkIfMultipleBus(string autobusAPedir){
    vector<string> datos = separarDatos(autobusAPedir);

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


    return (fechaVacia || origenVacio || turnoVacio);
}

string doIfMultipleBuses(Client *&Cliente, string autobusAPedir){

    if(checkIfMultipleBus(autobusAPedir)){

        string numero = Cliente->Recibir();
        vector<string> autobusesDisponibles;

        for(int i = 0; i < stoi(numero) ; i++){
            autobusesDisponibles.push_back(Cliente->Recibir());
        }

        int numeroAutobusElegido=0;
        system("cls");
        cout<<"Seleccione el autobus que quiere usar."<<endl<<endl;
        for(int i = 0 ; i < autobusesDisponibles.size(); i++){

            cout<<(i+1)<<"- "<<autobusesDisponibles[i]<<endl;
        }
        cin>>numeroAutobusElegido;

        Cliente->Enviar( autobusesDisponibles[numeroAutobusElegido-1] );

        return autobusesDisponibles[numeroAutobusElegido-1];
    }
    else{
        return autobusAPedir;
    }
}

/***********************************************************************/
void gestionarPasajes(Client *&Cliente){
    int opcionElegida = 0;

    int gestionarCnt = 0; //Es por la estrategia que usamos en manejarPeticion en la parte del servidor.

    Cliente->Enviar("Gestionar");
    string autobusAPedir = autobusAUsar(false); //Estamos eligiendo un servicio, no creandolo.
    Cliente->Enviar(autobusAPedir);

    autobusAPedir = doIfMultipleBuses(Cliente,autobusAPedir);

    string butacas = Cliente->Recibir();

    _getch();

    if(butacas!="ServicioInexistente"){

        vector <string> vectorButacas = recibirButacas_Y_separar(butacas);

        LINEA_A_GLOBAL=vectorButacas[0]; LINEA_B_GLOBAL=vectorButacas[1];  LINEA_C_GLOBAL=vectorButacas[2];

        while(opcionElegida!=4){
            system("cls");
            mostrarButacasCliente();

            cout<<"1-Reservar un asiento"<<endl;
            cout<<"2-Liberar un asiento"<<endl;
            cout<<"3-Elegir otro servicio"<<endl;
            cout<<"4-Volver al menu anterior"<<endl;
            cin>>opcionElegida;

            if(gestionarCnt!=0){

                Cliente->Enviar("Gestionar");

                Cliente->Enviar(autobusAPedir);

                string butacas = Cliente->Recibir();
            }

            gestionarCnt = 1;

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
                case 3:{ system("CLS");
                        Cliente->Enviar("ElegirOtroServicio");

                        string autobusAPedirAux = autobusAUsar(false); //Estamos eligiendo un servicio, no creandolo.
                        Cliente->Enviar(autobusAPedirAux);
                        autobusAPedirAux = doIfMultipleBuses(Cliente,autobusAPedirAux);
                        string butacasAux = Cliente->Recibir();

                        if(butacasAux!="ServicioInexistente"){
                            autobusAPedir = autobusAPedirAux;
                            vectorButacas = recibirButacas_Y_separar(butacasAux);
                            LINEA_A_GLOBAL=vectorButacas[0]; LINEA_B_GLOBAL=vectorButacas[1];  LINEA_C_GLOBAL=vectorButacas[2];
                        }
                        else{
                            cout<<"No existe ningun servicio con esas caracteristicas. Presione enter para continuar."<<endl;
                            _getch();  system("CLS");
                        }

                        system("CLS");
                        break;
                        }
                case 4: Cliente->Enviar("false");system("CLS");
                        break;
                default: Cliente->Enviar("false");
                        cout<<"Ingreso una opcion incorrecta."<<endl<<endl<<"Pulse cualquier tecla para continuar..."<<endl;
                         _getch();  system("CLS");//  gestionarPasajes(Cliente);
                         break;
            }

        }
    }
    else{
        cout<<"No existe ningun servicio con esas caracteristicas. Presione enter para continuar."<<endl;
        _getch();  system("CLS");

    }
}
/***********************************************************************/


/***********************************************************************/
void reservarAsiento(Client *&Cliente, bool reservar){

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
