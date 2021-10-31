// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2020-2021
// André Lopes - 2019139754

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"
#include "utils.h"
#include "dados.h"

#define JOGOGUARDADO "jogo.bin"


void iniciarJogo() {

    Jogador jogadores[2];
    pInfoJogada infoJogada = NULL;
    pInfoJogada listaJogadas = NULL;
    int opcao = 2;

    infoJogada = malloc(sizeof(InfoJogada));
    if (infoJogada == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    if(verificarFicheiro() == 1) {
        printf("\nDeseja continuar o jogo anterior?\n");
        do {
            printf("\n 1. Sim\n 2. Nao\n\nOpcao: ");
            scanf(" %d", &opcao);
            if (opcao > 2 || opcao < 1)
                printf("\n[INFO] Introduza uma opcao valida!\n");
        } while (opcao > 2 || opcao < 1);

        if (opcao == 1) {

            listaJogadas = malloc(sizeof(InfoJogada));
            if (listaJogadas == NULL) {
                fprintf(stderr, "Erro a alocar memoria!");
                exit(1);
            }
            carregarJogo(infoJogada, listaJogadas, jogadores);
        }
    }

    if(opcao == 2){

        infoJogada->numJogada = 1;
        infoJogada->modo = modoJogo();
        infoJogada->jogadorJogar = 1;

        //Inicializar Tabuleiro
        inicializaTabuleiro(infoJogada);

        //Inicializar Jogadores
        adicionarJogador(jogadores, "Jogador A",0);
        if(infoJogada->modo == 1)
            adicionarJogador(jogadores, "Jogador B",1);
        else
            adicionarJogador(jogadores, "Jogador Virtual",1);
    }


    do {
        printf("\n------------- %dª Jogada -------------\n\n", infoJogada->numJogada);
        mostraTabuleiro(infoJogada);
        if(jogada(listaJogadas, infoJogada, jogadores) == 1)
            break;

        listaJogadas = guardaTabuleiro(infoJogada, listaJogadas);
        if (fimJogo(infoJogada)) {
            printf("\n------------- Fim do Jogo -------------\n\n");
            mostraTabuleiro(infoJogada);
            printf("\n[Info] O vencedor foi o %s!\n",jogadores[infoJogada->jogadorJogar - 1].nome);
            gravaJogoTexto(listaJogadas);
        } else {
            infoJogada->jogadorJogar = infoJogada->jogadorJogar % 2 + 1;
            infoJogada->numJogada++;
        }

    }while (fimJogo(infoJogada) != 1);


    libertarMemoria(infoJogada,listaJogadas);

    printf("[Info] Memoria libertada!\n");

    printf("\nPressione qualquer tecla para sair do jogo...\n");
    getchar(); //receber qualquer tecla para continuar
    getchar();

}


void libertarMemoria(pInfoJogada jogada, pInfoJogada todasJogadas){
    //Libertar memoria
    for (int i = 0; i < jogada->tabuleiro->nLinhas; i++)
        free(jogada->tabuleiro->Tab[i]);

    free(jogada->tabuleiro);
    free(jogada);

    pInfoJogada aux = todasJogadas;

    while(aux != NULL){
        aux = aux->prox;
        for (int i = 0; i < todasJogadas->tabuleiro->nLinhas; i++)
            free(todasJogadas->tabuleiro->Tab[i]);

        free(todasJogadas->tabuleiro);
        free(todasJogadas);
        todasJogadas = aux;
    }
}


int jogada(pInfoJogada lista, pInfoJogada inf, Jogador jogadores[]){

    int opc, op, num;
    int valida = 0;

    printf("\n[Info] É a vez do %s!\n",jogadores[inf->jogadorJogar-1].nome);

    if (inf->jogadorJogar == 2 && inf->modo == 2) {
        jogarPecaAutomatico(inf);
    }else
        mostrarDadosJogador(jogadores[inf->jogadorJogar - 1]);

    if (inf->modo == 1 || (inf->modo == 2 && inf->jogadorJogar == 1)) {
          do{
            opc = menuJogada();
            switch (opc) {
                case 1: // Jogar Peca
                    printf("Indique a posição da peça a jogar (lin,col): \n");
                    jogarPecaHumano(inf);
                    valida = 1;
                    break;
                case 2: // Jogar Pedra
                    if (jogadores[inf->jogadorJogar - 1].pedras > 0) {
                        jogarPedra(inf, &jogadores[inf->jogadorJogar - 1]);
                        valida = 1;
                    } else
                        printf("\n[Info] Nao tem mais pedras para poder jogar!\n");
                    break;
                case 3: // Adicionar linha/coluna
                    if (jogadores[inf->jogadorJogar - 1].linCol > 0) {
                        do {
                            printf("\n 1. Adicionar Linha\n"
                                   " 2. Adicionar Coluna\n\n"
                                   "Opcao: ");
                            scanf(" %d", &op);
                            if (op > 2 || op < 1)
                                printf("\n[Info] Introduza uma opcao valida!\n");
                        } while (op > 2 || op < 1);
                        if (op == 1)
                            adicionaLinha(inf, &jogadores[inf->jogadorJogar - 1]);
                        else
                            adicionaColuna(inf, &jogadores[inf->jogadorJogar - 1]);
                        valida = 1;
                    } else
                        printf("[Info] Nao tem mais jogadas para adicionar linhas/colunas!\n");
                    break;
                case 4: // Visualizar 'K' jogadas anteriores
                    printf("\n[INFO] Numero de jogadas realizadas: %d\n", contaJogadas(lista));
                    if(contaJogadas(lista) == 0){
                        printf("[Info] Ainda nao existem jogadas para poder visualizar!\n");
                        break;
                    }
                    do {
                        printf("\nIntroduza o numero de jogadas anteriores que pretende visualizar: ");
                        scanf(" %d", &num);
                        if (num > contaJogadas(lista) || num < 1)
                            printf("\n[Info] Introduza uma opcao valida!\n");
                    } while (num > contaJogadas(lista) || num < 1);

                    printf("\n--------------------------------------");
                    mostrarJogadasAnteriores(lista,  num, contaJogadas(lista));
                    printf("\n--------------------------------------\n");

                    break;
                case 5: // Guardar o jogo
                    gravarJogo(inf, lista, jogadores);
                    break;
                case 0:
                    return 1;
                default:
                    printf("[Info] Introduza uma opcao valida!\n");
                    break;
            }
          } while (valida == 0);
        }
    return 0;
}

void jogarPecaHumano(pInfoJogada info){

    int pos_Lin, pos_Col;

    do{
        printf("Lin: ");
        scanf("%d", &pos_Lin);

        printf("Col: ");
        scanf("%d", &pos_Col);

        if(pos_Lin < 1 || pos_Col < 1 || pos_Col > info->tabuleiro->nColunas || pos_Lin > info->tabuleiro->nLinhas || info->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] == 'R' || info->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] == 'O')
            printf("[Info] Introduza uma posicao valida!\n");

    }while(pos_Lin < 1 || pos_Col < 1 || pos_Col > info->tabuleiro->nColunas || pos_Lin > info->tabuleiro->nLinhas || info->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] == 'R' || info->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] == 'O');

    if(info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == '_')
        info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'G';
    else if( info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == 'G')
        info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'Y';
    else if( info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == 'Y')
        info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'R';
}

