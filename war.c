// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_NOME 30
#define MAX_COR 10
#define QUANTIDADE_TERRITORIOS 5

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:

// Função utilitária:
// Limpa o buffer do teclado (Remove o '\n')
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Criando vetor que guarda as structs do tipo territorio
    Territorio territorios[QUANTIDADE_TERRITORIOS];

    // Preenchendo dados
    printf("\n======== Cadastrando Territórios ========\n");
    printf("Vamos cadastrar os %d territórios iniciais do nosso mundo\n", QUANTIDADE_TERRITORIOS);

    for (int i = 0; i < QUANTIDADE_TERRITORIOS; i ++) {        
        printf("\n======== Cadastrando %d° Território ========\n", i + 1);
        // Cadastrando nome
        printf("\nNome do território: ");
        // Lendo string e removendo '\n'
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        // Cadastrando cor
        printf("\nCor do território: ");
        // Lendo string e removendo '\n'
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        // Cadastrando tropas
        printf("\nNúmero de tropas: ");
        // Lendo string e removendo '\n'
        scanf("%d", &territorios[i].tropas);
        limparBufferEntrada();
    }// Fim do loop de cadastro

    // Exibindo dados
    printf("\nCadastro inicial concluído com sucesso!\n");
    printf("\n======== MAPA DO MUNDO - ESTADO ATUAL ========\n");
    for(int i = 0; i < QUANTIDADE_TERRITORIOS; i ++) {
        printf("\nTERRITÓRIO %d:\n", i+1);
        printf("\t- Nome: %s\n", territorios[i].nome);
        printf("\t- Dominado por: Exército %s\n", territorios[i].cor);
        printf("\t- Tropas: %d\n", territorios[i].tropas
        );
    }
    
    return 0;
}
// --- Implementação das Funções ---

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
