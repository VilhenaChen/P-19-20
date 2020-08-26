//
// Created by Vilhena on 6/3/2020.
//

#include "../Headers/Funcoes.h"
void limpa_input() //Limpar input
{
    int c;
    // Limpa os caracteres que ainda estejam no buffer
    while ((c=getchar())!='\n' && c!=EOF);
}

void avanca(pPessoa pessoa, psalas salas, int *contSalas) { //Funcao que trata de avancar com a Simulacao
    pPessoa aux;
    int cont = 0;
    int Disseminacao;
    for (int i = 0; i < *contSalas; i++) {
        aux = pessoa;
        Disseminacao = trunc(salas[i].taxaDisseminacao);
        printf("-----------------------------------------------------------------\n");
        printf("Sala: %d\n", salas[i].id);
        while (aux != NULL) {
            if (aux->estado == 'D') { //Caso a pessoa esteja Doente
                if (probEvento(aux->Probrecuperacao) == 1) { //Caso recupere por causa da Taxa
                    if (probEvento(0.20) == 1) {
                        aux->estado = 'I';
                        aux->diasInfetado = 0;
                        printf("\tA pessoa %s de %d anos recuperou da Infecao tornado-se Imune\n", aux->id, aux->idade);
                    } else {
                        aux->estado = 'S';
                        aux->diasInfetado = 0;
                        printf("\tA pessoa %s de %d anos recuperou da Infecao\n", aux->id, aux->idade);
                    }
                } else { //Caso seja por Dias
                    aux->diasInfetado = aux->diasInfetado - 1;
                    if (aux->diasInfetado == 0) {
                        if (probEvento(0.20) == 1) {
                            aux->estado = 'I';
                            printf("\tA pessoa %s de %d anos recuperou da Infecao tornado-se Imune\n", aux->id,
                                   aux->idade);
                        } else {
                            aux->estado = 'S';
                            aux->diasInfetado = 0;
                            printf("\tA pessoa %s de %d anos recuperou da Infecao porque passaram os dias necessarios\n",
                                   aux->id, aux->idade);
                        }
                    }
                }
            } else { //Caso nao esteja doente
                if (aux->estado == 'I') {
                    if (cont < Disseminacao)
                        cont = cont + 1;
                } else if (aux->estado == 'S') {
                    if (cont < Disseminacao) {
                        aux->estado = 'D';
                        aux->diasInfetado = aux->maxInfecao;
                        printf("\tA pessoa %s de %d anos foi Infetada durante %d Dias\n", aux->id, aux->idade,
                               aux->diasInfetado);
                        cont = cont + 1;
                    }
                }
            }
            aux = aux->prox;
        }
    }
}

void estatistica(pPessoa pessoa, psalas salas, int *contSalas) { //Funcao responsavel pela estatistica
    pPessoa aux;
    aux = pessoa;
    float taxaSauda, taxaInfe, taxaImu;
    int cont = 0;
    taxaSauda = taxaSaudaveis(aux);
    taxaInfe = taxaInfetados(aux);
    taxaImu = taxaImunes(aux);
    printf("\n---------------------------- PESSOAS --------------------------------\n");
    while (aux != NULL) {
        if (aux->estado == 'D')
            printf("ID: %s || Idade: %d || Estado: %c || Dias de Infecao: %d\n", aux->id,aux->idade,aux->estado,aux->diasInfetado);
        else
            printf("ID: %s || Idade: %d || Estado: %c\n", aux->id,aux->idade,aux->estado);
        aux = aux->prox;
    }
    printf("\n---------------------------- DISTRIBUCAO ----------------------------\n");
    for (int i = 0; i < *contSalas; i++) {
        printf("Sala: %d Pesosas dentro: %d de %d\n", salas[i].id,  salas[i].totalp, salas[i].capacidade);
        for (int k = 0; k < 3; k++) {
            printf("\tLigada a: %d\n", salas[i].liga[k]);
        }
    }
    printf("\n--------------------------- TAXAS -----------------------------\n");
    printf("Taxa de Pessoas Saudaveis: %.2f\n", taxaSauda);
    printf("Taxa de Pessoas Infetadas: %.2f\n", taxaInfe);
    printf("Taxa de Pessoas Imunes: %.2f\n", taxaImu);
}