void jogarPecaAutomatico(pInfoJogada info){

    int pos_Lin, pos_Col;
    do{
        pos_Lin = intUniformRnd(0, info->tabuleiro->nLinhas);
        pos_Col = intUniformRnd(0, info->tabuleiro->nColunas);
    }while(pos_Lin < 1 || pos_Col < 1 || pos_Col > info->tabuleiro->nColunas || pos_Lin > info->tabuleiro->nLinhas || info->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] == 'R' || info->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] == 'O');

    if( info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == '_')
        info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'G';
    else if( info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == 'G')
        info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'Y';
    else if( info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == 'Y')
        info->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'R';

    printf("\nA posição jogada pelo Computador foi: (%d,%d)\n",pos_Lin,pos_Col);
    printf("\nPressione qualquer tecla para continuar...");
    getchar();
    getchar();

}

void jogarPedra(pInfoJogada inf, pJogador jogador) {

    int pos_Lin, pos_Col;

    printf("\nIndique a posicao onde quer jogar a pedra!\n");
    do{
        printf("Lin: ");
        scanf("%d", &pos_Lin);

        printf("Col: ");
        scanf("%d", &pos_Col);

        if(pos_Lin < 1 || pos_Col < 1 || pos_Col > inf->tabuleiro->nColunas || pos_Lin > inf->tabuleiro->nLinhas || inf->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] != '_')
            printf("[Info] Introduza uma posicao valida!\n");

    }while(pos_Lin < 1 || pos_Col < 1 || pos_Col > inf->tabuleiro->nColunas || pos_Lin > inf->tabuleiro->nLinhas || inf->tabuleiro->Tab[(pos_Lin-1)][pos_Col-1] != '_');

    if(inf->tabuleiro->Tab[pos_Lin-1][pos_Col-1] == '_')
        inf->tabuleiro->Tab[pos_Lin-1][pos_Col-1] = 'O';

    //Retirar pedra ao jogador
    jogador->pedras -= 1;
}

