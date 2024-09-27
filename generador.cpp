// Gestor de transacciones para el cliente final --> Diego
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

    if (archivo == NULL)
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return 0;
    }

    while (fread(&transaccion, sizeof(transaccion), 1, archivo) == 1)
    {
        if (strcmp(transaccion.user, usuarioBuscado) == 0)
        {
            saldoTotal += transaccion.amount;
        }
    }

    fclose(archivo);
    return saldoTotal;
}

int CalcularID()
{
    FILE *archivo = fopen("transactionDB.dat", "rb");
    int mayor = 0;
    Transaction transaccion;

    if (archivo == NULL)
    {
        cout << "Archivo no encontrado." << endl;
        return -1;
    }

    while (fread(&transaccion, sizeof(Transaction), 1, archivo) == 1)
    {
        if (transaccion.id > mayor)
        {
            mayor = transaccion.id;
        }
    }

    fclose(archivo);
    return mayor + 1;
}

int mostrarTransaccion(const char *usuarioActual)
{
    FILE *archivo = fopen("transactionDB.dat", "rb");
    if (archivo != NULL)
    {
        Transaction transaccion;
        cout << "Transacciones del usuario: " << usuarioActual << endl;
        while (fread(&transaccion, sizeof(Transaction), 1, archivo) == 1)
        {
            if (strcmp(transaccion.user, usuarioActual) == 0)
            {
                cout << "ID: " << transaccion.id << endl;
                cout << "Cantidad: " << transaccion.amount << endl;
                cout << "Fecha: " << transaccion.date << endl;
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

void GestionarTransaccion(Client cliente)
{
    FILE *archivo = fopen("transactionDB.dat", "ab");
    char tipo;
    float monto;
    int fecha;
    Transaction transaccion;

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
        return;
    }

    transaccion.id = CalcularID();
    cout << "Por favor ingrese nuevamente su usuario para confirmar la transacción" << endl;
    cin.ignore();
    cin.getline(transaccion.user, sizeof(transaccion.user));
    transaccion.amount = monto;
    transaccion.date = fecha;

    fwrite(&transaccion, sizeof(Transaction), 1, archivo);
    fclose(archivo);

    cout << "Transacción realizada con éxito. Para realizar otra, debe salir y volver a ingresar." << endl;
}

void EliminarTransaccionPorID()
{
    FILE *archivo = fopen("transactionDB.dat", "rb");
    FILE *archivoTemporal = fopen("tempTransactionDB.dat", "wb");
    if (archivo != NULL)
    {
        Transaction transaccion;
        int id;
        bool encontrado;
        cout << "Ingrese el ID de la transaccion que desea eliminar: " << endl;
        cin >> id;

        while (fread(&transaccion, sizeof(transaccion), 1, archivo) == 1)
        {
            if (transaccion.id == id)
            {
                encontrado = true;
            }
            else
            {
                fwrite(&transaccion, sizeof(Transaction), 1, archivoTemporal);
            }
        }
        fclose(archivo);
        fclose(archivoTemporal);

        if (encontrado)
        {
            remove("transactionDB.dat");
            rename("tempTransactionDB.dat", "transactionDB.dat");
            cout << "La transacción ha sido eliminada con éxito." << endl;
        }
        else
        {
            remove("tempTransactionDB.dat");
            cout << "No se encontró ninguna transacción con el ID proporcionado." << endl;
        }
    }
}

int main()
{
    char user[12];
    char password[9];
    Client cliente;
    int continuar;
    bool usuarioActivo = true;

    cout << "Ingrese su username: " << endl;
    cin >> user;
    cout << "Ingrese su clave: " << endl;
    cin >> password;

    if (validarUsuario(user, password) == 1)
    {
        cout << "Usuario encontrado exitosamente" << endl;
        cout << "Bienvenido!!" << endl;
        cout << "Su saldo inicial en este momento es el siguiente: " << calcularSaldoUsuario(user) << endl;
        bool sesionActiva = true;

        while (sesionActiva)
        {
            cout << "Menu principal:" << endl;
            cout << "1. Ver información de todas las transacciones" << endl;
            cout << "2. Realizar una transacción" << endl;
            cout << "3. Eliminar una transacción" << endl;
            cout << "4. Salir" << endl;
            cout << "Seleccione una opción: ";
            cin >> continuar;

            switch (continuar)
            {
            case 1:
                mostrarTransaccion(user);
                break;
            case 2:
                GestionarTransaccion(cliente);
                usuarioActivo = false;
                break;
            case 3:
                EliminarTransaccionPorID();
                break;
            case 4:
                cout << "Saliendo del programa." << endl;
                sesionActiva = false;
                break;
            default:
                cout << "Caracter invalido. Por favor, seleccione una opción válida." << endl;
            }

            if (continuar != 4 && sesionActiva == true)
            {
                cout << "Presione cualquier tecla para volver al menú principal..." << endl;
                cin.ignore();
                cin.get();
            }
        }
    }
    else
    {
        cout << "Usuario no encontrado." << endl;
    }

    return 0;
}