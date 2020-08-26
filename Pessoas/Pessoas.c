//
// Created by Vilhena on 5/27/2020.
//

#include "../Headers/Pessoas.h"
#include "../Util/utils.h"
#include "../Headers/Funcoes.h"

pPessoa leficheiroPessoas(char ficheiro[100], pPessoa pessoa) { // le ficheiro TXT das pessoas
    FILE *f;
    char linha[200];
    char nome[100];
    int idade;
    char estado;
    int dinfetado;
    int mais;
    pPessoa aux = NULL;
    pPessoa testeID = NULL;
    f = fopen(ficheiro, "r");
    if (f == NULL) {
        printf("Erro no acesso ao ficheiro!!!\n");
        exit(1);
    }
    while (fgets(linha, 200, f) != NULL) {
        if (sscanf(linha, "%s %d %c %d", &nome, &idade, &estado, &dinfetado) < 3) {
            printf("Erro!! A linha nao tem os parametros certos: %s", linha);
        } else {
            if (pessoa != NULL) {
                testeID = pessoa;
                while (testeID != NULL) {
                    if (strcmp(nome,testeID->id) == 0) {
                        printf("ERRO!!! OS IDS DEVEM SER UNICOS");
                        exit(1);
                    }
                    testeID = testeID->prox;
                }
            }
            if (idade <= 0) { //Verifica a idade
                printf("ERRO!! A idade nao pode ser negativa ou igual a 0");
                exit(1);
            }
            if (estado != 'S' && estado != 'D' && estado != 'I') { //Verifica Estado
                printf("ERRO!! O estado apenas pode ser 'S'(Saudavel), 'D'(Doente) ou 'I'(Imune)");
                exit(1);
            }
            if (estado != 'D' && dinfetado < 0) { //Verifica dias infetado
                printf("O Infetado nao pode estar 0 dias infetado");
                exit(1);
            }
            //Escreve na Lista Pessoa
            if (pessoa == NULL) {
                pessoa = malloc(sizeof(epessoa));
                if (pessoa == NULL) {
                    printf("ERRO NA ALOCACAO DE MEMORIA\n");
                    exit(1);
                }
                strcpy(pessoa->id, nome);
                pessoa->idade = idade;
                pessoa->estado = estado;
                if (estado == 'D') {
                    pessoa->diasInfetado = dinfetado;
                }
                pessoa->Probrecuperacao = (1/(float)pessoa->idade);
                if (pessoa->idade > 10) {
                    mais = 0;
                    mais = trunc(((float)pessoa->idade/10));
                    pessoa->maxInfecao = 5 + mais;
                } else {
                    pessoa->maxInfecao = 5;
                }
                pessoa->pontsal = NULL;
                pessoa->prox = NULL;
            }
            else {
                aux = pessoa;
                while (aux->prox != NULL) {
                    aux = aux->prox;
                }
                aux->prox = malloc(sizeof(epessoa));
                if (aux->prox == NULL) {
                    printf("ERRO NA ALOCACAO DE MEMORIA\n");
                    exit(1);
                }
                strcpy(aux->prox->id, nome);
                aux->prox->idade = idade;
                aux->prox->estado = estado;
                if (estado == 'D') {
                    aux->prox->diasInfetado = dinfetado;
                }
                aux->prox->Probrecuperacao = (1/(float)aux->prox->idade);
                if (aux->prox->idade > 10) {
                    mais = 0;
                    mais = trunc(((float)aux->prox->idade/10));
                    aux->prox->maxInfecao = 5 + mais;
                } else {
                    aux->prox->maxInfecao = 5;
                }
                aux->prox->pontsal = NULL;
                aux->prox->prox = NULL;
            }
        }
    }
    fclose(f);
    return pessoa;
}

pPessoa Associa(pPessoa pessoa, psalas salas, int *contsalas){ //Funcao que associa as pessoas as salas
    pPessoa aux, auxb;
    int num;
    int flag = 0, cheio = 0;
    aux = pessoa;
    auxb = pessoa;
    while(aux != NULL) {
        do {
            for (int i = 0; i < *contsalas; i++) {
                if (salas[i].totalp < salas[i].capacidade) {
                    cheio = 1;
                }
            }
            if (cheio == 0) {
                while (auxb->prox->id != aux->id) {
                    auxb = auxb->prox;
                }
                Limpa_Pessoas(aux);
                auxb->prox = NULL;
                return pessoa;
            }
            num = intUniformRnd(0,*contsalas);
            if (salas[num].totalp < salas[num].capacidade) {
                aux->pontsal = &salas[num];
                salas[num].totalp++;
                salas[num].taxaDisseminacao = 0.75 * salas[num].totalp; //Alterar
                flag = 1;
            }
            else {
                flag = 0;
            }
        } while (flag == 0);
        aux = aux->prox;
    }
    return pessoa;
}

//Calculos

int totalPessoas(pPessoa pessoa) {
    pPessoa aux;
    int contador = 0;
    aux = pessoa;
    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }
    return contador;
}

