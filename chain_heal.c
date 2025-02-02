//Lab 1: Chain Heal
//Madelyn Gross

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h> 

//./chain_heal 2 1 4 500 0.25 < small.txt  
//./chain_heal 1 2 4 500 0.25 < small.txt 
//./chain_heal 2 1 4 500 0.25 < small.txt | /home/jplank/cs360/labs/Lab-1-Chain-Heal/gradeall

typedef struct node{
    char *name;
    int x, y;
    int cur_PP, max_PP;
    struct node *prev;
    int adj_size;
    struct node **adj;
    int visited;
    int withinInitialRange;
    int healing;
} Node;

typedef struct game{
    int initial_range;
    int jump_range;
    int num_jumps;
    int initial_power;
    double power_reduction;
    int best_healing;
    int best_path_length;
    Node **best_path;
    int *healing;
} Game;

void DFS(Node *player, int hopNumber, Game *game, double total_healing, Node *from){

    //base cases
    if (hopNumber > game->num_jumps) {
        return;
    }
    if (player == NULL) {
        return;
    }
    if (player->visited == 1) {
        return;
    }

    player->visited = 1;
    
    //need to keep these two as doubles so we do not end up with an off by one error
    double fullHealing = player->cur_PP + total_healing;//what is possible to be healed
    double playerPower = fmin(fullHealing, player->max_PP);//the power level of player after healing
    int actuallyHealed = rint(playerPower-player->cur_PP);//how much the player was actually healed
    player->healing = actuallyHealed;

    player->prev = from;

    int allHealing = 0;

    Node *current = player;
    for(int i=hopNumber-1; i >=0; i--){
        allHealing += current->healing;
        current = current->prev;
    } 

    if(allHealing > game->best_healing){
        game->best_healing = allHealing;
        game->best_path_length = hopNumber;
        Node *current = player;

        for(int i=hopNumber-1; i >=0; i--){
            game->best_path[i] = current;
            game->healing[i] = current->healing;
            current = current->prev;
        }        
    }

    double reducedHealing = (total_healing * (1-game->power_reduction));
    for(int i = 0; i < player->adj_size; i++){
        DFS(player->adj[i], hopNumber+1, game, reducedHealing, player);
    }
    player->visited = 0;
}

int main(int argc, char const *argv[])
{
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
    game.best_path = malloc((game.num_jumps)*sizeof(Node *));
    game.best_healing = 0;

    game.healing = malloc((game.num_jumps)*sizeof(int));
    for (int i = 0; i < game.num_jumps; i++) {
        game.healing[i] = 0;
    }

    int x, y, current_pp, max_pp;
    char name[101] = "";

    int size=0;
    Node *prev = NULL;

    while(scanf("%d %d %d %d %s", &x, &y, &current_pp, &max_pp, name) == 5){

        Node *player = (Node *)malloc(sizeof(Node));

        player->x = x;
        player->y = y;
        player->cur_PP = current_pp;
        player->max_PP = max_pp;
        player->healing = 0;
        player->name = strdup(name);
        player->prev = prev;
        player->adj_size = 0;
        player->visited = 0;
        prev = player;
        size++;
    }

    //With C, you need to use a linked data structure to read an arbitraty number of elements, 
    //and when you're done, you can allocate and create an array.
    Node *players[size];

    //putting it into the array
    for(int i = size-1; i >= 0 ; i--){
        players[i] = prev;
        prev = prev->prev;
        players[i]->prev = NULL;
    }

    for(int i=0; i < size; i++){
        int x = players[i]->x;
        int y = players[i]->y;

        int initialRangeSquared = game.initial_range*game.initial_range;

        for(int j=0; j < size; j++){
            int xDiff = (players[j]->x - x);
            int yDiff = (players[j]->y - y);

            int hopsNeeded = (xDiff*xDiff) + (yDiff*yDiff);

            if((hopsNeeded <= (game.jump_range*game.jump_range)) && strcmp(players[j]->name, players[i]->name) != 0){
                players[i]->adj_size++;
            }

            if(hopsNeeded <= initialRangeSquared && strcmp("Urgosa_the_Healing_Shaman", players[i]->name)==0){
                players[j]->withinInitialRange = 1;
            }
        }

        players[i]->adj = malloc((players[i]->adj_size)*sizeof(Node *));

        int neighborsFound = 0;
        for(int j=0; j < size; j++){
            int xDiff = (players[j]->x - x);
            int yDiff = (players[j]->y - y);
            int hopsNeeded = (xDiff*xDiff) + (yDiff*yDiff);

            if((hopsNeeded <= (game.jump_range*game.jump_range)) && strcmp(players[j]->name, players[i]->name) != 0){
                players[i]->adj[neighborsFound] = players[j];
                neighborsFound++;
            }
        }
    }

    for(int i = 0; i < size; i++){
        if(players[i]->withinInitialRange == 1){
            DFS(players[i], 1, &game, game.initial_power, players[i]->prev);
        }
    }

    for(int i = 0; i < game.best_path_length; i++){
        printf("%s %i\n", game.best_path[i]->name, game.healing[i]);
    }

    printf("Total_Healing %i\n", game.best_healing);

    for (int i = 0; i < size; i++){
        free(players[i]->adj);
        free(players[i]->name);
        free(players[i]);
    }

    free(game.best_path);
    game.best_path = NULL;
    free(game.healing);
    game.healing = NULL;

    return 0;
}
