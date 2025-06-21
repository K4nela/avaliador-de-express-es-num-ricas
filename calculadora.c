#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX 512
#define PI 3.14159265358979323846

// Estrutura da pilha para números
typedef struct {
    float itens[MAX];
    int topo;
} PilhaFloat;

// Estrutura da pilha para operadores
typedef struct {
    char itens[MAX];
    int topo;
} PilhaChar;

// Inicializa pilha de floats
void initFloat(PilhaFloat *p) {
    p->topo = -1;
}

// Verifica se pilha de floats esta vazia
int isEmptyFloat(PilhaFloat *p) {
    return p->topo == -1;
}

// Empilha float
void pushFloat(PilhaFloat *p, float valor) {
    if (p->topo < MAX-1) {
        p->itens[++p->topo] = valor;
    }
}

// Desempilha float
float popFloat(PilhaFloat *p) {
    if (!isEmptyFloat(p)) {
        return p->itens[p->topo--];
    }
    return 0;
}

// Inicializa pilha de chars
void initChar(PilhaChar *p) {
    p->topo = -1;
}

// Verifica se pilha de chars esta vazia
int isEmptyChar(PilhaChar *p) {
    return p->topo == -1;
}

// Empilha char
void pushChar(PilhaChar *p, char valor) {
    if (p->topo < MAX-1) {
        p->itens[++p->topo] = valor;
    }
}

// Desempilha char
char popChar(PilhaChar *p) {
    if (!isEmptyChar(p)) {
        return p->itens[p->topo--];
    }
    return '\0';
}

// Verifica precedencia de operadores
int precedencia(char op) {
    if (op == '^') return 4;
    if (op == '*' || op == '/' || op == '%') return 3;
    if (op == '+' || op == '-') return 2;
    return 0;
}

// Verifica se e operador
int isOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Verifica se e funcao especial
int isFuncaoEspecial(const char *str) {
    return strcmp(str, "raiz") == 0 || strcmp(str, "sen") == 0 || 
           strcmp(str, "cos") == 0 || strcmp(str, "tg") == 0 || 
           strcmp(str, "log") == 0;
}

// Converte graus para radianos
float grausParaRadianos(float graus) {
    return graus * PI / 180.0;
}

// Valida se a expressão é simples (pode ser expandida)
int expressaoValida(const char *expr) {
    if (!expr || strlen(expr) == 0) return 0;
    return 1;
}

// Converte expressão infixa para pós-fixa
char *getFormaPosFixa(char *infixa) {
    static char posfixa[MAX];
    if (!expressaoValida(infixa)) {
        printf("Erro: Expressao infixa vazia ou invalida!\n");
        posfixa[0] = '\0';
        return posfixa;
    }
    PilhaChar operadores;
    initChar(&operadores);
    posfixa[0] = '\0';
    
    int i = 0, j = 0;
    int erro = 0;
    while (infixa[i] != '\0') {
        if (infixa[i] == ' ') {
            i++;
            continue;
        }
        if (isdigit(infixa[i]) || infixa[i] == '.') {
            while (isdigit(infixa[i]) || infixa[i] == '.') {
                posfixa[j++] = infixa[i++];
            }
            posfixa[j++] = ' ';
            continue;
        }
        if (isalpha(infixa[i])) {
            char func[5] = {0};
            int k = 0;
            while (isalpha(infixa[i])) {
                func[k++] = infixa[i++];
            }
            func[k] = '\0';
            
            if (isFuncaoEspecial(func)) {
                pushChar(&operadores, '#');
                if (infixa[i] != '(') {
                    printf("Erro: Esperado '(' apos funcao especial!\n");
                    erro = 1;
                    break;
                }
                i++;
                while (infixa[i] != ')' && infixa[i] != '\0') {
                    posfixa[j++] = infixa[i++];
                }
                if (infixa[i] != ')') {
                    printf("Erro: Parentese de fechamento ')' ausente na funcao especial!\n");
                    erro = 1;
                    break;
                }
                posfixa[j++] = ' ';
                posfixa[j++] = func[0];
                if (strlen(func) > 1) {
                    posfixa[j++] = func[1];
                    if (strlen(func) > 2) {
                        posfixa[j++] = func[2];
                    }
                }
                posfixa[j++] = ' ';
                i++;
                continue;
            } else {
                printf("Erro: Funcao especial desconhecida!\n");
                erro = 1;
                break;
            }
        }
        if (infixa[i] == '(') {
            pushChar(&operadores, infixa[i]);
        } else if (infixa[i] == ')') {
            int encontrou = 0;
            while (!isEmptyChar(&operadores) && operadores.itens[operadores.topo] != '(') {
                posfixa[j++] = popChar(&operadores);
                posfixa[j++] = ' ';
            }
            if (!isEmptyChar(&operadores) && operadores.itens[operadores.topo] == '(') {
                popChar(&operadores);
                encontrou = 1;
            }
            if (!encontrou) {
                printf("Erro: Parentese de fechamento ')' sem correspondente de abertura!\n");
                erro = 1;
                break;
            }
        } 
        else if (isOperador(infixa[i])) {
            while (!isEmptyChar(&operadores) && 
                   precedencia(operadores.itens[operadores.topo]) >= precedencia(infixa[i])) {
                posfixa[j++] = popChar(&operadores);
                posfixa[j++] = ' ';
            }
            pushChar(&operadores, infixa[i]);
        }
        i++;
    }
    while (!isEmptyChar(&operadores)) {
        if (operadores.itens[operadores.topo] == '(') {
            printf("Erro: Parentese de abertura '(' sem correspondente de fechamento!\n");
            erro = 1;
            break;
        }
        posfixa[j++] = popChar(&operadores);
        posfixa[j++] = ' ';
    }
    posfixa[j] = '\0';
    if (erro) posfixa[0] = '\0';
    return posfixa;
}