void transferePesssoas(pPessoa pessoa, psalas salas, int idOrigem, int idDestino, int nPessoas, int *contSalas) { //Funcao que trata da transferencia de pessoas entre salas
    pPessoa aux;
    int id, totalp, contador = 0, step;
    aux = pessoa;
    totalp = totalPessoas(aux);
    printf("\n---------------------------------------------------------------\n");
    do {
        step = 0;
        aux = pessoa;
        id = intUniformRnd(0, totalp - 1);
        while (aux != NULL && id != step) { //Percorre ate encontrar a pessoa com o id igual ao que saiu random
            aux = aux->prox;
            step++;
        }
        if (aux->pontsal->id == idOrigem) { //Caso essa pessoa esteja naquela sala
            for (int i = 0; i < *contSalas; i++) { //Percorre as salas ate encontrar a sala que tem o id de Destino
                if (salas[i].id == idDestino) {
                    if (salas[i].capacidade != salas[i].totalp) { //verifica se a sala de destino nao esta cheia
                        aux->pontsal = &salas[i];
                        salas[i].totalp = salas[i].totalp + 1; //Aumenta o total de pessoas na sala de destino e atualiza a taxa de disseminacao
                        salas[i].taxaDisseminacao = 0.05 * salas[i].totalp;
                        for(int j = 0; j < *contSalas; j++) {
                            if(salas[j].id == idOrigem) { // Tira uma pessoa do total de pessoas dentro da sala de Origem e atualiza a taxa de disseminacao
                                salas[j].totalp = salas[j].totalp - 1;
                                salas[j].taxaDisseminacao = 0.05 * salas[j].totalp;
                                printf("A pessoa %s foi movida da sala %d para a sala %d\n", aux->id, salas[j].id, salas[i].id);
                            }
                        }
                        contador++;
                    } else
                        printf("A sala com id %d, esta cheia\n",salas[i].id);
                }
            }
        }
    }while(contador != nPessoas);
}

void atualiza_Informacao_Salas(pPessoa pessoas, psalas salas, int *contSalas) { //Funcao responsavel por Atualizar a informacao das salas, util quando voltamos atras
    pPessoa aux;
    int contador = 0;
    aux = pessoas;
    for (int i = 0; i < *contSalas; i++) {
        while (aux != NULL) {
            if (aux->pontsal->id == salas[i].id) {
                salas[i].totalp = salas[i].totalp + 1;
                salas[i].taxaDisseminacao = 0.05 * salas[i].totalp;
            }
            aux = aux->prox;
        }
    }
}


void Escreve_Ficheiro(pPessoa pessoa, psalas salas, int *contSalas, int it, int itmax) { //Escreve o ficheiro Report.txt no final da Simulacao
    FILE *f;
    pPessoa aux;
    aux = pessoa;
    float taxaSauda, taxaInfe, taxaImu;
    taxaSauda = taxaSaudaveis(aux);
    taxaInfe = taxaInfetados(aux);
    taxaImu = taxaImunes(aux);
    f = fopen("../report.txt","w");
    if (f == NULL) {
        printf("ERRO na Criacao do ficheiro!!!");
        exit(1);
    }
    fprintf(f,"Relatorio final passadas %d Iteracoes de %d\n", it, itmax);
    fprintf(f,"\n---------------------------- DISTRIBUCAO ----------------------------\n");
    for (int i = 0; i < *contSalas; i++) {
        fprintf(f,"Sala: %d Pesosas dentro: %d de %d\n", salas[i].id,  salas[i].totalp, salas[i].capacidade);
        for (int k = 0; k < 3; k++) {
            fprintf(f,"\tLigada a: %d\n", salas[i].liga[k]);
        }
    }
    fprintf(f,"\n--------------------------- TAXAS -----------------------------\n");
    fprintf(f,"Taxa de Pessoas Saudaveis: %.2f\n", taxaSauda);
    fprintf(f,"Taxa de Pessoas Infetadas: %.2f\n", taxaInfe);
    fprintf(f,"Taxa de Pessoas Imunes: %.2f\n", taxaImu);
    fprintf(f,"---------------------------------------------------------------\n");
    fclose(f);
}

