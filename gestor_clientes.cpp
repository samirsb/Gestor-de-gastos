// ABM de usuarios
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Client
{
    int dni;
    string name;
    string lastname;
    string user;
    string pass;
};

bool validatePass(string input, string input2)
{
    if (input.length() != 8 || input != input2)
    {
        return false;
    }

    bool hasNumber = false;
    bool hasUpper = false;

    for (char pass : input)
    {
        if (isupper(pass))
        {
            hasUpper = true;
        }
        else if (isdigit(pass))
        {
            hasNumber = true;
        }

        if (hasUpper && hasNumber)
        {
            break;
        }
    }
    return hasUpper && hasNumber;
}

string enterPass()
{
    string input1;
    string input2;
    bool isValid = false;
    do
    {
        cout << "Ingrese su contraseña\n  - Debe ser de 8 caracteres\n  - Debe tener al menos una letra mayuscula\n  - Debe tener al menos un numero" << endl;
        cin >> input1;
        cout << "Ingrese nuevamente su contraseña" << endl;
        cin >> input2;
        isValid = validatePass(input1, input2);
    } while (isValid == false);
    return input1;
}

void saveUserDB(Client newClient)

{
    Client clients[1];
    clients[0] = newClient;
    // Client fede;
    // fede.dni = 23123;
    // fede.name = "djfsdf";
    // fede.lastname = "asdasdas";
    // fede.user = "fmga";
    // fede.pass = "1234";

    FILE *file = fopen("userDB.txt", "a+");
    if (file != NULL)
    {
        for (Client &client : clients)
        {
            fprintf(file, "%d,%s,%s,%s,%s\n",
                    client.dni,
                    // c_str() convierte de string a char*, los archivos se deben escribir en char*
                    client.name.c_str(),
                    client.lastname.c_str(),
                    client.user.c_str(),
                    client.pass.c_str());
        }
        cout << "Archivo creado exitosamente." << endl;
    }
    else
    {
        cout << "No se pudo crear el archivo." << endl;
    }
};

Client registration()
{
    // Se instancia el struct para registro de nuevo cliente
    Client newClient;

    cout << "Ingrese su numero de DNI, sin puntos ni espacios" << endl;
    cin >> newClient.dni;

    // agregar validacion para verificar si el DNI ingresado existe dentro del archivo userDB.txt, si existe se debera salir del proceso.

    cout << "Ingrese su nombre" << endl;
    cin >> newClient.name;

    cout << "Ingrese su apellido" << endl;
    cin >> newClient.lastname;

    cout << "Ingrese su usuario" << endl;
    cin >> newClient.user;

    newClient.pass = enterPass();

    // Para asignacion automatica de saldo inicial deber llamar a analista.cpp y pasar por parametro para registro de transaccion

    saveUserDB(newClient);

    return newClient;
}

int main()
{

    registration();

    return 0;
}
