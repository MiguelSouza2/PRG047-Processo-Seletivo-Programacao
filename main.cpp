#include <iostream>
#include <random>
#include <ctime>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <format>
#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

void roulette(float bet);
void blackjack(float bet);
void slot_machine(float bet);
void clear_buffer(), clear_screen();
void pause_screen();
void calc_balance(float recalc);
void mysteryous_point(int it);
float bet(float bet);

int random_number(int min, int max);
int sum_blackjack(std::vector<std::string>& pdeck);

std::string roulette_colored(int r);
std::string draw_card(std::stack<std::string>& card_deck);
float bal = 500.00f;

std::vector<char> color = {
    'G',
    'R','P','R','P','R','P','R','P','R','P',
    'P','R','P','R','P','R','P','R',
    'R','P','R','P','R','P','R','P','R','P',
    'R','P','R','P','R','P','R','P'
};

std::random_device rd;
// gerador mt19937 iniciado com a seed
std::mt19937 gen(rd());

int main(){
    int opt = 0;
    while(true){
        // introdução
        clear_screen();
        std::cout << "\n▪───────────────────────────────────────▪\n";
        std::cout << " Bem-vind@ ao cassino virtual CrossBETS!";
        std::cout << "\n▪───────────────────────────────────────▪\n";
        std::cout << "[Saldo atual: " << bal << " TOKENS]\n\n";
        pause_screen();
        // escolha das opções
        // só tem a roleta por enquanto
        std::cout << "Escolha uma das 3 opções a seguir para continuar\n[1] Roleta\n[2] Blackjack\n[3] Caça-Níquel\n";
        std::cin >> opt;
        clear_buffer();
        if (opt == 1) roulette(bet(bal));
        else if (opt == 2) blackjack(bet(bal));
        else if (opt == 3) slot_machine(bet(bal));
        else std::cout << "Opção inválida!\n";
        
    } 
    return 0;
}

// LÓGICA ESTRITAMENTE DA ROLETA
void roulette(float bet){
    /* O jogador vai poder escolher entre 3 opções pra apostar, de acordo com o primeiro modelo de 
    roleta que eu achei na internet (roleta europeia). Se 
    */
    int opt, opt2, min, max=0;
    clear_screen();
    // receber o número apostado
    int user_num;
    // o usuário pode escolher entre o apostar em um número, grupo de número ou cor
    std::cout << "\n¹²³ ROLETA ³²¹\nEscolha uma opção para apostar:\n[1] Número (35x a aposta)\n[2] Grupo (2x a aposta) de números\n[3] Cor\n";
    std::cin >> opt;

    switch(opt){
        case 1:
        // caso seja número
            std::cout << "Digite um número pra apostar (1 - 36): "; 
            if (!(std::cin >> user_num) || user_num <= 0 || user_num > 36) {
                std::cout << "Número inválido! Aposta cancelada\n";
                clear_buffer();
                pause_screen();
                return;
            }
            break;
        case 2:
        // caso seja um intervalo de números
            std::cout << "Escolha um dos grupos para apostar:\n[1] 1-12\n[2] 13-24\n[3] 25-36\n";
            std::cin >> opt2;
            if(!std::cin || opt2 < 1 || opt2 > 3){
                std::cout << "Grupo inválido! Aposta cancelada\n";
                clear_buffer();
                pause_screen();
                return;
            }
            if(opt2==1) min=1, max=12;
            else if(opt2==2) min=13, max=24;
            else min=25, max=36;
            break;
        case 3: 
        // caso seja nas cores (acho que é essa correspondência)
        // (0 é verde)
        // de 1 a 10 e 19 a 28, números pares são pretos e ímpares são vermelhos
        // de 11 a 18 e 29 a 36, números pares são vermelhos e ímpares são pretos
            std::cout << "Escolha uma cor para apostar:\n[1] Vermelho\n[2] Preto\n";
            std::cin >> opt2;
            if(!std::cin || (opt2!=1 && opt2 !=2)){
                std::cout << "Cor inválida! Aposta cancelada\n";
                clear_buffer();
                pause_screen();
                return;
            }
            break;
        default:
            std::cout << "Seleção inválida! Aposta cancelada\n";
            clear_buffer();
            pause_screen();
            return;
        
    }
    
    std::cout << "Girando a roleta...\n";
    // animação legal
    for (int i = 0; i < 3; i++){
        mysteryous_point(3);
        int r = random_number(0, 36);
        std::cout << "(" << roulette_colored(r) << ")";
    }
    // número sorteado
    int drawn_num = random_number(0,36);
    mysteryous_point(3);
    std::cout << "\n ★ " << roulette_colored(drawn_num) << " ★\n";

    // lógica de vitória baseado no opt escolhido  
    if(opt==1 && drawn_num == user_num){
        std::cout << "Parabéns, você ganhou!";
        calc_balance(10*bet);
    }else if(opt == 2 && drawn_num>=min && drawn_num <=max){
        std::cout << "Parabéns, você ganhou!";
        calc_balance(2*bet);
    }else if(opt == 3 && color[drawn_num] == (opt2==1 ? 'R' : 'P') ){
        std::cout << "Parabéns, você ganhou!";
        calc_balance(bet);
    }else{
        std::cout << "Infelizmente você perdeu...";
        calc_balance(-bet);
    }
    clear_buffer();
    pause_screen();
}

