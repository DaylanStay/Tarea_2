#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct {
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

pokedex *crearPokedex() {
    pokedex *aux = (pokedex *) calloc(100, sizeof(pokedex));
    aux->nombre = (char *) malloc(sizeof(char) * 12);
    aux->sexo = (char *) malloc(sizeof(char) * 7);
    aux->tipo = (char *) malloc(sizeof(char) * 20);
    aux->evolucionPrevia = (char *) malloc(sizeof(char) * 11);
    aux->evolucionPosterior = (char *) malloc(sizeof(char) * 11);
    aux->region = (char *) malloc(sizeof(char) * 8);
    return aux;
}

const char *get_csv_field (char *, int);
void atraparPokemon(HashMap *, int *, HashMap *mapPokedex);
void filtro(HashMap *, pokedex *);
void EvolucionarPokemon(HashMap *, HashMap *);
void BuscarTipo(HashMap *);
void BuscarNombre(HashMap *);
void BuscarNombrePokedex(HashMap *);
void mostrarPokedex(HashMap *);
void MostrarPorPC(HashMap *);
void EliminarPokemon(HashMap *, HashMap *);
void menu(HashMap *, HashMap *mapPokedex);

int main()
{
    FILE *archivo = fopen("pokemon Archivo.csv", "r");
    if(archivo == NULL) return 1;

    HashMap *mapPokedex = createMap(100);
    HashMap *mapMisPokemones = createMap(100);

    char line[85];
    int i = 0, j = 0;
    pokedex *datos = crearPokedex();
    while(fgets(line, 85, archivo) != NULL) {
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
    //Menú
    menu(mapMisPokemones, mapPokedex);






    return 0;
}

const char *get_csv_field (char * tmp, int k) {
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

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

void menu(HashMap *mapMisPokemones, HashMap *mapPokedex){ // Funcion para la creacion de menu menu
    int n = 1;
    int *auxID = 1;
    while(n != 0){
         printf("Pokemones\n");
         printf("1. Atrapar Pokemon\n");
         printf("2. Evolucionar Pokemon\n");
         printf("3. Buscar Pokemon por tipo\n");
         printf("4. Buscar Pokemon por nombre\n");
         printf("5. Buscar Pokemon por nombre pokedex\n");
         printf("6. Mostrar todos los pokemones en la pokedex\n");
         printf("7. \n");
         printf("8. \n");
         printf("0. Salir\n");
         scanf("%d",&n);
         fflush(stdin);
       switch(n){
            case 1: atraparPokemon(mapMisPokemones, &auxID, mapPokedex); break;
            case 2: EvolucionarPokemon(mapMisPokemones,mapPokedex); break;
            case 3: BuscarTipo(mapPokedex); break;
            case 4: BuscarNombre(mapPokedex); break;
            case 5: BuscarNombrePokedex(mapPokedex); break;
            case 6: mostrarPokedex(mapPokedex); break;
            case 7: MostrarPorPC(mapMisPokemones); break;
            case 8: EliminarPokemon(mapMisPokemones, mapPokedex); break;
            case 0: break;
            default: printf("\nEl numero que ingreso es incorrecto\n"); break;
        }
        printf("\n");
     }



}

void filtro(HashMap* mapPokedex, pokedex* datos) //Filtro para insertar en funcion importar y agregarPokemon (retorna 0 en caso de que ID no exista y 1 en el contrario)
{
    int flag = 0;
    int i,j;
    for(i = 0 ; i < 95 ; i++){
        for(j = i + 1 ; j < 95 ; j++) {
            if(datos[i].numeroPokedex == datos[j].numeroPokedex){
                datos[j].existencia++;
                flag = 1;
            }
        }
        if(flag == 0) {
            insertMap(mapPokedex, &datos[i].nombre, &datos[i]);
        }
        flag = 0;
    }
}

void atraparPokemon(HashMap *mapMisPokemones, int *auxID, HashMap *mapPokedex) {
    printf("%i\n", *auxID);
    pokedex *aux = crearPokedex(aux);
    printf("Ingrese el nombre\n");
    scanf("%s", aux->nombre);
    fflush(stdin);
    printf("Ingrese el tipo\n");
    scanf("%[^\n]",aux->tipo);
    fflush(stdin);
    printf("Ingrese el ps\n");
    scanf("%d", &aux->ps);
    fflush(stdin);
    printf("Ingrese el pc\n");
    scanf("%d", &aux->pc);
    fflush(stdin);
    printf("Ingrese el sexo\n");
    scanf("%s", aux->sexo);
    fflush(stdin);
    printf("Ingrese la evolucion previa\n");
    scanf("%s", aux->evolucionPrevia);
    fflush(stdin);
    printf("Ingrese la evolucion posterior\n");
    scanf("%s", aux->evolucionPosterior);
    fflush(stdin);
    printf("Ingrese la region\n");
    scanf("%s", aux->region);
    fflush(stdin);
    printf("Ingrese el numero pokedex\n");
    scanf("%d", &aux->numeroPokedex);
    fflush(stdin);

    aux->id= *auxID;
    insertMap(mapMisPokemones, &aux->id, aux);
    //aux->id=96;
    int flag=0;
    pokedex *c= firstMap(mapPokedex);
    while(c!=NULL){
        if(is_equal(c->nombre,aux->nombre) == 1){
            flag = 1;
            aux=c;
            aux->existencia = c->existencia + 1;
            //printf("%i\n",aux->existencia);

            eraseMap(mapPokedex, &c->nombre);
            insertMap(mapPokedex, &aux->nombre, aux);
            break;
        }
        c = nextMap(mapPokedex);
    }
    if(flag == 0){
        insertMap(mapPokedex, &aux->nombre,aux);
    }

    pokedex *p = firstMap(mapMisPokemones);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }

    *auxID += 1;
}

void EvolucionarPokemon(HashMap *mapMisPokemones,HashMap *mapPokedex){

    pokedex *p = firstMap(mapMisPokemones);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }

    int aux;

    scanf("%d",&aux);
    fflush(stdin);
    pokedex *c = searchMap(mapMisPokemones, &aux);

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
        while(p!=NULL) {
            printf("%s | %i | %s | %i | %i | %s |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
            p = nextMap(mapMisPokemones);
        }
    }
}

