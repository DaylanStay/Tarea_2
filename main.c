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
        //printf("%i-%s-%i-%i-%s-%s-%s-%s-%i-%s\n", datos[i].id,datos[i].nombre, datos[i].pc, datos[i].ps, datos[i].sexo, datos[i].tipo, datos[i].evolucionPrevia, datos[i].evolucionPosterior,
        //      datos[i].numeroPokedex, datos[i].region);

        //insertMap(mapPokedex, &datos[i].nombre, &datos[i]);
        i++;
    }
    filtro(mapPokedex, datos);
  /*  int flag = 0;
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
*/



    /*pokedex *p = firstMap(mapPokedex);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s | %s | %s | %i | %s | %i\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo, p->evolucionPrevia, p->evolucionPosterior, p->numeroPokedex, p->region, p->existencia);
        p = nextMap(mapPokedex);
    }*/

    //Menú
    int *auxID = 1;
    atraparPokemon(mapMisPokemones, &auxID, mapPokedex);
    //printf("%d", auxID);
    EvolucionarPokemon(mapMisPokemones,mapPokedex);
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

/*void menu(List * estacion, bomberos * datos){ // Funcion para la creacion de menu menu
    //agregarBombero(estacion);
    int n;

    while(n != 0){
         printf("Estacion De Bomberos\n");
         printf("1. Agregar Bombero\n");
         printf("2. Despedir Bombero\n");
         printf("3. Buscar Bombero por dia\n");
         printf("4. Buscar Bomberos por rut\n");
         printf("5. Modificar disponibilidad de un bombero\n");
         printf("6. Crear horario de la semana\n");
         printf("7. Mostrar horario de la semana\n");
         printf("8. Mostrar todos los bomberos\n");
         printf("0. Salir\n");
         scanf("%d",&n);
         fflush(stdin);
       switch(n){
            case 1: agregarBombero(estacion); break;
            case 2: eliminarBomberos(estacion); break;
            case 3: busquedaDias(estacion); break;
            case 4: busquedaRUT(estacion); break;
            case 5: disponibilidadBomberos(estacion); break;
            case 6: horarioCreado(); break;
            case 7: crearHorario(estacion); break;
            case 8: mostrar(estacion); break;
            case 0: break;
            default: printf("\nEl numero que ingreso es incorrecto\n"); break;
        }
        printf("\n");
     }



}*/

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
    pokedex *aux = crearPokedex(aux);

    scanf("%s", aux->nombre);
    fflush(stdin);
    scanf("%s", aux->tipo);
    fflush(stdin);
    scanf("%d", &aux->ps);
    fflush(stdin);
    scanf("%d", &aux->pc);
    fflush(stdin);
    scanf("%s", aux->sexo);
    fflush(stdin);
    scanf("%s", aux->evolucionPrevia);
    fflush(stdin);
    scanf("%s", aux->evolucionPosterior);
    fflush(stdin);
    scanf("%s", aux->region);
    fflush(stdin);
    scanf("%d", &aux->numeroPokedex);
    fflush(stdin);

    aux->id= *auxID;
    insertMap(mapMisPokemones, &aux->nombre, aux);
    //aux->id=96;
    int flag=0;
    pokedex *c= firstMap(mapPokedex);
    while(c!=NULL){
        if(is_equal(c->nombre,aux->nombre) == 1){
            flag = 1;
            aux=c;
            aux->existencia = c->existencia + 1;
            printf("%i\n",aux->existencia);
            eraseMap(mapPokedex, &c->nombre);
            insertMap(mapPokedex, &aux->nombre, aux);
            break;
        }
        c = nextMap(mapPokedex);
    }
    if(flag == 0){
        insertMap(mapPokedex, &aux->nombre,aux);
    }

  /* pokedex *p = firstMap(mapPokedex);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s | %s | %s | %i | %s | %i\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo, p->evolucionPrevia, p->evolucionPosterior, p->numeroPokedex, p->region, p->existencia);
        p = nextMap(mapPokedex);
    }*/
}

void EvolucionarPokemon(HashMap *mapMisPokemones,HashMap *mapPokedex){

    pokedex *p = firstMap(mapMisPokemones);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }

    int aux;

    scanf("%d",&aux);

    pokedex *c = firstMap(mapMisPokemones);
    while(c != NULL) {
        if(c->id == aux){
            if(strcmp(c->evolucionPosterior,"No tiene") == 0){
                printf("ERROR! POKEMON NO TIENE EVOLUCION");
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
            }
        }
        c = nextMap(mapMisPokemones);
    }

    p = firstMap(mapMisPokemones);
    while(p!=NULL) {
        printf("%s | %i | %s | %i | %i | %s |\n", p->nombre, p->id, p->tipo, p->pc, p->ps, p->sexo);
        p = nextMap(mapMisPokemones);
    }


}

