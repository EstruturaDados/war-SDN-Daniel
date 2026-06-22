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

    // Simulação de ataques
    void atacar(Territorio *atacante, int quantidade, int *jogando, int idBaseJogador, char *missao);
    void recebeAtacanteDefensor(int *idAtacante, int *idDefensor, int quantidade, int *jogando, int idBaseJogador);

    // Sorteios
    int definirTerritorioJogador(Territorio *mapa, int quantidade, char *corJogador);
    void atribuirMissao(char *destino, const char *missoes[], int totalMissoes, char *corJogador);
    
    // Validar missão
    int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador);

// Funções de setup e gerenciamento de memória:
    void liberarMemoria(Territorio *mapa, char *missao);

// Funções de interface com o usuário:

    // Exibe o mapa do mundo
    void exibeMapa(const Territorio *territorios, int quantidade);
    void pressioneEnter();

    //Cadastro inicial
    void inicializarTerritorios(Territorio *cadastro, int quantidade);

// Função utilitária:

    // Limpa o buffer do teclado (Remove o '\n')
    void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.

int main() {    
    setlocale(LC_ALL, ""); // Utilizando acentuação com locale    
    srand(time(NULL)); // Criando números aleatórios com rand

    //Declarando variáveis
    Territorio *territorios;
    int quantidadeTerritorios;
    int jogando = 1; // Controla o estado do jogo; Se valor for 0, encerra o programa. 
    char *missao; // Recebe a missão
    char corJogador[MAX_COR];
    quantidadeTerritorios = 5;
    int idBaseJogador; // Guarda o número do território sorteado para o jogador (de 1 a 5, não pelo índex)
    
    // Vetor de missões
    const char *missoes[5] = {
        "Destruir totalmente os exércitos da cor Verde.\n",
        "Destruir totalmente os exércitos da cor Azul.\n",
        "Conquistar 2 territórios.\n",
        "Conquistar 1 território.\n",
        "Destruir totalmente os exércitos da cor Amarelo.\n"
    };

    printf("\nBem-vindo ao WAR!\nCarregando mapa...");   
    
    // Alocando dinamicamente a memória para o vetor de territórios usando calloc.
    territorios = (Territorio *) calloc(quantidadeTerritorios, sizeof(Territorio)); // Vetor de structs
    if(territorios == NULL) { //verificando erro
        printf("Erro! \nComputador sem memória.\n");
        return 1;
    }

    // Alocando dinamicamente a memória para o vetor de territórios usando calloc.
    missao = (char *) malloc(150 * sizeof(char));
    if (missao == NULL) { //verificando erro
        printf("Erro! \nComputador sem memória.\n");
        return 1;
    }

    // Preenchendo dados
    inicializarTerritorios(territorios, quantidadeTerritorios);
    exibeMapa(territorios, quantidadeTerritorios);

    // Definindo território e exército do jogador
    // idBaseJogador Guarda o número do território sorteado para o jogador (de 1 a 5, não pelo índex)
    idBaseJogador = definirTerritorioJogador(territorios, quantidadeTerritorios, corJogador);
    
    atribuirMissao(missao, missoes, 5, corJogador);

    do {
        // Simulando ataque
        atacar(territorios, quantidadeTerritorios, &jogando, idBaseJogador, missao);
        
        // Exibindo dados    
        exibeMapa(territorios, quantidadeTerritorios);

        // Verificando cumprimento da missão
        if (jogando == 1) {
            if (verificarMissao(missao, territorios, quantidadeTerritorios, corJogador) == 1) 
            {
                printf("\n\n VITORIA! VOCE CONCLUIU SUA MISSAO: \n%s\n\n", missao);
                jogando = 0; 
            }            
        }

    } while (jogando == 1);

    //Encerrando o programa
    liberarMemoria(territorios, missao);   

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
    printf("\n\n======== MAPA DO MUNDO - ESTADO ATUAL ========\n");

    for(int i = 0; i < quantidade; i ++) {
        printf("\nTERRITÓRIO %d:\n", i+1);
        printf("\t- Nome: %s\n", territorios[i].nome);
        printf("\t- Dominado por: Exército %s\n", territorios[i].cor);
        printf("\t- Tropas: %d\n", territorios[i].tropas);
    }

};
//Cadastro inicial - cadastra nome, cor do exército e quantidade de tropas para cada território de acordo com a quantidade de territórios informada pelo jogador