void BuscarTipo(HashMap *mapMisPokemones){
    char auxTipo[20];
    scanf("%[^\n]",auxTipo);
    fflush(stdin);
    pokedex *c = firstMap(mapMisPokemones);
    while(c!=NULL){
       // pokedex *encontrar = searchMap(mapMisPokemones,&c->id);
        if(strcmp(c->tipo, auxTipo) == 0){
            printf("Se encontro el tipo %s pokemon es %s \n", c->tipo,c->nombre);
        }
        c = nextMap(mapMisPokemones);
    }

}

void BuscarNombre(HashMap *mapPokedex){
    char auxNombre[12];
    scanf("%[^\n]",auxNombre);
    fflush(stdin);
    pokedex *c = firstMap(mapPokedex);
    while(c!=NULL){
       // pokedex *encontrar = searchMap(mapMisPokemones,&c->id);
        if(strcmp(c->nombre, auxNombre) == 0){
            printf("Su pokemon se llama %s", c->nombre);
        }
        c = nextMap(mapPokedex);
    }

}

void BuscarNombrePokedex(HashMap *mapPokedex){
    char auxNombre[12];
    scanf("%[^\n]",auxNombre);
    fflush(stdin);
    pokedex *c = firstMap(mapPokedex);
    while(c!=NULL){
       // pokedex *encontrar = searchMap(mapMisPokemones,&c->id);
        if(strcmp(c->nombre, auxNombre) == 0){
            printf("Su pokemon se llama %s", c->nombre);
        }
        c = nextMap(mapPokedex);
    }

}


void mostrarPokedex (HashMap *mapPokedex){

    pokedex *p = firstMap(mapPokedex);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s | %s | %s | %i | %s | %i\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo, p->evolucionPrevia, p->evolucionPosterior, p->numeroPokedex, p->region, p->existencia);
        p = nextMap(mapPokedex);
    }

}

void MostrarPorPC(HashMap *mapMisPokemones) {

}

void EliminarPokemon(HashMap * mapMisPokemones, HashMap * mapPokedex) {
    printf("Ingrese la ID del pokemon que desea eliminar\n");
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
            //printf("%i\n",aux->existencia);

            eraseMap(mapPokedex, &p->nombre);
            insertMap(mapPokedex, &aux->nombre, aux);
            break;
        }
        p = nextMap(mapPokedex);
    }
    eraseMap(mapMisPokemones, &c->id);

    /*pokedex *p = firstMap(mapMisPokemones);
    while(p !=NULL) {
            printf("%s | %i | %s | %i | %i | %s |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }*/
}
