//
// Created by Vilhena on 5/27/2020.
//

#include "../Headers/Espacos.h"

psalas leFicheiroEspacos(char ficheiro [100], psalas salas, int *contador) { //Le ficheiro Binario dos Espacos
    FILE *f;
    plocal aux;
    int i = 0;
    int cont = 0; //Contador para saber quantas salas existem
    f = fopen(ficheiro, "rb");
    if (f == NULL) {
        printf("Erro no acesso ao ficheiro!!!\n");
        exit(1);
    }
    aux = malloc(sizeof(local));
    while (fread(aux, sizeof(local),1, f) == 1) {
    //for(int i = 0; fread(aux, sizeof(local),1, f) == 1; i++) {
        salas = realloc(salas, (i+1) * (sizeof(esala)));
        cont = cont + 1;
        *contador = cont;
        if (aux->id < 0) {
            printf("ERRO O ID TEM DE SER POSITIVO\n");
            exit(1);
        }
        if (i > 0) {
            for (int j = 0; j < cont; j++) {
                if (aux->id == salas[j].id) {
                    printf("ERRO OS IDS DEVEM SER UNICOS\n");
                    exit(1);
                }
            }
        }
        salas[i].id = aux->id;
        if (aux->capacidade <= 0) {
            printf("ERRO A CAPACIDADE TEM DE SER POSITIVA\n");
            exit(1);
        }
        salas[i].capacidade = aux->capacidade;
        for (int j = 0; j < 3; j++) {
            if (aux->liga[j] > 0) {
            salas[i].liga[j] = aux->liga[j];
            }
            else {
                salas[i].liga[j] = 0;
            }
        }
        salas[i].taxaDisseminacao = 0;
        salas[i].totalp = 0;
        i = i + 1;
    }
    free(aux);
    fclose(f);
    return salas;
}

void LigacoesSalas(psalas salas,int *tam) {
    int contador = 0;
    // si -> Sala Inicial ||li -> Ligacao Inicial ||sd -> Sala de Dentro ||ld -> Ligacao de Dentro
    for (int si = 0; si < *tam; si++) {
        contador = 0;
        for (int li = 0; li < 3; li++) {
            if (salas[si].liga[li] == 0) {
                //break;
            } else {
                for (int sd = 0; sd < *tam; sd++) {
                    if (salas[si].liga[li] == salas[sd].id) { //Caso o Id da 2' volta seja igual ao ID do Liga
                        for (int ld = 0; ld < 3; ld++) {
                            if (salas[sd].liga[ld] == 0) {
                                contador++;
                            } else {
                                if (salas[sd].liga[ld] != salas[si].id) {
                                    contador++;
                                }
                            }
                        }
                    }
                }
                if (contador == 3) {
                    printf("ERRO EM ALGUMA LIGACAO ENTRE SALAS");
                    exit(1);
                }
            }
        }
    }
}