int fimJogo(pInfoJogada info){

    if(verificaLinhas(info) == 1 || verificaColunas(info) == 1 || verificaDiagonais(info) == 1)
        return 1; // final do jogo

    return 0;
}

int colunasIguais(pInfoJogada inf, char letra){

    int i, j;
    int letraIgual = 0;

    for (i = 0; i < inf->tabuleiro->nColunas; i++) {
        for(j = 0; j < inf->tabuleiro->nLinhas; j++){
            if(inf->tabuleiro->Tab[j][i] == letra)
                letraIgual++;
        }
        if (letraIgual == inf->tabuleiro->nLinhas)
            return 1;
        else
            letraIgual = 0;
    }
    return 0;
}

int verificaColunas(pInfoJogada info){

    if(colunasIguais(info,'G'))
        return 1;
    if(colunasIguais(info,'Y'))
        return 1;
    if(colunasIguais(info,'R'))
        return 1;

    return 0;
}

int linhasIguais(pInfoJogada inf, char letra){

    int i, j;
    int letraIgual = 0;

    for (i = 0; i < inf->tabuleiro->nLinhas; i++) {
        for(j = 0; j < inf->tabuleiro->nColunas; j++){
            if(inf->tabuleiro->Tab[i][j] == letra)
                letraIgual++;
        }
        if (letraIgual == inf->tabuleiro->nColunas)
            return 1;
        else
            letraIgual = 0;
    }
    return 0;
}

int verificaLinhas(pInfoJogada inf){

    if(linhasIguais(inf,'G'))
        return 1;
    if(linhasIguais(inf,'Y'))
        return 1;
    if(linhasIguais(inf,'R'))
        return 1;

    return 0;

}

int diagonaisIguais(pInfoJogada inf, char letra){

    int i,j;
    int letraIgual = 0;

    if(inf->tabuleiro->nColunas == inf->tabuleiro->nLinhas) {

        for (i = 0; i < inf->tabuleiro->nColunas; i++) {
            if (inf->tabuleiro->Tab[i][i] == letra) {
                letraIgual++;
            } else
                letraIgual = 0;
        }
        if (letraIgual == inf->tabuleiro->nColunas) {
            return 1;
        }

        for (j = 0, i = inf->tabuleiro->nColunas - 1; i >= 0; i--, j++) {
            if (inf->tabuleiro->Tab[j][i] == letra) {
                letraIgual++;
            } else
                letraIgual = 0;
        }
        if (letraIgual == inf->tabuleiro->nColunas) {
            return 1;
        }
    }

    return 0;
}

int verificaDiagonais(pInfoJogada inf){

    if(diagonaisIguais(inf,'G'))
        return 1;
    if(diagonaisIguais(inf,'Y'))
        return 1;
    if(diagonaisIguais(inf,'R'))
        return 1;

    return 0;

}

