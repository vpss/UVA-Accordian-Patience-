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

} Pack;

Pack pack;


Card* new_card(char rank, char suit)
{
    Card* card = (Card*) malloc(sizeof(Card));
    assert(card != NULL);
    card->rank = rank;
    card->suit = suit;
    return card;
}

/* forma o as cartas do baralho a partir da string entrada */
Pack start(char* line1, char* line2)
{
    //vetor line1
    int i,j, rank, suit;
    for(i=0, j=0; i<(N/2); i++,j+=2)
    {
        rank = line1[j];
        suit = line1[++j];
        pack.stacks[i].cards = new_card(rank, suit);
        pack.stacks[i].top = &(pack.stacks[i].cards[0]);
        pack.stacks[i].qtd = 1;
    }
    // vetor line2
    for(j=0; i<N; i++, j+=2)
    {
        rank =  line2[j];
        suit =  line2[++j];
        pack.stacks[i].cards = new_card(rank, suit);
        pack.stacks[i].top = &(pack.stacks[i].cards[0]);
        pack.stacks[i].qtd = 1;
    }

    return pack;
}

/* faz com que a pilha 'prox' seja "trazida" para o lugar da atual.
   O que realmente acontece é que a pilha atual de cartas é realocada para
   conter a quantidade de elementos da pilha seguinte, então o "vetor de cartas",
   que na verdade é um ponteiro, é preenchido com as cartas da próxima pilha */
Card* equals_next(int atual, int prox)
{
    int i;
    pack.stacks[atual].cards = realloc(pack.stacks[atual].cards, sizeof(Card) * pack.stacks[prox].qtd);
    assert(pack.stacks[atual].cards);

    for(i=0; i < pack.stacks[prox].qtd; i++)
        pack.stacks[atual].cards[i] = pack.stacks[prox].cards[i];

    pack.stacks[atual].qtd = pack.stacks[prox].qtd;
    pack.stacks[atual].top = &(pack.stacks[atual].cards[pack.stacks[atual].qtd -1]);
    return pack.stacks[atual].cards;
}

/* se ao uma carta ser deslocada, não houver mais cartas sob ela, essa função é
   chamada para "preencher" o espaço vazio, recebe a posição do espaço vazio */
void close_gaps(int pos)
{
    int atual=pos, prox=atual+1;

    while(atual < N)
    {
        pack.stacks[atual].cards = equals_next(atual, prox);  // faz com que a pilha atual seja igual a proxima
        atual++;
        prox++;

    /* a segunda condição é para a primeira vez que essa função for chamada,
       ao trazer todas as cartas para a esquerda, a última posição(N-1) já fica
       inutilizada, já que não poderá receber mais nenhuma carta */
        if(pack.stacks[prox].qtd == -1 || atual == N-1)       //uma pilha tem qtd = -1 quando ela não existe mais, ou seja,
        {                                                     // não há possibilidade de receber mais nenhuma carta
            pack.stacks[atual].qtd = -1;
            pack.stacks[atual].top = NULL;
            break;
        }
    }
}

/* Põe a carta que estava na pilha de origem(stacks[orig]) sobre a pilha destino(pilha stacks[dest])
   se a carta retirada da pilha origem for a única dessa pilha, haverá um espaço entre as pilhas, então
   a função close_gaps() será chamada. */
void move_onto(int orig, int dest)
{
    pack.stacks[dest].cards = (Card*) realloc(pack.stacks[dest].cards, sizeof(Card) * (pack.stacks[dest].qtd+1));
    assert(pack.stacks[dest].cards);
    pack.stacks[dest].qtd++;
    pack.stacks[dest].cards[pack.stacks[dest].qtd - 1] = *(pack.stacks[orig].top);
    pack.stacks[dest].top = &(pack.stacks[dest].cards[pack.stacks[dest].qtd-1]);

    pack.stacks[orig].qtd--;
    if(pack.stacks[orig].qtd == 0)
    {
        if(pack.stacks[orig+1].qtd < 1)
        {
            pack.stacks[orig].top = NULL;
            pack.stacks[orig].qtd = -1;
        }
        else    close_gaps(orig);
    }
    else
    {
        pack.stacks[orig].cards = (Card*) realloc(pack.stacks[orig].cards, sizeof(Card) * pack.stacks[orig].qtd);
        assert(pack.stacks[orig].cards);
        pack.stacks[orig].top = &(pack.stacks[orig].cards[pack.stacks[orig].qtd - 1]);
    }
}
/*
//VERSÃO RECURSIVA
void accordian_patience(int atual)
{
    int i = atual, j;

    if(pack.stacks[i].qtd == -1)
        return;

    //MACRO
    if(i>2 && (SAME_RANK_THIRD_LEFT || SAME_SUIT_THIRD_LEFT))
    {
        move_onto(i, i-3);
        return accordian_patience(1);
    }

    //MACRO
    if(SAME_RANK_FIRST_LEFT || SAME_SUIT_FIRST_LEFT)
    {
        move_onto(i, i-1);
        return accordian_patience(1);
    }

    return accordian_patience(i+1);
}*/


//VERSÃO ITERATIVA
void accordian_patience()
{
    int i,j;
    for(i=1; i < N; i++)
    {
        if(pack.stacks[i].qtd == -1)
            break;

        if(i>2 && (SAME_RANK_THIRD_LEFT || SAME_SUIT_THIRD_LEFT))
        {
            move_onto(i, i-3);
            accordian_patience();
        }

        else if(SAME_RANK_FIRST_LEFT || SAME_SUIT_FIRST_LEFT)
        {
            move_onto(i, i-1);
            accordian_patience();
        }
    }
}


int main()
{
    int i=0, j;
    //char entrada[156] = "QD AD 8H 5S 3H 5H TC 4D JH KS 6H 8S JS AC AS 8D 2H QS TS 3S AH 4H TH TD 3C 6S 8C 7D 4C 4S 7S 9H 7C 5D 2S KD 2D QH JD 6D 9D JC 2C KH 3D QC 6C 9S KC 7H 9C 5C";
    //char entrada[156] = "AC 2C 3C 4C 5C 6C 7C 8C 9C TC JC QC KC AD 2D 3D 4D 5D 6D 7D 8D TD 9D JD QD KD AH 2H 3H 4H 5H 6H 7H 8H 9H KH 6S QH TH AS 2S 3S 4S 5S JH 7S 8S 9S TS JS QS KS";
    char line1[78];// = "QD AD 8H 5S 3D ADH 5H TC 4D JH KS 6H 8S JS AC AS 8D 2H QS TS 3S AH 4H TH TD 3C 6S";
    char line2[78];// = "8C 7D 4C 4S 7S 9H 7C 5D 2S KD 2D QH JD 6D 9D JC 2C KH 3D QC 6C 9S KC 7H 9C 5C";
    //while(b1[0] != '#')
    //{
        fgets(line1, 78, stdin);
        j = 0;
        fgets(line2, 78, stdin);
        start(line1, line2);
        accordian_patience();
    //}
    while(pack.stacks[i].qtd > 0) i++;

    printf("%d piles remaining: ",i);

    for(j=0; j<i; j++)
        printf("%d ",pack.stacks[j].qtd);

    for(i=0; i<N; i++)
        free(pack.stacks[i].cards);
}


