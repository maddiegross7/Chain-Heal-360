#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h> 

//./chain_heal 2 1 4 500 0.25 < small.txt  
//./chain_heal 1 2 4 500 0.25 < small.txt 

typedef struct node{
    char *name;
    int x, y;
    int cur_PP, max_PP;
    struct node *prev;
    int adj_size;
    struct node **adj;
    int visited;
    int withinInitialRange;
} Node;

typedef struct game{
    int initial_range;
    int jump_range;
    int num_jumps;
    int initial_power;
    double power_reduction;
    
} Game;

void DFS(Node *player, int hopNumber, Game *game){
    //printf("Hello");

    if(hopNumber > game->num_jumps){
        //printf("Node: %s Hop %i\n", player->name, hopNumber);
        return;
    }

    if(player == NULL || player->visited == 1){
        //printf("Node: %s Hop %i\n", player->name, hopNumber);
        return;
    }
    player->visited = 1;
    //printf("Visiting: %s\n", player->name);

    printf("Node: %s Hop %i\n", player->name, hopNumber);

    for(int i = 0; i < player->adj_size; i++){
        //printf("Calling from %s loop\n", player->name);
        DFS(player->adj[i], hopNumber+1, game);
    }

    player->visited = 0;
}

int main(int argc, char const *argv[])
{
    //printf("hello world\n");

    if(argc != 6){
        printf("Usage: %s <initial_range> <jump_range> <num_jumps> <initial_power> <power_reduction>\n", argv[0]);
        return 1;
    }
    
    Game game;

    game.initial_range = atoi(argv[1]);
    game.jump_range = atoi(argv[2]);
    game.num_jumps = atoi(argv[3]);
    game.initial_power = atoi(argv[4]);
    game.power_reduction = atof(argv[5]);

    //printf("Initial range: %i, Jump Range: %i, Num of Jumps: %i, Initial Power: %i, Power Reduction: %f\n", initial_range, jump_range, num_jumps, initial_power, power_reduction);

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

        //player->name = malloc(strlen(name) + 1);
        player->name = strdup(name);

        player->prev = prev;

        player->adj_size = 0;
        player->visited = 0;
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

    //putting it into the array
    for(int i = size-1; i >= 0 ; i--){
        players[i] = prev;
        //printf("X-cood: %i, Y-cood: %i, current power: %i, max power: %i, name: %s \n", players[i]->x, players[i]->y, players[i]->cur_PP, players[i]->max_PP, players[i]->name);
        prev = prev->prev;
    }

    for(int i=0; i < size; i++){
        int x = players[i]->x;
        int y = players[i]->y;

        int initialRangeSquared = game.initial_range*game.initial_range;

        for(int j=0; j < size; j++){
            //he can jump diagnally!!! YOU HAVE TO CHANGE THIS
            int xDiff = (players[j]->x - x);
            int yDiff = (players[j]->y - y);

            //printf("XDiff: %i, YDiff: %i\n", xDiff, yDiff);

            int hopsNeeded = (xDiff*xDiff) + (yDiff*yDiff);

            if((hopsNeeded <= (game.jump_range*game.jump_range)) && players[j]->name != players[i]->name){
                //printf("Adding to size of %s\n", players[i]->name);
                players[i]->adj_size++;
            }

            //Urgosa_the_Healing_Shaman
            //do something here that if initial players name matches this and 
            //current player is within initial range you set the flag to true
            //printf("Hops needed: %i RangeSquared: %i\n", hopsNeeded, initialRangeSquared);
            if(hopsNeeded <= initialRangeSquared && strcmp("Urgosa_the_Healing_Shaman", players[i]->name)==0){
                //printf("%s within range of Urgosa\n", players[j]->name);
                players[j]->withinInitialRange = 1;
            }

            
        }

        players[i]->adj = malloc((players[i]->adj_size)*sizeof(Node *));

        int neighborsFound = 0;

        for(int j=0; j < size; j++){
            if(players[j]->name != players[i]->name){
                int xDiff = abs((players[j]->x - x));
                int yDiff = abs((players[j]->y - y));

                //printf("XDiff: %i, YDiff: %i\n", xDiff, yDiff);

                int hopsNeeded = xDiff + yDiff;

                if(hopsNeeded <= game.jump_range){
                    //printf("Adding %s to adj list of %s\n",players[j]->name, players[i]->name);
                    players[i]->adj[neighborsFound] = players[j];
                    neighborsFound++;
                }

                
            }
        }
    }

    //just for checking
    // for(int i = 0; i < size; i++){
    //     printf("Name: %s Links: %i \n", players[i]->name, players[i]->adj_size);

    //     for(int j = 0; j < players[i]->adj_size; j++){
    //         Node *neighbor = players[i]->adj[j];
    //         //printf("hello\n");
    //         if(neighbor != NULL){
    //             printf("  -> %s \n", neighbor->name);
    //         }
    //     }
    // }

    for(int i = 0; i < size; i++){
        //printf("We are calling DFS from the beginning again\n");
        if(players[i]->withinInitialRange == 1){
            DFS(players[i], 1, &game);
        }
        
    }

    return 0;
}