void adicionaLinha(pInfoJogada info, pJogador jogador) {

    char **Tab_aux;

    //Realocar memoria para acrescentar a nova linha
    Tab_aux = realloc(info->tabuleiro->Tab, (info->tabuleiro->nLinhas + 1) * sizeof(char *));
    if (Tab_aux == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }else
        info->tabuleiro->Tab = Tab_aux;

    //Alocar a memoria para as ncolunas da nova linha
    info->tabuleiro->Tab[info->tabuleiro->nLinhas] = malloc(info->tabuleiro->nColunas * sizeof(char));

    if(info->tabuleiro->Tab[info->tabuleiro->nLinhas] == NULL){
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //Escrever valores na nova linha
    for (int i = 0; i < info->tabuleiro->nColunas; i++) {
        info->tabuleiro->Tab[info->tabuleiro->nLinhas][i] = '_';
    }

    info-> tabuleiro->nLinhas += 1;
    //Retirar jogada de adicionar col/lin ao jogador
    jogador->linCol -=1;

}

void adicionaColuna(pInfoJogada info, pJogador jogador){

    char **Tab_aux; // = tabuleiro->Tab;

    //Realocar memoria para acrescentar a nova linha
    Tab_aux = realloc(info->tabuleiro->Tab, (info->tabuleiro->nLinhas) * sizeof(char *));
    if (Tab_aux  == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //Realocar memoria para as linhas todas
    for (int i = 0; i < info->tabuleiro->nLinhas; i++) {
        Tab_aux[i] = realloc(info->tabuleiro->Tab[i], (info->tabuleiro->nColunas + 1) * sizeof(char));
        if (Tab_aux[i]  == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }
        else
            info->tabuleiro->Tab[i] = Tab_aux[i];
    }

    //Escrever valores na nova coluna
    for (int j = 0; j < info->tabuleiro->nLinhas; j++){
        info->tabuleiro->Tab[j][info->tabuleiro->nColunas] = '_';
    }

    info->tabuleiro->nColunas +=  1;
    //Retirar jogada de adicionar col/lin ao jogador
    jogador->linCol -=1;

}

void mostrarJogadasAnteriores(pInfoJogada lista, int num, int total){

    int i, j;

    int min = total - num; //jogada onde comeca a mostrar
    int cont=0;

    while ( lista != NULL ){
        if(cont >= min) {
            printf("\n\n Jogada %d: \n\n", lista->numJogada);
            printf("              ");
            for (i = 1; i <= lista->tabuleiro->nColunas; i++)
                printf("%d  ", i);
            printf("\n");
            for (i = 0; i < lista->tabuleiro->nLinhas; i++) {
                printf("          %d   ", i + 1);
                for (j = 0; j < lista->tabuleiro->nColunas; j++) {
                    printf("%c  ", lista->tabuleiro->Tab[i][j]);
                }
                printf("\n");
            }
        }
        cont++;
        lista = lista->prox;
    }


}


pInfoJogada guardaTabuleiro(pInfoJogada atual, pInfoJogada lista){

    pInfoJogada aux;

    if(lista == NULL) {

        lista = malloc(sizeof(InfoJogada));
        if (lista == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

        // Alocar espaço para a estrutura
        lista->tabuleiro = malloc(sizeof(Tabuleiro));
        if (lista->tabuleiro == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

        lista->tabuleiro->nLinhas = atual->tabuleiro->nLinhas;
        lista->tabuleiro->nColunas = atual->tabuleiro->nColunas;
        lista->numJogada = atual->numJogada;
        lista->modo = atual->modo;
        lista->jogadorJogar = atual->jogadorJogar;


        lista->tabuleiro->Tab = malloc(atual->tabuleiro->nLinhas * sizeof(char *));
        if (lista->tabuleiro->Tab == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

        //Alocar a memoria das ncolunas para cada linha
        for (int i = 0; i < atual->tabuleiro->nLinhas; i++) {
            lista->tabuleiro->Tab[i] = malloc( atual->tabuleiro->nColunas * sizeof(char));
            if (lista->tabuleiro->Tab[i] == NULL) {
                fprintf(stderr, "Erro a alocar memoria!");
                exit(1);
            }
        }

        //Escrever valores na tabela
        for (int i = 0; i <  atual->tabuleiro->nLinhas; i++)
            for (int j = 0; j <  atual->tabuleiro->nColunas; j++)
                lista->tabuleiro->Tab[i][j] = atual->tabuleiro->Tab[i][j];

    }else{

       // aux = lista;
      //  while (aux->prox != NULL)
       //     aux = aux->prox;

    //    aux->prox = malloc(sizeof(InfoJogada));
    //    if (aux->prox == NULL) {
    //        fprintf(stderr, "Erro a alocar memoria!");
    //        exit(1);
    //    }else
    //        aux = aux->prox;

        aux = malloc(sizeof(InfoJogada));
           if (aux == NULL) {
                fprintf(stderr, "Erro a alocar memoria!");
                exit(1);
            }



        aux->tabuleiro = malloc(sizeof(Tabuleiro));
        if (aux->tabuleiro == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

        aux->jogadorJogar = atual->jogadorJogar;
        aux->modo = atual->modo;
        aux->numJogada = atual->numJogada;
        aux->tabuleiro->nLinhas = atual->tabuleiro->nLinhas;
        aux->tabuleiro->nColunas = atual->tabuleiro->nColunas;

        aux->tabuleiro->Tab = malloc(atual->tabuleiro->nLinhas * sizeof(char *));

        if (aux->tabuleiro->Tab == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

        //Alocar a memoria das ncolunas para cada linha
        for (int i = 0; i < atual->tabuleiro->nLinhas; i++) {
            aux->tabuleiro->Tab[i] = malloc(atual->tabuleiro->nColunas * sizeof(char));
            if (aux->tabuleiro->Tab[i] == NULL) {
                fprintf(stderr, "Erro a alocar memoria!");
                exit(1);
            }
        }

        //Escrever valores na tabela
        for (int i = 0; i < atual->tabuleiro->nLinhas; i++)
            for (int j = 0; j < atual->tabuleiro->nColunas; j++)
                aux->tabuleiro->Tab[i][j] = atual->tabuleiro->Tab[i][j];


       // aux->prox=NULL;
      aux->prox = lista;
      lista = aux;


    }


    return lista;
}

int contaJogadas(pInfoJogada lista){

    int cont=0;

    while (lista != NULL){
        cont++;
        lista = lista->prox;
    }

    return cont;
}

void gravaJogoTexto(pInfoJogada lista){

    int i,j;
    char nomeFichCompleto[50] = "JogosRealizados/";
    char nomeFich[30];

    printf("\nIntroduza o nome do ficheiro para guardar os dados do jogo: ");
    scanf("%s", nomeFich);
    strcat(nomeFich,".txt");
    strcat(nomeFichCompleto,nomeFich);

    FILE * f = fopen(nomeFichCompleto,"wt"); //  Abrir o ficheiro para escrita

    if (f == NULL){
        fprintf(stderr, "\n[Info] Erro ao abrir o ficheiro para escrita!\n");
        return;
    }

    fprintf(f,"------- Jogo Realizado -------\n");

    while (lista != NULL ){
        fprintf(f,"\n\n Jogada %d: \n\n", lista->numJogada);
        fprintf(f,"              ");
            for (i = 1; i <= lista->tabuleiro->nColunas; i++)
                fprintf(f,"%d  ", i);
        fprintf(f,"\n");
            for (i = 0; i < lista->tabuleiro->nLinhas; i++) {
                fprintf(f,"          %d   ", i + 1);
                for (j = 0; j < lista->tabuleiro->nColunas; j++) {
                    fprintf(f,"%c  ", lista->tabuleiro->Tab[i][j]);
                }
                fprintf(f,"\n");
            }
        lista = lista->prox;
    }

    printf("\n[Info] Informacao relativa ao jogo guardada com sucesso no ficheiro '%s'!\n", nomeFich);

    fclose(f);
}

void gravarJogo(pInfoJogada info, pInfoJogada lista, Jogador *jogadores){

    FILE * f = NULL;
    f = fopen(JOGOGUARDADO,"wb"); //  Abrir o ficheiro para escrita

    if (f == NULL){
        fprintf(stderr, "\n[Info] Erro a abrir o ficheiro para escrita!\n");
        return;
    }

    //Escrever dados relativos aos jogadores
    for(int i = 0; i < 2; i++)
        fwrite(&jogadores[i], sizeof(Jogador), 1, f);

    //Escrever dados relativo ao jogo
    fwrite(&info->jogadorJogar, sizeof(int),1, f);
    fwrite(&info->numJogada, sizeof(int),1, f);
    fwrite(&info->modo, sizeof(int),1, f);

    //Escrever dados relativos aos Tabuleiro atual

    fwrite(&info->tabuleiro->nColunas, sizeof (int), 1, f);
    fwrite(&info->tabuleiro->nLinhas, sizeof (int), 1, f);
    fwrite(&info->tabuleiro->Tab, sizeof (char **), 1, f);

    for (int i = 0; i < info->tabuleiro->nLinhas; i++)
        for (int j = 0; j < info->tabuleiro->nColunas; j++)
            fwrite(&info->tabuleiro->Tab[i][j], sizeof (char), 1, f);


    while (lista != NULL) {

        //Escrever dados relativo à lista ligada
        fwrite(&lista->numJogada, sizeof(int),1, f);
        fwrite(&lista->tabuleiro->nColunas, sizeof(int), 1, f);
        fwrite(&lista->tabuleiro->nLinhas, sizeof(int), 1, f);
        fwrite(&lista->prox, sizeof(Tabuleiro), 1, f);
        fwrite(&lista->tabuleiro->Tab, sizeof(char **), 1, f);


        for (int i = 0; i < lista->tabuleiro->nLinhas; i++)
            for (int j = 0; j < lista->tabuleiro->nColunas; j++)
                fwrite(&lista->tabuleiro->Tab[i][j], sizeof(char), 1, f);

        lista = lista->prox;
    }
    printf("\n[Info] Jogo guardado com sucesso!\n");

    fclose(f);
}

void carregarJogo(pInfoJogada info, pInfoJogada lista, Jogador *jogadores) {


    FILE *f = fopen(JOGOGUARDADO, "rb"); //  Abrir o ficheiro para leitura
    if (f == NULL) {
        printf("\nNao existem jogos guardados com o nome '%s'!\n", JOGOGUARDADO);
        exit(1);
    }

    //Ler dados relativos aos jogadores
    for (int i = 0; i < 2; i++)
        fread(&jogadores[i], sizeof(Jogador), 1, f);

    //Ler dados relativos ao jogo
    fread(&info->jogadorJogar, sizeof(int), 1, f);
    fread(&info->numJogada, sizeof(int), 1, f);
    fread(&info->modo, sizeof(int), 1, f);


    // Alocar memoria para o Tabuleiro
    info->tabuleiro = malloc(sizeof(Tabuleiro));
    if (info->tabuleiro == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //Ler dados relativos ao Tabuleiro
    fread(&info->tabuleiro->nColunas, sizeof(int), 1, f);
    fread(&info->tabuleiro->nLinhas, sizeof(int), 1, f);
    fread(&info->tabuleiro->Tab, sizeof(char **), 1, f);

    info->tabuleiro->Tab = malloc(info->tabuleiro->nLinhas * sizeof(char *));
    if (info->tabuleiro->Tab == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //para cada linha,  alocar a memoria para ncolunas
    for (int i = 0; i < info->tabuleiro->nLinhas; i++) {
        info->tabuleiro->Tab[i] = malloc(info->tabuleiro->nColunas * sizeof(char));
        if (info->tabuleiro->Tab[i] == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }
    }

    //lER VALORES DA TABELA
    for (int i = 0; i < info->tabuleiro->nLinhas; i++)
        for (int j = 0; j < info->tabuleiro->nColunas; j++)
            fread(&info->tabuleiro->Tab[i][j], sizeof(char), 1, f);


    // Lista ligada com as jogadas todas

    //Para a 1 jogada apenas
    lista->tabuleiro = malloc(sizeof(Tabuleiro));
    if (lista->tabuleiro == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    fread(&lista->numJogada, sizeof(int), 1, f);
    fread(&lista->tabuleiro->nColunas, sizeof(int), 1, f);
    fread(&lista->tabuleiro->nLinhas, sizeof(int), 1, f);
    fread(&lista->prox, sizeof(Tabuleiro), 1, f);
    fread(&lista->tabuleiro->Tab, sizeof(char **), 1, f);

    lista->tabuleiro->Tab = malloc(lista->tabuleiro->nLinhas * sizeof(char *));

    if (lista->tabuleiro->Tab == NULL) {
        fprintf(stderr, "Erro a alocar memoria!");
        exit(1);
    }

    //Alocar a memoria das ncolunas para cada linha
    for (int i = 0; i < lista->tabuleiro->nLinhas; i++) {
        lista->tabuleiro->Tab[i] = malloc(lista->tabuleiro->nColunas * sizeof(char));
        if (lista->tabuleiro->Tab[i] == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }
    }
    //Escrever valores na tabela
    for (int i = 0; i < lista->tabuleiro->nLinhas; i++)
        for (int j = 0; j < lista->tabuleiro->nColunas; j++)
            fread(&lista->tabuleiro->Tab[i][j], sizeof(char), 1, f);


    mostraTabuleiro(lista);

    if (lista->prox != NULL) {
        lista->prox = malloc(sizeof(InfoJogada));

        if (lista->prox == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

    }else{
        fclose(f);
        return;
    }

    //Para as restantes jogadas
    while (!feof(f)) {

        while (lista->prox != NULL)
            lista = lista->prox;

        lista->tabuleiro = malloc(sizeof(Tabuleiro));
        fread(&lista->numJogada, sizeof(int), 1, f);
        fread(&lista->tabuleiro->nColunas, sizeof(int), 1, f);
        fread(&lista->tabuleiro->nLinhas, sizeof(int), 1, f);
        fread(&lista->prox, sizeof(Tabuleiro), 1, f);
        fread(&lista->tabuleiro->Tab, sizeof(char **), 1, f);

        lista->tabuleiro->Tab = malloc(lista->tabuleiro->nLinhas * sizeof(char *));
        if (lista->tabuleiro->Tab == NULL) {
            fprintf(stderr, "Erro a alocar memoria!");
            exit(1);
        }

        //Alocar a memoria das ncolunas para cada linha
        for (int i = 0; i < lista->tabuleiro->nLinhas; i++) {
            lista->tabuleiro->Tab[i] = malloc(lista->tabuleiro->nColunas * sizeof(char));
            if (lista->tabuleiro->Tab[i] == NULL) {
                fprintf(stderr, "Erro a alocar memoria!");
                exit(1);
            }
        }

        //Ler valores
        for (int i = 0; i < lista->tabuleiro->nLinhas; i++)
            for (int j = 0; j < lista->tabuleiro->nColunas; j++)
                fread(&lista->tabuleiro->Tab[i][j], sizeof(char), 1, f);

        if (lista->prox == NULL)
            break;
        else {
            lista->prox = malloc(sizeof(InfoJogada));
            if (lista->prox == NULL) {
                fprintf(stderr, "Erro a alocar memoria!");
                exit(1);
            }
            lista = lista->prox;
        }
    }

    printf("\n[Info] Jogo carregado com sucesso!\n");
    fclose(f);

}

int verificarFicheiro(){

    FILE *f;
    if((f = fopen(JOGOGUARDADO,"rb")) != NULL) {
        fclose(f);
        return 1; // ficheiro existe
    }

    return 0;
}

int menuPrincipal(){
    int op;
    printf("\n----------- Jogo do Semaforo -----------\n\n");
    mostraTabuleiroVazio();

    printf(" 1. Iniciar Jogo\n"
           " 2. Regras do Jogo\n"
           " 3. Sair\n");
    printf("\nOpcao: ");
    scanf(" %d", &op);

    return op;
}

int modoJogo(){
    int opt;
    printf("\nIndique o modo de jogo que pretende jogar:\n\n");
    printf(" 1. Jogador A(Humano) VS Jogador B(Humano)\n"
           " 2. Jogador A(Humano) VS Computador\n");

    do{
        printf("\nOpcao: ");
        scanf("%d", &opt);

        if(opt > 2 || opt < 1)
            printf("[Info] Introduza uma opcao valida!\n");
    } while (opt > 2 || opt < 1);

    return opt;
}

int menuJogada(){

    int opt;
    printf("\n 1. Colocar peca\n"
           " 2. Colocar pedra\n"
           " 3. Adicionar linha/coluna\n"
           " 4. Visualizar 'K' jogadas anteriores\n"
           " 5. Guardar o jogo\n\n"
           " 0. Sair do jogo\n");

    printf("\nOpcao: ");
    scanf("%d", &opt);

    return opt;
}

void regrasJogo(){
    printf("\n------------- Regras do Jogo -------------\n\n");
    printf("O jogo do semaforo desenrola-se num tabuleiro dividido em celulas. No inicio, o tabuleiro \n"
           "esta vazio. Alternadamente, os jogadores A e B vao colocando pecas de cor Verde (G), Amarela \n"
           "(Y) ou Vermelha (R). Ganha o jogador que coloque uma peca que permita formar uma linha, \n"
           "coluna ou diagonal completa com pecas da mesma cor.\n\n");
    printf("Pressione qualquer tecla para voltar ao menu principal...\n");
    getchar(); //receber qualquer tecla para voltar atras
    getchar();
}

