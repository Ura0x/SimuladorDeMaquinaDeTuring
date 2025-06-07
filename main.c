#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS 100
#define MAX_COLUNAS 100
#define MAX_CARACTERES 500

void mostrarFuncao(char matriz[MAX_LINHAS][MAX_COLUNAS][MAX_CARACTERES], int linhaAtual, int linha, int coluna) {
    int colunas = 0;
    while (colunas < MAX_COLUNAS && matriz[0][colunas][0] != '\0') {
        colunas++;
    }

    printf("Função de transição:\n");
    for (int i = 0; i < linhaAtual; i++) {
        for (int j = 0; j < MAX_COLUNAS && matriz[i][j][0] != '\0'; j++) {
            printf("| ");
            if (i == linha && j == coluna) printf("\033[41m");
            printf("%-10s", matriz[i][j]);
            printf("\033[0m");
        }

        printf("\n");
        for (int k = 0; k < colunas * 12; k++) {
            printf("-");
        }
        printf("\n");
    }
}

void computar(char matriz[MAX_LINHAS][MAX_COLUNAS][MAX_CARACTERES], char fita[MAX_CARACTERES], int tamanho) {
    char estadoAtual[4] = "q0";
    int agulha = 0;
    char comando = 'R';
    char opcao[5] = "-1";

    while (comando != 'P') {
        int change = 0;
        int linha = 1;
        while (linha < MAX_LINHAS && strcmp(matriz[linha][0], estadoAtual) != 0) linha++;
        if (linha >= MAX_LINHAS || matriz[linha][0][0] == '\0') {
            printf("Erro: estado %s não encontrado\n", estadoAtual);
            break;
        }

        int coluna = 1;
        while (coluna < MAX_COLUNAS && matriz[0][coluna][0] != '\0' && matriz[0][coluna][0] != fita[agulha]) coluna++;
        if (coluna >= MAX_COLUNAS || matriz[0][coluna][0] == '\0') {
            printf("Erro: símbolo '%c' não encontrado na fita\n", fita[agulha]);
            break;
        }

        system("reset");

        if (strcmp(opcao, "1")) {

           
            mostrarFuncao(matriz, tamanho, linha, coluna);
        }

        
        char transicao[10];
        strcpy(transicao, matriz[linha][coluna]);
        char *novoEstado = strtok(transicao, " ");
        char *escrever = strtok(NULL, " ");
        char *direcao = strtok(NULL, " ");

        if (!novoEstado || !escrever || !direcao) {
            printf("Erro: transição malformada no estado %s lendo %c\n", estadoAtual, fita[agulha]);
            break;
        }

       
        if(strcmp(opcao, "1")) {
            
            printf("\n");
            for (int i = 0; i < MAX_CARACTERES;i++){
                if(agulha == i) {
                    printf("\033[41m");
                }
                printf("%c",fita[i]);
                printf("\033[0m");
                
                if(fita[i] == 'b') break;
            }
            printf("bbbbbb...\n");
        }


        strcpy(estadoAtual, novoEstado);
        if (fita[agulha] != escrever[0]) change = 1;
        fita[agulha] = escrever[0];

        if (direcao[0] == 'R') {
                agulha++;
            } else if (direcao[0] == 'L') {
                agulha--;
            } else if (direcao[0] == 'P') {
                comando = 'P';
            } else {
                printf("Erro: direção inválida '%s'\n", direcao);
                break;
            }




        if (strcmp(opcao, "0") && strcmp(opcao, "1")) {
            printf("Aperte ENTER para avançar um passo, 0 para avançar tudo passo a passo, ou 1 para ir direto para o final\n");

            if (fgets(opcao, sizeof(opcao), stdin)) {
                opcao[strcspn(opcao, "\n")] = 0;
            }
        }

        if (strcmp(opcao, "0") == 0) {
            system("sleep .2");
        }
        
    }

    if(strcmp(opcao, "1") == 0 && comando == 'P') {
        mostrarFuncao(matriz, tamanho, -1, -1);
    }

    printf("\nFita final: ");
    int i;
    for (i = 0; i < MAX_CARACTERES & fita[i] != 'b';i++){
        printf("%c",fita[i]);
    }
    printf("bbbbbb...\n");
    printf("\nResposta em base decimal: %d\n\n",i-1);
}

