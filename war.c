// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================

// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_NOME 30
#define MAX_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de lógica principal do jogo:

    //Simulação de ataques
    void atacar(Territorio *atacante, int quantidade, int *jogando);
    void recebeAtacanteDefensor(int *idAtacante, int *idDefensor, int quantidade, int *jogando);

// Funções de setup e gerenciamento de memória:
    void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:

    // Exibe o mapa do mundo
    void exibeMapa(const Territorio *territorios, int quantidade);
    void pressioneEnter();

    //Cadastro inicial
    void cadastroInicial(Territorio *cadastro, int quantidade);

// Função utilitária:

    // Limpa o buffer do teclado (Remove o '\n')
    void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.

int main() {
    // Utilizando acentuação com locale
    setlocale(LC_ALL, "");

    // Criando números aleatórios com rand
    srand(time(NULL));

    //Declarando variáveis
    int quantidadeTerritorios;
    Territorio *territorios;
    int jogando = 1; // Controla o estado do jogo; Se valor for 0, encerra o programa. 

    // Alocando dinamicamente a memória para o vetor de territórios usando calloc.
    printf("Bem-vindo ao WAR! \nCom quantos territórios deseja jogar? ");
    scanf("%d", &quantidadeTerritorios);    
    territorios = (Territorio *) calloc(quantidadeTerritorios, sizeof(Territorio)); // Vetor de structs

    //verificando erro
    if(territorios == NULL) {
        printf("Erro! \nComputador sem memória.\n");
        return 1;
    }

    // limpando buffer do teclado
    limparBufferEntrada();

    // Preenchendo dados
    cadastroInicial(territorios, quantidadeTerritorios);

    do {
        // Exibindo dados    
        exibeMapa(territorios, quantidadeTerritorios);
    
        // Simulando ataque
        atacar(territorios, quantidadeTerritorios, &jogando);
    } while (jogando == 1);


    //Encerrando o programa
    liberarMemoria(territorios);   

    return 0;
}// fim da função main

// --- Implementação das Funções ---

// limparBufferEntrada:
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//===================================
// Funções de interface com o usuário 
//===================================
// Exibe o mapa do mundo
void exibeMapa(const Territorio *territorios, int quantidade) {

    printf("\n======== MAPA DO MUNDO - ESTADO ATUAL ========\n");

    for(int i = 0; i < quantidade; i ++) {

        printf("\nTERRITÓRIO %d:\n", i+1);

        printf("\t- Nome: %s\n", territorios[i].nome);

        printf("\t- Dominado por: Exército %s\n", territorios[i].cor);

        printf("\t- Tropas: %d\n", territorios[i].tropas

        );

    }

};
//Cadastro inicial - cadastra nome, cor do exército e quantidade de tropas para cada território de acordo com a quantidade de territórios informada pelo jogador

void cadastroInicial(Territorio *cadastro, int quantidade) {
    printf("\n======== Cadastrando Territórios ========\n");
    printf("Vamos cadastrar os %d territórios iniciais do nosso mundo\n", quantidade);
    
    for (int i = 0; i < quantidade; i ++) {        
        printf("\n======== Cadastrando %d° Território ========\n", i + 1);
        
        // Cadastrando nome
        printf("\nNome do território: ");
        // Lendo string e removendo '\n'
        fgets(cadastro[i].nome, sizeof(cadastro[i].nome), stdin);
        cadastro[i].nome[strcspn(cadastro[i].nome, "\n")] = '\0';
        
        // Cadastrando cor
        printf("\nCor do território: ");

        // Lendo string e removendo '\n'
        fgets(cadastro[i].cor, sizeof(cadastro[i].cor), stdin);
        cadastro[i].cor[strcspn(cadastro[i].cor, "\n")] = '\0';         
        
        // Cadastrando tropas
        printf("\nNúmero de tropas: ");
        // Lendo string e removendo '\n'
        scanf("%d", &cadastro[i].tropas);    
        limparBufferEntrada();
    }
    printf("\nCadastro inicial concluído com sucesso!\n");   
};
// Pressione Enter para continuar
void pressioneEnter() {
    printf("\nPressione Enter para continuar\n");
    getchar();
}; 

