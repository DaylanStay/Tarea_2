#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct{//Estructura de pokemones.
    int id;
    char *nombre;
    int pc;
    int ps;
    char *sexo;
    char *tipo;
    char *evolucionPrevia;
    char *evolucionPosterior;
    int numeroPokedex;
    char *region;
    int existencia;
} pokedex;

pokedex *crearPokedex(){//Función para crear la pokedex.
    pokedex *aux = (pokedex *) malloc(sizeof(pokedex) * 100);
    aux->nombre = (char *) malloc(sizeof(char) * 17);
    aux->sexo = (char *) malloc(sizeof(char) * 7);
    aux->tipo = (char *) malloc(sizeof(char) * 28);
    aux->evolucionPrevia = (char *) malloc(sizeof(char) * 17);
    aux->evolucionPosterior = (char *) malloc(sizeof(char) * 17);
    aux->region = (char *) malloc(sizeof(char) * 8);
    return aux;
}

//Prototipos de funciones utilizadas en el programa.
const char *get_csv_field (char *, int);
void atraparPokemon(HashMap *, int *, HashMap *mapPokedex);
void filtro(HashMap *, pokedex *);
void EvolucionarPokemon(HashMap *, HashMap *);
void BuscarTipo(HashMap *);
void BuscarNombre(HashMap *);
void BuscarNombrePokedex(HashMap *);
void mostrarPokedex(HashMap *);
int comparar(const void *, const void *);
void MostrarPorPC(HashMap *);
void EliminarPokemon(HashMap *, HashMap *);
void MostrarPorRegion(HashMap *, HashMap *);
void menu(HashMap *, HashMap *mapPokedex);
void* exportar(HashMap *);

int main(){
    FILE *archivo = fopen("pokemon Archivo.csv", "r");
    if(archivo == NULL) return 1;
    //228 considerando el mayor caso posible y tener un 70% utilizado y 130 por lo mismo
    HashMap *mapPokedex = createMap(228);
    HashMap *mapMisPokemones = createMap(130);

    char line[86];
    int i = 0, j = 0;
    pokedex *datos = crearPokedex();
    while(fgets(line, 85, archivo) != NULL){
        datos[i].id = atoi(get_csv_field (line, 0));
        datos[i].nombre = (char *)get_csv_field (line, 1);
        datos[i].tipo = (char *)get_csv_field (line, 2);
        datos[i].pc = atoi(get_csv_field (line, 3));
        datos[i].ps = atoi(get_csv_field (line, 4));
        datos[i].sexo = (char *)get_csv_field (line, 5);
        datos[i].evolucionPrevia = (char *)get_csv_field (line, 6);
        datos[i].evolucionPosterior = (char *)get_csv_field (line, 7);
        datos[i].numeroPokedex = atoi(get_csv_field (line, 8));
        datos[i].region = (char *)get_csv_field (line, 9);
        datos[i].existencia = 1;
        i++;
    }

    filtro(mapPokedex, datos);
    menu(mapMisPokemones, mapPokedex);

    free(datos);
    free(mapPokedex);
    free(mapMisPokemones);
    fclose(archivo);
    return 0;
}

