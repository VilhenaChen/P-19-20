//
// Created by Vilhena on 5/27/2020.
//

#ifndef TP_ESPACOS_H
#define TP_ESPACOS_H

//#include "Pessoas.h"

typedef struct sala local, *plocal; //Ponteiro para o array de salas diretamente lido do ficheiro
typedef struct esalas esala, *psalas ; // Ponteiro para o array de salas final
struct sala { //Estrutura para guardar as Espacos lidos
    int id; //Id Numerico do local
    int capacidade; //Capacidade maxima
    int liga[3]; //Id das ligacoes
};

struct esalas { //Estrutura que guarda as salas e é usada no programa
    int id; //Id Numerico do local
    int capacidade; //Capacidade maxima
    int totalp; //Total de pessoas na sala
    float taxaDisseminacao; //Taxa de Disseminacao naquela Sala 5% do Total de Pessoas
    int liga[3]; //Id das ligacoes
};

//Funcoes Adicionais
psalas leFicheiroEspacos(char ficheiro [100] ,psalas salas, int *contador); //Le ficheiro Binario dos Espacos
void LigacoesSalas(psalas salas, int *tam); //Verifica as Ligacoes das salas


#endif //TP_ESPACOS_H
