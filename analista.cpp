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

// - Listar las transacciones de un cliente ordenadas por fecha con paginas de a 5 transacciones mostrando id,
// fecha y monto.

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

void listarTransacciones(const char *nombreCliente)
{
    Transaction transTemp;
    Transaction transaccionArray[100];
    int cantTransacciones = 0;
    FILE *archivo = fopen("transactionDB.dat", "rb");
    if (archivo != NULL)
    {
        while (fread(&transTemp, sizeof(Transaction), 1, archivo) == 1)
        {
            if (strcmp(transTemp.user, nombreCliente) == 0)
            {
                transaccionArray[cantTransacciones] = transTemp;
                cantTransacciones++;
            }
        }
        fclose(archivo);
    }
    else
    {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    ordenarPorFecha(transaccionArray, cantTransacciones);
    for (int i = 0; i < cantTransacciones; i++)
    {
        cout << i + 1 << ": " << transaccionArray[i].id << " - " << transaccionArray[i].date << " - " << transaccionArray[i].amount << endl;
        if ((i + 1) % 5 == 0)
        {
            cout << "Presione enter para continuar visualizando las transacciones" << endl;
            cin.ignore();
        }
    }
}

// - Listar la cantidad de ingresos y egresos por mes de un cliente.
void imprimirIngresosEgresosPorMes(Transaction transacciones[], int cantTransacciones)
{
    int ingresosMes = 0;
    int egresosMes = 0;
    int mesActual = 0;

    cout << "Ingresos y Egresos del cliente: " << transacciones->user << endl;
    for (int i = 0; i < cantTransacciones; i++)
    {
        int mesTransaccion = (transacciones[i].date / 100) % 100;

        if (mesActual != mesTransaccion && mesActual != 0)
        {
            cout << "En el mes " << mesActual << "El total de ingreso fue de: " << "El total de egresos fue de: " << egresosMes << endl;
            ingresosMes = 0;
            egresosMes = 0;
        }
        mesActual = mesTransaccion;

        if (transacciones[i].amount > 0)
        {
            ingresosMes += transacciones[i].amount;
        }
        else
        {
            egresosMes += transacciones[i].amount;
        }
    }
    cout << "En el mes " << mesActual << "El total de ingreso fue de: " << "El total de egresos fue de: " << egresosMes << endl;
}

void IngresosyEgresos(const char *nombreCliente)
{

    Transaction transTemp;
    Transaction transaccionArray[100];
    int cantTransacciones = 0;
    FILE *archivo = fopen("transactionDB.dat", "rb");
    if (archivo != NULL)
    {
        while (fread(&transTemp, sizeof(Transaction), 1, archivo) == 1)
        {
            if (strcmp(transTemp.user, nombreCliente) == 0)
            {
                transaccionArray[cantTransacciones] = transTemp;
                cantTransacciones++;
            }
        }
        fclose(archivo);
    }
    else
    {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    ordenarPorFecha(transaccionArray, cantTransacciones);
    imprimirIngresosEgresosPorMes(transaccionArray, cantTransacciones);
}

// - Mostrar el username del cliente, la fecha y monto de la transacción de monto máximo de todos los clientes

void mostrarClientesMaximo()
{

    Transaction transTemp;
    Transaction transMax[100];
    int contadorClientes = 0;

    FILE *archivo = fopen("transactionDB.dat", "rb");
    if (archivo != NULL)
    {

        while (fread(&transTemp, sizeof(Transaction), 1, archivo) == 1)
        {
            bool existeCliente = false;
            for (int i = 0; i < contadorClientes; i++)
            {
                if (strcmp(transMax[i].user, transTemp.user) == 0)
                {

                    if (transTemp.amount > transMax[i].amount)
                    {
                        transMax[i].amount = transTemp.amount;
                        transMax[i].date = transTemp.date;
                    }
                    existeCliente = true;
                    break;
                }
            }
            if (!existeCliente)
            {
                strcpy(transMax[contadorClientes].user, transTemp.user);
                transMax[contadorClientes].amount = transTemp.amount;
                transMax[contadorClientes].date = transTemp.date;
                contadorClientes++;
            }
        }
        fclose(archivo);

        cout << "Transacciones máximas por cliente:" << endl;
        for (int i = 0; i < contadorClientes; i++)
        {
            cout << "El monto maximo de " << transMax[i].user << "con fecha: " << transMax[i].date << "es de: " << transMax[i].amount << endl;
        }
    }
    else
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
}

// - Mostrar el cliente que más ingresos tuvo en los últimos 30 días.

void mayorIngresosUltimoMes()
{

    char fechaActual;
    cout << "Ingrese la fecha actual como AAAAMMDD: " << endl;
    cin >> fechaActual;
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
                IngresosyEgresos(nombreCliente);
            }
            else
            {
                cout << "El cliente ingresado no existe." << endl;
            }
            break;
        case '3':
            mostrarClientesMaximo();
            break;
        case '4':
            mayorIngresosUltimoMes();
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