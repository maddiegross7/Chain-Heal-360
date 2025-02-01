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

    printf("starting with %s, hop: %i current power: %i, max power %i\n", player->name,hopNumber, player->cur_PP, player->max_PP);
    //base cases
    // if(hopNumber > game->num_jumps || player == NULL || player->visited == 1){
    //     printf("base case reached\n");
    //     return;
    // }

    if (hopNumber > game->num_jumps) {
        //printf("Base case reached: hopNumber exceeded num_jumps\n");
        return;
    }

    if (player == NULL) {
        //printf("Base case reached: player is NULL\n");
        return;
    }

    if (player->visited == 1) {
        //printf("Base case reached: player already visited\n");
        return;
    }


    player->visited = 1;

    int fullHealing = player->cur_PP + total_healing;//what is possible to be healed
    int playerPower = fmin(fullHealing, player->max_PP);//the power level of player after healing
    int actuallyHealed = (playerPower-player->cur_PP);//how much the player was actually healed
    player->healing = actuallyHealed;

    player->prev = from;

    int allHealing = 0;
    // for(int i=0; i < hopNumber; i++){
    //     allHealing += game->healing[i];
    //     printf("Hop %i healing amount: %i\n",i+1, game->healing[i]);
    // }

    Node *current = player;

    for(int i=hopNumber-1; i >=0; i--){
        allHealing += current->healing;
        current = current->prev;
    } 

    printf("allHealing: %i", allHealing);

    printf("Who: %s, Hop #: %i, Total Healing: %i\n", player->name, hopNumber, allHealing);

    if(allHealing > game->best_healing){
        printf("Old score: %i, New Score: %i\n", game->best_healing, allHealing);
        game->best_healing = allHealing;
        game->best_path_length = hopNumber;
        Node *current = player;

        for(int i=hopNumber-1; i >=0; i--){
            // printf("player: %s, ", player->name);
            // if(player->prev != NULL){
            //     printf("prev: %s\n", player->prev->name);
            // }
            //printf("\n");
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
    //player->cur_PP -= actuallyHealed;
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
    game.healing = malloc((game.num_jumps)*sizeof(int));
    game.best_healing = 0;
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
            //printf("initial range squared: %i, hopsNeeded: %i\n", initialRangeSquared, hopsNeeded);

            if((hopsNeeded <= (game.jump_range*game.jump_range)) && strcmp(players[j]->name, players[i]->name) != 0){
                players[i]->adj_size++;
                //printf("player within jumprange\n");
            }

            if(hopsNeeded <= initialRangeSquared && strcmp("Urgosa_the_Healing_Shaman", players[i]->name)==0){
                players[j]->withinInitialRange = 1;
                //printf("%s is within initial range\n", players[j]->name);
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
                //printf("we are adding to adjacency list!");
                neighborsFound++;
            }
        }
    }

    for(int i = 0; i < size; i++){
        if(players[i]->withinInitialRange == 1){
            DFS(players[i], 1, &game, game.initial_power, players[i]->prev);
        }
    }

    //printf("heyyy");
    for(int i = 0; i < game.best_path_length; i++){
        printf("%s %i\n", game.best_path[i]->name, game.healing[i]);
    }

    printf("Total_Healing %i\n", game.best_healing);

    return 0;
}