int main() {
    char nomeArquivo[50];
    char nomeArquivoFita[50];

    printf("Nenhuma função de transição carregada, por favor insira uma.\n");

    while(1){

        printf("Digite o nome do arquivo da função de transição (sem a extensão): ");
        scanf("%s", nomeArquivo);
    
        FILE *arquivo = fopen(strcat(nomeArquivo, ".csv"), "r");
        if (!arquivo) {
            perror("Erro ao abrir o arquivo CSV");
            return 1;
        }
    
        char matriz[MAX_LINHAS][MAX_COLUNAS][MAX_CARACTERES] = {{{0}}};
        char linha[1024];
        int linhaAtual = 0;
    
        while (fgets(linha, sizeof(linha), arquivo) && linhaAtual < MAX_LINHAS) {
            linha[strcspn(linha, "\n")] = 0;
    
            int colunaAtual = 0;
            char *token = strtok(linha, ",");
            while (token && colunaAtual < MAX_COLUNAS) {
                strncpy(matriz[linhaAtual][colunaAtual], token, MAX_CARACTERES - 1);
                matriz[linhaAtual][colunaAtual][MAX_CARACTERES - 1] = '\0';
                token = strtok(NULL, ",");
                colunaAtual++;
            }
            linhaAtual++;
        }
        fclose(arquivo);

        system("reset");
    
        mostrarFuncao(matriz, linhaAtual,-1,-1);
    
        while(1) {
            int opcao = 0;
            printf("1 - Escolher um arquivo de fita\n2 - Digitar a fita\n3 - Mostrar a função de transição\n4 - Escolher um novo arquivo de função de transição\n");
            scanf("%d", &opcao);
        
            char fita[MAX_CARACTERES];
        
            if (opcao == 1) {
        
                printf("Digite o nome do arquivo da fita (sem a extensão): ");
                scanf("%s", nomeArquivoFita);
                
                arquivo = fopen(strcat(nomeArquivoFita, ".txt"), "r");
                if (!arquivo) {
                    perror("Erro ao abrir o arquivo da fita");
                    return 1;
                }
                
                if (fgets(linha, sizeof(linha), arquivo)) {
                    linha[strcspn(linha, "\n")] = 0;
                    
                    int tamanho = strlen(linha);
                    for (int i = 0; i < MAX_CARACTERES; i++) {
                        if (i < tamanho) {
                            fita[i] = linha[i];
                        } else {
                            fita[i] = 'b';
                        }
                    }
                    fita[MAX_CARACTERES - 1] = '\0'; 
                }
        
                fclose(arquivo);
        
            } else if(opcao == 2){
                printf("Digite a fita: ");
                while ((getchar()) != '\n'); // limpa o buffer do teclado
                fgets(linha, sizeof(linha), stdin);
                linha[strcspn(linha, "\n")] = 0;  // Remove o '\n'
        
                int tamanho = strlen(linha);
                for (int i = 0; i < MAX_CARACTERES; i++) {
                    if (i < tamanho)
                        fita[i] = linha[i];
                    else
                        fita[i] = 'b';
                }
                fita[MAX_CARACTERES - 1] = '\0';
        
            }else if(opcao == 3) {
                system("reset");

                mostrarFuncao(matriz, linhaAtual,-1,-1);
                continue;

            } else if(opcao == 4) {
                break;
            } else {
                printf("Opção inválida\n");
                continue;
            }

            computar(matriz, fita, linhaAtual);

            printf("Aperte ENTER para continuar.\n");

            while ((getchar()) != '\n'); // limpa o buffer do teclado

            scanf("");

        }
    }
    return 0;
}
