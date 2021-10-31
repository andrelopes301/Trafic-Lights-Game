// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2020-2021
// André Lopes - 2019139754
#include <stdio.h>
#include <stdlib.h>

#ifndef TP_DADOS_H
#define TP_DADOS_H


typedef struct jogador Jogador, *pJogador;
struct jogador{
    char nome[20];
    int pedras;
    int linCol;
};


typedef struct tabuleiro Tabuleiro, *pTabuleiro;
struct tabuleiro{

    int nLinhas;
    int nColunas;
    char **Tab;
};


typedef struct infoJogada InfoJogada, *pInfoJogada;
    struct infoJogada{

        int jogadorJogar;
        int numJogada;
        int modo;
        pTabuleiro tabuleiro;
        pInfoJogada prox;
    };



//DADOS DO TABULEIRO
void mostraTabuleiro(pInfoJogada info);
void inicializaTabuleiro(pInfoJogada jogada);
void mostraTabuleiroVazio();

//DADOS DOS JOGADORES
Jogador atribuirDadosJogador(char *nome);
void adicionarJogador(Jogador jogador[], char *nome,  int id);
void mostrarDadosJogador(Jogador jogador);



#endif //TP_DADOS_H
