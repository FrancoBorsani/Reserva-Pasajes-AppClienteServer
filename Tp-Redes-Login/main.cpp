#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

class LoginManager
{
public:
    LoginManager()
    {
        intentos = 0;
    }

    void login()
    {

        while(intentos<3){

            cout << "Bienvenido al Sistema de reserva de pasajes"<<endl<<endl;

            cout << "Ingrese su nombre de usuario: ";

            cin >> usernameTemp;

            cout << endl << "Ingrese su contraseña: ";

            cin >> passwordTemp;

            cout << endl;

            checkUser("users.dat");

            system("pause");

            system("cls");

            intentos++;

        }
    }

    void checkUser(const char* p_fileName)
    {
        string linea;
        fstream file;
        bool usuarioEncontrado = false;
        char delimitador = ';';
        vector<string> resultados;

        file.open(p_fileName, ios::in);

        if(file.is_open())
        {
            while(!file.eof() && !usuarioEncontrado ){

                getline(file, linea);

                resultados = getUsernameAndPassword(linea,delimitador);

                (resultados[0] == usernameTemp && resultados[1] == passwordTemp) ? usuarioEncontrado = true : usuarioEncontrado = false;

            }
        }

        file.close();

        usuarioEncontrado ? cout<<"Usuario Encontrado"<<endl<<endl : cout<<"Crendenciales invalidas..."<<endl<<endl<<"Por favor ingrese sus datos nuevamente (Le quedan "<<3-(intentos+1)<< " intentos)"<<endl<<endl;
    }

    vector<string> getUsernameAndPassword(string str, char pattern) {

        int posInit = 0;
        int posFound = 0;
        string splitted;
        vector<string> resultados;

        while(posFound >= 0){
            posFound = str.find(pattern, posInit);
            splitted = str.substr(posInit, posFound - posInit);
            posInit = posFound + 1;
            resultados.push_back(splitted);
        }

        return resultados;

    }


private:
    string password;
    string username;
    string usernameTemp;
    string passwordTemp;
    int intentos;
};

int main()
{

    LoginManager loginAdmin;
    loginAdmin.login();

}
