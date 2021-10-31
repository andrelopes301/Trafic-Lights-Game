// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2020-2021
// André Lopes - 2019139754

#include "utils.h"
#include "jogo.h"


int main() {

    initRandom();
    int opcao;

    while(1){
        opcao = menuPrincipal();
        switch (opcao) {
            case 1: // Iniciar Jogo
                iniciarJogo();
                return 0;
            case 2: // Regras do Jogo
                regrasJogo();
                break;
            case 0: // Sair do Jogo
                printf("A terminar o jogo...\n");
                return 0;
            default:
                printf("[Info] Introduza uma opcao valida!\n");
                break;
        }
    }
}
