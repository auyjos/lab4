#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Definir dos recursos (locks)
pthread_mutex_t resource_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource_2 = PTHREAD_MUTEX_INITIALIZER;

// Función para el proceso 1
void *process_1(void *arg)
{
    while (1)
    {
        printf("Proceso 1 intentando adquirir recurso 1\n");
        pthread_mutex_lock(&resource_1);
        printf("Proceso 1 adquirió recurso 1\n");
        sleep(1); // Simula trabajo
        printf("Proceso 1 intentando adquirir recurso 2\n");
        pthread_mutex_lock(&resource_2);
        printf("Proceso 1 adquirió recurso 2\n");
        sleep(1); // Simula trabajo
        printf("Proceso 1 liberando recursos\n");
        pthread_mutex_unlock(&resource_2);
        pthread_mutex_unlock(&resource_1);
        sleep(1); // Breve pausa para mejor visibilidad
    }
}

// Función para el proceso 2
void *process_2(void *arg)
{
    while (1)
    {
        printf("Proceso 2 intentando adquirir recurso 2\n");
        pthread_mutex_lock(&resource_2);
        printf("Proceso 2 adquirió recurso 2\n");
        sleep(1); // Simula trabajo
        printf("Proceso 2 intentando adquirir recurso 1\n");
        pthread_mutex_lock(&resource_1);
        printf("Proceso 2 adquirió recurso 1\n");
        sleep(1); // Simula trabajo
        printf("Proceso 2 liberando recursos\n");
        pthread_mutex_unlock(&resource_1);
        pthread_mutex_unlock(&resource_2);
        sleep(1); // Breve pausa para mejor visibilidad
    }
}

int main()
{
    pthread_t thread_1, thread_2;

    // Crear dos hilos para ejecutar los procesos
    pthread_create(&thread_1, NULL, process_1, NULL);
    pthread_create(&thread_2, NULL, process_2, NULL);

    // Esperar a que los hilos terminen (esto nunca debería ocurrir)
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return 0;
}
