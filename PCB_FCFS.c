//FCFS
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define CREAR 1
#define NADA 0

typedef struct Proceso PROCESO;

int estado = NADA;

struct Proceso
{
    pid_t pid;
    int duracion;
    PROCESO *sig;
};

void sig_handler(int);
PROCESO *nuevoElem();
void insertarFinal(PROCESO **, PROCESO **);
void recorrer(PROCESO *);
void mostrar(PROCESO *);
PROCESO *extraerInicio(PROCESO **);
void crearHijo(PROCESO **);
void realizarTarea();

int main()
{

    int pidControlador = getpid();
    printf("Proceso %d\n", pidControlador);
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    PROCESO *cabeza = NULL;
    PROCESO *elem = NULL;
    pid_t procesoActual;
    for (;;)
    { //Gestion de procesos
        if (estado == CREAR)
        {
            crearHijo(&cabeza);
            estado = NADA;
        }
        else
        { //Gestion
            if (cabeza != NULL)
            {
                procesoActual = cabeza->pid;
                if (cabeza->duracion <= 0)
                {
                    extraerInicio(&cabeza);
                    printf("---------\n");
                    printf("¡Terminé! %d\n", procesoActual);
                    printf("---------\n");
                }
                if (cabeza != NULL)
                {
                    kill(procesoActual, SIGCONT);
                    sleep(1);
                    kill(procesoActual, SIGSTOP);
                    cabeza->duracion = cabeza->duracion - 1;
                    elem = extraerInicio(&cabeza);
                    insertarFinal(&elem, &cabeza);
                }
                //Hacer trabajar al que esta a la cabeza 1 unidad de tiempo
                //Pausarlo
                //Reducir 1 unidad de tiempo su tiempo de ejecucion
                //Si acaba sacarlo de la cola
                //Si no mandarlo al final
            }
            else
            {
                printf("No hay procesos en cola\n");
                sleep(1);
            }
        }
    }
}
void sig_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        estado = CREAR;
    }
}

PROCESO *nuevoElem(pid_t unProc)
{
    PROCESO *tmp = NULL;
    tmp = (PROCESO *)malloc(sizeof(PROCESO));
    tmp->pid = unProc;
    tmp->duracion = rand() % 11 + 1;
    tmp->sig = NULL;
    return tmp;
}
void insertarFinal(PROCESO **elem, PROCESO **cabeza)
{
    if (*cabeza == NULL)
    {
        *cabeza = *elem;
        *elem = NULL;
    }
    else
    {
        PROCESO *tmp = *cabeza;
        while (tmp->sig != NULL)
        {
            tmp = tmp->sig;
        }
        if (tmp == NULL)
            fprintf(stderr, "error es NULL\n");
        else
            tmp->sig = *elem;
        *elem = NULL;
    }
}
PROCESO *extraerInicio(PROCESO **cabeza)
{
    PROCESO *elem = NULL;
    if (*cabeza == NULL)
    {
        printf("La cadena esta vacia\n");
        return NULL;
    }
    else
    {
        elem = *cabeza;
        if (elem->sig != NULL)
        {
            *cabeza = elem->sig;
            elem->sig = NULL;
        }
        else
        {
            *cabeza = NULL;
        }
    }
    return elem;
}
void recorrer(PROCESO *cabeza)
{
    PROCESO *tmp = cabeza;
    while (tmp != NULL)
    {
        mostrar(tmp);
        tmp = tmp->sig;
    }
}
void crearHijo(PROCESO **cabeza)
{
    pid_t proceso;
    PROCESO *elem = NULL;
    proceso = fork();
    if (proceso != 0)
    {
        kill(proceso, SIGSTOP);
        elem = nuevoElem(proceso);
        insertarFinal(&elem, cabeza);
    }
    else
    {
        realizarTarea();
        exit(EXIT_SUCCESS);
    }
}
void realizarTarea()
{
    for (;;)
    {
        printf("%d trabajando\n", getpid());
        sleep(1);
    }
}
void mostrar(PROCESO *elem)
{
    printf("Pid %i\n", elem->pid);
}