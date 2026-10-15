#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <optional>
#include <string>
#include <cmath>
#include <vector>
#include <ctime>
#include <SFML/Audio.hpp>

//g++ pacman.cpp -o pacman -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio && ./pacman
// ============================================================================
// PARTE 1: CONFIGURAÇÕES INICIAIS E ESTRUTURAS DE DADOS
// ============================================================================


// 0 = Vazio (espaço livre)
// 1 = Pílula Normal (vale 10 pontos)
// 2 = Parede (bloqueia a passagem)
// 3 = Pílula de Força (vale 50 pontos e deixa os fantasmas assustados)
// 4 = Porta da base dos fantasmas (Pac-Man não pode passar, mas fantasmas sim)
const int mapaOriginal[33][28] = {
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
    {2,3,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,3,2},
    {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,1,2},
    {2,1,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,1,2},
    {2,1,1,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,1,1,2},
    {2,2,2,2,2,2,1,2,2,2,2,2,0,2,2,0,2,2,2,2,2,1,2,2,2,2,2,2},
    {0,0,0,0,0,2,1,2,2,2,2,2,0,2,2,0,2,2,2,2,2,1,2,0,0,0,0,0},
    {0,0,0,0,0,2,1,2,2,0,0,0,0,0,0,0,0,0,0,2,2,1,2,0,0,0,0,0},
    {2,2,2,2,2,2,1,2,2,0,2,2,2,4,4,2,2,2,0,2,2,1,2,2,2,2,2,2},
    {0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,2,0,0,0,1,0,0,0,0,0,0},
    {2,2,2,2,2,2,1,2,2,0,2,0,0,0,0,0,0,2,0,2,2,1,2,2,2,2,2,2},
    {0,0,0,0,0,2,1,2,2,0,2,2,2,2,2,2,2,2,0,2,2,1,2,0,0,0,0,0},
    {0,0,0,0,0,2,1,2,2,1,1,1,1,1,1,1,1,1,1,2,2,1,2,0,0,0,0,0},
    {2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
    {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
    {2,3,1,1,2,2,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,2,2,1,1,3,2},
    {2,2,2,1,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,1,2,2,2},
    {2,2,2,1,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,1,2,2,2},
    {2,1,1,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,1,1,2},
    {2,1,2,2,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,2,2,1,2},
    {2,1,2,2,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,2,2,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

// Mapa atual do jogo - copiado do original e modificado durante a partida
// (pílulas são removidas quando o Pac-Man as coleta)
int mapaAtual[33][28];

// Enumeração para as direções possíveis de movimento
enum Direcao { NENHUMA, CIMA, BAIXO, ESQUERDA, DIREITA };

// Enumeração para os estados do jogo
enum ESTADO { MENU, JOGANDO, GAMEOVER, VITORIA };

// ============================================================================
// SISTEMA DE FRUTAS
// ============================================================================

// Tipos de frutas que podem aparecer no jogo
enum TipoFruta { 
    FRUTA_NENHUMA = -1,  // Nenhuma fruta ativa no momento
    FRUTA_CEREJA = 0,    // Cereja - 100 pontos
    FRUTA_LARANJA = 1,   // Laranja - 300 pontos
    FRUTA_ANANAS = 2,    // Ananás - 500 pontos
    FRUTA_MORANGO = 3    // Morango - 700 pontos
};

// Pontuação de cada tipo de fruta (índice corresponde ao enum)
const int PONTOS_FRUTA[4] = { 100, 300, 500, 700 };

// Tempo que a fruta fica visível no mapa (em segundos)
const float TEMPO_FRUTA_VISIVEL = 9.0f;

// Posição fixa onde a fruta aparece no labirinto (centro inferior)
const int FRUTA_X = 13;
const int FRUTA_Y = 21;

// Quantas pílulas o jogador precisa comer para cada fruta aparecer
// 30 pílulas -> cereja, 70 -> laranja, 110 -> ananás, 150 -> morango
const int PILULAS_PARA_FRUTA[4] = { 30, 70, 110, 150 };

// Estrutura que gerencia todo o sistema de frutas
struct SistemaFrutas {
    TipoFruta tipoAtivo;          // Qual fruta está ativa no momento
    float tempoRestante;          // Tempo restante para a fruta desaparecer
    int pilulasComidas;           // Contador total de pílulas comidas
    int proximaFruta;             // Índice da próxima fruta a ser spawnada (0-3)
    std::vector<TipoFruta> coletadas; // Histórico de frutas já coletadas (para o HUD)
};

// Instância global do sistema de frutas
SistemaFrutas sistemaFrutas;

// ============================================================================
// ESTRUTURAS DO PAC-MAN E FANTASMAS
// ============================================================================

// Estrutura que armazena todos os dados do Pac-Man
struct Pacman {
    int x;                      // Posição X atual (coluna no grid)
    int y;                      // Posição Y atual (linha no grid)
    int xAnterior;              // Posição X anterior (para detecção de colisão)
    int yAnterior;              // Posição Y anterior (para detecção de colisão)
    Direcao direcao;            // Direção atual do movimento
    Direcao proxima_direcao;    // Próxima direção que o jogador quer tomar
};

// Estrutura que armazena todos os dados de um fantasma
struct Fantasma {
    int x;                      // Posição X atual
    int y;                      // Posição Y atual
    int xAnterior;              // Posição X anterior
    int yAnterior;              // Posição Y anterior
    Direcao direcao;            // Direção atual do movimento
    int tipo;                   // 0,1 = perseguidores | 2,3 = aleatórios
    bool assustado;             // Está assustado (modo fuga)?
    bool retornando;            // Está retornando para a base?
    bool sinalizadorVelocidade; // Controla a animação de velocidade quando assustado
    float tempoPreso;           // Tempo que o fantasma fica preso na base
    bool estaSaindo;            // Está no processo de sair da base?
};

// Instâncias globais do Pac-Man e dos 4 fantasmas
Pacman pacman;
Fantasma fantasmas[4];

// Variáveis de controle do jogo
bool olhadireita = true;     // Para animação: Pac-Man olhando para direita
bool olhaesquerda = false;   // Para animação: Pac-Man olhando para esquerda
bool olhacima = false;       // Para animação: Pac-Man olhando para cima
bool olhabaixo = false;      // Para animação: Pac-Man olhando para baixo
int score = 0;               // Pontuação atual do jogador
int highScore = 0;           // Melhor pontuação já alcançada
float tempoAssustadoRestante = 0.0f; // Tempo restante do modo assustado
int frameAnimacaoPacman = 0; // Controla a animação da boca do Pac-Man (0=aberta, 1=fechada)

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

// Centraliza um texto na tela (útil para menus e mensagens)
void centralizarTexto(sf::Text& texto) {
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin({bounds.position.x + bounds.size.x / 2.0f, 
                     bounds.position.y + bounds.size.y / 2.0f});
}

// Verifica se o jogador venceu (todas as pílulas foram comidas)
bool VerificarVitoria() {
    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 28; j++)
            // Se ainda existe alguma pílula (1) ou pílula de força (3), o jogo continua
            if (mapaAtual[i][j] == 1 || mapaAtual[i][j] == 3)
                return false;
    return true; // Todas as pílulas foram comidas -> vitória!
}

// Reinicia o jogo para o estado inicial
void ResetarJogo() {
    // Coloca o Pac-Man na posição inicial
    pacman.x = 14; 
    pacman.y = 21;
    pacman.xAnterior = 14; 
    pacman.yAnterior = 21;
    pacman.direcao = NENHUMA; 
    pacman.proxima_direcao = NENHUMA;
    
    // Reseta a orientação do Pac-Man (olhando para direita)
    olhadireita = true; 
    olhaesquerda = false; 
    olhacima = false; 
    olhabaixo = false;

    // Configura os 4 fantasmas: 2 perseguidores (tipos 0 e 1) e 2 aleatórios (tipos 2 e 3)
    fantasmas[0] = {12, 14, 12, 14, NENHUMA, 0, false, false, false, 0.0f,  false};  // Perseguidor vermelho
    fantasmas[1] = {15, 14, 15, 14, NENHUMA, 1, false, false, false, 4.0f,  false};  // Perseguidor rosa
    fantasmas[2] = {13, 14, 13, 14, NENHUMA, 2, false, false, false, 8.0f,  false};  // Aleatório azul
    fantasmas[3] = {14, 14, 14, 14, NENHUMA, 3, false, false, false, 12.0f, false};  // Aleatório laranja

    // Reseta pontuação e estado dos poderes
    score = 0;
    tempoAssustadoRestante = 0.0f;

    // Reseta o sistema de frutas
    sistemaFrutas.tipoAtivo = FRUTA_NENHUMA;
    sistemaFrutas.tempoRestante = 0.0f;
    sistemaFrutas.pilulasComidas = 0;
    sistemaFrutas.proximaFruta = 0;
    sistemaFrutas.coletadas.clear();

    // Restaura o mapa original (todas as pílulas reaparecem)
    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 28; j++)
            mapaAtual[i][j] = mapaOriginal[i][j];
}

// Calcula a distância quadrática entre dois pontos (mais rápido que sqrt)
// Usado pelos fantasmas perseguidores para calcular a melhor rota
float obterDistanciaQuad(int x1, int y1, int x2, int y2) {
    return static_cast<float>((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Retorna a direção oposta a uma direção dada
Direcao obterOposta(Direcao d) {
    if (d == CIMA) return BAIXO;
    if (d == BAIXO) return CIMA;
    if (d == ESQUERDA) return DIREITA;
    if (d == DIREITA) return ESQUERDA;
    return NENHUMA;
}

// ============================================================================
// LÓGICA DOS FANTASMAS
// ============================================================================

// Função para movimento aleatório (usada pelos fantasmas tipo 2 e 3)
// Escolhe uma direção aleatória entre as possíveis, evitando voltar para trás
Direcao escolherDirecaoAleatoria(int fx, int fy, Direcao dirAtual) {
    std::vector<Direcao> direcoesPossiveis;
    Direcao oposta = obterOposta(dirAtual);
    
    // Verifica cada direção: se não for parede e não for a direção oposta, é uma opção válida
    // CIMA
    if (fy > 0 && mapaAtual[fy - 1][fx] != 2 && mapaAtual[fy - 1][fx] != 4) {
        if (dirAtual == NENHUMA || dirAtual != BAIXO)
            direcoesPossiveis.push_back(CIMA);
    }
    // BAIXO
    if (fy < 32 && mapaAtual[fy + 1][fx] != 2 && mapaAtual[fy + 1][fx] != 4) {
        if (dirAtual == NENHUMA || dirAtual != CIMA)
            direcoesPossiveis.push_back(BAIXO);
    }
    // ESQUERDA (com túnel: se x==0, o túnel permite ir para o outro lado)
    if (fx == 0 || (fx > 0 && mapaAtual[fy][fx - 1] != 2 && mapaAtual[fy][fx - 1] != 4)) {
        if (dirAtual == NENHUMA || dirAtual != DIREITA)
            direcoesPossiveis.push_back(ESQUERDA);
    }
    // DIREITA (com túnel)
    if (fx == 27 || (fx < 27 && mapaAtual[fy][fx + 1] != 2 && mapaAtual[fy][fx + 1] != 4)) {
        if (dirAtual == NENHUMA || dirAtual != ESQUERDA)
            direcoesPossiveis.push_back(DIREITA);
    }
    
    // Se não encontrou caminhos válidos, tenta qualquer direção que não seja parede
    if (direcoesPossiveis.empty()) {
        if (fy > 0 && mapaAtual[fy - 1][fx] != 2) direcoesPossiveis.push_back(CIMA);
        if (fy < 32 && mapaAtual[fy + 1][fx] != 2) direcoesPossiveis.push_back(BAIXO);
        if (fx > 0 && mapaAtual[fy][fx - 1] != 2) direcoesPossiveis.push_back(ESQUERDA);
        if (fx < 27 && mapaAtual[fy][fx + 1] != 2) direcoesPossiveis.push_back(DIREITA);
    }
    
    if (direcoesPossiveis.empty()) return NENHUMA; // Sem saída? Fica parado
    
    // Escolhe uma direção aleatória da lista
    int idx = rand() % direcoesPossiveis.size();
    return direcoesPossiveis[idx];
}

// Função principal de atualização de cada fantasma
void AtualizarFantasma(int index) {
    // Se o fantasma está preso na base, não faz nada
    if (fantasmas[index].tempoPreso > 0.0f) return;

    // Guarda a posição anterior para detecção de colisão
    fantasmas[index].xAnterior = fantasmas[index].x;
    fantasmas[index].yAnterior = fantasmas[index].y;

    int fx = fantasmas[index].x;
    int fy = fantasmas[index].y;
    Direcao dirAtual = fantasmas[index].direcao;
    int tipo = fantasmas[index].tipo;

    // ===== VERIFICA SE O FANTASMA ESTÁ RETORNANDO PARA A BASE =====
    // Se chegou na base (posição 13,14 ou 14,14), desativa o modo retornando
    if (fantasmas[index].retornando && (fx == 13 || fx == 14) && fy == 14) {
        fantasmas[index].retornando = false;
        fantasmas[index].assustado = false;
        fantasmas[index].tempoPreso = 2.0f; // Fica preso por 2 segundos
        fantasmas[index].estaSaindo = false;
        return;
    }

    // ===== VERIFICA SE O FANTASMA ESTÁ SAINDO DA BASE =====
    // Se está dentro da base e não está retornando, inicia o processo de saída
    if (fy >= 13 && fy <= 14 && fx >= 11 && fx <= 16 && !fantasmas[index].retornando)
        fantasmas[index].estaSaindo = true;
    // Se já saiu da base (chegou na linha 11), desativa o modo saindo
    if (fantasmas[index].estaSaindo && fy <= 11)
        fantasmas[index].estaSaindo = false;

    // ===== ANIMAÇÃO DE VELOCIDADE QUANDO ASSUSTADO =====
    // O fantasma pisca quando está assustado e prestes a voltar ao normal
    if (fantasmas[index].assustado && !fantasmas[index].retornando && !fantasmas[index].estaSaindo) {
        fantasmas[index].sinalizadorVelocidade = !fantasmas[index].sinalizadorVelocidade;
        // Se o sinalizador está ativo, o fantasma não se move neste tick (efeito de pisca)
        if (fantasmas[index].sinalizadorVelocidade) return;
    }

    // ===== TÚNEL TELEPORTE =====
    // Se o fantasma saiu pela esquerda, aparece na direita (e vice-versa)
    if (fx < 0) { fantasmas[index].x = 27; return; }
    if (fx > 27) { fantasmas[index].x = 0; return; }

    int targetX, targetY;
    
    // ============================================================
    // DETERMINA O ALVO DO FANTASMA BASEADO NO SEU ESTADO E TIPO
    // ============================================================
    
    // Caso 1: Está retornando para a base
    if (fantasmas[index].retornando) {
        targetX = 14; targetY = 14;
    }
    // Caso 2: Está saindo da base
    else if (fantasmas[index].estaSaindo) {
        targetX = 14; targetY = 11; // Ponto de saída da base
    }
    // Caso 3: Está assustado - foge para os cantos do mapa
    else if (fantasmas[index].assustado) {
        // Cada fantasma tem um canto diferente para fugir
        if (tipo == 0) { targetX = 0; targetY = 0; }      // Canto superior esquerdo
        else if (tipo == 1) { targetX = 27; targetY = 0; } // Canto superior direito
        else if (tipo == 2) { targetX = 0; targetY = 32; } // Canto inferior esquerdo
        else { targetX = 27; targetY = 32; }              // Canto inferior direito
    }
    else {
        // ===== FANTASMAS PERSEGUIDORES (tipos 0 e 1) =====
        if (tipo == 0 || tipo == 1) {
            // Alvo principal: a posição do Pac-Man
            targetX = pacman.x;
            targetY = pacman.y;
            
            // Tipo 1 (rosa) - persegue com um offset baseado na direção do Pac-Man
            // Isso cria um comportamento mais interessante, antecipando os movimentos
            if (tipo == 1) {
                if (pacman.direcao == CIMA) { 
                    targetY -= 4; 
                    targetX -= 4; 
                }
                else if (pacman.direcao == BAIXO) targetY += 4;
                else if (pacman.direcao == ESQUERDA) targetX -= 4;
                else if (pacman.direcao == DIREITA) targetX += 4;
            }
        }
        // ===== FANTASMAS ALEATÓRIOS (tipos 2 e 3) =====
        else {
            // Comportamento aleatório: escolhe uma direção aleatória a cada movimento
            Direcao novaDirecao = escolherDirecaoAleatoria(fx, fy, dirAtual);
            if (novaDirecao != NENHUMA) {
                fantasmas[index].direcao = novaDirecao;
            }
            
            // Move o fantasma na direção escolhida
            if (fantasmas[index].direcao == CIMA) fantasmas[index].y--;
            else if (fantasmas[index].direcao == BAIXO) fantasmas[index].y++;
            else if (fantasmas[index].direcao == ESQUERDA) fantasmas[index].x--;
            else if (fantasmas[index].direcao == DIREITA) fantasmas[index].x++;
            return; // Sai da função, pois o aleatório já se moveu
        }
    }

    // ===== LÓGICA DE MOVIMENTO PARA PERSEGUIDORES =====
    // Aqui entra o algoritmo de busca de caminho: o fantasma escolhe a direção
    // que mais se aproxima do seu alvo (usando distância quadrática)
    
    Direcao direcoesPossiveis[4] = { CIMA, BAIXO, ESQUERDA, DIREITA };
    Direcao melhorDirecao = NENHUMA;
    float menorDistancia = 9999999.f;
    Direcao oposta = obterOposta(dirAtual);

    // Testa cada direção possível
    for (int i = 0; i < 4; i++) {
        Direcao d = direcoesPossiveis[i];
        // Não permite voltar para trás (evita ficar oscilando)
        if (d == oposta && dirAtual != NENHUMA && !fantasmas[index].estaSaindo) continue;

        // Calcula a próxima posição se for nessa direção
        int proximoX = fx, proximoY = fy;
        if (d == CIMA) proximoY--;
        else if (d == BAIXO) proximoY++;
        else if (d == ESQUERDA) proximoX--;
        else if (d == DIREITA) proximoX++;

        // Verifica se o movimento é válido (não sai do mapa e não bate em parede)
        if (proximoY < 0 || proximoY >= 33 || proximoX < 0 || proximoX >= 28) continue;
        if (mapaAtual[proximoY][proximoX] == 2) continue;
        // A porta (4) só pode ser atravessada se estiver saindo ou retornando
        if (mapaAtual[proximoY][proximoX] == 4) {
            if (!fantasmas[index].retornando && !fantasmas[index].estaSaindo) continue;
        }

        // Calcula a distância do alvo e mantém a que for menor
        float dist = obterDistanciaQuad(proximoX, proximoY, targetX, targetY);
        if (dist < menorDistancia) { 
            menorDistancia = dist; 
            melhorDirecao = d; 
        }
    }

    // Se não encontrou direção, tenta qualquer uma que não seja parede (fallback)
    if (melhorDirecao == NENHUMA) {
        for (int i = 0; i < 4; i++) {
            Direcao d = direcoesPossiveis[i];
            int px = fx + (d == DIREITA) - (d == ESQUERDA);
            int py = fy + (d == BAIXO) - (d == CIMA);
            if (py >= 0 && py < 33 && px >= 0 && px < 28 && mapaAtual[py][px] != 2) {
                melhorDirecao = d; 
                break;
            }
        }
    }

    // Aplica o movimento na direção escolhida
    fantasmas[index].direcao = melhorDirecao;
    if (melhorDirecao == CIMA) fantasmas[index].y--;
    else if (melhorDirecao == BAIXO) fantasmas[index].y++;
    else if (melhorDirecao == ESQUERDA) fantasmas[index].x--;
    else if (melhorDirecao == DIREITA) fantasmas[index].x++;
}

// ============================================================================
// SISTEMA DE FRUTAS
// ============================================================================

// Tenta spawnar uma nova fruta se o limite de pílulas foi atingido
void TentarSpawnarFruta() {
    // Se já spawnou todas as 4 frutas, não faz nada
    if (sistemaFrutas.proximaFruta >= 4) return;
    // Se já tem uma fruta ativa, não faz nada
    if (sistemaFrutas.tipoAtivo != FRUTA_NENHUMA) return;

    // Verifica se o jogador já comeu pílulas suficientes para a próxima fruta
    int limiar = PILULAS_PARA_FRUTA[sistemaFrutas.proximaFruta];
    if (sistemaFrutas.pilulasComidas >= limiar) {
        // Ativa a próxima fruta e inicia o contador de tempo
        sistemaFrutas.tipoAtivo = static_cast<TipoFruta>(sistemaFrutas.proximaFruta);
        sistemaFrutas.tempoRestante = TEMPO_FRUTA_VISIVEL;
        sistemaFrutas.proximaFruta++; // Avança para a próxima fruta
    }
}

// Verifica se o Pac-Man coletou a fruta ativa
void VerificarColetaFruta() {
    // Se não tem fruta ativa, não faz nada
    if (sistemaFrutas.tipoAtivo == FRUTA_NENHUMA) return;
    
    // Verifica se o Pac-Man está na posição da fruta
    if (pacman.x == FRUTA_X && pacman.y == FRUTA_Y) {
        // Adiciona os pontos da fruta ao score
        score += PONTOS_FRUTA[static_cast<int>(sistemaFrutas.tipoAtivo)];
        // Registra a fruta como coletada (para exibir no HUD)
        sistemaFrutas.coletadas.push_back(sistemaFrutas.tipoAtivo);
        // Remove a fruta do mapa
        sistemaFrutas.tipoAtivo = FRUTA_NENHUMA;
        sistemaFrutas.tempoRestante = 0.0f;
    }
}

// ============================================================================
// FUNÇÃO MAIN - LOOP PRINCIPAL DO JOGO
// ============================================================================

int main() {
    // Inicializa o gerador de números aleatórios para os fantasmas
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Reseta o jogo para o estado inicial
    ResetarJogo();
    
    // Estado inicial do jogo: MENU
    ESTADO estadoAtual = MENU;

    // ===== CONFIGURAÇÕES DE TAMANHO E JANELA =====
    const float TAMANHO_BLOCO = 32.f;     // Tamanho de cada bloco do mapa (em pixels)
    const float OFFSET_Y = 64.f;          // Espaço no topo para o HUD (score, etc)

    // Calcula o tamanho da janela baseado no mapa + espaço para as frutas
    const unsigned int larguraJanela = static_cast<unsigned int>(28 * TAMANHO_BLOCO);
    const unsigned int alturaJanela = static_cast<unsigned int>(33 * TAMANHO_BLOCO + OFFSET_Y + 80.f); // +80px para as frutas

    // Cria a janela do jogo
    sf::RenderWindow window(sf::VideoMode({larguraJanela, alturaJanela}), "Pac-Man");

    // ===== CARREGA A MÚSICA DE FUNDO =====
    sf::Music musicaFundo;
    if (!musicaFundo.openFromFile("samba.mp3")) {
        std::cout << "Erro ao carregar a samba.mp3\n";
    } else {
        musicaFundo.setLooping(true);   // Toca em loop infinito
        musicaFundo.setVolume(3.f);     // Volume baixo para não incomodar
        musicaFundo.play();             // Inicia a música
    }

    // ===== ELEMENTOS GRÁFICOS DO MAPA =====
    // Quadrado para as paredes
    sf::RectangleShape quad({TAMANHO_BLOCO, TAMANHO_BLOCO});
    quad.setFillColor(sf::Color::Black);
    quad.setOutlineThickness(-3.f);
    quad.setOutlineColor(sf::Color(25, 25, 180)); // Azul escuro para as bordas

    // Retângulo para a porta da base dos fantasmas
    sf::RectangleShape portaVisual({TAMANHO_BLOCO, TAMANHO_BLOCO / 4.f});
    portaVisual.setFillColor(sf::Color(255, 184, 222, 200)); // Rosa translúcido

    // Painel de fundo para o Game Over / Vitória
    sf::RectangleShape painelGameOver({static_cast<float>(larguraJanela), 
                                       static_cast<float>(alturaJanela)});
    painelGameOver.setFillColor(sf::Color(0, 0, 0, 180)); // Preto semi-transparente

    // Círculo para as pílulas normais
    sf::CircleShape circle(4.f);
    circle.setFillColor(sf::Color(255, 184, 151)); // Laranja claro

    // Círculo maior para as pílulas de força
    sf::CircleShape powerPellet(8.f);
    powerPellet.setFillColor(sf::Color(255, 184, 151)); // Laranja claro

    // ===== CARREGA A FONTE =====
    sf::Font fonte;
    if (!fonte.openFromFile("retrofonte.ttf"))
        std::cout << "Aviso: retrofonte.ttf não encontrada.\n";

    // ===== TEXTOS DO HUD =====
    // Texto do score
    sf::Text textoScore(fonte);
    textoScore.setCharacterSize(18);
    textoScore.setFillColor(sf::Color::White);
    textoScore.setPosition({20.f, 24.f});

    // Título "PAC-MAN" no topo
    sf::Text textoPacmanTopo(fonte);
    textoPacmanTopo.setString("PAC-MAN");
    textoPacmanTopo.setCharacterSize(30);
    textoPacmanTopo.setFillColor(sf::Color::Yellow);
    centralizarTexto(textoPacmanTopo);
    textoPacmanTopo.setPosition({larguraJanela / 2.f, 32.f});

    // High Score
    sf::Text textoHighScore(fonte);
    textoHighScore.setCharacterSize(18);
    textoHighScore.setFillColor(sf::Color::White);

    // ===== TEXTOS DOS MENUS =====
    // Título do menu principal
    sf::Text textoTitulo(fonte);
    textoTitulo.setString("PAC-MAN");
    textoTitulo.setCharacterSize(80);
    textoTitulo.setFillColor(sf::Color::Yellow);
    centralizarTexto(textoTitulo);
    textoTitulo.setPosition({larguraJanela / 2.f, alturaJanela / 3.f});

    // Instrução para iniciar
    sf::Text textoIniciar(fonte);
    textoIniciar.setString("PRESS ENTER TO START");
    textoIniciar.setCharacterSize(20);
    textoIniciar.setFillColor(sf::Color::White);
    centralizarTexto(textoIniciar);
    textoIniciar.setPosition({larguraJanela / 2.f, alturaJanela / 1.5f});

    // Texto de Game Over
    sf::Text textoGameOver(fonte);
    textoGameOver.setString("GAME OVER");
    textoGameOver.setCharacterSize(60);
    textoGameOver.setFillColor(sf::Color::Red);
    centralizarTexto(textoGameOver);
    textoGameOver.setPosition({larguraJanela / 2.f, alturaJanela / 3.f});

    // Texto de Vitória
    sf::Text textoVitoria(fonte);
    textoVitoria.setString("YOU WIN!");
    textoVitoria.setCharacterSize(60);
    textoVitoria.setFillColor(sf::Color::Green);
    centralizarTexto(textoVitoria);
    textoVitoria.setPosition({larguraJanela / 2.f, alturaJanela / 3.f});

    // Opções após Game Over / Vitória
    sf::Text textoOpcoesGO(fonte);
    textoOpcoesGO.setString("PRESS 'R' TO RESTART\n\nPRESS 'M' FOR MENU");
    textoOpcoesGO.setCharacterSize(20);
    textoOpcoesGO.setFillColor(sf::Color::White);
    centralizarTexto(textoOpcoesGO);
    textoOpcoesGO.setPosition({larguraJanela / 2.f, alturaJanela / 1.5f});

    // Score final exibido no Game Over
    sf::Text textoFinalScore(fonte);
    textoFinalScore.setCharacterSize(25);
    textoFinalScore.setFillColor(sf::Color::Yellow);

    // ===== TEXTURAS DO PAC-MAN =====
    // Carrega as imagens do Pac-Man em diferentes direções
    sf::Texture texture, textureCima, textureBaixo, textureEsq;
    sf::Texture texFechadoDir, texFechadoCima, texFechadoBaixo, texFechadoEsq;

    if (!texture.loadFromFile("pacman.png") || !textureCima.loadFromFile("pacman-cima.png") ||
        !textureBaixo.loadFromFile("pacman-baixo.png") || !textureEsq.loadFromFile("pacman-esq.png") ||
        !texFechadoDir.loadFromFile("fechado-dir.png") || !texFechadoCima.loadFromFile("fechado-cima.png") ||
        !texFechadoBaixo.loadFromFile("fechado-baixo.png") || !texFechadoEsq.loadFromFile("fechado-esq.png"))
        std::cout << "Aviso: Imagens do Pac-Man ausentes.\n";

    // Cria os sprites do Pac-Man com boca aberta
    sf::Sprite spritedir{texture}, spritecima{textureCima}, spritebaixo{textureBaixo}, spriteesq{textureEsq};
    spritedir.setScale({TAMANHO_BLOCO / texture.getSize().x, TAMANHO_BLOCO / texture.getSize().y});
    spritecima.setScale({TAMANHO_BLOCO / textureCima.getSize().x, TAMANHO_BLOCO / textureCima.getSize().y});
    spritebaixo.setScale({TAMANHO_BLOCO / textureBaixo.getSize().x, TAMANHO_BLOCO / textureBaixo.getSize().y});
    spriteesq.setScale({TAMANHO_BLOCO / textureEsq.getSize().x, TAMANHO_BLOCO / textureEsq.getSize().y});

    // Cria os sprites do Pac-Man com boca fechada
    sf::Sprite sFechadoDir{texFechadoDir}, sFechadoCima{texFechadoCima}, sFechadoBaixo{texFechadoBaixo}, sFechadoEsq{texFechadoEsq};
    sFechadoDir.setScale({TAMANHO_BLOCO / texFechadoDir.getSize().x, TAMANHO_BLOCO / texFechadoDir.getSize().y});
    sFechadoCima.setScale({TAMANHO_BLOCO / texFechadoCima.getSize().x, TAMANHO_BLOCO / texFechadoCima.getSize().y});
    sFechadoBaixo.setScale({TAMANHO_BLOCO / texFechadoBaixo.getSize().x, TAMANHO_BLOCO / texFechadoBaixo.getSize().y});
    sFechadoEsq.setScale({TAMANHO_BLOCO / texFechadoEsq.getSize().x, TAMANHO_BLOCO / texFechadoEsq.getSize().y});

    // ===== TEXTURAS DOS FANTASMAS =====
    // Nomes das cores e direções para carregar as imagens
    std::string coresFantasmas[4] = {"vermelho", "rosa", "azul", "laranja"};
    std::string sufixosDir[4] = {"cima", "baixo", "esq", "dir"};

    // Array de texturas para cada fantasma em cada direção
    sf::Texture texFantasmas[4][4];
    std::vector<std::vector<sf::Sprite>> spriteFantasmas(4, std::vector<sf::Sprite>());

    // Carrega todas as imagens dos fantasmas
    for (int f = 0; f < 4; f++) {
        for (int d = 0; d < 4; d++) {
            std::string nomeArquivo = "fantasma-" + coresFantasmas[f] + "-" + sufixosDir[d] + ".png";
            if (!texFantasmas[f][d].loadFromFile(nomeArquivo)) {
                std::cout << "Erro ao carregar: " << nomeArquivo << "\n";
                // Se não carregar, cria um sprite vazio
                spriteFantasmas[f].push_back(sf::Sprite(texFantasmas[f][d]));
            } else {
                sf::Sprite novoSprite(texFantasmas[f][d]);
                // Ajusta a escala para caber no bloco (um pouco maior para efeito visual)
                float escalaX = (TAMANHO_BLOCO / texFantasmas[f][d].getSize().x) * 1.2f;
                float escalaY = (TAMANHO_BLOCO / texFantasmas[f][d].getSize().y) * 1.2f;
                novoSprite.setScale({escalaX, escalaY});
                // Centraliza o sprite no bloco
                float difX = (texFantasmas[f][d].getSize().x * escalaX - TAMANHO_BLOCO) / 2.0f;
                float difY = (texFantasmas[f][d].getSize().y * escalaY - TAMANHO_BLOCO) / 2.0f;
                novoSprite.setOrigin({difX / escalaX, difY / escalaY});
                spriteFantasmas[f].push_back(novoSprite);
            }
        }
    }

    // ===== TEXTURAS DAS FRUTAS =====
    std::string nomesFrutas[4] = {"cereja", "laranja", "ananas", "morango"};
    sf::Texture texFrutas[4];
    const float TAMANHO_FRUTA_MAPA = TAMANHO_BLOCO * 1.85f; // Tamanho da fruta no mapa
    const float TAMANHO_FRUTA_HUD  = 76.f;                  // Tamanho da fruta no HUD

    // Carrega as imagens das frutas
    for (int i = 0; i < 4; i++) {
        std::string arq = nomesFrutas[i] + ".png";
        if (!texFrutas[i].loadFromFile(arq))
            std::cout << "Aviso: " << arq << " não encontrada.\n";
    }

    // Cria os sprites das frutas
    std::vector<sf::Sprite> spriteFrutas;
    for (int i = 0; i < 4; i++)
        spriteFrutas.emplace_back(texFrutas[i]);

    // ===== RELÓGIOS DO JOGO =====
    sf::Clock clock;          // Controla o tick do jogo (movimentos)
    sf::Clock blinkClock;     // Controla a animação de piscar do texto do menu
    sf::Clock tempoDelta;     // Mede o tempo entre frames (delta time)
    
    bool drawTextoIniciar = true;  // Controla a animação do texto "PRESS ENTER"
    bool piscarOlhosMorte = false; // Controla a animação dos olhos dos fantasmas

    // ============================================================================
    // LOOP PRINCIPAL DO JOGO
    // ============================================================================

    while (window.isOpen()) {
        // Calcula o delta time (tempo desde o último frame)
        float dt = tempoDelta.restart().asSeconds();

        // ===== PROCESSAMENTO DE EVENTOS =====
        while (const std::optional event = window.pollEvent()) {
            // Evento de fechar a janela
            if (event->is<sf::Event::Closed>()) 
                window.close();
            
            // ===== MENU PRINCIPAL =====
            else if (estadoAtual == MENU) {
                if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
                    if (kp->scancode == sf::Keyboard::Scancode::Enter) { 
                        ResetarJogo(); 
                        estadoAtual = JOGANDO; 
                    }
            }
            
            // ===== GAME OVER / VITÓRIA =====
            else if (estadoAtual == GAMEOVER || estadoAtual == VITORIA) {
                if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
                    if (kp->scancode == sf::Keyboard::Scancode::R) { 
                        ResetarJogo(); 
                        estadoAtual = JOGANDO; 
                    }
                    else if (kp->scancode == sf::Keyboard::Scancode::M) 
                        estadoAtual = MENU;
                }
            }
            
            // ===== JOGANDO =====
            else if (estadoAtual == JOGANDO) {
                if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
                    if (kp->scancode == sf::Keyboard::Scancode::Escape) 
                        window.close();
                    // Direções do Pac-Man (guarda a próxima direção)
                    else if (kp->scancode == sf::Keyboard::Scancode::Left)  
                        pacman.proxima_direcao = ESQUERDA;
                    else if (kp->scancode == sf::Keyboard::Scancode::Right) 
                        pacman.proxima_direcao = DIREITA;
                    else if (kp->scancode == sf::Keyboard::Scancode::Up)    
                        pacman.proxima_direcao = CIMA;
                    else if (kp->scancode == sf::Keyboard::Scancode::Down)  
                        pacman.proxima_direcao = BAIXO;
                }
            }
        }

        // ============================================================================
        // LÓGICA DO JOGO (atualizações de estado)
        // ============================================================================
        
        if (estadoAtual == JOGANDO) {
            // ===== ATUALIZA O TEMPO DO MODO ASSUSTADO =====
            if (tempoAssustadoRestante > 0.0f) {
                tempoAssustadoRestante -= dt;
                // Se o tempo acabou, desassusta todos os fantasmas
                if (tempoAssustadoRestante <= 0.0f)
                    for (int i = 0; i < 4; i++)
                        if (!fantasmas[i].retornando) 
                            fantasmas[i].assustado = false;
            }

            // ===== ATUALIZA O TEMPO DOS FANTASMAS PRESOS =====
            for (int i = 0; i < 4; i++)
                if (fantasmas[i].tempoPreso > 0.0f) 
                    fantasmas[i].tempoPreso -= dt;

            // ===== ATUALIZA O TEMPO DA FRUTA ATIVA =====
            if (sistemaFrutas.tipoAtivo != FRUTA_NENHUMA) {
                sistemaFrutas.tempoRestante -= dt;
                // Se o tempo acabou, a fruta desaparece
                if (sistemaFrutas.tempoRestante <= 0.0f)
                    sistemaFrutas.tipoAtivo = FRUTA_NENHUMA;
            }

            // ===== TICK DO JOGO (a cada 0.18 segundos) =====
            // Controla a velocidade de movimento de Pac-Man e fantasmas
            if (clock.getElapsedTime() > sf::seconds(0.18f)) {
                clock.restart();

                // Alterna o frame de animação do Pac-Man (abre/fecha a boca)
                frameAnimacaoPacman = (frameAnimacaoPacman == 0) ? 1 : 0;
                piscarOlhosMorte = !piscarOlhosMorte;

                // ===== MOVIMENTO DO PAC-MAN =====
                pacman.xAnterior = pacman.x;
                pacman.yAnterior = pacman.y;

                // Tenta aplicar a próxima direção se for possível
                if (pacman.proxima_direcao == CIMA && 
                    mapaAtual[pacman.y - 1][pacman.x] != 2 && 
                    mapaAtual[pacman.y - 1][pacman.x] != 4)
                    pacman.direcao = CIMA;
                else if (pacman.proxima_direcao == BAIXO && 
                         mapaAtual[pacman.y + 1][pacman.x] != 2 && 
                         mapaAtual[pacman.y + 1][pacman.x] != 4)
                    pacman.direcao = BAIXO;
                else if (pacman.proxima_direcao == ESQUERDA && 
                         (pacman.x == 0 || (mapaAtual[pacman.y][pacman.x - 1] != 2 && 
                                           mapaAtual[pacman.y][pacman.x - 1] != 4)))
                    pacman.direcao = ESQUERDA;
                else if (pacman.proxima_direcao == DIREITA && 
                         (pacman.x == 27 || (mapaAtual[pacman.y][pacman.x + 1] != 2 && 
                                            mapaAtual[pacman.y][pacman.x + 1] != 4)))
                    pacman.direcao = DIREITA;

                // Executa o movimento na direção atual
                if (pacman.direcao == CIMA) {
                    if (mapaAtual[pacman.y - 1][pacman.x] != 2 && 
                        mapaAtual[pacman.y - 1][pacman.x] != 4) { 
                        olhacima = true; olhabaixo = false; olhadireita = false; olhaesquerda = false; 
                        pacman.y--; 
                    }
                    else pacman.direcao = NENHUMA;
                } 
                else if (pacman.direcao == BAIXO) {
                    if (mapaAtual[pacman.y + 1][pacman.x] != 2 && 
                        mapaAtual[pacman.y + 1][pacman.x] != 4) { 
                        olhabaixo = true; olhacima = false; olhadireita = false; olhaesquerda = false; 
                        pacman.y++; 
                    }
                    else pacman.direcao = NENHUMA;
                } 
                else if (pacman.direcao == ESQUERDA) {
                    // Túnel: se sair pela esquerda, aparece na direita
                    if (pacman.x == 0) { 
                        pacman.x = 27; 
                    }
                    else if (mapaAtual[pacman.y][pacman.x - 1] != 2 && 
                             mapaAtual[pacman.y][pacman.x - 1] != 4) { 
                        olhaesquerda = true; olhadireita = false; olhacima = false; olhabaixo = false; 
                        pacman.x--; 
                    }
                    else pacman.direcao = NENHUMA;
                } 
                else if (pacman.direcao == DIREITA) {
                    // Túnel: se sair pela direita, aparece na esquerda
                    if (pacman.x == 27) { 
                        pacman.x = 0; 
                    }
                    else if (mapaAtual[pacman.y][pacman.x + 1] != 2 && 
                             mapaAtual[pacman.y][pacman.x + 1] != 4) { 
                        olhadireita = true; olhacima = false; olhabaixo = false; olhaesquerda = false; 
                        pacman.x++; 
                    }
                    else pacman.direcao = NENHUMA;
                }

                // ===== COLETA DE PÍLULAS =====
                // Pílula normal (1) -> 10 pontos
                if (mapaAtual[pacman.y][pacman.x] == 1) {
                    score += 10;
                    mapaAtual[pacman.y][pacman.x] = 0; // Remove a pílula do mapa
                    sistemaFrutas.pilulasComidas++;
                    TentarSpawnarFruta(); // Verifica se deve spawnar uma fruta
                    if (VerificarVitoria()) estadoAtual = VITORIA;
                } 
                // Pílula de força (3) -> 50 pontos + assusta fantasmas
                else if (mapaAtual[pacman.y][pacman.x] == 3) {
                    score += 50;
                    mapaAtual[pacman.y][pacman.x] = 0;
                    sistemaFrutas.pilulasComidas++;
                    TentarSpawnarFruta();
                    // Ativa o modo assustado por 7 segundos
                    tempoAssustadoRestante = 7.0f;
                    for (int i = 0; i < 4; i++)
                        if (!fantasmas[i].retornando && fantasmas[i].tempoPreso <= 0.0f)
                            fantasmas[i].assustado = true;
                    if (VerificarVitoria()) estadoAtual = VITORIA;
                }

                // ===== COLETA DE FRUTA =====
                VerificarColetaFruta();

                // ===== ATUALIZA HIGH SCORE =====
                if (score > highScore) highScore = score;

                // ===== ATUALIZA TEXTOS DO HUD =====
                textoScore.setString("PONTOS\n" + std::to_string(score));
                textoHighScore.setString("RECORDE\n" + std::to_string(highScore));
                sf::FloatRect boundsHS = textoHighScore.getLocalBounds();
                textoHighScore.setOrigin({boundsHS.position.x + boundsHS.size.x, boundsHS.position.y});
                textoHighScore.setPosition({larguraJanela - 20.f, 24.f});

                // ===== ATUALIZA FANTASMAS E VERIFICA COLISÕES =====
                for (int i = 0; i < 4; i++) {
                    AtualizarFantasma(i);

                    // Verifica colisão entre Pac-Man e o fantasma
                    bool mesmaCelula = (pacman.x == fantasmas[i].x && pacman.y == fantasmas[i].y);
                    // Verifica se houve um cruzamento (passaram um pelo outro)
                    bool cruzamento = (pacman.x == fantasmas[i].xAnterior && 
                                       pacman.y == fantasmas[i].yAnterior &&
                                       pacman.xAnterior == fantasmas[i].x && 
                                       pacman.yAnterior == fantasmas[i].y);

                    if ((mesmaCelula || cruzamento) && 
                        fantasmas[i].tempoPreso <= 0.0f && 
                        !fantasmas[i].estaSaindo) {
                        
                        if (fantasmas[i].retornando) {
                            // Se está retornando, não faz nada
                            continue;
                        }
                        else if (fantasmas[i].assustado) {
                            // Se está assustado, Pac-Man come o fantasma
                            score += 200;
                            fantasmas[i].retornando = true;
                            fantasmas[i].assustado = false;
                        } 
                        else {
                            // Se não está assustado, Pac-Man morre
                            estadoAtual = GAMEOVER;
                        }
                    }
                }
            }
        }

        // ============================================================================
        // RENDERIZAÇÃO (DESENHO NA TELA)
        // ============================================================================

        window.clear(sf::Color::Black);

        // ===== MENU PRINCIPAL =====
        if (estadoAtual == MENU) {
            // Animação piscante do texto "PRESS ENTER"
            if (blinkClock.getElapsedTime() > sf::seconds(0.5f)) { 
                drawTextoIniciar = !drawTextoIniciar; 
                blinkClock.restart(); 
            }
            window.draw(textoTitulo);
            if (drawTextoIniciar) window.draw(textoIniciar);
        }
        
        // ===== JOGO ATIVO / GAME OVER / VITÓRIA =====
        else if (estadoAtual == JOGANDO || estadoAtual == GAMEOVER || estadoAtual == VITORIA) {
            // Desenha o HUD superior
            window.draw(textoScore);
            window.draw(textoPacmanTopo);
            window.draw(textoHighScore);

            // ===== DESENHA O MAPA =====
            for (int i = 0; i < 33; i++) {
                for (int j = 0; j < 28; j++) {
                    if (mapaAtual[i][j] == 2) {
                        // Parede
                        quad.setPosition({j * TAMANHO_BLOCO, i * TAMANHO_BLOCO + OFFSET_Y});
                        window.draw(quad);
                    } 
                    else if (mapaAtual[i][j] == 4) {
                        // Porta da base dos fantasmas
                        portaVisual.setPosition({j * TAMANHO_BLOCO, i * TAMANHO_BLOCO + OFFSET_Y + (TAMANHO_BLOCO / 4.f)});
                        window.draw(portaVisual);
                    } 
                    else if (mapaAtual[i][j] == 1) {
                        // Pílula normal
                        circle.setPosition({(j * TAMANHO_BLOCO) + (TAMANHO_BLOCO / 2.f - circle.getRadius()),
                                            (i * TAMANHO_BLOCO) + OFFSET_Y + (TAMANHO_BLOCO / 2.f - circle.getRadius())});
                        window.draw(circle);
                    } 
                    else if (mapaAtual[i][j] == 3) {
                        // Pílula de força
                        powerPellet.setPosition({(j * TAMANHO_BLOCO) + (TAMANHO_BLOCO / 2.f - powerPellet.getRadius()),
                                                 (i * TAMANHO_BLOCO) + OFFSET_Y + (TAMANHO_BLOCO / 2.f - powerPellet.getRadius())});
                        window.draw(powerPellet);
                    }
                }
            }

            // ===== DESENHA A FRUTA NO LABIRINTO =====
            if (sistemaFrutas.tipoAtivo != FRUTA_NENHUMA) {
                int idx = static_cast<int>(sistemaFrutas.tipoAtivo);
                float fx = FRUTA_X * TAMANHO_BLOCO + (TAMANHO_BLOCO - TAMANHO_FRUTA_MAPA) / 2.f;
                float fy = FRUTA_Y * TAMANHO_BLOCO + OFFSET_Y + (TAMANHO_BLOCO - TAMANHO_FRUTA_MAPA) / 2.f;
                spriteFrutas[idx].setScale({TAMANHO_FRUTA_MAPA / texFrutas[idx].getSize().x,
                                            TAMANHO_FRUTA_MAPA / texFrutas[idx].getSize().y});
                spriteFrutas[idx].setPosition({fx, fy});
                spriteFrutas[idx].setColor(sf::Color::White);
                window.draw(spriteFrutas[idx]);
            }

            // ===== DESENHA O PAC-MAN =====
            // Se o frame de animação for 1 ou estiver parado, desenha com boca fechada
            if (frameAnimacaoPacman == 1 || pacman.direcao == NENHUMA) {
                if (olhadireita) { 
                    sFechadoDir.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y});  
                    window.draw(sFechadoDir); 
                }
                else if (olhabaixo) { 
                    sFechadoBaixo.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y}); 
                    window.draw(sFechadoBaixo); 
                }
                else if (olhaesquerda) { 
                    sFechadoEsq.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y});  
                    window.draw(sFechadoEsq); 
                }
                else if (olhacima) { 
                    sFechadoCima.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y}); 
                    window.draw(sFechadoCima); 
                }
            } 
            // Senão, desenha com boca aberta
            else {
                if (olhadireita) { 
                    spritedir.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y});  
                    window.draw(spritedir); 
                }
                else if (olhabaixo) { 
                    spritebaixo.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y}); 
                    window.draw(spritebaixo); 
                }
                else if (olhaesquerda) { 
                    spriteesq.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y});  
                    window.draw(spriteesq); 
                }
                else if (olhacima) { 
                    spritecima.setPosition({pacman.x * TAMANHO_BLOCO, pacman.y * TAMANHO_BLOCO + OFFSET_Y}); 
                    window.draw(spritecima); 
                }
            }

            // ===== DESENHA OS FANTASMAS =====
            for (int i = 0; i < 4; i++) {
                // Determina qual sprite usar baseado na direção
                int indiceDirecao = 3; // Default: direita
                if (fantasmas[i].direcao == CIMA) indiceDirecao = 0;
                else if (fantasmas[i].direcao == BAIXO) indiceDirecao = 1;
                else if (fantasmas[i].direcao == ESQUERDA) indiceDirecao = 2;
                else if (fantasmas[i].direcao == DIREITA) indiceDirecao = 3;

                sf::Sprite& spriteAtual = spriteFantasmas[i][indiceDirecao];

                // Define a cor do fantasma baseado no estado
                if (fantasmas[i].retornando) {
                    // Retornando: olhos brancos piscando
                    spriteAtual.setColor(piscarOlhosMorte ? sf::Color(255, 50, 50, 100) : sf::Color(255, 255, 255, 80));
                }
                else if (fantasmas[i].assustado) {
                    // Assustado: azul
                    spriteAtual.setColor(sf::Color(30, 60, 255, 255));
                }
                else {
                    // Normal: cor original
                    spriteAtual.setColor(sf::Color::White);
                }

                // Posiciona e desenha o fantasma
                spriteAtual.setPosition({fantasmas[i].x * TAMANHO_BLOCO, 
                                         fantasmas[i].y * TAMANHO_BLOCO + OFFSET_Y});
                window.draw(spriteAtual);
            }

            // ===== HUD INFERIOR: FRUTAS COLETADAS =====
            {
                // Posiciona as frutas no espaço disponível abaixo do mapa
                const float yHUD = 33 * TAMANHO_BLOCO + OFFSET_Y + 15.f; // Logo abaixo do mapa
                const float TAMANHO_FRUTA_HUD = 70.f; // Tamanho das frutas
                const float espacamento = 52.f; // Espaçamento entre as frutas
                
                int total = static_cast<int>(sistemaFrutas.coletadas.size());
                
                // Rótulo "FRUTAS:" 
                sf::Text textoFrutasLabel(fonte);
                textoFrutasLabel.setString("FRUTAS:");
                textoFrutasLabel.setCharacterSize(16);
                textoFrutasLabel.setFillColor(sf::Color(200, 200, 200));
                textoFrutasLabel.setPosition({10.f, yHUD + (TAMANHO_FRUTA_HUD - 16.f) / 2.f});
                window.draw(textoFrutasLabel);
                
                // Calcula a posição inicial para centralizar as frutas no HUD
                float larguraTotal = total * espacamento - (espacamento - TAMANHO_FRUTA_HUD);
                float xInicio = (larguraJanela - larguraTotal - 100.f) / 2.f + 50.f;
                
                // Desenha as frutas da esquerda para a direita
                for (int i = 0; i < total; i++) {
                    int idx = static_cast<int>(sistemaFrutas.coletadas[i]);
                    float xPos = xInicio + i * espacamento;
                    
                    spriteFrutas[idx].setScale({TAMANHO_FRUTA_HUD / texFrutas[idx].getSize().x,
                                                TAMANHO_FRUTA_HUD / texFrutas[idx].getSize().y});
                    spriteFrutas[idx].setPosition({xPos, yHUD});
                    spriteFrutas[idx].setColor(sf::Color::White);
                    window.draw(spriteFrutas[idx]);
                }
            }

            // ===== TELAS DE GAME OVER / VITÓRIA =====
            if (estadoAtual == GAMEOVER || estadoAtual == VITORIA) {
                // Fundo semi-transparente
                window.draw(painelGameOver);
                
                // Score final
                textoFinalScore.setString("SCORE: " + std::to_string(score));
                centralizarTexto(textoFinalScore);
                textoFinalScore.setPosition({larguraJanela / 2.f, alturaJanela / 2.f});
                
                // Mensagem principal
                if (estadoAtual == GAMEOVER) 
                    window.draw(textoGameOver);
                else 
                    window.draw(textoVitoria);
                
                window.draw(textoFinalScore);
                window.draw(textoOpcoesGO);
            }
        }

        // Atualiza a tela
        window.display();
    }

    return 0;
}