#include <stdio.h>
#include <stdlib.h>
#define NUM_RECURSOS 3
#define NUM_PROCESOS 5

int recursos_disponibles[NUM_RECURSOS];
int matriz_asignacion[NUM_PROCESOS][NUM_RECURSOS];
int matriz_maxima[NUM_PROCESOS][NUM_RECURSOS];
int matriz_necesidades[NUM_PROCESOS][NUM_RECURSOS];
void inicializar_recursos_disponibles();
void inicializar_matrices();
void mostrar_estado_actual();
void mostrar_matrices();
int verificar_solicitud_segura(int proceso, int solicitud[]);
int verificar_sistema_seguro(int recursos_disponibles[], int matriz_asignacion[][NUM_RECURSOS], int matriz_necesidades[][NUM_RECURSOS]);

void inicializar_recursos_disponibles()
{
    // Inicializar recursos disponibles de forma predeterminada
    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        recursos_disponibles[i] = 1; // Se puede establecer de manera manual
    }
}

void inicializar_matrices()
{
    // Inicializar matrices de forma predeterminada
    for (int i = 0; i < NUM_PROCESOS; ++i)
    {
        for (int j = 0; j < NUM_RECURSOS; ++j)
        {
            matriz_asignacion[i][j] = rand() % (recursos_disponibles[j] + 1);
            matriz_maxima[i][j] = matriz_asignacion[i][j] + rand() % (recursos_disponibles[j] + 1);

            matriz_necesidades[i][j] = matriz_maxima[i][j] - matriz_asignacion[i][j];
        }
    }
}

void mostrar_estado_actual()
{
    printf("\nEstado Actual:\n");
    printf("Recursos Disponibles: ");
    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        printf("%d ", recursos_disponibles[i]);
    }
    printf("\n");
}

void mostrar_matrices()
{
    printf("\nMatrices:\n");
    printf("Matriz de Asignación:\n");
    for (int i = 0; i < NUM_PROCESOS; ++i)
    {
        for (int j = 0; j < NUM_RECURSOS; ++j)
        {
            printf("%d ", matriz_asignacion[i][j]);
        }
        printf("\n");
    }

    printf("Matriz Máxima:\n");
    for (int i = 0; i < NUM_PROCESOS; ++i)
    {
        for (int j = 0; j < NUM_RECURSOS; ++j)
        {
            printf("%d ", matriz_maxima[i][j]);
        }
        printf("\n");
    }

    printf("Matriz Necesidades:\n");
    for (int i = 0; i < NUM_PROCESOS; ++i)
    {
        for (int j = 0; j < NUM_RECURSOS; ++j)
        {
            printf("%d ", matriz_necesidades[i][j]);
        }
        printf("\n");
    }
}

int verificar_solicitud_segura(int proceso, int solicitud[])
{
    // Verificar si la solicitud excede las necesidades del proceso
    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        if (solicitud[i] > matriz_necesidades[proceso][i])
        {
            printf("Error: La solicitud excede las necesidades del proceso %d.\n", proceso + 1);
            return 0;
        }
    }

    // Verificar si la solicitud excede los recursos disponibles
    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        if (solicitud[i] > recursos_disponibles[i])
        {
            printf("Error: La solicitud excede los recursos disponibles.\n");
            return 0;
        }
    }

    // Simular asignación temporal para verificar si el sistema sigue siendo seguro
    int recursos_disponibles_temp[NUM_RECURSOS];
    int matriz_asignacion_temp[NUM_PROCESOS][NUM_RECURSOS];
    int matriz_necesidades_temp[NUM_PROCESOS][NUM_RECURSOS];

    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        recursos_disponibles_temp[i] = recursos_disponibles[i] - solicitud[i];
        matriz_asignacion_temp[proceso][i] = matriz_asignacion[proceso][i] + solicitud[i];
        matriz_necesidades_temp[proceso][i] = matriz_necesidades[proceso][i] - solicitud[i];
    }

    // Verificar si el sistema sigue siendo seguro después de la asignación temporal
    if (verificar_sistema_seguro(recursos_disponibles_temp, matriz_asignacion_temp, matriz_necesidades_temp))
    {
        // Asignar los recursos de manera permanente si la asignación temporal es segura
        for (int i = 0; i < NUM_RECURSOS; ++i)
        {
            recursos_disponibles[i] = recursos_disponibles_temp[i];
            matriz_asignacion[proceso][i] = matriz_asignacion_temp[proceso][i];
            matriz_necesidades[proceso][i] = matriz_necesidades_temp[proceso][i];
        }
        return 1; // La asignación es segura
    }
    else
    {
        printf("Error: La asignación no es segura.\n");
        return 0; // La asignación no es segura
    }
}

int verificar_sistema_seguro(int recursos_disponibles[], int matriz_asignacion[][NUM_RECURSOS], int matriz_necesidades[][NUM_RECURSOS])
{
    int procesos_finalizados[NUM_PROCESOS] = {0}; // Inicialmente, ningún proceso está terminado

    int recursos_disponibles_temp[NUM_RECURSOS];
    int proceso_encontrado;

    // Copiar recursos disponibles
    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        recursos_disponibles_temp[i] = recursos_disponibles[i];
    }

    // Verificar si hay algún proceso no terminado que puede ejecutar
    for (int contador = 0; contador < NUM_PROCESOS; ++contador)
    {
        int proceso_encontrado = -1;

        for (int i = 0; i < NUM_PROCESOS; ++i)
        {
            if (!procesos_finalizados[i])
            {
                // Verificar si el proceso puede ejecutar con los recursos disponibles
                int ejecutable = 1;
                for (int j = 0; j < NUM_RECURSOS; ++j)
                {
                    if (matriz_necesidades[i][j] > recursos_disponibles_temp[j])
                    {
                        ejecutable = 0;
                        break;
                    }
                }

                if (ejecutable)
                {
                    proceso_encontrado = i;
                    break;
                }
            }
        }

        if (proceso_encontrado != -1)
        {
            // Ejecutar el proceso y liberar sus recursos
            procesos_finalizados[proceso_encontrado] = 1;
            for (int j = 0; j < NUM_RECURSOS; ++j)
            {
                recursos_disponibles_temp[j] += matriz_asignacion[proceso_encontrado][j];
            }
        }
        else
        {
            // No se encontró un proceso ejecutable, el sistema no es seguro
            return 0;
        }
    }

    // Si todos los procesos se ejecutaron, el sistema es seguro
    return 1;
}

int main()
{
    inicializar_recursos_disponibles();
    inicializar_matrices();
    mostrar_estado_actual();
    mostrar_matrices();

    // Simular solicitudes y liberaciones de recursos
    int proceso_solicitante = 3; // Se puede escoger que proceso se evaluará como solicitante.
    int solicitud_recursos[NUM_RECURSOS];

    printf("\nIngrese la cantidad de recursos solicitados:\n");
    for (int i = 0; i < NUM_RECURSOS; ++i)
    {
        solicitud_recursos[i] = rand() % (recursos_disponibles[i] + 1);
        printf("Recurso %d: %d\n", i + 1, solicitud_recursos[i]);
    }

    if (verificar_solicitud_segura(proceso_solicitante - 1, solicitud_recursos))
    {
        printf("\nSolicitud concedida.\n");
    }
    else
    {
        printf("\nSolicitud denegada.\n");
    }

    return 0;
}