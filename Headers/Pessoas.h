//
// Created by Vilhena on 5/27/2020.
//

#ifndef TP_PESSOAS_H
#define TP_PESSOAS_H
#include "Espacos.h"
typedef struct Pessoas epessoa, *pPessoa;
struct Pessoas {
    char id[100]; //Id da Pessoa
    int idade;  //Idade
    char estado;    //Estado da Pessoa S-> Saudavel I->Imune D->Doente
    int diasInfetado;   //Caso esteja Doente numero de dias que esta doente
    float Probrecuperacao;  //Probabilidade de se recuperar 1/idade
    int maxInfecao;     //Numero maximo de dias que a doenca demora a passar
    psalas pontsal; //Ponteiro para a sala onde se encontra
    pPessoa prox;
};

pPessoa leficheiroPessoas(char ficheiro[100], pPessoa pessoa); // le ficheiro TXT das pessoas
pPessoa Associa(pPessoa pessoa, psalas salas, int *contsalas); //Funcao que associa as pessoas as salas

//totais
int totalPessoas(pPessoa pessoa);
int totalInfetados(pPessoa pessoa);
int totalImunes(pPessoa pessoa);
int totalSaudaveis(pPessoa pessoa);

//Taxas
float taxaInfetados(pPessoa pessoa);
float taxaImunes(pPessoa pessoa);
float taxaSaudaveis(pPessoa pessoa);

//Funcoes Adicionais
pPessoa Adiciona(pPessoa pessoa, psalas salas, int *contSalas, int IDLocal, char nome[100], int idade, int dias); //Adcionar uma pessoa
pPessoa guarda_Iteracoes(pPessoa pessoa1, pPessoa pessoa2); //Funcao que guarda as iteracoes passadas
pPessoa recupera_Iteracao(pPessoa pessoa1, pPessoa pessoa2); //Recupera a iteracao

#endif //TP_PESSOAS_H