std::string roulette_colored(int r){
    std::string res_color;
    if (color[r] == 'G')
        res_color = "\033[32m" + std::to_string(r) + "\033[0m";
    else if (color[r] == 'R')
        res_color = "\033[31m" + std::to_string(r) + "\033[0m";
    else
        res_color = "\033[30m" + std::to_string(r) + "\033[0m";
    
    return res_color;
}
// ---



// LÓGICA ESTRITAMENTE DO BLACKJACK 
void blackjack(float bet){
    /* blackjack ou 21:
    o jogador e a casa vão receber duas cartas, das quais apenas uma da carta vai estar visível de primeira.
    O jogador poderá sacar mais uma carta ou parar. Se ele parar, a última carta da casa é revelada e vence quem estiver mais perto de 21.
    Se o jogador tirar exatamente 21, o jogo para na hora.
    
    Uma carta sacada uma vez não pode ser sacada de novo.
    Os valores das cartas variam:
    A é 1 ou 11
    2-10 valem o seu numero
    J,Q,K valem 10
    */
   std::vector<std::string> player_cards;
   std::vector<std::string> house_cards;
   int player_sum = 0;
   std::vector<std::string> deck={"A","2", "3", "4","5","6","7","8","9","10","J","Q","K",
                                  "A","2", "3", "4","5","6","7","8","9","10","J","Q","K",
                                  "A","2", "3", "4","5","6","7","8","9","10","J","Q","K",
                                  "A","2", "3", "4","5","6","7","8","9","10","J","Q","K"};
    std::stack<std::string>card_deck;

    // embaralhando o vetor
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    // guardando os valores do vetor no stack pra usar como um baralho de cartas com o método top()
    for(std::string i : deck){
        card_deck.push(i);
    }

    // começando o jogo
    clear_screen();
    // cartas do jogador
    player_cards.push_back(draw_card(card_deck)), player_cards.push_back(draw_card(card_deck));
    // cartas da casa
    house_cards.push_back(draw_card(card_deck)), house_cards.push_back(draw_card(card_deck));

    std::cout << "¹²³³ Blackjack ³²¹\n";
    clear_buffer();
    pause_screen();
    
    // enquanto o jogador não parar de jogar, as cartas da casa não serão reveladas ainda
    while(true){
        char opt; 
        std::cout << "Distribuindo as cartas...\n";
        mysteryous_point(3);
        std::cout << "\n[Cartas da casa]\n{" << house_cards[0] << "} | {???}\n\n[Suas cartas]\n";
        // mostrar as cartas do jogador
        for(std::string i : player_cards){
            std::cout << "{"<< i <<"} ";
        }

        // fazer a soma de pontos usando uma função
        player_sum = sum_blackjack(player_cards);
        // verificar se estourou ou deu 21
        if(player_sum>=21) break;
        std::cout << "\nSoma: " << player_sum << "\n\n[D] Pegar mais uma carta\n[S] Parar de jogar\n";
        std::cin >> opt;
        if (!std::cin || (opt != 'D' && opt != 'S')) {
            std::cout << "Opção inválida! Aposta cancelada\n";
            clear_buffer();
            return;
        }
        if(opt == 'D'){
            // pegar mais uma carta para o player
            player_cards.push_back(draw_card(card_deck));
        }else if (opt=='S'){
            // parar o jogo
            break;
        }
        std::cout << "\n---------\n";
    }
    std::cout << "\nFIM DE JOGO!!\n";
    mysteryous_point(3);
    std::cout << "\nSuas cartas: "; 
    for(std::string i : player_cards){
        std::cout << "{"<< i <<"} ";
    }
    std::cout << "Sua pontuação: " << player_sum;
    
    // revelar as casas da casa e comprar se faltar mais de 5 pontos
    std::cout << "\nCartas da casa: ";
    int house_sum = sum_blackjack(house_cards);
    while(house_sum <= 15 && player_sum!=21){
        house_cards.push_back(draw_card(card_deck));
        house_sum = sum_blackjack(house_cards);
        if(house_sum > 15){
            break;
        }
    }
    for(std::string i : house_cards){
        std::cout << "{" << i << "} ";
    }
    std::cout << "\nPontuação da casa: " << house_sum <<"\n";
    mysteryous_point(3);

    // verificar quem tá mais próximo de 21 usando abs
    int abs_player = std::abs(21-player_sum);
    int abs_house = std::abs(21-house_sum);

    if(abs_player<abs_house){
        std::cout << "VOCÊ VENCEU!!";
        calc_balance(bet);
    }else if(abs_player>abs_house || player_sum>21){
        std::cout << "VOCÊ PERDEU...";
        calc_balance(-bet);
    }else{
        std::cout << "FOI UM EMPATE!!";
    }
    clear_buffer();
    pause_screen();
}