void inicializarTerritorios(Territorio *cadastro, int quantidade) {

    // 0- America
    strcpy(cadastro[0].nome, "America");
    strcpy(cadastro[0].cor, "Verde");
    cadastro[0].tropas = 5; 
    
    // 1- Europa
    strcpy(cadastro[1].nome, "Europa");
    strcpy(cadastro[1].cor, "Azul");
    cadastro[1].tropas = 3;
    
    // 2- Asia
    strcpy(cadastro[2].nome, "Asia");
    strcpy(cadastro[2].cor, "Vermelho");
    cadastro[2].tropas = 2;

    // 3- Africa
    strcpy(cadastro[3].nome, "Africa");
    strcpy(cadastro[3].cor, "Amarelo");
    cadastro[3].tropas = 4;

    // 4- Oceania
    strcpy(cadastro[4].nome, "Oceania");
    strcpy(cadastro[4].cor, "Branco");
    cadastro[4].tropas = 1;
};
// Pressione Enter para continuar
void pressioneEnter() {
    printf("\nPressione Enter para continuar\n");
    getchar();
}; 

//==============================================
// Fim das funções de interface com o usuário
//==============================================

//==============================================
// lógica principal do jogo - Funções de ataque
//==============================================
void atacar(Territorio *territorios, int quantidade, int *jogando, int idBaseJogador, char *missao) {
    int idAtacante, idDefensor;
    Territorio *atacante;
    Territorio *defensor;

    printf("\n======== Fase de Ataque ========\n");
    printf("Sua missao: %s\n", missao);
    
    // Recebendo a escolha do jogador para o território atacante e o território defensor
    // Validando o número de território escolhido
    while (1) {
        recebeAtacanteDefensor(&idAtacante,&idDefensor, quantidade, jogando, idBaseJogador);

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
// Função usada na função atacar()
void recebeAtacanteDefensor(int *idAtacante, int *idDefensor, int quantidade, int *jogando, int idBaseJogador) {

    *idAtacante = idBaseJogador; // recebendo o atacante que foi definido previamente por sorteio    
    
    printf("Você ataca com o territorio %d\n", *idAtacante);
    printf("Digite o número do território defensor (1 a %d), ou 0 para sair\n", quantidade);
    scanf("%d", idDefensor);
    limparBufferEntrada();
    
    // Se o jogador escolher encerrar o programa
    // Ao digitar zero, atualiza a variável "jogando" para 0 e encerra esta função recebeAtacanteDefensor
    if (*idDefensor == 0) {
        *jogando = 0;
        return;
    }
}; // Fim da função recebeAtacanteDefensor
//=====================================
// Fim das funções de ataque
//=====================================

//===============================================
// lógica principal do jogo - Funções de sorteio
//===============================================

void atribuirMissao(char *destino, const char *missoes[], int totalMissoes, char *corJogador) {
    int sorteio;

    do {
        sorteio = rand() % totalMissoes;
    } while (strstr(missoes[sorteio], corJogador) != NULL);
    
    strcpy(destino, missoes[sorteio]);
    
    printf("\n--- SUA MISSAO ---\n%s\n", destino);
    pressioneEnter();
};

// Sorteia o território do Jogador
int definirTerritorioJogador(Territorio *mapa, int quantidade, char *corJogador) {
    int sorteio = rand() % quantidade;
    strcpy(corJogador, mapa[sorteio].cor);

    printf("\nVocê assumiu o comando do Territorio %s, com o Exercito %s\n", mapa[sorteio].nome, corJogador);
    return sorteio +1; // retorna o id (index + 1)
}; // fim da função definirTerritorioJogador

//============================
// Fim das funções de sorteio
//============================

//===========================================================================
// lógica principal do jogo - Função de verificação do cumprimento da missão
//===========================================================================

// Validar missão
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador) {
    //Verificando quantos territorios o jogador possui agora
    int meusTerritorios = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            meusTerritorios++;
        }
    }
    // Avaliando missões que dependem de quantidade de territórios
    if (strstr(missao, "Conquistar 2") != NULL && meusTerritorios > 2) { return 1; };
    if (strstr(missao, "Conquistar 1") != NULL && meusTerritorios > 1) { return 1; };

    // Avaliando missões que dependem de cor
    char alvos[3][10] = {"Verde", "Azul", "Amarelo"};

    for (int c = 0; c < 3; c++) {
        if (strstr(missao, alvos[c]) != NULL) {
            
            // Verifica se alvo ainda possui territorios
            int alvoPossuiTerritorio = 0;
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, alvos[c]) == 0) { // strcmp devolve o número 0 quando as palavras são idênticas
                    alvoPossuiTerritorio = 1;
                    break;
                }
            }

            if (alvoPossuiTerritorio == 0) { return 1; }; // se alvo possui território, retorna 1
        }
    }
    return 0;
}; // Fim da função verificarMissao()

//==============================================
//Fim das funções de lógica principal do jogo
//==============================================

//==============================================
// Funções de setup e gerenciamento de memória
//==============================================
// Liberando memória

void liberarMemoria(Territorio *mapa, char *missao) {
    free(mapa);
    free(missao);
    printf("\nJogo encerrado.\n Até a próxima!\n");
};

//====================================================
// Fim das funções de setup e gerenciamento de memória
//====================================================