void Escreve_Ficheiro_Pessoas(pPessoa pessoa, char nome[20]) { //Funcao que escreve o ficheiro das pessaos
    pPessoa aux;
    FILE *f;
    char fich[23] = "../";
    aux = pessoa;
    strcat(fich,nome);
    f = fopen(fich, "w");
    if (f == NULL) {
        printf("ERRO NO ACESSO AO FICHEIRO\n");
        exit(1);
    }
    while (aux != NULL) {
        if (aux->estado == 'D') {
            fprintf(f,"%s %d %c %d\n", aux->id,aux->idade,aux->estado,aux->diasInfetado);
            }
        else {
            fprintf(f, "%s %d %c \n", aux->id, aux->idade, aux->estado);
        }
        aux = aux->prox;
    }
    fclose(f);
}


void Limpa_Pessoas(pPessoa pessoa) { //Limpa lista de Pessoas
    if (pessoa->prox != NULL) {
        Limpa_Pessoas(pessoa->prox);
    }
    free(pessoa);
}

void simulacao(pPessoa pessoa, psalas salas, int *contSalas) { //Reponsalvel por toda a simulacao
    int op;
    int maxIt = 0;
    int it = 0;
    int TrasIt = 0;
    char nome[20];
    int flag = 0;
    pPessoa auxP;
    pPessoa antiga1 = NULL;
    pPessoa antiga2 = NULL;
    pPessoa antiga3 = NULL;
    pPessoa  testeID= NULL;
    psalas auxS;
    int origem, destino, nPessoas;
    auxP = pessoa;
    auxS = salas;
    do {
        printf("-------------------------------------------------------------------\n");
        printf("Insira o numero maximo de iteracoes: ");
        scanf("%d", &maxIt);
        limpa_input();
    } while (maxIt <= 0);
    do {
        printf("-------------------------------------------------------------------\n");
        printf("Iteracao %d de %d\n",it, maxIt);
        printf("1 - Transferir Pessoas\n");
        printf("2 - Adicionar um Doente\n");
        printf("3 - Avancar 1 iteracao\n");
        printf("4 - Recuar  iteracoes\n");
        printf("5 - Mostrar Estatistica\n");
        printf("6 - Sair\n");
        printf("Indique a Opcao que pretende: ");
        scanf("%d", &op);
        limpa_input();
        //Transferir Pessoas
        if (op == 1) {
            flag = 0;
            printf("Indique a Sala de Origem: ");
            scanf("%d", &origem);
            printf("%d\n", origem);
            limpa_input();
            while (flag == 0) {
                for (int i = 0; i < *contSalas; i++) {
                    if (salas[i].id == origem) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    printf("Por Favor isnira um valor de ID valido de Origem: ");
                    scanf("%d", &origem);
                    limpa_input();
                }
            }
            flag = 0;
            printf("Indique o ID da sala de Destino: ");
            scanf("%d", &destino);
            limpa_input();
            while (flag == 0) {
                for (int i = 0; i < *contSalas; i++) {
                    if (salas[i].id == origem) {
                        for (int j = 0; j < 3; j++) {
                            if (salas[i].liga[j] == destino) {
                                flag = 1;
                            }
                        }
                    }
                }
                if (flag == 0) {
                    printf("\nPor Favor insira um valor de ID valido de Destino: ");
                    scanf("%d", &destino);
                    limpa_input();
                }
            }
            flag = 0;
            printf("Indique a quantidade de pessoas que quer mover: ");
            scanf("%d", &nPessoas);
            while (flag == 0) {
                for (int i = 0; i < *contSalas; i++) {
                    if (salas[i].id == origem && salas[i].totalp >= nPessoas) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    printf("Por Favor insira um valor de pessoas a mover valido: ");
                    scanf("%d", &nPessoas);
                    limpa_input();
                }
            }
            transferePesssoas(auxP, auxS, origem, destino, nPessoas, contSalas);
            //Adiciona doente
        } else if (op == 2) {
            int IDLocal = 0, idade = 0, dias = 0;
            flag = 0;
            char nome[100];
            printf("Indique o ID da sala que pretende: ");
            scanf("%d", &IDLocal);
            limpa_input();
            while(flag == 0) {
                for (int k = 0; k < *contSalas; k++) {
                    if (salas[k].id == IDLocal && salas[k].totalp < salas[k].capacidade) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    printf("A Sala com ID %d nao existe ou esta Cheia!!! Por insira um ID valido: ",IDLocal);
                    scanf("%d", &IDLocal);
                    limpa_input();
                }
            }
            printf("\nIndique o Nome(ID) da Pessoa a Adicionar: ");
            scanf("%s", &nome);
            limpa_input();
            testeID = pessoa;
            while (testeID != NULL) {
                while (strcmp(nome,testeID->id) == 0) {
                    printf("ERRO!!! OS IDS DEVEM SER UNICOS\n");
                    printf("Por favor insira um novo Nome(ID): ");
                    scanf("%s", &nome);
                    limpa_input();
                }
                testeID = testeID->prox;
            }
            printf("\nIndique a Idade da Pessoa a Adicionar: ");
            scanf("%d", &idade);
            limpa_input();
            while(idade <= 0) {
                printf("\nIdade Invalida, por favor insira uma idade valida: ");
                scanf("%d", &idade);
                limpa_input();
            }
            printf("\nIndique os dias de Doenca da Pessoa a Adicionar: ");
            scanf("%d", &dias);
            limpa_input();
            while(dias <= 0) {
                printf("\nDias de Infecao Invalidos, por favor insira um valor valido: ");
                scanf("%d", &dias);
                limpa_input();
            }
            auxP = Adiciona(auxP, auxS, contSalas, IDLocal, nome, idade, dias);
            printf("\n-------------------------------- PESSOAS ------------------------------\n");
            while (auxP != NULL) {
                printf("Nome: %s, %d, %c\n", auxP->id, auxP->idade, auxP->estado);
                auxP = auxP->prox;
            }
            auxP = pessoa;
            //Avancar Iteracoes
        } else if (op == 3) {
            if (it < maxIt) {
                it++;
                if (it == 1) {
                    antiga1 = guarda_Iteracoes(auxP, antiga1);
                } else if (it == 2) {
                    antiga2 = guarda_Iteracoes(antiga1, antiga2);
                    antiga1 = guarda_Iteracoes(auxP, antiga1);
                } else {
                    antiga3 = guarda_Iteracoes(antiga2, antiga3);
                    antiga2 = guarda_Iteracoes(antiga1, antiga2);
                    antiga1 = guarda_Iteracoes(auxP, antiga1);
                }
                avanca(auxP,auxS, contSalas);
            }
            if (it == maxIt) {
                printf("Chegou ao Maximo de iteracoes, o programa ira agora sair e gerar um Relatorio\n");
                op = 6;
            }
            //Recurar Iteracoes
        }else if (op == 4) {
            do {
                TrasIt = 0;
                printf("Quantas iteracoes deseja voltar atras?(Max 3):");
                scanf("%d",&TrasIt);
                if (TrasIt == 1 && it > 1) {
                    auxP = recupera_Iteracao(antiga1, auxP);
                    it = it - 1;
                    atualiza_Informacao_Salas(auxP, auxS, contSalas);
                    estatistica(auxP,auxS,contSalas);
                } else if (TrasIt == 2 && it > 2) {
                    auxP = recupera_Iteracao(antiga2, auxP);
                    it = it - 2;
                    atualiza_Informacao_Salas(auxP, auxS, contSalas);
                    estatistica(auxP,auxS,contSalas);
                } else if (TrasIt == 3 && it > 3) {
                    auxP = recupera_Iteracao(antiga3, auxP);
                    it = it - 3;
                    atualiza_Informacao_Salas(auxP, auxS, contSalas);
                    estatistica(auxP,auxS,contSalas);
                } else
                    printf("ERRO!!! POR FAVOR INSIRA UM NUMERO VALIDO\n");
            }while(TrasIt > 3 || TrasIt < 1);
            //Mostrar Estatistica
        }else if (op == 5) {
            estatistica(auxP, auxS, contSalas);
        }
    }while (op != 6);
    printf("Insira o nome do ficheiro onde quer guardar as informcaoes das Pessoas:");
    gets(nome);
    Escreve_Ficheiro(auxP, auxS, contSalas, it, maxIt);
    Escreve_Ficheiro_Pessoas(auxP,nome);
}
