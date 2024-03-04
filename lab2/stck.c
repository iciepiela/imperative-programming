#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define DECK_SIZE 52


int cbuff_push(int deck[], int num_of_cards, int out, int card) {

    deck[(out + num_of_cards) % DECK_SIZE] = card;
//    printf("\ndodaje %d na pozycje %d\n",card,(out + num_of_cards) % DECK_SIZE);
    num_of_cards++;
    return num_of_cards;
}

int cbuff_pop(int deck[], int *num_of_cards, int *out) {
    int tym;
    if ((*num_of_cards)==0) {
        return -1;
    }

    (*num_of_cards)--;
    tym = (*out);
    (*out) = ((*out)+1)%DECK_SIZE;

    return deck[tym];
}

void cbuff_print(int deck[],int num_of_cards,int out) {
    for (int i = 0; i < num_of_cards; i++) {
        printf("%d ", deck[(out + i) % DECK_SIZE]);
    }
    // printf("\n");
    // for (int i = 0; i < num_of_cards; i++) {
    //     printf("%d ", (out + i) % (num_of_cards));
    // }
}

int rand_from_interval(int a, int b) {
    if(a>b){
        return INT_MIN;}
    if(b-a>RAND_MAX){
        return INT_MAX;}
    if(a==b){
        return a;}

    return (a+rand()%(b-a+1));

}

void swap (int array[], int i, int k) {
    int tem;
    tem = array[i];
    array[i] = array[k];
    array[k] = tem;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
    int k;
    if(n>=0){
        for(int i = 0;i<n;++i){
            array[i] = i;
        }
        for(int i =0;i<n-1;++i){
            k = rand_from_interval(i,n-1);
            swap(array, i, k);
        }
    }
}

int war_res(int playerA[],int playerB[], int table[], int *num_of_cardsA, int *outA, int *num_of_cardsB, int *outB, int i,int to_do, int *con_num){
    table[i] = cbuff_pop(playerA,num_of_cardsA,outA);
    table[26+i] = cbuff_pop(playerB,num_of_cardsB,outB);
//    printf("\nkarty na stole: %d %d",table[i],table[i+26]);
    if ((table[i] == -1) || (table[i+26]) == -1){
        return 1;
    }

    if ((table[i]/4)>(table[i+26])/4){
//        printf("\nwygrwa A");
        return 2;
    }

    if ((table[i]/4)<(table[i+26])/4){
//        printf("\nwygrwa B");
        return 3;
    }
    if (((table[i]/4)==(table[i+26])/4)&&to_do==1){
        (*con_num)++;
        i++;
        table[i] = cbuff_pop(playerA,num_of_cardsA,outA);
        table[26+i] = cbuff_pop(playerB,num_of_cardsB,outB);
        i++;
        return war_res(playerA,playerB,table, num_of_cardsA,outA,num_of_cardsB,outB,i ,to_do,con_num);
    }
    if (((table[i]/4)==(table[i+26])/4)&&to_do==0){
        return 4;
    }


}

int main(void) {
    int playerA[DECK_SIZE],playerB[DECK_SIZE],table[DECK_SIZE];
    int num_of_cardsA = 26, num_of_cardsB = 26, outA = 0, outB = 0;
    int to_do, seed, max_con_num, con_num = 0, i, res;

    scanf("%d %d %d", &seed, &to_do,&max_con_num);
    srand((unsigned) seed); // set the seed

    rand_permutation(DECK_SIZE,table);


    for(int i = 0;i<26;++i){
        playerA[i] = table[i];
        playerB[i] = table[i+26];
    }


    while(num_of_cardsA!=0 && num_of_cardsB!=0 && con_num<max_con_num) {
        i = 0;

        for (int k = 0; k < 52; ++k) {
//            printf("%d", table[k]);
            table[k] = 0;
        } // zerowanie table

        res = war_res(playerA, playerB, table, &num_of_cardsA, &outA, &num_of_cardsB, &outB, 0, to_do, &con_num);

        switch (res) {
            case 1:
                printf("1\n%d", con_num);
                return 0;
            case 2:
                for (int j = 0; j <= i; ++j) {
                    num_of_cardsA = cbuff_push(playerA, num_of_cardsA, outA, table[j]);
                }
                for (int j = 0; j <= i; ++j) {
                    num_of_cardsA = cbuff_push(playerA, num_of_cardsA, outA, table[j + 26]);
                }
                break;

            case 3:
                for (int j = 0; j <= i; ++j) {
                    // printf("\ndodawana karta %d",table[j+26] );
                    num_of_cardsB = cbuff_push(playerB, num_of_cardsB, outB, table[j + 26]);
                }

                for (int j = 0; j <= i; ++j) {
                    // printf("\ndodawana karta %d",table[j] );
                    num_of_cardsB = cbuff_push(playerB, num_of_cardsB, outB, table[j]);
                }
                break;

            case 4:
                for (int j = 0; j <= i; ++j) {
                    num_of_cardsA = cbuff_push(playerA, num_of_cardsA, outA, table[j]);
                }
                for (int j = 0; j <= i; ++j) {
                    num_of_cardsB = cbuff_push(playerB, num_of_cardsB, outB, table[j + 26]);
                }
                break;

        }
        con_num++;
    }
    if (con_num>=max_con_num){
        printf("\n0\n%d\n%d",num_of_cardsA,num_of_cardsB);
    }

    if(num_of_cardsB==0){
        printf("\n2\n%d",con_num);
    }
    if(num_of_cardsA==0){
        printf("\n3\n");
        cbuff_print(playerB,num_of_cardsB,outB);
    }

    return 0;
}