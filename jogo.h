// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2020-2021
// André Lopes - 2019139754

#ifndef TP_JOGO_H
#define TP_JOGO_H
#include "dados.h"

int menuPrincipal();
int modoJogo();
int menuJogada();
void regrasJogo();

void iniciarJogo();
int jogada(pInfoJogada lista, pInfoJogada inf, Jogador jogadores[]);
void jogarPecaHumano(pInfoJogada inf);
void jogarPecaAutomatico(pInfoJogada inf);
int fimJogo(pInfoJogada inf);

int colunasIguais(pInfoJogada inf, char letra);
int verificaColunas(pInfoJogada inf);
int linhasIguais(pInfoJogada inf, char letra);
int verificaLinhas(pInfoJogada inf);
int diagonaisIguais(pInfoJogada inf, char letra);
int verificaDiagonais(pInfoJogada inf);

void adicionaLinha(pInfoJogada inf, pJogador jogador);
void adicionaColuna(pInfoJogada inf, pJogador jogador);
void jogarPedra(pInfoJogada inf, pJogador jogador);

void mostrarJogadasAnteriores(pInfoJogada lista, int num, int total);

int contaJogadas(pInfoJogada lista);
pInfoJogada guardaTabuleiro(pInfoJogada  atual, pInfoJogada lista);

void gravaJogoTexto(pInfoJogada lista);
void gravarJogo(pInfoJogada info, pInfoJogada  lista, Jogador *jogadores);
void carregarJogo(pInfoJogada info, pInfoJogada  lista, Jogador *jogadores);
int verificarFicheiro();
void libertarMemoria(pInfoJogada jogada, pInfoJogada todasJogadas);


#endif //TP_JOGO_H