// Converte expressão pós-fixa para infixa
char *getFormaInFixa(char *posfixa) {
    static char infixa[MAX];
    if (!expressaoValida(posfixa)) {
        printf("Erro: Expressao posfixa vazia ou invalida!\n");
        infixa[0] = '\0';
        return infixa;
    }
    char *tokens[MAX];
    int tokenCount = 0;
    char buffer[MAX];
    char *exprStack[MAX];
    int exprTop = -1;

    strcpy(buffer, posfixa);
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, " ");
    }

    int erro = 0;
    for (int i = 0; i < tokenCount; i++) {
        token = tokens[i];
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            exprStack[++exprTop] = strdup(token);
        } else if (isOperador(token[0]) && strlen(token) == 1) {
            if (exprTop < 1) {
                printf("Erro: Operador '%c' sem operandos suficientes!\n", token[0]);
                erro = 1;
                break;
            }
            char *b = exprStack[exprTop--];
            char *a = exprStack[exprTop--];
            char *expr = malloc(strlen(a) + strlen(b) + 5);
            sprintf(expr, "(%s%c%s)", a, token[0], b);
            free(a);
            free(b);
            exprStack[++exprTop] = expr;
        } else if (isFuncaoEspecial(token)) {
            if (exprTop < 0) {
                printf("Erro: Funcao especial '%s' sem operando!\n", token);
                erro = 1;
                break;
            }
            char *a = exprStack[exprTop--];
            char *expr = malloc(strlen(a) + strlen(token) + 4);
            sprintf(expr, "%s(%s)", token, a);
            free(a);
            exprStack[++exprTop] = expr;
        } else {
            printf("Erro: Token desconhecido na expressao posfixa: '%s'\n", token);
            erro = 1;
            break;
        }
    }

    if (erro || exprTop != 0) {
        printf("Erro: Expressao posfixa mal formada!\n");
        infixa[0] = '\0';
        while (exprTop >= 0) free(exprStack[exprTop--]);
    } else {
        strncpy(infixa, exprStack[0], MAX-1);
        infixa[MAX-1] = '\0';
        free(exprStack[0]);
    }
    return infixa;
}

// Calcula valor de expressão pós-fixa
float getValorPosFixa(char *posfixa) {
    if (!expressaoValida(posfixa)) {
        printf("Erro: Expressao posfixa vazia ou invalida!\n");
        return NAN;
    }
    PilhaFloat pilha;
    initFloat(&pilha);

    char buffer[MAX];
    strncpy(buffer, posfixa, MAX-1);
    buffer[MAX-1] = '\0';

    char *token = strtok(buffer, " ");
    int erro = 0;
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushFloat(&pilha, atof(token));
        } 
        else if (isOperador(token[0])) {
            if (pilha.topo < 1) {
                printf("Erro: Operador '%c' sem operandos suficientes!\n", token[0]);
                erro = 1;
                break;
            }
            float b = popFloat(&pilha);
            float a = popFloat(&pilha);
            float resultado;
            switch (token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': 
                    if (b == 0) {
                        printf("Erro: Divisao por zero!\n");
                        erro = 1;
                        break;
                    }
                    resultado = a / b; 
                    break;
                case '%': resultado = fmod(a, b); break;
                case '^': resultado = pow(a, b); break;
                default: resultado = 0;
            }
            if (erro) break;
            pushFloat(&pilha, resultado);
        }
        else if (isFuncaoEspecial(token)) {
            if (pilha.topo < 0) {
                printf("Erro: Funcao especial '%s' sem operando!\n", token);
                erro = 1;
                break;
            }
            float a = popFloat(&pilha);
            float resultado;
            if (strcmp(token, "raiz") == 0) {
                if (a < 0) {
                    printf("Erro: Raiz de numero negativo!\n");
                    erro = 1;
                    break;
                }
                resultado = sqrt(a);
            }
            else if (strcmp(token, "sen") == 0) resultado = sin(grausParaRadianos(a));
            else if (strcmp(token, "cos") == 0) resultado = cos(grausParaRadianos(a));
            else if (strcmp(token, "tg") == 0) resultado = tan(grausParaRadianos(a));
            else if (strcmp(token, "log") == 0) {
                if (a <= 0) {
                    printf("Erro: Logaritmo de numero nao positivo!\n");
                    erro = 1;
                    break;
                }
                resultado = log10(a);
            }
            else resultado = 0;
            if (erro) break;
            pushFloat(&pilha, resultado);
        } else {
            printf("Erro: Token desconhecido na expressao posfixa: '%s'\n", token);
            erro = 1;
            break;
        }
        token = strtok(NULL, " ");
    }

    if (erro || pilha.topo != 0) {
        printf("Erro: Expressao posfixa mal formada!\n");
        return NAN;
    }
    return popFloat(&pilha);
}

// Calcula valor de expressão infixa
float getValorInFixa(char *infixa) {
    if (!expressaoValida(infixa)) {
        printf("Erro: Expressao infixa vazia ou invalida!\n");
        return NAN;
    }
    char *posfixa = getFormaPosFixa(infixa);
    if (!posfixa || strlen(posfixa) == 0) {
        printf("Erro: Falha ao converter infixa para posfixa!\n");
        return NAN;
    }
    return getValorPosFixa(posfixa);
}