const char *get_csv_field (char * tmp, int k){ //Función para leer distintos datos del archivo.
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){ //mientras el tmp [siguiente del indice] sea distinto a null

        if(tmp[i]== '\"'){  //si el tmp [indice] es igual a comillas (las comillas son cuando hay mas de un tag)
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1; //(si openmark es un numero distinto a cero, ini_i = i++)
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){ // si no encuentra coma y openmark es numero !=0, avanza 1 y continua
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){ // si encuentra coma, retorna
            if(k==j) {
               ret[i-ini_i] = 0; //
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j){
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}

void menu(HashMap *mapMisPokemones, HashMap *mapPokedex){ // Función para la creación de menu.
    int n = 1;
    int *auxID = 1;
    while(n != 0){
         printf("Pokemones\n");
         printf("1. Atrapar pokemon\n");
         printf("2. Evolucionar pokemon\n");
         printf("3. Buscar pokemon por tipo\n");
         printf("4. Buscar pokemon por nombre (Almacenamiento) \n");
         printf("5. Buscar pokemon por nombre (Pokedex)\n");
         printf("6. Mostrar todos los pokemones en la pokedex\n");
         printf("7. Mostrar mis pokemones por PC\n");
         printf("8. Eliminar pokemon\n");
         printf("9. Mostrar mis pokemones por region\n");
         printf("10. Exportar archivo\n");
         printf("0. Salir\n");
         scanf("%d",&n);
         fflush(stdin);
       switch(n){
            case 1: atraparPokemon(mapMisPokemones, &auxID, mapPokedex); break;
            case 2: EvolucionarPokemon(mapMisPokemones,mapPokedex); break;
            case 3: BuscarTipo(mapMisPokemones); break;
            case 4: BuscarNombre(mapMisPokemones); break;
            case 5: BuscarNombrePokedex(mapPokedex); break;
            case 6: mostrarPokedex(mapPokedex); break;
            case 7: MostrarPorPC(mapMisPokemones); break;
            case 8: EliminarPokemon(mapMisPokemones, mapPokedex); break;
            case 9: MostrarPorRegion(mapMisPokemones, mapPokedex); break;
            case 10: exportar(mapPokedex); break;
            case 0: break;
            default: printf("\nEl numero que ingreso es incorrecto\n"); break;
        }
        printf("\n");
     }
}

void filtro(HashMap* mapPokedex, pokedex* datos){ //Filtro para insertar en funcion importar y agregarPokemon (retorna 0 en caso de que ID no exista y 1 en el contrario).
    int flag = 0;
    int i,j;
    int cont = 0;
    for(i = 0 ; i < 95 ; i++){
        for(j = i + 1 ; j < 95 ; j++) {
            if(datos[i].numeroPokedex == datos[j].numeroPokedex){
                datos[j].existencia++;
                flag = 1;
            }
        }
        if(flag == 0) {
            insertMap(mapPokedex, datos[i].nombre, &datos[i]);
            cont++;
        }
        flag = 0;
    }
}

void atraparPokemon(HashMap *mapMisPokemones, int *auxID, HashMap *mapPokedex){ //Función para atrapar pokemon e insertarloo en el almacenamiento.
    pokedex *aux = crearPokedex(aux);
    printf("\nIngrese el nombre del pokemon: ");
    scanf("%s", aux->nombre);
    fflush(stdin);
    printf("Ingrese el tipo: ");
    scanf("%[^\n]",aux->tipo);
    fflush(stdin);
    printf("Ingrese sus puntos de vida: ");
    scanf("%d", &aux->ps);
    fflush(stdin);
    printf("Ingrese sus puntos de combate: ");
    scanf("%d", &aux->pc);
    fflush(stdin);
    printf("Ingrese el sexo: ");
    scanf("%s", aux->sexo);
    fflush(stdin);
    printf("Ingrese la evolucion previa: ");
    scanf("%s", aux->evolucionPrevia);
    fflush(stdin);
    printf("Ingrese la evolucion posterior: ");
    scanf("%s", aux->evolucionPosterior);
    fflush(stdin);
    printf("Ingrese la region: ");
    scanf("%s", aux->region);
    fflush(stdin);
    printf("Ingrese el numero pokedex: ");
    scanf("%d", &aux->numeroPokedex);
    fflush(stdin);

    aux->id= *auxID;
    aux->existencia = 1;
    insertMap(mapMisPokemones, &aux->id, aux);
    int flag=0;
    pokedex *c= firstMap(mapPokedex);

    while(c!=NULL){
        if(is_equal(c->nombre,aux->nombre) == 1){
            flag = 1;
            aux=c;
            aux->existencia = c->existencia + 1;
            eraseMap(mapPokedex, &c->nombre);
            insertMap(mapPokedex, &aux->nombre, aux);
            break;
        }
        c = nextMap(mapPokedex);
    }
    if(flag == 0){
        insertMap(mapPokedex, &aux->nombre,aux);
    }

    printf("\n***ALMACENAMIENTO POKEMON***\n");
    printf("      NOMBRE     | ID |             TIPO            |  PC  |  PS  |  SEXO    |\n");
    pokedex *p = firstMap(mapMisPokemones);
    while(p!=NULL) {
        printf("%16s |  %i | %27s | %4i | %4i | %7s  |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }

    *auxID += 1;


}

void EvolucionarPokemon(HashMap *mapMisPokemones,HashMap *mapPokedex){ //Función para evolucionar al pokemon actualizando sus datos en el almacenamiento.

    pokedex *p = firstMap(mapMisPokemones);
    printf("\n***ALMACENAMIENTO POKEMON***\n");
    printf("      NOMBRE     | ID |             TIPO            |  PC  |  PS  |  SEXO    |\n");
    while(p!=NULL) {
        printf("%16s |  %i | %27s | %4i | %4i | %7s  |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }

    int aux;

    printf("\nIngrese el id de su pokemon a evolucionar: ");
    scanf("%d",&aux);
    fflush(stdin);
    pokedex *c = searchMap(mapMisPokemones, &aux);

    if(c == NULL)
    {
        printf("\n**NO EXISTE EL POKEMON***\n");
        return;
    }

    if(strcmp(c->evolucionPosterior,"No") == 0){
        printf("ERROR! POKEMON NO TIENE EVOLUCION\n");
    }else{
        char respaldo[12];
        strcpy(respaldo,c->nombre);
        strcpy(c->nombre,c->evolucionPosterior);
        strcpy(c->evolucionPrevia,respaldo);
        strcpy(c->evolucionPosterior,"No tiene");
        c->pc += c->pc * 0.5;
        c->ps += c->ps * 0.25;
        eraseMap(mapMisPokemones, &c->id);
        insertMap(mapMisPokemones, &c->id,c);
        p = firstMap(mapMisPokemones);
        printf("\n***ALMACENAMIENTO POKEMON***");
        printf("\n      NOMBRE     | ID |             TIPO            |  PC  |  PS  |  SEXO    |\n");
        while(p!=NULL) {
            printf("%16s |  %i | %27s | %4i | %4i | %7s  |\n", c->nombre, c->id, c->tipo, c->pc, c->ps, c->sexo);
            p = nextMap(mapMisPokemones);
        }
    }
}

void BuscarTipo(HashMap *mapMisPokemones){ //Función para buscar pokemones por tipo en el almacenamiento.
    char auxTipo[28];
    int flag = 0;
    printf("\nIngrese el tipo que desea buscar: ");
    scanf("%[^\n]",auxTipo);
    fflush(stdin);
    pokedex *c = firstMap(mapMisPokemones);

    while(c!=NULL){
        if(strcmp(c->tipo, auxTipo) == 0){
            flag++;
            if(flag <= 1){
                printf("\nPOKEMONES DE TIPO (%s) \n", auxTipo);
                printf("      NOMBRE     |  PC  |  PS  |\n");
            }
            printf("%16s | %4d | %4d |\n", c->nombre, c->pc, c->ps);
        }
        c = nextMap(mapMisPokemones);
    }

    if(flag != 0){
        return;
    }else{
        printf("\n***TIPO DE POKEMON NO ENCONTRADO***\n");
        return;
    }
}

void BuscarNombre(HashMap *mapMisPokemones){ //Función para buscar por nombre en el almacenamiento y mostrar por pantalla los o el encontrado.
    char auxNombre[12];
    int flag = 0;
    printf("\nIngrese el nombre del pokemon: ");
    scanf("%[^\n]",auxNombre);
    fflush(stdin);
    pokedex *c = firstMap(mapMisPokemones);

    while(c!=NULL){
        if(strcmp(c->nombre, auxNombre) == 0){
            flag++;
            if(flag <= 1){
                printf("\nPOKEMON/ES LLAMADO/S (%s)\n", c->nombre);
                printf("      NOMBRE     |  PC  |  PS  |\n");
            }
            printf("%16s | %4d | %4d |\n", c->nombre, c->pc, c->ps);
        }
        c = nextMap(mapMisPokemones);
    }

    if(flag != 0){
        return;
    }else{
        printf("\n***POKEMON NO ENCONTRADO***\n");
        return;
    }
}

void BuscarNombrePokedex(HashMap *mapPokedex){ //Función para buscar por nombre en la pokedex, se muestra por pantalla los o el pokemon encontrado.
    char auxNombre[12];
    int flag = 0;
    printf("\nIngrese el nombre del pokemon: ");
    scanf("%[^\n]",auxNombre);
    fflush(stdin);
    pokedex *c = firstMap(mapPokedex);

    while(c!=NULL){
        if(strcmp(c->nombre, auxNombre) == 0){
            flag++;
            printf("\nPOKEMONES EN LA POKEDEX CON NOMBRE (%s)\n", auxNombre);
            printf("      NOMBRE     | EXISTENCIA |             TIPO            |      PREVIA      |     POSTERIOR    | NUMERO POKEDEX |  REGION  |\n");
            printf("%16s | %10i | %27s | %16s | %16s | %14i | %8s |\n", c->nombre, c->existencia, c->tipo, c->evolucionPrevia, c->evolucionPosterior, c->numeroPokedex, c->region);
            return;
        }
        c = nextMap(mapPokedex);
    }

    if(flag != 0){
        return;
    }else{
        printf("\n***POKEMON NO ENCONTRADO EN LA POKEDEX***\n");
        return;
    }
}

void mostrarPokedex (HashMap *mapPokedex){ //Muestra todos los pokemones de la pokedex actualmente.
    pokedex *p = firstMap(mapPokedex);
    printf("\n***POKEDEX***\n");
    printf("      NOMBRE     |             TIPO            |      PREVIA      |     POSTERIOR    | NUMERO POKEDEX |  REGION  | EXISTENCIA |\n");
    while(p!=NULL) {
        printf("%16s | %27s | %16s | %16s | %14i | %8s | %10i |\n", p->nombre, p->tipo, p->evolucionPrevia, p->evolucionPosterior, p->numeroPokedex, p->region, p->existencia);
        p = nextMap(mapPokedex);
    }
}

int comparar(const void *pivote, const void *item){ //Funcion QSORT que sirve para ordenar.
   pokedex *ptrPivote = (pokedex *) pivote;
   pokedex *ptrItem = (pokedex *) item;
   if(ptrPivote->pc < ptrItem ->pc) return 1;
   return 0;
}

void MostrarPorPC(HashMap *mapMisPokemones){ //Función para mostrar tus pokemones ordenados por puntos de combate.

    pokedex *a = NULL;
    a = (pokedex *) malloc(sizeof(pokedex) * 100);
    if(a == NULL) exit(EXIT_FAILURE);

    int i = 0;
    pokedex *c = firstMap(mapMisPokemones);
    while(c!=NULL){
        if(c->pc != NULL){
        a[i].pc = c->pc;
        a[i].nombre = c->nombre;
        a[i].ps = c->ps;
        i = i + 1;
        }
        c = nextMap(mapMisPokemones);
    }

    printf("\n***POKEMONES ORDENADOS POR PC***\n");
    printf("      NOMBRE     |  PC  |  PS  |\n");
    qsort(a,i,sizeof(pokedex),comparar);
    int j;
    for(j = 0; j < i; j++){
        printf("%16s | %4d | %4d |\n",a[j].nombre,a[j].pc,a[j].ps);
    }

}

void EliminarPokemon(HashMap * mapMisPokemones, HashMap * mapPokedex){ //Función para eliminar un pokemon de tu almacenamiento.

    pokedex *a = firstMap(mapMisPokemones);
    printf("\n***ALMACENAMIENTO POKEMON***\n");
    printf("      NOMBRE     | ID |             TIPO            |  PC  |  PS  |  SEXO    |\n");
    while(a!=NULL) {
        printf("%16s |  %i | %27s | %4i | %4i | %7s  |\n", a->nombre, a->id, a->tipo, a->pc, a->ps, a->sexo);
        a = nextMap(mapMisPokemones);
    }

    printf("\nIngrese la ID del pokemon que desea eliminar: ");
    int auxID;

    scanf("%i", &auxID);
    fflush(stdin);
    pokedex *c = firstMap(mapMisPokemones);
    while(c!=NULL){
        if(c->id == auxID) break;
        c = nextMap(mapMisPokemones);
    }

    pokedex *aux;
    pokedex *p = firstMap(mapPokedex);
    while(p!=NULL){
        if(is_equal(p->nombre, c->nombre) == 1){
            aux=p;
            aux->existencia = p->existencia - 1;

            eraseMap(mapPokedex, &p->nombre);
            insertMap(mapPokedex, &aux->nombre, aux);
            break;
        }
        p = nextMap(mapPokedex);
    }
    eraseMap(mapMisPokemones, &c->id);

    printf("\n***POKEMON ELIMINADO***\n");
}

void MostrarPorRegion(HashMap *mapMisPokemones,HashMap *mapPokedex){ //Función para mostrar tus pokemones de la región que se escriba.
    char auxRegion[20];
    int cont = 0;
    printf("\nIngrese Region: ");
    scanf("%[^\n]",auxRegion);
    fflush(stdin);
    printf("\nRegion Pokedex: %s\n",auxRegion);
    pokedex *c = firstMap(mapPokedex);

    printf("      NOMBRE     |             TIPO            |      PREVIA      |     POSTERIOR    | NUMERO POKEDEX |  REGION  | EXISTENCIA |\n");
    while(c != NULL){
        if(strcmp(c->region, auxRegion) == 0){
        printf("%16s | %27s | %16s | %16s | %14i | %8s | %10i |\n", c->nombre, c->tipo, c->evolucionPrevia, c->evolucionPosterior, c->numeroPokedex, c->region, c->existencia);
        }
        c = nextMap(mapPokedex);
    }

    pokedex *p = firstMap(mapMisPokemones);


    while(p != NULL){
        if(strcmp(p->region, auxRegion) == 0){
            cont = cont + 1;
        }
        p = nextMap(mapMisPokemones);
    }

    printf("\nCANTIDAD DE POKEMONES DE LA REGION (%s) EN ALMACENAMIENTO: %d\n", auxRegion, cont);
}

void* exportar(HashMap * mapPokedex){ //Función para exportar la pokedex actualizada en la carpeta del mismo programa con el nombre indicado.
    char nombre[14];
    printf("Ingrese el nombre del archivo: ");
    fflush(stdin);
    scanf("%[^\n]",nombre);
    fflush(stdin);
    FILE *salidaArchivo;
    char nombreArchivo[20];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "%s%s", nombre, ".csv");

    salidaArchivo = fopen(nombreArchivo, "w");

    pokedex *d = (pokedex*) malloc (sizeof(pokedex) * 1);

    d = firstMap(mapPokedex);

    char line[86];
    while(d != NULL){
        snprintf(line, sizeof(line), "%i,%s,%s,%i,%i,%s,%s,%s,%i,%s\n", d->id, d->nombre, d->tipo, d->pc, d->ps
                , d->sexo, d->evolucionPrevia, d->evolucionPosterior, d->numeroPokedex, d->region);
        fputs(line, salidaArchivo);
        d = nextMap(mapPokedex);
    }
    fclose(salidaArchivo);
    free(nombreArchivo);
    free(d);
}
