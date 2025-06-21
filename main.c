#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <locale.h>
#include "expressao.h"

// Limpa o buffer do teclado após leitura de dados
void limparBuffer() {
    while (getchar() != '\n');
}

// Exibe o menu principal do programa
void exibirMenu() {
    printf("\n=== Avaliador de Expressoes Numericas ===\n");
    printf("1. Converter Infixa para Pos-fixa\n");
    printf("2. Converter Pos-fixa para Infixa\n");
    printf("3. Calcular valor de expressao Infixa\n");
    printf("4. Calcular valor de expressao Pos-fixa\n");
    printf("0. Sair\n");
    printf("=======================================\n");
    printf("Escolha uma opcao: ");
}

int main() {    
    setlocale(LC_ALL, "");
    int opcao;
    char expressao[512];
    Expressao exp;

    // Loop principal do menu
    do {
        exibirMenu();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: {
                // Converte expressão infixa para pós-fixa
                printf("Digite a expressao infixa: ");
                if (!fgets(expressao, sizeof(expressao), stdin)) break;
                expressao[strcspn(expressao, "\n")] = '\0';

                strcpy(exp.infixa, expressao);
                char *posfixa = getFormaPosFixa(exp.infixa);
                if (posfixa && strlen(posfixa) > 0)
                    printf("Expressao pos-fixa: %s\n", posfixa);
                break;
            }
            case 2: {
                // Converte expressão pós-fixa para infixa
                printf("Digite a expressao pos-fixa: ");
                if (!fgets(expressao, sizeof(expressao), stdin)) break;
                expressao[strcspn(expressao, "\n")] = '\0';

                strcpy(exp.posFixa, expressao);
                char *infixa = getFormaInFixa(exp.posFixa);
                if (infixa && strlen(infixa) > 0)
                    printf("Expressao infixa: %s\n", infixa);
                break;
            }
            case 3: {
                // Calcula o valor de uma expressão infixa
                printf("Digite a expressao infixa: ");
                if (!fgets(expressao, sizeof(expressao), stdin)) break;
                expressao[strcspn(expressao, "\n")] = '\0';

                exp.Valor = getValorInFixa(expressao);
                if (!isnan(exp.Valor))
                    printf("Resultado: %.2f\n", exp.Valor);
                break;
            }
            case 4: {
                // Calcula o valor de uma expressão pós-fixa
                printf("Digite a expressao pos-fixa: ");
                if (!fgets(expressao, sizeof(expressao), stdin)) break;
                expressao[strcspn(expressao, "\n")] = '\0';

                exp.Valor = getValorPosFixa(expressao);
                if (!isnan(exp.Valor))
                    printf("Resultado: %.2f\n", exp.Valor);
                break;
            }
            case 0:
                // Sai do programa
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}