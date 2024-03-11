#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

sem_t tenedores[NUM_FILOSOFOS]; // Semáforos para representar los tenedores

void *filosofo(void *arg)
{
    int id = *((int *)arg);
    int tenedor_izquierdo = id;
    int tenedor_derecho = (id + 1) % NUM_FILOSOFOS;

    while (1)
    {
        printf("Filósofo %d pensando\n", id);
        sleep(rand() % 3); // Simula tiempo pensando

        printf("Filósofo %d tiene hambre\n", id);

        // Intenta adquirir el tenedor izquierdo
        if (sem_trywait(&tenedores[tenedor_izquierdo]) == 0)
        {
            printf("Filósofo %d adquirió tenedor izquierdo (%d)\n", id, tenedor_izquierdo);
            // Intenta adquirir el tenedor derecho
            if (sem_trywait(&tenedores[tenedor_derecho]) == 0)
            {
                printf("Filósofo %d adquirió tenedor derecho (%d)\n", id, tenedor_derecho);
                printf("Filósofo %d está comiendo\n", id);
                sleep(rand() % 3); // Simula tiempo comiendo
                printf("Filósofo %d liberando tenedores\n", id);
                sem_post(&tenedores[tenedor_izquierdo]);
                sem_post(&tenedores[tenedor_derecho]);
            }
            else
            {
                printf("Filósofo %d no pudo adquirir tenedor derecho (%d), liberando izquierdo\n", id, tenedor_derecho);
                sem_post(&tenedores[tenedor_izquierdo]);
                sleep(1); // Pequeña pausa antes de intentar nuevamente
            }
        }
        else
        {
            printf("Filósofo %d no pudo adquirir tenedor izquierdo (%d), esperando...\n", id, tenedor_izquierdo);
            sleep(1); // Pequeña pausa antes de intentar nuevamente
        }
    }
}

int main()
{
    pthread_t filosofos[NUM_FILOSOFOS];
    int id_filosofos[NUM_FILOSOFOS];

    // Inicializa los semáforos para representar los tenedores
    for (int i = 0; i < NUM_FILOSOFOS; ++i)
    {
        sem_init(&tenedores[i], 0, 1); // Inicializa los semáforos a 1 (disponibles)
    }

    // Inicializa los hilos para representar a los filósofos
    for (int i = 0; i < NUM_FILOSOFOS; ++i)
    {
        id_filosofos[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, (void *)&id_filosofos[i]);
    }

    // Espera a que todos los hilos terminen (no debería ocurrir)
    for (int i = 0; i < NUM_FILOSOFOS; ++i)
    {
        pthread_join(filosofos[i], NULL);
    }

    // Destruye los semáforos
    for (int i = 0; i < NUM_FILOSOFOS; ++i)
    {
        sem_destroy(&tenedores[i]);
    }

    return 0;
}
