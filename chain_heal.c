#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h> 

typedef struct node{
    char *name;
    int x, y;
    int cur_PP, max_PP;
    struct node *prev;
    int adj_size;
    struct node **adj;
} Node;

int main(int argc, char const *argv[])
{
    printf("hello world\n");

    if(argc != 6){
        printf("Usage: %s <initial_range> <jump_range> <num_jumps> <initial_power> <power_reduction>\n", argv[0]);
        return 1;
    }

    int initial_range = atoi(argv[1]);
    int jump_range = atoi(argv[2]);
    int num_jumps = atoi(argv[3]);
    int initial_power = atoi(argv[4]);
    double power_reduction = atof(argv[5]);

    printf("Initial range: %i, Jump Range: %i, Num of Jumps: %i, Initial Power: %i, Power Reduction: %f\n", initial_range, jump_range, num_jumps, initial_power, power_reduction);

    int x, y, current_pp, max_pp;
    char name[101] = "";

    int size=0;
    
    Node *prev = NULL;

    while(scanf("%d %d %d %d %s", &x, &y, &current_pp, &max_pp, name) == 5){
        //printf("X-cood: %i, Y-cood: %i, current power: %i, max power: %i, name: %s \n", x, y, current_pp, max_pp, name);

        Node *player = (Node *)malloc(sizeof(Node));

        player->x = x;
        player->y = y;
        player->cur_PP = current_pp;
        player->max_PP = max_pp;

        player->name = malloc(strlen(name) + 1);
        strcpy(player->name, name);

        player->prev = prev;

        //printf("After assignment\n");
        //printf("X-cood: %i, Y-cood: %i, current power: %i, max power: %i, name: %s \n", player->x, player->y, player->cur_PP, player->max_PP, player->name);

        prev = player;
        size++;
    }

    //With C, you need to use a linked data structure to read an arbitraty number of elements, 
    //and when you're done, you can allocate and create an array.
    Node *players[size];

    //does the order in which I assign the players into the array matter?
    //actually I am assuming yes because I do not have a next so you can only go backward
    // while(prev != NULL){
    //     printf("X-cood: %i, Y-cood: %i, current power: %i, max power: %i, name: %s \n", prev->x, prev->y, prev->cur_PP, prev->max_PP, prev->name);

    //     prev = prev->prev;
    // }

    for(int i = 0; i < size; i++){
        players[i] = prev;
        printf("X-cood: %i, Y-cood: %i, current power: %i, max power: %i, name: %s \n", players[i]->x, players[i]->y, players[i]->cur_PP, players[i]->max_PP, players[i]->name);
        prev = prev->prev;
    }




    return 0;
}
