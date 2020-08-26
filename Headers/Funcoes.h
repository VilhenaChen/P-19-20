//
// Created by Vilhena on 6/3/2020.
//

#ifndef TP_FUNCOES_H
#define TP_FUNCOES_H

#include "Pessoas.h"

void limpa_input();
void avanca(pPessoa pessoa, psalas salas, int *contSalas); //Funcao responsavel por fazer avancar as iteracoes
//void Adiciona(pPessoa pessoa, psalas salas, int *contSalas, char IDPessoa[100], int IDLocal, int Idade, int Dias); //Funcao responsavel por Adicionar Doentes a uma sala
void estatistica(pPessoa pessoa, psalas salas, int *contSalas); //Funcao responsavel pela estatistica
void transferePesssoas(pPessoa pessoa, psalas salas, int idOrigem, int idDestino, int nPessoas, int *contSalas); //Funcao que trata da transferencia de pessoas entre salas
void atualiza_Informacao_Salas(pPessoa pessoas, psalas salas, int *contSalas); //Atualiza o Numero de pessoas e a taxa de Disseminacao
void Escreve_Ficheiro(pPessoa pessoa, psalas salas, int *contSalas, int it, int itmax); //Escreve o ficehiro de report.txt
void Escreve_Ficheiro_Pessoas(pPessoa pessoa, char nome[20]); //Escreve o ficheiro final txt sobre as pessoas
void Limpa_Pessoas(pPessoa pessoa); //Limpar a lista de Pessoas
void simulacao(pPessoa pessoa, psalas salas, int *contSalas); //Reponsalvel pela simulacao

#endif //TP_FUNCOES_H
