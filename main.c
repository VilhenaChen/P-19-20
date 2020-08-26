/*
 * Trabalho realizado por: Diogo Vilhena 2018020207
 * Programacao 2019/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Espacos/Espacos.c"
#include "Pessoas/Pessoas.c"
#include "Util/utils.c"
#include "Util/Funcoes.c"

int main() {
    initRandom();
    char fpessoa[100], fespacos[100];
    pPessoa pessoa = NULL;
    pPessoa aux;
    psalas salas = NULL;
    int contSalas = 0;
    printf("\n"
           " _   _ ___________ _   _ _____        _____ _____ _____ _____ \n"
           "| | | |_   _| ___ \\ | | /  ___|  ____|_   _/  ___|  ___/  __ \\\n"
           "| | | | | | | |_/ / | | \\ `--.  / __ \\ | | \\ `--.| |__ | /  \\/\n"
           "| | | | | | |    /| | | |`--. \\/ / _` || |  `--. \\  __|| |    \n"
           "\\ \\_/ /_| |_| |\\ \\| |_| /\\__/ / | (_| || |_/\\__/ / |___| \\__/\\\n"
           " \\___/ \\___/\\_| \\_|\\___/\\____/ \\ \\__,_\\___/\\____/\\____/ \\____/\n"
           "                                \\____/                        \n"
           "                                                              \n");
    printf("Insira o nome do ficheiro das Pessoas: ");
    gets(fpessoa);
    pessoa = leficheiroPessoas(fpessoa, pessoa);
    aux = pessoa;
    while (aux != NULL) {
        printf("Nome: %s, %d, %c\n", aux->id, aux->idade, aux->estado);
        aux = aux->prox;
    }
    printf("Insira o nome do ficheiro dos Espacos:");
    gets(fespacos);
    salas = leFicheiroEspacos(fespacos, salas, &contSalas);
    LigacoesSalas(salas, &contSalas);
    for (int i = 0; i < contSalas; i++) {
        printf("Id sala: %d Capacidade %d\n", salas[i].id, salas[i].capacidade);
        for (int j = 0; j < 3; j++) {
            printf("\tLigada a: %d\n", salas[i].liga[j]);
        }
    }
    Associa(pessoa, salas, &contSalas);
    estatistica(pessoa, salas, &contSalas);
    simulacao(pessoa, salas, &contSalas);
    Limpa_Pessoas(pessoa);  //Limpar Pessoas
    free(salas);    //Limpar o array de Salas
    return 0;
}
