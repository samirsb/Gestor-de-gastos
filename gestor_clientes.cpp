// ABM de usuarios
#include <iostream>
using namespace std;

struct Client
{
    int dni;
    char name[20];
    char lastname[20];
    char user[12];
    char pass[9];
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

int checkUser(int searchUser)
{
    FILE *file = fopen("userDB.dat", "rb");
    if (file != NULL)
    {
        Client newClient;
        while (fread(&newClient, sizeof(Client), 1, file) == 1)
        {
            if (newClient.dni == searchUser)
            {
                cout << "Cliente ya registrado" << endl;
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    else
    {
        cout << "No se pudo abrir el archivo para lectura" << endl;
    }
    return 0;
}

void saveUserDB(Client newClient)

{
    Client clients[1];
    clients[0] = newClient;

    FILE *file = fopen("userDB.dat", "ab");
    if (file != NULL)
    {
        fwrite(clients, sizeof(Client), 1, file);
        fclose(file);
        cout << "Archivo creado exitosamente." << endl;
    }
    else
    {
        cout << "No se pudo crear el archivo." << endl;
    }
};

Client registration()
{
    Client newClient;

    cout << "Ingrese su numero de DNI, sin puntos ni espacios" << endl;
    cin >> newClient.dni;

    if (!checkUser(newClient.dni))
    {
        cout << "Ingrese su nombre" << endl;
        cin.ignore();
        cin.getline(newClient.name, sizeof(newClient.name));

        cout << "Ingrese su apellido" << endl;
        cin.getline(newClient.lastname, sizeof(newClient.lastname));

        cout << "Ingrese su usuario" << endl;
        cin.getline(newClient.user, sizeof(newClient.user));

        string password = enterPass();
        strcpy(newClient.pass, password.c_str()); // convierte de string a char

        // Para asignacion automatica de saldo inicial deber llamar a analista.cpp y pasar por parametro para registro de transaccion

        saveUserDB(newClient);

        return newClient;
    }
    return newClient;
}

int main()
{

    registration();

    return 0;
}
