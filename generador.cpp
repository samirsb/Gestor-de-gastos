// Gestor de transacciones para el cliente final
#include <_string.h>
#include <iostream>
#include <cstring>
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

int validarUsuario(const char *UsuarioBuscado, const char *PasswordBuscada)
{
    FILE *archivo = fopen("userDB.dat", "rb");
    if (archivo != NULL)
    {
        Client usuario;
        while (fread(&usuario, sizeof(Client), 1, archivo) == 1)
        {
            if (strcmp(usuario.user, UsuarioBuscado) == 0)
            {
                if (strcmp(usuario.pass, PasswordBuscada) == 0)
                {
                    fclose(archivo);
                    return 1;
                }
            }
        }
        fclose(archivo);
    }
    else
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    return 0;
}

float calcularSaldoUsuario(const char *usuarioBuscado)
{
    FILE *archivo = fopen("transactionDB.dat", "rb");
    Transaction transaccion;
    float saldoTotal = 0;

    while (fread(&transaccion, sizeof(transaccion), 1, archivo) == 1)
    {
        if (strcmp(transaccion.user, usuarioBuscado) == 0)
        {
            saldoTotal += transaccion.amount;
        }
        else
        {
            break;
        }
    }
    fclose(archivo);
    return saldoTotal;
}

int CalcularID()
{
    FILE *archivo = fopen("transactionDB.dat", "rb");
    int mayor;
    Transaction transaccion;
    while (fread(&transaccion, sizeof(Transaction), 1, archivo) == 1)
    {
        mayor = transaccion.id;
        if (transaccion.id > mayor)
        {
            mayor = transaccion.id;
        }
    }
    return mayor + 1;
}

void GestionarTransaccion(Client cliente)
{
    FILE *archivo = fopen("transactionDB.dat", "ab");
    char tipo;
    float monto;
    int fecha;
    Transaction transaccion;
    bool continuar = 1;
    while (continuar == 1)
    {
        cout << "Desea realizar una transaccion de ingreso o egreso de dinero?" << endl
             << "i: ingreso / e: egreso" << endl;
        cin >> tipo;
        switch (tipo)
        {
        case 'i':
            cout << "Monto a ingresar: " << endl;
            cin >> monto;
            cout << "Fecha del ingreso (AAAAMMDD):" << endl;
            cin >> fecha;
            break;
        case 'e':
            cout << "Monto a egresar: " << endl;
            cin >> monto;
            cout << "Fecha del egreso (AAAAMMDD):" << endl;
            cin >> fecha;
            monto *= -1;
            break;
        default:
            cout << "Caracter invalido." << endl;
            break;
        }
        transaccion.id = CalcularID();
        cout << "Por favor ingrese nuevamente su usuario para confirmar la transacción" << endl;
        cin.ignore();
        cin.getline(transaccion.user, sizeof(transaccion.user));
        transaccion.amount = monto;
        transaccion.date = fecha;

        fwrite(&transaccion, sizeof(Transaction), 1, archivo);
        cout << "Desea realizar otra transaccion? (0 = No / 1 = Si)" << endl;
        cin >> continuar;
        cin.ignore();
    }
}

void EliminarTransaccionPorID()
{
}

int mostrarTransaccion()
{
    FILE *archivo = fopen("transactionDB.dat", "rb");
    if (archivo != NULL)
    {
        Transaction transaccion;
        while (fread(&transaccion, sizeof(Transaction), 1, archivo) == 1)
        {
            cout << "ID: " << transaccion.id << endl;
            cout << "Usuario: " << transaccion.user << endl;
            cout << "Cantidad: " << transaccion.amount << endl;
            cout << "Fecha: " << transaccion.date << endl;
        }
        fclose(archivo);
    }
    else
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    return 0;
}

int main()
{
    char user[12];
    char password[9];
    Client cliente;
    char continuar;

    cout << "Ingrese su username: " << endl;
    cin >> user;
    cout << "Ingrese su clave: " << endl;
    cin >> password;

    if (validarUsuario(user, password) == 1)
    {
        cout << "Usuario encontrado exitosamente" << endl;
        cout << "Bienvenido!!" << endl;
        cout << "Su saldo inicial en este momento es el siguiente: " << calcularSaldoUsuario(user) << endl;
        mostrarTransaccion();
        cout << "Desea realizar una transaccion? (s si desea, n si no.)" << endl;
        cin >> continuar;
        switch (continuar)
        {
        case 's':
            GestionarTransaccion(cliente);
            break;
        case 'n':
            cout << "El usuario ha decidido no realizar ninguna transaccion. Salir" << endl;
            break;
        default:
            cout << "Caracter invalido." << endl;
        }
    }
    else
    {
        cout << "Usuario no encontrado." << endl;
    }

    return 0;
}