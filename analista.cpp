// Gestor de transacciones para el banco --> Barbi
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

int validarUsuarioAdmin(char *UsuarioBuscado, char *PasswordBuscada)
{

    if (strcmp("admin", UsuarioBuscado) == 0)
    {
        if (strcmp("admin", PasswordBuscada) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int validarUsuarioCliente(const char *UsuarioBuscado)
{
    FILE *archivo = fopen("userDB.dat", "rb");
    if (archivo != NULL)
    {
        Client usuario;
        while (fread(&usuario, sizeof(Client), 1, archivo) == 1)
        {
            if (strcmp(usuario.user, UsuarioBuscado) == 0)
            {
                fclose(archivo);
                return 1;
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

void listarTransacciones(const char *nombreCliente)
{

    FILE *file = fopen("userDB.dat", "rb");
    if (file != NULL)
    {
        Transaction transaccionArray[100];
        int cantTransacciones = 0;
        leerArchivoTransacciones(transaccionArray, &cantTransacciones, &nombreCliente);
        // ObtenerTransaccionesXCliente(&nombreCliente, cantTransacciones, transaccionArray);
        ordenarPorFecha(transaccionArray, cantTransacciones);
        for (int i = 0; i < cantTransacciones; i++)
        {
            cout << i + 1 << ": " << transaccionArray[i].id << "-" << transaccionArray[i].date << "-" << transaccionArray[i].amount << endl;
            if ((i + 1) % 5 == 0)
            {
                cout << "Presione enter para continuar visualizando las transacciones" << endl;
                cin.ignore();
            }
        }
    }
    else
    {
        cout << "No se pudo abrir el archivo para lectura" << endl;
    }
}

void leerArchivoTransacciones(Transaction transacciones[], int *cantTransacciones, char *nombreCliente)
{

    FILE *archivo = fopen("transactionDB.dat", "rb");
    if (archivo != NULL)
    {
        Transaction transTemp; // Variable temporal para leer transacciones
        *cantTransacciones = 0;

        while (fread(&transTemp, sizeof(Transaction), 1, archivo) == 1)
        {
            // Comparar el usuario de la transacción con el nombreCliente
            if (strcmp(transTemp.user, nombreCliente) == 0)
            {
                // Guardar la transacción en el array
                cout << "entro al if" << endl;
                cout << cantTransacciones << endl;
                transacciones[*cantTransacciones] = transTemp;
                (*cantTransacciones)++;
            }
        }

        fclose(archivo);
    }
    else
    {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
}

void ordenarPorFecha(Transaction transacciones[], int cantTransacciones)
{

    cout << "Entro en ordenar fecha" << endl;
    cout << cantTransacciones << endl;

    for (int i = 0; i < cantTransacciones - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < cantTransacciones; j++)
        {
            if (transacciones[j].date < transacciones[minIndex].date)
            {
                minIndex = j;
            }
        }
        Transaction temp = transacciones[i];
        transacciones[i] = transacciones[minIndex];
        transacciones[minIndex] = temp;
    }
}

int main()
{
    char user[12];
    char password[15];
    char continuar;
    char nombreCliente[15];

    cout << "Ingrese su username: " << endl;
    cin >> user;
    cout << "Ingrese su clave: " << endl;
    cin >> password;

    if (validarUsuarioAdmin(user, password) == 1)
    {
        cout << "Bienvenido Usuario Administrador" << endl;
        cout << "MENU:" << endl;
        cout << "1: Si desea visualizar las transacciones de un cliente ordenadas por fecha" << endl;
        cout << "2: Si desea visualizar la cantidad de ingresos y egresos del ultimo mes de un cliente" << endl;
        cout << "3: Si desea visualizar la transacción de monto maximo de todos los clientes:" << endl;
        cout << "4: Si desea visualizar el cliente que más ingresos tuvo en los ultimos 30 dias:" << endl;
        cin >> continuar;
        switch (continuar)
        {
        case '1':
            cout << "Ingrese el nombre del cliente para visualizar sus transacciones:" << endl;
            cin >> nombreCliente;
            if (validarUsuarioCliente(nombreCliente) == 1)
            {
                listarTransacciones(nombreCliente);
            }
            else
            {
                cout << "El cliente ingresado no existe." << endl;
            }
            break;
        case '2':
            cout << "Ingrese el nombre del cliente para visualizar sus transacciones:" << endl;
            cin >> nombreCliente;
            if (validarUsuarioCliente(nombreCliente) == 1)
            {
                // IngresosyEgresos(nombreCliente);
            }
            else
            {

                cout << "El cliente ingresado no existe." << endl;
            }
            break;
        case '3':
            // mostrarClientesMaximo();
            break;
        case '4':
            // FUNCION;
            break;
        default:
            cout << "Caracter invalido." << endl;
            break;
        }
    }
    else
    {
        cout << "El usuario ingresado no tiene permisos de administrador" << endl;
    }

    return 0;
}
