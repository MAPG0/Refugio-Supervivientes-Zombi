#include <stdio.h>
# include <string.h>
#include <time.h>

#define ARCH_SUP "supervivientes.txt"
#define ARCH_REC "recursos.txt"
#define ARCH_HIS "historial.txt"

/*
 * Proyecto: Refugio de supervivientes zombi
 * Practica 3 - Introduccion a la Ingenieria de Software
 * HU-00: Preparar entorno y tablero
 */

typedef struct {
    char *nombre;
    int edad;
    char *habilidad;
    int salud;
} Superviviente;

typedef struct {
    int agua, comida, medicamentos, municiones;
} Recursos;

typedef struct {
    int dia;
    char *descripcion;
} Evento;

typedef struct {
    Superviviente *lista;
    int n, capacidad;
    Recursos recursos;
    Evento *historial;
    int nh, ch;
    int dia;
} Refugio;


char *leer_cadena() {
  void inicializar_refugio(Refugio *r) {
    r->lista = malloc(sizeof(Superviviente));
    r->n = 0;
    r->capacidad = 1;

    r->historial = malloc(sizeof(Evento));
    r->nh = 0;
    r->ch = 1;

    r->recursos.agua = 10;
    r->recursos.comida = 10;
    r->recursos.medicamentos = 5;
    r->recursos.municiones = 10;
  char *cadena;
    char caracter;
    int n;

    cadena = malloc(sizeof(char));
    n = 0;

    if (cadena == NULL) {
        printf("No hay memoria disponible.\n");
        exit(1);
    }

    while ((caracter = getchar()) != '\n' && caracter != EOF) {
        cadena = realloc(cadena, (n + 2) * sizeof(char));

        if (cadena == NULL) {
            printf("No hay memoria disponible.\n");
            exit(1);
        }

        cadena[n] = caracter;
        n++;
    }

    cadena[n] = '\0';
    return cadena;
}

int leer_entero() {
    int numero;
    scanf("%d", &numero);
    getchar();
    return numero;
}

void inicializar_refugio(Refugio *r);
void registrar_evento(Refugio *r, char *descripcion);
void agregar_superviviente(Refugio *r);
void listar_supervivientes(Refugio *r);
void mostrar_recursos(Refugio *r);

int main(void)
{
    printf("HU-00: Preparacion del entorno\n");
    printf("Refugio de supervivientes zombi\n");

    return 0;
}


void inicializar_refugio(Refugio *r) {
    r->lista = malloc(sizeof(Superviviente));
    r->n = 0;
    r->capacidad = 1;

    r->historial = malloc(sizeof(Evento));
    r->nh = 0;
    r->ch = 1;

    r->recursos.agua = 10;
    r->recursos.comida = 10;
    r->recursos.medicamentos = 5;
    r->recursos.municiones = 10;

    r->dia = 1;
}

void registrar_evento(Refugio *r, char *descripcion) {
    if (r->nh == r->ch) {
        r->ch = r->ch * 2;
        r->historial = realloc(r->historial, r->ch * sizeof(Evento));
    }

    r->historial[r->nh].dia = r->dia;
    r->historial[r->nh].descripcion = malloc(strlen(descripcion) + 1);
    strcpy(r->historial[r->nh].descripcion, descripcion);

    r->nh++;
}

void agregar_superviviente(Refugio *r) {
    char *nombre;
    char *habilidad;
    int edad;

    printf("Nombre: ");
    nombre = leer_cadena();

    printf("Edad: ");
    edad = leer_entero();

    printf("Habilidad: ");
    habilidad = leer_cadena();

    if (r->n == r->capacidad) {
        r->capacidad = r->capacidad * 2;
        r->lista = realloc(r->lista, r->capacidad * sizeof(Superviviente));
    }

    r->lista[r->n].nombre = nombre;
    r->lista[r->n].edad = edad;
    r->lista[r->n].habilidad = habilidad;
    r->lista[r->n].salud = 100;

    r->n++;

    registrar_evento(r, "Se agrego un superviviente.");
    printf("Superviviente agregado.\n");
}

void listar_supervivientes(Refugio *r) {
    int i;

    if (r->n == 0) {
        printf("No hay supervivientes.\n");
        return;
    }

    for (i = 0; i < r->n; i++) {
        printf("\nNombre: %s\n", r->lista[i].nombre);
        printf("Edad: %d\n", r->lista[i].edad);
        printf("Habilidad: %s\n", r->lista[i].habilidad);
        printf("Salud: %d\n", r->lista[i].salud);
    }
}

void mostrar_recursos(Refugio *r) {
    printf("\nAgua: %d\n", r->recursos.agua);
    printf("Comida: %d\n", r->recursos.comida);
    printf("Medicamentos: %d\n", r->recursos.medicamentos);
    printf("Municiones: %d\n", r->recursos.municiones);
}

