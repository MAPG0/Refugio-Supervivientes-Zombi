#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Proyecto: Refugio de supervivientes zombi
 * Practica 3 - Introduccion a la Ingenieria de Software
 * HU-00: Preparar entorno y tablero
 * HU-01: Definir superviviente y refugio
 * HU-02: Registrar supervivientes
 */

typedef struct {
    char nombre[30];
    int edad;
    char habilidad[30];
    int salud;
} Superviviente;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
} Refugio;

void inicializar_refugio(Refugio *r);
void leer_texto(char texto[], int tamanio);
int leer_entero();
int agregar_superviviente(Refugio *r);

int main(void) {
    Refugio refugio;
    int opcion;

    inicializar_refugio(&refugio);

    printf("HU-00: Preparacion del entorno\n");
    printf("Refugio de supervivientes zombi\n");

    do {
        printf("\n===== MENU DEL REFUGIO =====\n");
        printf("1. Registrar superviviente\n");
        printf("0. Salir\n");
        printf("Selecciona una opcion: ");

        opcion = leer_entero();

        switch (opcion) {
            case 1:
                agregar_superviviente(&refugio);
                break;

            case 0:
                printf("Saliendo del programa.\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 0);

    free(refugio.lista);
    refugio.lista = NULL;

    return 0;
}

void inicializar_refugio(Refugio *r) {
    r->lista = NULL;
    r->n = 0;
    r->capacidad = 0;
}

void leer_texto(char texto[], int tamanio) {
    int c;

    fgets(texto, tamanio, stdin);

    if (strchr(texto, '\n') != NULL) {
        texto[strcspn(texto, "\n")] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

int leer_entero() {
    char texto[30];
    int numero;

    if (fgets(texto, 30, stdin) == NULL) {
        return 0;
    }

    while (sscanf(texto, "%d", &numero) != 1) {
        printf("Ingresa un numero valido: ");

        if (fgets(texto, 30, stdin) == NULL) {
            return 0;
        }
    }

    return numero;
}

int agregar_superviviente(Refugio *r) {
    Superviviente nuevo;
    Superviviente *temporal;
    int nueva_capacidad;

    printf("\nNombre: ");
    leer_texto(nuevo.nombre, 30);

    if (strlen(nuevo.nombre) == 0) {
        printf("El nombre no puede estar vacio.\n");
        return 0;
    }

    printf("Edad: ");
    nuevo.edad = leer_entero();

    if (nuevo.edad <= 0) {
        printf("La edad debe ser mayor que cero.\n");
        return 0;
    }

    printf("Habilidad especial: ");
    leer_texto(nuevo.habilidad, 30);

    printf("Salud (0 a 100): ");
    nuevo.salud = leer_entero();

    if (nuevo.salud < 0 || nuevo.salud > 100) {
        printf("La salud debe estar entre 0 y 100.\n");
        return 0;
    }

    if (r->n == r->capacidad) {

        if (r->capacidad == 0) {
            nueva_capacidad = 1;
        } else {
            nueva_capacidad = r->capacidad * 2;
        }

        /* Reservar memoria */
        if (r->lista == NULL) {
            temporal = malloc(nueva_capacidad * sizeof(Superviviente));
        } else {
            temporal = realloc(r->lista,
                               nueva_capacidad * sizeof(Superviviente));
        }

        if (temporal == NULL) {
            printf("No se pudo reservar memoria.\n");
            return 0;
        }

        r->lista = temporal;
        r->capacidad = nueva_capacidad;
    }
    r->lista[r->n] = nuevo;
    r->n++;

    printf("\nSuperviviente registrado correctamente.\n");

    return 1;
}


