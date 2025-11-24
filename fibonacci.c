#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct 
{
    int *arr;
    int n;
} FibData;

// Función que ejecutará el hilo trabajador
void* generate_fibonacci(void* arg) {
    FibData* data = (FibData*) arg;
    int n = data->n;
    int* arr = data->arr;

    if (n >= 1) arr[0] = 0;
    if (n >= 2) arr[1] = 1;

    for (int i = 2; i < n; i++) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Uso: %s <num_elementos>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        printf("El número de elementos debe ser mayor a 0.\n");
        return 1;
    }

    double fTimeStart, fTimeEnd;

    // Reservar arreglo dinámico
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Estructura que pasaremos al hilo
    FibData data;
    data.arr = arr;
    data.n = n;

    pthread_t worker;

    // Medición del tiempo
    fTimeStart = (double)clock() / CLOCKS_PER_SEC;

    // Crear hilo trabajador
    pthread_create(&worker, NULL, generate_fibonacci, (void*)&data);

    // Esperar a que termine
    pthread_join(worker, NULL);

    fTimeEnd = (double)clock() / CLOCKS_PER_SEC;

    // Imprimir secuencia calculada
    printf("Secuencia de Fibonacci (%d elementos):\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Tiempo de ejecución: %f segundos\n", fTimeEnd - fTimeStart);

    free(arr);
    return 0;
}
