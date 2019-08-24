///SRTF
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
void insertarMedio(PROCESO **, PROCESO **);
void insertarInicio(PROCESO **, PROCESO **);
void recorrer(PROCESO *);
void mostrar(PROCESO *);
PROCESO *extraerInicio(PROCESO **);
PROCESO *extraerMedio(PROCESO **, pid_t);
void crearHijo(PROCESO **);
void realizarTarea();
void ordenar(PROCESO **);
int main()
{
    int pidControlador = getpid();
    printf("Proceso %d\n", pidControlador);
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    PROCESO *cabeza = NULL;
    PROCESO *elem = NULL;
    pid_t procesoActual;
    for (int i = 0; i < 5; i++)
    {
        elem = nuevoElem(i);
        insertarFinal(&elem, &cabeza);
    }
    recorrer(cabeza);
    ordenar(&cabeza);
    printf("------\n");
    recorrer(cabeza);
    sleep(10);
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
                if (cabeza != NULL)
                {
                    kill(procesoActual, SIGCONT);
                    sleep(1);
                    printf("Proceso %d duracion%d\n", cabeza->pid, cabeza->duracion);
                    kill(procesoActual, SIGSTOP);
                    cabeza->duracion = cabeza->duracion - 1;
                    if (cabeza->duracion <= 0)
                    {
                        extraerInicio(&cabeza);
                        printf("---------\n");
                        printf("¡Terminé! %d\n", procesoActual);
                        printf("---------\n");
                    }
                    else
                    {
                        ordenar(&cabeza);
                    }
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
    tmp->duracion = rand() % 121 + 1;
    printf("----%d\n", tmp->duracion);
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
void insertarMedio(PROCESO **elem, PROCESO **aux1)
{
    if (aux1 == NULL)
    {
        *aux1 = *elem;
        elem = NULL;
    }
    else
    {
        (*elem)->sig = (*aux1)->sig;
        (*aux1)->sig = *elem;
    }
}
void insertarInicio(PROCESO **elem, PROCESO **cabeza)
{
    if (cabeza == NULL)
    {
        *cabeza = *elem;
        *elem = NULL;
    }
    else
    {
        if ((*cabeza)->sig == NULL)
        {
            printf("sig es null\n");
        }
        (*elem)->sig = *cabeza;
        *cabeza = *elem;
        printf(" sig es %d\n", ((*cabeza)->sig)->pid);
        printf("apunto a %d\n", (*cabeza)->pid);
        recorrer(*cabeza);
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
PROCESO *extraerMedio(PROCESO **cabeza, pid_t pid)
{
    PROCESO *elem = *cabeza;
    PROCESO *elem1 = NULL;
    if (elem->sig == NULL)
    {
        cabeza = NULL;
        return elem;
    }
    else
    {
        elem1 = elem->sig;
        if (elem->pid == pid)
        {
            return extraerInicio(cabeza);
        }
        else if (elem1->pid == pid)
        {
            elem->sig = elem1->sig;
            elem1->sig = NULL;
            return elem1;
        }
        else
        {
            while (elem1->pid != pid)
            {
                elem = elem1;
                if (elem1->sig == NULL)
                {
                    printf("elem1 es null\n");
                }
                elem1 = elem1->sig;
            }
            elem->sig = elem1->sig;
            elem1->sig = NULL;
            return elem1;
        }
    }
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
void ordenar(PROCESO **cabeza)
{
    PROCESO *tmp = NULL;
    PROCESO *aux = *cabeza;
    PROCESO *elem = NULL;
    int pidMenor;
    while (aux->sig != NULL)
    {
        tmp = aux;
        //Obtener el más pequeño de todos
        int tMenor = 100;
        while (tmp->sig != NULL)
        {
            if (tmp->duracion < tMenor)
            {
                tMenor = tmp->duracion;
                pidMenor = tmp->pid;
            }
            tmp = tmp->sig;
        }
        if (tmp->sig == NULL)
        {
            if (tmp->duracion < tMenor)
            {
                tMenor = tmp->duracion;
                pidMenor = tmp->pid;
            }
        }
        //Colocarlo en la cabeza
        if (pidMenor != (*cabeza)->pid)
        {
            if ((aux)->pid == 0)
            {
                printf(" esto de aqui%d\n", pidMenor);
            }
            elem = extraerMedio(cabeza, pidMenor);
            printf("llegue\n");
            insertarInicio(&elem, cabeza);
        }
        aux = aux->sig;
    }
}