int totalInfetados(pPessoa pessoa) {
    pPessoa aux;
    int contador = 0;
    aux = pessoa;
    while (aux != NULL) {
        if (aux->estado == 'D') {
            contador++;
        }
        aux = aux->prox;
    }
    return contador;
}

int totalImunes(pPessoa pessoa) {
    pPessoa aux;
    int contador = 0;
    aux = pessoa;
    while (aux != NULL) {
        if(aux->estado == 'I') {
            contador++;
        }
        aux = aux->prox;
    }
    return contador;
}

int totalSaudaveis(pPessoa pessoa) {
    pPessoa aux;
    int contador = 0;
    aux = pessoa;
    while (aux != NULL) {
        if(aux->estado == 'S') {
            contador++;
        }
        aux = aux->prox;
    }
    return contador;
}

float taxaInfetados(pPessoa pessoa) {
    float taxa = ((float)totalInfetados(pessoa) / (float)totalPessoas(pessoa));
    return taxa;
}
float taxaImunes(pPessoa pessoa) {
    float taxa = ((float)totalImunes(pessoa) / (float)totalPessoas(pessoa));
    return taxa;
}
float taxaSaudaveis(pPessoa pessoa) {
    float taxa = ((float)totalSaudaveis(pessoa) / (float)totalPessoas(pessoa));
    return taxa;
}

pPessoa Adiciona(pPessoa pessoa, psalas salas, int *contSalas, int IDLocal, char nome[100], int idade, int dias) {
    pPessoa aux = pessoa;
    int found = 0;
    int mais;
    for (int i = 0; i < *contSalas; i++) { //Percorrer o Array de salas ate encontrar a Sala com ID passado
        if (salas[i].id == IDLocal) {
            found = 1;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = malloc(sizeof(epessoa));
            if (aux->prox == NULL) {
                printf("ERRO NA ALOCACAO DE MEMORIA\n");
                exit(1);
            }
            strcpy(aux->prox->id, nome);
            aux->prox->idade = idade;
            aux->prox->estado = 'D';
            aux->prox->diasInfetado = dias;
            aux->prox->pontsal = &salas[i];
            aux->prox->Probrecuperacao = (1/(float)aux->prox->idade);
            if (aux->prox->idade > 10) {
                mais = 0;
                mais = trunc(((float)aux->prox->idade/10));
                aux->prox->maxInfecao = 5 + mais;
            } else {
                aux->prox->maxInfecao = 5;
            }
            salas[i].totalp = salas[i].totalp + 1;
            salas[i].taxaDisseminacao = 0.05 * salas[i].totalp;
            aux->prox->prox = NULL;
            return pessoa;

        } else {
            found = 0;
        }
    }
    if (found == 0) { //Caso nao encontre apresenta um Erro
        printf("ERRO NAO FOI POSSIVEL ENCONTRAR A SALA COM ID: %d", IDLocal);
    }
}

pPessoa guarda_Iteracoes(pPessoa pessoa1, pPessoa pessoa2) { //Guarda da pessoa1 para a pessoa2
    pPessoa aux1, aux2;
    aux1 = pessoa1;
    if (pessoa2 == NULL) {
        pessoa2 = malloc(sizeof(epessoa));
        pessoa2->prox = NULL;
        if (pessoa2 == NULL) {
            printf("ERRO DE ALOCACAO DE MEMORIA");
            exit(1);
        }
    }
    aux2 = pessoa2;
    while(aux1 != NULL) {
        strcpy(aux2->id, aux1->id);
        aux2->idade = aux1->idade;
        aux2->estado = aux1->estado;
        aux2->diasInfetado = aux1->diasInfetado;
        aux2->Probrecuperacao = aux1->Probrecuperacao;
        aux2->maxInfecao = aux1->maxInfecao;
        aux2->pontsal = aux1->pontsal;
        if(aux2->prox == NULL && aux1->prox != NULL) {
            aux2->prox = malloc(sizeof(epessoa));
            aux2->prox->prox = NULL;
        }
        aux2 = aux2->prox;
        aux1 = aux1->prox;
    }
    return pessoa2;
}

pPessoa recupera_Iteracao(pPessoa pessoa1, pPessoa pessoa2) { //Copia da 1 para a 2
    pPessoa aux1, aux2;
    aux1 = pessoa1;
    aux2 = pessoa2;
    while(aux2 != NULL) {
        strcpy(aux1->id, aux2->id);
        aux2->idade = aux1->idade;
        aux2->estado = aux1->estado;
        aux2->diasInfetado = aux1->diasInfetado;
        aux2->Probrecuperacao = aux1->Probrecuperacao;
        aux2->maxInfecao = aux1->maxInfecao;
        aux2->pontsal = aux1->pontsal;
        aux2->prox = aux1->prox;
        if(aux2->prox != NULL && aux1->prox == NULL) {
            Limpa_Pessoas(aux2->prox);
            aux2->prox = NULL;
        }
        aux2 = aux2->prox;
        aux1 = aux1->prox;
    }
    return pessoa2;
}



