#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define SAME_RANK_FIRST_LEFT (pack.stacks[i].top->rank == pack.stacks[i-1].top->rank)
#define SAME_SUIT_FIRST_LEFT (pack.stacks[i].top->suit == pack.stacks[i-1].top->suit)
#define SAME_RANK_THIRD_LEFT (pack.stacks[i].top->rank == pack.stacks[i-3].top->rank)
#define SAME_SUIT_THIRD_LEFT (pack.stacks[i].top->suit == pack.stacks[i-3].top->suit)
#define N 52
typedef struct Card
{
    char rank;
    char suit;

} Card;

typedef struct Stack
{
    Card* cards;
    Card* top;
    int qtd;

} Stack;

typedef struct Pack
{
    Stack stacks[N];
    int piles;

} Pack;

Pack pack;


Card* new_card(int size, char rank, char suit)
{
    Card* card = (Card*) malloc(sizeof(Card));
    assert(card != NULL);

    card->rank = rank;
    card->suit = suit;
    return card;
}

Pack start(const char* entrada)
{
    int i,j, rank, suit;
    for(i=0, j=0; i<N; i++,j+=2)
    {
        rank = entrada[j];
        j++;
        suit = entrada[j];
        pack.stacks[i].cards = new_card(1, rank, suit);
        pack.stacks[i].top = &(pack.stacks[i].cards[0]);
        pack.stacks[i].qtd = 1;
    }

    return pack;
}

Card* func(int atual, int prox)
{
    int i;
    Card* card = realloc(pack.stacks[atual].cards, pack.stacks[prox].qtd);
    assert(card);

    for(i=0; i < pack.stacks[prox].qtd; i++)
        pack.stacks[atual].cards[i] = pack.stacks[prox].cards[i];

    pack.stacks[atual].qtd = pack.stacks[prox].qtd;
    pack.stacks[atual].top = &(pack.stacks[atual].cards[pack.stacks[atual].qtd -1]);
    return pack.stacks[atual].cards;
}


void close_gaps(int pos)
{
    int atual=pos, prox=atual+1;

    while(atual < N)
    {
        pack.stacks[atual].cards = func(atual, prox);
        atual++;
        prox++;

        if(pack.stacks[prox].qtd == -1 || atual == N-1)
        {
            pack.piles++;
            pack.stacks[atual].qtd = -1;
            free(pack.stacks[atual].cards);
            pack.stacks[atual].top = NULL;
            break;;
        }
    }
}


void move_onto(int orig, int dest)
{
    Stack* p_orig = &(pack.stacks[orig]);  /* ponteiro para a pilha de onde a carta estava */
    Stack* p_dest = &(pack.stacks[dest]);  /* ponteiro para a pilha aonda a carta será empilhada */

    /* ponteiros criados para apontarem diretamente para a quantidade de cada pilha
     * para não precisar fazer: "pack.stacks[orig].qtd" sempre que for necessário alterar
     * o qtd de uma das pilhas
     */
    int*   qtd_orig   = &(p_orig->qtd);
    int*   qtd_dest   = &(p_dest->qtd);

    (*qtd_dest)++;

    p_dest->cards = realloc(p_dest->cards, sizeof(Card) * (*qtd_dest));  /* pilha de destino aloca espaço para mais uma carta */

    assert(p_dest->cards);                             /* certifica-se que a alocação ocorreu bem */
    p_dest->cards[*qtd_dest-1] = *(p_orig->top);        /* empilha a carta da pilha de origem no espaço alocado anteriormente na pilha destino */
    p_dest->top = &(p_dest->cards[*qtd_dest-1]);        /* faz com que a carta que foi empilhada seja o novo topo */

    (*qtd_orig)--;

    if(pack.stacks[orig+1].qtd < 1)
        return;

    if((*qtd_orig) == 0)
        close_gaps(orig);
    else
    {
        p_orig->cards = realloc(p_orig->cards, *qtd_orig);  /* diminui a pilha origem */
        assert(p_orig->cards);
        p_orig->top = &(p_orig->cards[*qtd_orig -1]);       /* novo topo da pilha origem é a carta anterior(pode ser NULL) */
    }
}


void accordian_patience()
{
    int i,j;
    for(i=1; i < N; i++)
    {
        if(pack.stacks[i].qtd == -1)
            break;
        /*
        printf("i=%d ",i);
        for(j=0;j<26;j++)
        {
            if(pack.stacks[j].qtd > 0)
            printf("%c%c ",pack.stacks[j].top->rank,pack.stacks[j].top->suit);
        }

        putchar('\n');
        */
        if(pack.stacks[i].qtd == -1)
            break;

        if(i>2 && (SAME_RANK_THIRD_LEFT || SAME_SUIT_THIRD_LEFT))
        {   
            move_onto(i, i-3);
            return accordian_patience();
            //accordian_patience(i);
        }

        else if(SAME_RANK_FIRST_LEFT || SAME_SUIT_FIRST_LEFT)
        {
            move_onto(i, i-1);
            //if(i>0)
                return accordian_patience();
        }

    }

}

int main()
{   pack.piles = 0;
    int i=0;
    char entrada[156] = "QD AD 8H 5S 3H 5H TC 4D JH KS 6H 8S JS AC AS 8D 2H QS TS 3S AH 4H TH TD 3C 6S 8C 7D 4C 4S 7S 9H 7C 5D 2S KD 2D QH JD 6D 9D JC 2C KH 3D QC 6C 9S KC 7H 9C 5C";
    start(entrada);
    //move_onto(1, 0);
    accordian_patience();
    //printf("%c%c ",pack.stacks[i].top->rank, pack.stacks[i].top->suit);
    //printf("%d",pack.stacks[6].qtd);

    for(i=0; i<N;i++)
        free(pack.stacks[i].cards);
}