//==============================================
// Fim das funções de interface com o usuário
//==============================================

//=====================================
// Funções de lógica principal do jogo
//=====================================

//Simulação de ataques
void atacar(Territorio *territorios, int quantidade, int *jogando) {
    int idAtacante, idDefensor;
    Territorio *atacante;
    Territorio *defensor;

    printf("\n======== Fase de Ataque ========\n");
    
    // Recebendo a escolha do jogador para o território atacante e o território defensor
    // Validando o número de território escolhido
    while (1) {
        recebeAtacanteDefensor(&idAtacante,&idDefensor, quantidade, jogando);

        // Verificando se o jogador escolheu encerrar o programa (*jogando == 0)
        if (*jogando == 0) {
            return;// Encerrando o ataque.
        }

        // se escolha do jogador for inválida, cancela o ataque e recomeça o loop
        if (idAtacante < 1 || idAtacante > quantidade || 
            idDefensor < 1 || idDefensor > quantidade) 
        {
            printf("Ataque cancelado: Território inválido escolhido.\n");
            pressioneEnter();
            continue;
        } else if (idAtacante == idDefensor) {
            printf("Ataque cancelado: Um território não pode atacar a si mesmo.\n");
            pressioneEnter();
            continue;
        } 

        //Convertendo a escolha do jogador em um ídice do vetor de territórios
        atacante = &territorios[idAtacante - 1];
        defensor = &territorios[idDefensor - 1];

        // Impedindo ataques dentre territórios do mesmo exército
        // Se forem da mesma cor, reinicia o loop
        if (strcmp(atacante -> cor, defensor -> cor) == 0) {
            printf("Ataque cancelado: Você não pode atacar um território do seu próprio exército.\n");
            pressioneEnter();
            continue;
        }
        if (atacante -> tropas <= 0) {
            printf("Ataque cancelado: Tropas insuficientes.\n");
            pressioneEnter();
            continue;
        }
        break;
    }

    // Iniciando combate
    printf("\n%s (Exército %s) ataca %s (Exército %s)\n", atacante -> nome, atacante -> cor, defensor -> nome, defensor -> cor);

    // Dados
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- Rolagem de dados ---\n");
    printf("O atacante tirou: %d\n", dadoAtacante);
    printf("O defensor tirou: %d\n", dadoDefensor);
    
    //Comparando dados
    printf("\n--- Resultado da Batalha ---\n");
    if (dadoAtacante > dadoDefensor) {
        printf("Vitória do atacante! \nDefensor perde 1 tropa\n");
        defensor -> tropas --;
    } else {
        printf("Vitória do defensor! \nAtacante perde 1 tropa\n");
        atacante -> tropas --;
    }
    
    // Verificando conquista
    if (defensor -> tropas == 0) {
        printf("\nTerritório conquistado!\n");
        printf("O Exército %s conquistou %s\n", atacante -> cor, defensor -> nome);
        strcpy(defensor -> cor, atacante -> cor); // Atualizando cor
    }
    pressioneEnter();
};// Fim da função atacar

// Recebendo O atacante e o defensor
void recebeAtacanteDefensor(int *idAtacante, int *idDefensor, int quantidade, int *jogando) {

    printf("Digite o número do território atacante (1 a %d), ou 0 para sair\n", quantidade);
    scanf("%d", idAtacante);
    limparBufferEntrada();

    // Se o jogador escolher encerrar o programa
    // Ao digitar zero, atualiza a variável jogando para 0 e encerra esta função
    if (*idAtacante == 0) {
        *jogando = 0;
        return;
    }

    printf("Digite o número do território defensor (1 a %d)\n", quantidade);
    scanf("%d", idDefensor);
    limparBufferEntrada();
};


//==============================================
//Fim das funções de lógica principal do jogo
//==============================================

//==============================================
// Funções de setup e gerenciamento de memória
//==============================================
// Liberando memória

void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nJogo encerrado.\n Até a próxima!\n");
};

//====================================================
// Fim das funções de setup e gerenciamento de memória
//====================================================