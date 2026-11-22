#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Proyecto: Refugio de supervivientes zombi
 * Practica 3 - Introduccion a la Ingenieria de Software
 */

typedef struct {
    char nombre[30];
    int edad;
    char habilidad[30];
    int salud;
} Superviviente;

typedef struct {
    int agua;
    int comida;
    int medicamentos;
    int municiones;
} Recursos;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
    Recursos recursos;
} Refugio;

void inicializar_refugio(Refugio *r);
void inicializar_recursos(Refugio *r);
void leer_texto(char texto[], int tamanio);
int leer_entero(void);

int agregar_superviviente(Refugio *r);
void listar_supervivientes(const Refugio *r);
void mostrar_recursos(const Refugio *r);

int modificar_recurso(Refugio *r, int tipo, int cantidad);

int main(void) {

    Refugio refugio;
    int opcion;
    int tipo;
    int cantidad;

    inicializar_refugio(&refugio);

    printf("HU-00: Preparacion del entorno\n");
    printf("Refugio de supervivientes zombi\n");

    do {

        printf("\n===== MENU DEL REFUGIO =====\n");
        printf("1. Registrar superviviente\n");
        printf("2. Listar supervivientes\n");
        printf("3. Mostrar recursos\n");
        printf("4. Registrar entrada de recursos\n");
        printf("5. Registrar salida de recursos\n");
        printf("0. Salir\n");
        printf("Selecciona una opcion: ");

        opcion = leer_entero();

        switch (opcion) {

            case 1:
                agregar_superviviente(&refugio);
                break;

            case 2:
                listar_supervivientes(&refugio);
                break;

            case 3:
                mostrar_recursos(&refugio);
                break;

            case 4:

                printf("\n===== ENTRADA DE RECURSOS =====\n");
                printf("1. Agua\n");
                printf("2. Comida\n");
                printf("3. Medicamentos\n");
                printf("4. Municiones\n");
                printf("Selecciona el recurso: ");

                tipo = leer_entero();

                printf("Cantidad que ingresa: ");
                cantidad = leer_entero();

                if (modificar_recurso(&refugio, tipo, cantidad)) {
                    printf("\nEntrada registrada correctamente.\n");
                } else {
                    printf("\nNo se pudo registrar la entrada.\n");
                }

                break;

            case 5:

                printf("\n===== SALIDA DE RECURSOS =====\n");
                printf("1. Agua\n");
                printf("2. Comida\n");
                printf("3. Medicamentos\n");
                printf("4. Municiones\n");
                printf("Selecciona el recurso: ");

                tipo = leer_entero();

                printf("Cantidad que sale: ");
                cantidad = leer_entero();

                /* Los tipos del 5 al 8 representan salidas */
                tipo = tipo + 4;

                if (modificar_recurso(&refugio, tipo, cantidad)) {
                    printf("\nSalida registrada correctamente.\n");
                } else {
                    printf("\nNo se pudo registrar la salida.\n");
                }

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

    inicializar_recursos(r);
}
void inicializar_recursos(Refugio *r) {

    r->recursos.agua = 100;
    r->recursos.comida = 100;
    r->recursos.medicamentos = 20;
    r->recursos.municiones = 50;
}
void leer_texto(char texto[], int tamanio) {

    int c;

    if (fgets(texto, tamanio, stdin) == NULL) {
        texto[0] = '\0';
        return;
    }

    if (strchr(texto, '\n') != NULL) {
        texto[strcspn(texto, "\n")] = '\0';
    } else {

        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}
int leer_entero(void) {

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

        if (r->lista == NULL) {

            temporal = malloc(
                nueva_capacidad * sizeof(Superviviente)
            );

        } else {

            temporal = realloc(
                r->lista,
                nueva_capacidad * sizeof(Superviviente)
            );
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
void listar_supervivientes(const Refugio *r) {

    printf("\n===== SUPERVIVIENTES DEL REFUGIO =====\n");

    if (r->n == 0) {
        printf("No hay supervivientes registrados.\n");
        return;
    }

    for (int i = 0; i < r->n; i++) {

        printf("\nSuperviviente #%d\n", i + 1);
        printf("Nombre: %s\n", r->lista[i].nombre);
        printf("Edad: %d\n", r->lista[i].edad);
        printf("Habilidad: %s\n", r->lista[i].habilidad);
        printf("Salud: %d\n", r->lista[i].salud);
    }

    printf("\nTotal de supervivientes: %d\n", r->n);
}

void mostrar_recursos(const Refugio *r) {

    printf("\n===== RECURSOS DEL REFUGIO =====\n");

    printf("Agua: %d\n", r->recursos.agua);
    printf("Comida: %d\n", r->recursos.comida);
    printf("Medicamentos: %d\n", r->recursos.medicamentos);
    printf("Municiones: %d\n", r->recursos.municiones);
}
int modificar_recurso(Refugio *r, int tipo, int cantidad) {

    int *recurso;
    if (cantidad <= 0) {
        return 0;
    }
    switch (tipo) {

        case 1:
        case 5:
            recurso = &r->recursos.agua;
            break;

        case 2:
        case 6:
            recurso = &r->recursos.comida;
            break;

        case 3:
        case 7:
            recurso = &r->recursos.medicamentos;
            break;

        case 4:
        case 8:
            recurso = &r->recursos.municiones;
            break;

        default:
            return 0;
    }
    if (tipo >= 1 && tipo <= 4) {

        *recurso = *recurso + cantidad;

    }
    else if (tipo >= 5 && tipo <= 8) {

        if (cantidad > *recurso) {
            return 0;
        }

        *recurso = *recurso - cantidad;
    }

    return 1;
}
