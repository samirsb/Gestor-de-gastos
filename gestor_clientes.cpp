// ABM de usuarios --> Sami
#include <iostream>
#include <cstring>
#include <_stdio.h>
#include <_string.h>
using namespace std;

struct Client
{
    int dni;
    char name[20];
    char lastname[20];
    char user[12];
    char pass[9];
};

struct Transaction
{
    int id;
    char user[12];
    int amount;
    int date;
};

bool validatePass(char pass1[9], char pass2[9])
{
    int passSize = 0;
    while (pass1[passSize] != '\0' && passSize < 9)
    {
        if (pass1[passSize] == pass2[passSize])
        {
            passSize++;
        }
        else
        {
            cout << "Las contraseñas no coinciden" << endl;
            break;
        }
    }

    if (passSize != 8)
    {
        return false;
    }
    else
    {
        bool hasNumber = false;
        bool hasUpper = false;

        for (int i = 0; i < passSize; i++)
        {
            if (isupper(pass1[i]))
            {
                hasUpper = true;
            }
            else if (isdigit(pass1[i]))
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
}

char *enterPass()
{
    static char pass1[9];
    char pass2[9];
    bool isValid = false;
    do
    {
        cout << "Ingrese su contraseña\n  - Debe ser de 8 caracteres\n  - Debe tener al menos una letra mayuscula\n  - Debe tener al menos un numero" << endl;
        cin >> pass1;
        cout << "Ingrese nuevamente su contraseña" << endl;
        cin >> pass2;
        isValid = validatePass(pass1, pass2);
    } while (isValid == false);

    return pass1;
};

void saveUserDB(Client newClient)
{
    Client clients[1];
    clients[0] = newClient;

    FILE *file = fopen("userDB.dat", "ab");
    if (file != NULL)
    {
        fwrite(clients, sizeof(Client), 1, file);
        fclose(file);
        cout << "Registro de usuario exitoso" << endl;
    }
    else
    {
        cout << "No se pudo crear el archivo." << endl;
    }
};

void saveTransactionDB(Transaction newTransaction)
{
    Transaction transactions[1];
    transactions[0] = newTransaction;

    FILE *file = fopen("transactionDB.dat", "ab");
    if (file != NULL)
    {
        fwrite(transactions, sizeof(Transaction), 1, file);
        fclose(file);
        cout << "Registro de transaccion exitosa" << endl;
    }
    else
    {
        cout << "No se pudo crear el archivo." << endl;
    }
};

int checkDni(int searchDni)
{
    FILE *file = fopen("userDB.dat", "rb");
    if (file == NULL)
    {
        return 0;
    }
    Client newClient;
    while (fread(&newClient, sizeof(Client), 1, file) == 1)
    {
        if (newClient.dni == searchDni)
        {
            cout << "Cliente ya registrado" << endl;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int checkUser(const char *searchUser)
{
    FILE *file = fopen("userDB.dat", "rb");
    if (file == NULL)
    {
        return 0;
    }

    Client newClient;
    while (fread(&newClient, sizeof(newClient), 1, file) == 1)
    {
        if (strcmp(newClient.user, searchUser) == 0)
        {
            cout << "El usuario ingresado ya existe." << endl;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

Client registration()
{
    Client newClient;
    Transaction newTransaction;
    int validUser = 1;

    cout << "Ingrese su numero de DNI, sin puntos ni espacios" << endl;
    cin >> newClient.dni;

    if (checkDni(newClient.dni) == 0)
    {
        cout << "Ingrese su nombre" << endl;
        cin.ignore();
        cin.getline(newClient.name, sizeof(newClient.name));

        cout << "Ingrese su apellido" << endl;
        cin.getline(newClient.lastname, sizeof(newClient.lastname));

        while (validUser == 1)
        {
            cout << "Ingrese su usuario" << endl;
            cin.getline(newClient.user, sizeof(newClient.user));
            validUser = checkUser(newClient.user);
        }

        char *password = enterPass();
        strcpy(newClient.pass, password);

        saveUserDB(newClient);
        cout << "Por su registro se le otorgara una bonificación de 10.000$" << endl
             << "Por favor ingrese nuevamente su usuario para confirmar" << endl;
        cin.ignore();
        cin.getline(newTransaction.user, sizeof(newTransaction.user));

        newTransaction.id = 1;
        newTransaction.amount = 10000;
        cout << "Ingrese la fecha de hoy con formato AAAAMMDD" << endl;
        cin >> newTransaction.date;

        saveTransactionDB(newTransaction);

        return newClient;
    }
    return newClient;
}

int main()
{
    registration();

    return 0;
}