std::string draw_card(std::stack<std::string>& card_deck){
    // pegar uma carta do topo e tirar do baralho 
    std::string card= card_deck.top();
    card_deck.pop();
    return card;
    
}

int sum_blackjack(std::vector<std::string>& pdeck) {
    int sum = 0;
    int aces = 0;

    for (std::string& card : pdeck) {
        if (card == "A") {
            aces++;
            sum += 11; 
        } else if (card == "K" || card == "Q" || card == "J") {
            sum += 10;
        } else {
            sum += std::stoi(card);
        }
    }

    // Se a soma das cartas com Ás for maior que 21,então reduzimos 10 por cada Ás
    while (sum > 21 && aces > 0) {
        sum -= 10;
        aces--;
    }

    return sum;
}
// ---


void slot_machine(float bet){
    /* jogo 3:
        Vai ter 3 símbolos aleatórios (🔔 🍒 💎) que serão sorteados e o objetivo do jogador é 
        conseguir tirar 3 seguidos
        A lógica é sortear um número de 0 a 2 em cada uma das 3 posições e verificar se são iguais 
        no final
    */
    std::vector<std::string>symbols = {"🔔","🍒","💎"};
    std::vector<std::string>slots(3);
    char opt;

    clear_screen();
    std::cout << "¹²³ CAÇA NÍQUEL ³²¹\n[D] Jogar\n[S] Parar de jogar\n";
    std::cin >> opt;
    if (!std::cin || (opt != 'S' && opt != 'D')) {
        std::cout << "Opção inválida! Aposta cancelada\n";
        clear_buffer();
        return;
    }

    if(opt == 'S'){
        return;
    }else{
        // pra dar aquele efeito de cassino, vai ter um loop j pra fazer o efeito e outro para 
        // definir os números cada um 

        for(int i=0;i<3;i++){
            for (int j = 0; j < 15; j++){
                if(i==0) slots[0] = symbols[random_number(0, 2)];
                if(i<=1) slots[1] = symbols[random_number(0, 2)];
                slots[2] = symbols[random_number(0, 2)];

                std::cout << "\r [" << slots[0] << "] [" << slots[1] << "] [" << slots[2] << "]" << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(80)); 
            }
        }

        // verificar se os slots são simbolos iguais
        if(slots[0]==slots[1] && slots[0]==slots[2]){
            std::cout << "\nVOCÊ VENCEU!!";
            calc_balance(bet);
        }
        else{
            std::cout << "\nVOCÊ PERDEU...";
            calc_balance(-bet);
        }
        clear_buffer();
        pause_screen();
    }

    

}


int random_number(int min, int max){
    // distribuir os inteiros do minimo ao máximo
    std::uniform_int_distribution<int> distrib(min,max);
    return distrib(gen);
}

void pause_screen(){
    // pausa a tela e só continua ao receber um enter novo
    std::cout << "\nPressione ENTER para continuar...\n";
    std::cin.get();
}

void clear_buffer(){
    // limpar o buffer para não ter nenhum resíduo na hora de pausar a tela
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

void clear_screen(){
    // verificar qual SO o pessoal tá usando (eu to no linux ;-;)
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void mysteryous_point(int it){
    // pontinho do mistério
    for (int i = 0;i<it;i++){
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));        
    }
}

void calc_balance(float recalc){if(recalc != 0) bal+=recalc;}

float bet(float bal){
    float bet;
    if(bal <= 0){
            std::cout << "\nVocê está sem TOKENS! Fim de jogo.";
            std::exit(0);
        }
    while (true){   
        // definindo a aposta inicial
        std::cout << "\nAntes de iniciar, qual será a sua aposta inicial? (Saldo atual: "<<bal<<") ";
        std::cin >> bet;
        
        // verificação da aposta em relação ao saldo
        if (!(std::cin)) {
           std::cout << "Digite um valor numérico!\n";
           clear_buffer();
        }
        else if(bet > bal){
            std::cout << "Seu saldo é insuficiente! Tente uma aposta menor...";
            clear_buffer();
        }else if(bet <= 0){
            std::cout << "Você precisa fazer uma aposta mínima de 1 token";
        }else{
            return bet;
        }
    }
            
}
