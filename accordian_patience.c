#include <stdio.h>
#include <stdlib.h>
#define SAME_SUIT_FIRST_LEFT (pack->stacks[i].top->suit == pack->stacks[i-1].top->suit)
#define SAME_SUIT_THIRD_LEFT (i >= 3 && pack->stacks[i].top->suit == pack->stacks[i-3].top->suit)
#define SAME_RANK_FIRST_LEFT (pack->stacks[i].top->rank == pack->stacks[i-1].top->rank)
#define SAME_RANK_THIRD_LEFT (i >= 3 && pack->stacks[i].top->rank == pack->stacks[i-3].top->rank)


typedef struct Card
{
    struct Card* next;
    char suit;             /* Naipe da carta: D - diamons, H - heart, C- clubs, S - spades */
    char rank;             /* A(ace) 1-9 T(Ten), J(Jack), Q(Queen), K(King)
                            * A(ás)  1-9  (Dez), (Valete), (Rainha), (Rei)
                            */
} Card;


typedef struct Stack
{
    Card* top;
    int qtd;

} Stack;


typedef struct Pack //baralho
{
    Stack stacks[26];      /* Cada carta será tratada como possível formadora de uma pilha,
                             * por isso o baralho possui 26 "pilhas" ainda não formadas */
    int piles;              /* Quantidade real de pilhas formadas */

} Pack;


Card* new_card(char rank, char suit)
{
    Card* card = (Card*) malloc(sizeof(Card));
    if(!card)
        exit(1);
    card->next = NULL;
    card->suit = suit;
    card->rank = rank;
    return card;
}


Pack* start(const char* entrada)
{
    int i,j;
    Pack*  pack = (Pack*)malloc(sizeof(pack));

    for(i=0, j=0; j<26; i+=2, j++)
    {
        pack->stacks[j].top  = new_card(entrada[i], entrada[++i]);
        pack->stacks[j].qtd++;
    }
    return pack;
}

/* Faz com que a carta ao topo da pilha origem seja o novo topo da pilha destino, é removida da pilha origem */
void move_onto(Pack* pack, int orig, int dest)
{
    Card* temp_top = pack->stacks[orig].top;
    pack->stacks[orig].top = temp_top->next;
    temp_top->next = pack->stacks[dest].top;
    pack->stacks[dest].top = temp_top;
}

/* ****************************************************
    LOOP PRINCIPAL --> ACRESCENTAR CONDIÇÕES DE TÉRMINO
*******************************************************/
void accordian_patiente(Pack* pack)
{
    int i = 1;
    for(; i < 26; i++)
    {
        /* prioridade é da carta mais à esquerda */
        if(SAME_RANK_THIRD_LEFT || SAME_SUIT_THIRD_LEFT)
        {
            move_onto(pack, i, i-3);
        }

        else if(SAME_RANK_FIRST_LEFT || SAME_SUIT_FIRST_LEFT)
        {
            move_onto(pack, i, i-1);
        }

    }

}

int main()
{   int i=0;
    char* entrada = (char*) malloc(77+1);
    scanf("%[^\n]s",entrada);

    free(entrada);

}
