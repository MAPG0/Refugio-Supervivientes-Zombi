#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

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
    int dia;
    char descripcion[100];
} Evento;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
    Recursos recursos;
    Evento *historial;
    int cantidad_eventos;
    int capacidad_eventos;
    int dia;
} Refugio;

void inicializar_refugio(Refugio *r);
void inicializar_recursos(Refugio *r);
void leer_texto(char texto[], int tamanio);
int leer_entero(void);
int agregar_superviviente(Refugio *r);
void listar_supervivientes(const Refugio *r);
void mostrar_recursos(const Refugio *r);
int modificar_recurso(Refugio *r, int tipo, int cantidad);
int registrar_evento(Refugio *r, const char *descripcion);
void mostrar_eventos(const Refugio *r);
int dar_baja_superviviente(Refugio *r, const char *nombre);
void realizar_expedicion(Refugio *r);
void perder_recursos(Refugio *r, int tipo, int cantidad);
int guardar_partida(const Refugio *r, const char *archivo);
int cargar_partida(Refugio *r, const char *archivo);
void evento_aleatorio(Refugio *r);
void liberar_refugio(Refugio *r);
void mostrar_documentacion(void);
int seleccionar_recurso(void);

int main(void) {
    Refugio refugio;
    int opcion;

    srand((unsigned int)time(NULL));
    inicializar_refugio(&refugio);

    printf("HU-00: Preparacion del entorno\n");
    printf("Refugio de supervivientes zombi\n");

    do {
        printf("\n MENU DEL REFUGIO \n");
        printf("1. Registrar superviviente\n");
        printf("2. Listar supervivientes\n");
        printf("3. Mostrar recursos\n");
        printf("4. Modificar recursos\n");
        printf("5. Mostrar historial de eventos\n");
        printf("6. Dar de baja superviviente\n");
        printf("7. Realizar expedicion\n");
        printf("8. Perdida de recursos\n");
        printf("9. Guardar partida\n");
        printf("10. Cargar partida\n");
        printf("11. Ejecutar evento aleatorio\n");
        printf("12. Mostrar documentacion de funciones\n");
        printf("0. Guardar y salir\n");
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
            case 4: {
                int tipo = seleccionar_recurso();
                if (tipo != 0) {
                    printf("Cantidad (positiva agrega, negativa retira): ");
                    modificar_recurso(&refugio, tipo, leer_entero());
                }
                break;
            }
            case 5:
                mostrar_eventos(&refugio);
                break;
            case 6: {
                char nombre[30];
                printf("Nombre del superviviente a dar de baja: ");
                leer_texto(nombre, sizeof(nombre));
                dar_baja_superviviente(&refugio, nombre);
                break;
            }
            case 7:
                realizar_expedicion(&refugio);
                break;
            case 8: {
                int tipo = seleccionar_recurso();
                if (tipo != 0) {
                    printf("Cantidad perdida: ");
                    perder_recursos(&refugio, tipo, leer_entero());
                }
                break;
            }
            case 9:
                guardar_partida(&refugio, "partida.txt");
                break;
            case 10:
                cargar_partida(&refugio, "partida.txt");
                break;
            case 11:
                evento_aleatorio(&refugio);
                break;
            case 12:
                mostrar_documentacion();
                break;
            case 0:
                guardar_partida(&refugio, "partida.txt");
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    liberar_refugio(&refugio);
    return 0;
}


void inicializar_refugio(Refugio *r) {
    r->lista = NULL;
    r->n = 0;
    r->capacidad = 0;
    r->historial = NULL;
    r->cantidad_eventos = 0;
    r->capacidad_eventos = 0;
    r->dia = 1;
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
    char texto[100];
    char extra;
    int numero;

    while (1) {
        if (fgets(texto, sizeof(texto), stdin) == NULL) {
            return 0;
        }
        if (sscanf(texto, " %d %c", &numero, &extra) == 1) {
            return numero;
        }
        printf("Ingresa un numero entero valido: ");
    }
}


int agregar_superviviente(Refugio *r) {
    Superviviente nuevo;
    Superviviente *temporal;
    int nueva_capacidad;

    printf("\nNombre: ");
    leer_texto(nuevo.nombre, sizeof(nuevo.nombre));
    if (nuevo.nombre[0] == '\0') {
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
    leer_texto(nuevo.habilidad, sizeof(nuevo.habilidad));
    printf("Salud (0 a 100): ");
    nuevo.salud = leer_entero();
    if (nuevo.salud < 0 || nuevo.salud > 100) {
        printf("La salud debe estar entre 0 y 100.\n");
        return 0;
    }

    if (r->n == r->capacidad) {
        nueva_capacidad = (r->capacidad == 0) ? 1 : r->capacidad * 2;
        temporal = realloc(r->lista, (size_t)nueva_capacidad * sizeof(*temporal));
        if (temporal == NULL) {
            printf("No se pudo reservar memoria.\n");
            return 0;
        }
        r->lista = temporal;
        r->capacidad = nueva_capacidad;
    }
    r->lista[r->n] = nuevo;
    r->n++;
    printf("Superviviente registrado correctamente.\n");
    return 1;
}


void listar_supervivientes(const Refugio *r) {
    int i;
    printf("\n SUPERVIVIENTES DEL REFUGIO \n");
    if (r->n == 0) {
        printf("No hay supervivientes registrados.\n");
        return;
    }
    for (i = 0; i < r->n; i++) {
        printf("\nSuperviviente #%d\n", i + 1);
        printf("Nombre: %s\n", r->lista[i].nombre);
        printf("Edad: %d\n", r->lista[i].edad);
        printf("Habilidad: %s\n", r->lista[i].habilidad);
        printf("Salud: %d\n", r->lista[i].salud);
    }
    printf("\nTotal de supervivientes: %d\n", r->n);
}


void mostrar_recursos(const Refugio *r) {
    printf("\n RECURSOS DEL REFUGIO \n");
    printf("Agua: %d\n", r->recursos.agua);
    printf("Comida: %d\n", r->recursos.comida);
    printf("Medicamentos: %d\n", r->recursos.medicamentos);
    printf("Municiones: %d\n", r->recursos.municiones);
}


int seleccionar_recurso(void) {
    int tipo;
    printf("\n RECURSOS \n");
    printf("1. Agua\n2. Comida\n3. Medicamentos\n4. Municiones\n0. Cancelar\nSelecciona recurso: ");
    tipo = leer_entero();
    if (tipo < 0 || tipo > 4) {
        printf("Tipo de recurso invalido.\n");
        return 0;
    }
    return tipo;
}


int modificar_recurso(Refugio *r, int tipo, int cantidad) {
    int *recurso;
    const char *nombre;
    long long resultado;

    switch (tipo) {
        case 1: recurso = &r->recursos.agua; nombre = "Agua"; break;
        case 2: recurso = &r->recursos.comida; nombre = "Comida"; break;
        case 3: recurso = &r->recursos.medicamentos; nombre = "Medicamentos"; break;
        case 4: recurso = &r->recursos.municiones; nombre = "Municiones"; break;
        default:
            printf("Tipo de recurso invalido.\n");
            return 0;
    }
    if (cantidad == 0) {
        printf("La cantidad no puede ser cero.\n");
        return 0;
    }
    resultado = (long long)*recurso + cantidad;
    if (resultado < 0) {
        printf("No hay suficientes recursos disponibles.\n");
        return 0;
    }
    if (resultado > INT_MAX) {
        printf("La cantidad supera el limite permitido.\n");
        return 0;
    }
    *recurso = (int)resultado;
    printf("%s actualizado. Cantidad actual: %d\n", nombre, *recurso);
    return 1;
}


int registrar_evento(Refugio *r, const char *descripcion) {
    Evento *temporal;
    int nueva_capacidad;

    if (r->cantidad_eventos == r->capacidad_eventos) {
        nueva_capacidad = (r->capacidad_eventos == 0) ? 4 : r->capacidad_eventos * 2;
        temporal = realloc(r->historial, (size_t)nueva_capacidad * sizeof(*temporal));
        if (temporal == NULL) {
            printf("No se pudo reservar memoria para el historial.\n");
            return 0;
        }
        r->historial = temporal;
        r->capacidad_eventos = nueva_capacidad;
    }
    r->historial[r->cantidad_eventos].dia = r->dia;
    snprintf(r->historial[r->cantidad_eventos].descripcion,
             sizeof(r->historial[r->cantidad_eventos].descripcion), "%s", descripcion);
    r->cantidad_eventos++;
    return 1;
}


void mostrar_eventos(const Refugio *r) {
    int i;
    printf("\n HISTORIAL DE EVENTOS \n");
    if (r->cantidad_eventos == 0) {
        printf("No hay eventos registrados.\n");
        return;
    }
    for (i = 0; i < r->cantidad_eventos; i++) {
        printf("Dia %d: %s\n", r->historial[i].dia, r->historial[i].descripcion);
    }
}


int dar_baja_superviviente(Refugio *r, const char *nombre) {
    int i, encontrado = -1;
    char mensaje[100];

    for (i = 0; i < r->n; i++) {
        if (strcmp(r->lista[i].nombre, nombre) == 0) {
            encontrado = i;
            break;
        }
    }
    if (encontrado == -1) {
        printf("No se encontro al superviviente.\n");
        return 0;
    }
    for (i = encontrado; i < r->n - 1; i++) {
        r->lista[i] = r->lista[i + 1];
    }
    r->n--;
    snprintf(mensaje, sizeof(mensaje), "Baja del superviviente: %.70s", nombre);
    registrar_evento(r, mensaje);
    printf("Superviviente dado de baja.\n");
    return 1;
}


void realizar_expedicion(Refugio *r) {
    int indice, municiones_gastadas, resultado, i;
    char mensaje[100];

    if (r->n == 0) {
        printf("No hay supervivientes disponibles para la expedicion.\n");
        return;
    }
    printf("\n SUPERVIVIENTES \n");
    for (i = 0; i < r->n; i++) {
        printf("%d. %s (salud %d)\n", i + 1, r->lista[i].nombre, r->lista[i].salud);
    }
    printf("Selecciona quien ira: ");
    indice = leer_entero() - 1;
    if (indice < 0 || indice >= r->n || r->lista[indice].salud <= 0) {
        printf("Seleccion invalida o superviviente sin salud.\n");
        return;
    }
    if (r->recursos.municiones <= 0) {
        printf("No hay municiones; la expedicion no puede salir.\n");
        return;
    }
    municiones_gastadas = (r->recursos.municiones >= 5) ? 5 : r->recursos.municiones;
    modificar_recurso(r, 4, -municiones_gastadas);
    resultado = rand() % 100;
    if (resultado < 65) {
        int agua = 10 + rand() % 21;
        int comida = 10 + rand() % 21;
        modificar_recurso(r, 1, agua);
        modificar_recurso(r, 2, comida);
        r->lista[indice].salud -= (rand() % 16);
        if (r->lista[indice].salud < 0) r->lista[indice].salud = 0;
        snprintf(mensaje, sizeof(mensaje), "Expedicion exitosa de %.25s: agua +%d, comida +%d", r->lista[indice].nombre, agua, comida);
        printf("La expedicion regreso con recursos.\n");
    } else if (resultado < 90) {
        r->lista[indice].salud -= 20;
        if (r->lista[indice].salud < 0) r->lista[indice].salud = 0;
        snprintf(mensaje, sizeof(mensaje), "Expedicion con problemas: %.25s perdio 20 de salud", r->lista[indice].nombre);
        printf("La expedicion tuvo problemas; el superviviente perdio salud.\n");
    } else {
        snprintf(mensaje, sizeof(mensaje), "Desaparicion durante expedicion de %.25s", r->lista[indice].nombre);
        printf("El superviviente desaparecio durante la expedicion.\n");
        dar_baja_superviviente(r, r->lista[indice].nombre);
    }
    registrar_evento(r, mensaje);
    r->dia++;
}


void perder_recursos(Refugio *r, int tipo, int cantidad) {
    int *recurso;
    const char *nombre;
    char mensaje[100];

    switch (tipo) {
        case 1: recurso = &r->recursos.agua; nombre = "agua"; break;
        case 2: recurso = &r->recursos.comida; nombre = "comida"; break;
        case 3: recurso = &r->recursos.medicamentos; nombre = "medicamentos"; break;
        case 4: recurso = &r->recursos.municiones; nombre = "municiones"; break;
        default:
            printf("Tipo de recurso invalido.\n");
            return;
    }
    if (cantidad <= 0) {
        printf("La cantidad de perdida debe ser mayor que cero.\n");
        return;
    }
    if (cantidad >= *recurso) {
        *recurso = 0;
    } else {
        *recurso -= cantidad;
    }
    snprintf(mensaje, sizeof(mensaje), "Perdida de %d unidades de %s", cantidad, nombre);
    registrar_evento(r, mensaje);
    printf("Perdida aplicada. %s restantes: %d\n", nombre, *recurso);
    r->dia++;
}


int guardar_partida(const Refugio *r, const char *archivo) {
    FILE *f;
    int i;
    const char *temporal = "partida.tmp";

    f = fopen(temporal, "w");
    if (f == NULL) {
        printf("No se pudo abrir el archivo temporal para guardar.\n");
        return 0;
    }
    if (fprintf(f, "REFUGIO_V1\n%d %d %d %d %d %d\n%d\n",
                r->dia, r->recursos.agua, r->recursos.comida,
                r->recursos.medicamentos, r->recursos.municiones,
                r->n, r->cantidad_eventos) < 0) {
        fclose(f);
        remove(temporal);
        printf("Error al escribir el encabezado del archivo.\n");
        return 0;
    }
    for (i = 0; i < r->n; i++) {
        if (fprintf(f, "%s\n%d\n%s\n%d\n", r->lista[i].nombre,
                    r->lista[i].edad, r->lista[i].habilidad,
                    r->lista[i].salud) < 0) {
            fclose(f);
            remove(temporal);
            printf("Error al escribir supervivientes.\n");
            return 0;
        }
    }
    for (i = 0; i < r->cantidad_eventos; i++) {
        if (fprintf(f, "%d\n%s\n", r->historial[i].dia,
                    r->historial[i].descripcion) < 0) {
            fclose(f);
            remove(temporal);
            printf("Error al escribir eventos.\n");
            return 0;
        }
    }
    if (fclose(f) != 0) {
        remove(temporal);
        printf("Error al cerrar archivo de guardado.\n");
        return 0;
    }
    if (rename(temporal, archivo) != 0) {
        remove(temporal);
        printf("No se pudo reemplazar el archivo de partida.\n");
        return 0;
    }
    printf("Partida guardada en %s.\n", archivo);
    return 1;
}


int cargar_partida(Refugio *r, const char *archivo) {
    FILE *f;
    Refugio nuevo;
    char encabezado[30];
    int i, dia, agua, comida, medicamentos, municiones, n, neventos;

    f = fopen(archivo, "r");
    if (f == NULL) {
        printf("No existe archivo de partida; el refugio actual se conserva.\n");
        return 0;
    }
    inicializar_refugio(&nuevo);
    if (fgets(encabezado, sizeof(encabezado), f) == NULL || strcmp(encabezado, "REFUGIO_V1\n") != 0 ||
        fscanf(f, "%d %d %d %d %d %d %d\n", &dia, &agua, &comida, &medicamentos,
               &municiones, &n, &neventos) != 7) {
        fclose(f);
        printf("Archivo de partida invalido.\n");
        return 0;
    }
    if (dia < 1 || agua < 0 || comida < 0 || medicamentos < 0 || municiones < 0 ||
        n < 0 || n > 100000 || neventos < 0 || neventos > 100000) {
        fclose(f);
        printf("Datos fuera de rango en el archivo.\n");
        return 0;
    }
    nuevo.dia = dia;
    nuevo.recursos.agua = agua;
    nuevo.recursos.comida = comida;
    nuevo.recursos.medicamentos = medicamentos;
    nuevo.recursos.municiones = municiones;

    if (n > 0) {
        nuevo.lista = malloc((size_t)n * sizeof(*nuevo.lista));
        if (nuevo.lista == NULL) {
            fclose(f);
            printf("No hay memoria para cargar supervivientes.\n");
            return 0;
        }
        nuevo.capacidad = n;
    }
    for (i = 0; i < n; i++) {
        if (fgets(nuevo.lista[i].nombre, sizeof(nuevo.lista[i].nombre), f) == NULL ||
            fscanf(f, "%d\n", &nuevo.lista[i].edad) != 1 ||
            fgets(nuevo.lista[i].habilidad, sizeof(nuevo.lista[i].habilidad), f) == NULL ||
            fscanf(f, "%d\n", &nuevo.lista[i].salud) != 1) {
            fclose(f);
            liberar_refugio(&nuevo);
            printf("Archivo incompleto o corrupto al leer supervivientes.\n");
            return 0;
        }
        nuevo.lista[i].nombre[strcspn(nuevo.lista[i].nombre, "\n")] = '\0';
        nuevo.lista[i].habilidad[strcspn(nuevo.lista[i].habilidad, "\n")] = '\0';
        if (nuevo.lista[i].nombre[0] == '\0' || nuevo.lista[i].edad <= 0 ||
            nuevo.lista[i].salud < 0 || nuevo.lista[i].salud > 100) {
            fclose(f);
            liberar_refugio(&nuevo);
            printf("Superviviente invalido en el archivo.\n");
            return 0;
        }
    }
    if (neventos > 0) {
        nuevo.historial = malloc((size_t)neventos * sizeof(*nuevo.historial));
        if (nuevo.historial == NULL) {
            fclose(f);
            liberar_refugio(&nuevo);
            printf("No hay memoria para cargar historial.\n");
            return 0;
        }
        nuevo.capacidad_eventos = neventos;
    }
    for (i = 0; i < neventos; i++) {
        if (fscanf(f, "%d\n", &nuevo.historial[i].dia) != 1 ||
            fgets(nuevo.historial[i].descripcion, sizeof(nuevo.historial[i].descripcion), f) == NULL) {
            fclose(f);
            liberar_refugio(&nuevo);
            printf("Archivo incompleto o corrupto al leer eventos.\n");
            return 0;
        }
        nuevo.historial[i].descripcion[strcspn(nuevo.historial[i].descripcion, "\n")] = '\0';
        if (nuevo.historial[i].dia < 1 || nuevo.historial[i].descripcion[0] == '\0') {
            fclose(f);
            liberar_refugio(&nuevo);
            printf("Evento invalido en el archivo.\n");
            return 0;
        }
    }
    if (fclose(f) != 0) {
        liberar_refugio(&nuevo);
        printf("Error al cerrar archivo de carga.\n");
        return 0;
    }
    liberar_refugio(r);
    *r = nuevo;
    printf("Partida cargada correctamente.\n");
    return 1;
}


void evento_aleatorio(Refugio *r) {
    int evento = rand() % 5;
    char descripcion[100];

    switch (evento) {
        case 0: { /* Medico */
            Superviviente medico;
            Superviviente *temporal;
            int nueva_capacidad;
            snprintf(medico.nombre, sizeof(medico.nombre), "Medico_%d", r->dia);
            medico.edad = 35;
            snprintf(medico.habilidad, sizeof(medico.habilidad), "Medicina");
            medico.salud = 100;
            if (r->n == r->capacidad) {
                nueva_capacidad = (r->capacidad == 0) ? 1 : r->capacidad * 2;
                temporal = realloc(r->lista, (size_t)nueva_capacidad * sizeof(*temporal));
                if (temporal == NULL) {
                    printf("Evento medico: no se pudo reservar memoria.\n");
                    return;
                }
                r->lista = temporal;
                r->capacidad = nueva_capacidad;
            }
            r->lista[r->n++] = medico;
            snprintf(descripcion, sizeof(descripcion), "Un medico se unio al refugio (%s)", medico.nombre);
            printf("Un medico se unio al refugio.\n");
            break;
        }
        case 1: { /* Horda */
            int perdida_mun = 10 + rand() % 16;
            int perdida_com = 5 + rand() % 16;
            perder_recursos(r, 4, perdida_mun);
            perder_recursos(r, 2, perdida_com);
            snprintf(descripcion, sizeof(descripcion), "Horda zombi: municiones y comida perdidas");
            printf("Una horda ataco el refugio.\n");
            break;
        }
        case 2: { /* Robo */
            int perdida = 5 + rand() % 16;
            perder_recursos(r, 2, perdida);
            snprintf(descripcion, sizeof(descripcion), "Robo: se perdieron %d unidades de comida", perdida);
            printf("Alguien robo comida del refugio.\n");
            break;
        }
        case 3: { /* Mochila */
            int agua = 5 + rand() % 16;
            int comida = 5 + rand() % 16;
            modificar_recurso(r, 1, agua);
            modificar_recurso(r, 2, comida);
            snprintf(descripcion, sizeof(descripcion), "Mochila encontrada: agua +%d, comida +%d", agua, comida);
            printf("Se encontro una mochila con suministros.\n");
            break;
        }
        case 4: { /* Apagon */
            if (guardar_partida(r, "partida.txt") && cargar_partida(r, "partida.txt")) {
                snprintf(descripcion, sizeof(descripcion), "Se fue la luz: partida guardada y recargada");
                printf("Apagon: se guardo y recargo la partida.\n");
            } else {
                snprintf(descripcion, sizeof(descripcion), "Se fue la luz: no se pudo guardar y recargar");
                printf("Apagon: fallo al guardar o recargar.\n");
            }
            break;
        }
    }
    registrar_evento(r, descripcion);
    r->dia++;
}


void liberar_refugio(Refugio *r) {
    free(r->lista);
    free(r->historial);
    r->lista = NULL;
    r->historial = NULL;
    r->n = 0;
    r->capacidad = 0;
    r->cantidad_eventos = 0;
    r->capacidad_eventos = 0;
}


void mostrar_documentacion(void) {
    printf("\n===== HU-14: DOCUMENTACION DE FUNCIONES =====\n");
    printf("inicializar_refugio: recibe Refugio*; modifica el refugio; sin memoria externa.\n");
    printf("inicializar_recursos: recibe Refugio*; establece recursos iniciales.\n");
    printf("leer_texto: recibe char[] e int; modifica el buffer de texto.\n");
    printf("leer_entero: sin parametros; devuelve un entero leido por teclado.\n");
    printf("agregar_superviviente: recibe Refugio*; registra y puede reservar memoria.\n");
    printf("listar_supervivientes: recibe const Refugio*; solo consulta.\n");
    printf("mostrar_recursos: recibe const Refugio*; solo consulta.\n");
    printf("seleccionar_recurso: sin parametros; devuelve tipo de recurso.\n");
    printf("modificar_recurso: recibe Refugio*, tipo y cantidad; modifica recursos.\n");
    printf("registrar_evento: recibe Refugio* y texto; reserva memoria para historial.\n");
    printf("mostrar_eventos: recibe const Refugio*; solo consulta historial.\n");
    printf("dar_baja_superviviente: recibe Refugio* y nombre; elimina registro.\n");
    printf("realizar_expedicion: recibe Refugio*; consume municiones y puede modificar estado.\n");
    printf("perder_recursos: recibe Refugio*, tipo y cantidad; reduce recursos.\n");
    printf("guardar_partida: recibe const Refugio* y ruta; escribe archivo.\n");
    printf("cargar_partida: recibe Refugio* y ruta; reconstruye datos y reserva memoria.\n");
    printf("evento_aleatorio: recibe Refugio*; ejecuta uno de cinco eventos.\n");
    printf("liberar_refugio: recibe Refugio*; libera memoria y nulifica punteros.\n");
}
