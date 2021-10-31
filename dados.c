// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2020-2021
// André Lopes - 2019139754

#include "dados.h"
#include "utils.h"
#include <string.h>

//DADOS DO TABULEIRO

void inicializaTabuleiro(pInfoJogada jogada) {

    // Alocar memoria para o Tabuleiro
    jogada->tabuleiro = malloc(sizeof(Tabuleiro));
    if (jogada->tabuleiro == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //Definir a dimensao do tabuleiro aleatoriamente
    int nlin;
    nlin = intUniformRnd(3, 5);
    jogada->tabuleiro->nColunas = nlin;
    jogada->tabuleiro->nLinhas = nlin;

    jogada->tabuleiro->Tab = malloc(jogada->tabuleiro->nLinhas * sizeof(char *));
    if(jogada->tabuleiro->Tab == NULL){
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //Alocar a memoria para cada linha do tabuleiro
    for (int i = 0; i < nlin; i++){
        jogada->tabuleiro->Tab[i] = malloc(jogada->tabuleiro->nColunas * sizeof(char));
        if(jogada->tabuleiro->Tab[i] == NULL){
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }
    }

    //Escrever valores na tabela
    for (int i = 0; i < nlin; i++)
        for (int j = 0; j < nlin; j++)
            jogada->tabuleiro->Tab[i][j] = '_';

}

void mostraTabuleiro(pInfoJogada info){

    int i, j;

    printf("          Tabuleiro do Jogo:\n\n");
    printf("              ");
    for (i=1; i <= info->tabuleiro->nColunas; i++){
        printf("%d  ", i);
    }
    printf("\n");

    for (i = 0; i < info->tabuleiro->nLinhas; i++) {
        printf("          %d   ", i+1);
        for (j = 0; j < info->tabuleiro->nColunas; j++) {
            printf("%c  ", info->tabuleiro->Tab[i][j]);
        }
        printf("\n");
    }

}

void mostraTabuleiroVazio(){

    int i, j;

    for (i = 0; i < 6; i++) {
        printf("            ");
        for (j = 0; j < 8; j++) {
            printf("_ ");
        }
        printf("\n");
    }
    printf("\n");
}


//DADOS DOS JOGADORES
Jogador atribuirDadosJogador(char *nome) {

    Jogador j;

    strcpy(j.nome, nome);
    j.linCol = 2;
    j.pedras = 1;

    return j;

}

void adicionarJogador(Jogador jogador[], char *nome,  int id){
    jogador[id] = atribuirDadosJogador(nome);
}

void mostrarDadosJogador(Jogador jogador){
    printf("\n      Pedras: %d | Lin/Col: %d\n", jogador.pedras, jogador.linCol);
  }
