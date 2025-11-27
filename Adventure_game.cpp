/*
adventure game
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TOTAL_OBJECTS      3

////////Available Directions
#define NORTH              0
#define SOUTH              1
#define EAST               2
#define WEST               3
#define UP                 4
#define DOWN               5

////////////////Location IDs
#define NOWHERE           -1
#define FOREST_EDGE        0
#define FOREST_TRAIL       1
#define FOOTBRIDGE         2
#define MONOLITH           3
#define RIVERBANK          4
#define TOWER_ENTRANCE     5
#define CLEARING           6
#define CATACOMBS_ENTRANCE 7
#define HILLSIDE           8
#define TOWER_SUBLEVEL     9
#define CATACOMB_DEPTHS    10
#define CLIFF              11
#define TOMB               12
#define HIDDEN_PATH        13

class location
{
public:
    char name[25];
    char description[100];
    int id;
    int directions[6];
};

class object
{
public:
    char description[50];
    char name[20];
    int id;
    int loc;
};

location location[14];

void create_map()
{
    strcpy(location[FOREST_EDGE].description,   "A quiet treeline marking the boundary of the wild woods. Trails lead north and east.");
    strcpy(location[FOREST_TRAIL].description,  "A mossy path splitting in several directions. The trees whisper faintly.");
    strcpy(location[FOOTBRIDGE].description,    "A rotting wooden bridge over a dry gully. The forest thickens to the north.");
    strcpy(location[MONOLITH].description,      "A granite monolith covered in ancient runes. Paths run west, east, and south.");
    strcpy(location[RIVERBANK].description,     "A slow river curls through the forest. A narrow trail follows its southern edge.");
    strcpy(location[TOWER_ENTRANCE].description,"The base of a collapsed watchtower. Gaping stones offer a way inside.");
    strcpy(location[CLEARING].description,      "Sunlight filters through tangled branches. A faint trail heads east.");
    strcpy(location[CATACOMBS_ENTRANCE].description,"A stone arch descends into ancient burial tunnels.");
    strcpy(location[HILLSIDE].description,  "Jagged rocks and loose soil. A path climbs upward to the north.");
    strcpy(location[TOWER_SUBLEVEL].description,  "Dusty corridors beneath the broken tower. A narrow stair climbs back up.");
    strcpy(location[CATACOMB_DEPTHS].description, "Cold, stale air fills narrow stone passages.");
    strcpy(location[CLIFF].description, "A high vantage point with sweeping views of the valley below.");
    strcpy(location[TOMB].description, "An ornate burial room with cracked sarcophagi.");
    strcpy(location[HIDDEN_PATH].description, "A nearly invisible deer trail weaving through dense brush.");

    strcpy(location[FOREST_EDGE].name,        "Forest Edge");
    strcpy(location[FOREST_TRAIL].name,       "Forked Forest Trail");
    strcpy(location[FOOTBRIDGE].name,         "Old Footbridge");
    strcpy(location[MONOLITH].name,           "Lone Standing Stone");
    strcpy(location[RIVERBANK].name,          "Riverbank");
    strcpy(location[TOWER_ENTRANCE].name,     "Broken Tower Entrance");
    strcpy(location[CLEARING].name,           "Overgrown Clearing");
    strcpy(location[CATACOMBS_ENTRANCE].name, "Catacomb Archway");
    strcpy(location[HILLSIDE].name,           "Rocky Hillside");
    strcpy(location[TOWER_SUBLEVEL].name,     "Tower Sublevel");
    strcpy(location[CATACOMB_DEPTHS].name,    "Catacomb Depths");
    strcpy(location[CLIFF].name,              "Cliff Overlook");
    strcpy(location[TOMB].name,               "Tomb Chamber");
    strcpy(location[HIDDEN_PATH].name,        "Hidden Forest Path");

    location[FOREST_EDGE].id =        FOREST_EDGE;
    location[FOREST_TRAIL].id =       FOREST_TRAIL;
    location[FOOTBRIDGE].id =         FOOTBRIDGE;
    location[MONOLITH].id =           MONOLITH;
    location[RIVERBANK].id =          RIVERBANK;
    location[TOWER_ENTRANCE].id =     TOWER_ENTRANCE;
    location[CLEARING].id =           CLEARING;
    location[CATACOMBS_ENTRANCE].id = CATACOMBS_ENTRANCE;
    location[HILLSIDE].id =           HILLSIDE;
    location[TOWER_SUBLEVEL].id =     TOWER_SUBLEVEL;
    location[CATACOMB_DEPTHS].id =    CATACOMB_DEPTHS;
    location[CLIFF].id =              CLIFF;
    location[TOMB].id =               TOMB;
    location[HIDDEN_PATH].id =        HIDDEN_PATH;

    location[FOREST_EDGE].directions[NORTH] = FOOTBRIDGE;
    location[FOREST_EDGE].directions[SOUTH] = NOWHERE;
    location[FOREST_EDGE].directions[EAST]  = FOREST_TRAIL;
    location[FOREST_EDGE].directions[WEST]  = NOWHERE;
    location[FOREST_EDGE].directions[UP]    = NOWHERE;
    location[FOREST_EDGE].directions[DOWN]  = NOWHERE;

    location[FOREST_TRAIL].directions[NORTH] = MONOLITH;
    location[FOREST_TRAIL].directions[SOUTH] = NOWHERE;
    location[FOREST_TRAIL].directions[EAST]  = RIVERBANK;
    location[FOREST_TRAIL].directions[WEST]  = FOREST_EDGE;
    location[FOREST_TRAIL].directions[UP]    = NOWHERE;
    location[FOREST_TRAIL].directions[DOWN]  = NOWHERE;

    location[FOOTBRIDGE].directions[NORTH] = TOWER_ENTRANCE;
    location[FOOTBRIDGE].directions[SOUTH] = FOREST_EDGE;
    location[FOOTBRIDGE].directions[EAST]  = NOWHERE;
    location[FOOTBRIDGE].directions[WEST]  = NOWHERE;
    location[FOOTBRIDGE].directions[UP]    = NOWHERE;
    location[FOOTBRIDGE].directions[DOWN]  = NOWHERE;

    location[MONOLITH].directions[NORTH] = NOWHERE;
    location[MONOLITH].directions[SOUTH] = FOREST_TRAIL;
    location[MONOLITH].directions[EAST]  = CATACOMBS_ENTRANCE;
    location[MONOLITH].directions[WEST]  = CLEARING;
    location[MONOLITH].directions[UP]    = NOWHERE;
    location[MONOLITH].directions[DOWN]  = NOWHERE;

    location[RIVERBANK].directions[NORTH] = NOWHERE;
    location[RIVERBANK].directions[SOUTH] = NOWHERE;
    location[RIVERBANK].directions[EAST]  = HILLSIDE;
    location[RIVERBANK].directions[WEST]  = FOREST_TRAIL;
    location[RIVERBANK].directions[UP]    = NOWHERE;
    location[RIVERBANK].directions[DOWN]  = NOWHERE;

    location[TOWER_ENTRANCE].directions[NORTH] = NOWHERE;
    location[TOWER_ENTRANCE].directions[SOUTH] = FOOTBRIDGE;
    location[TOWER_ENTRANCE].directions[EAST]  = NOWHERE;
    location[TOWER_ENTRANCE].directions[WEST]  = NOWHERE;
    location[TOWER_ENTRANCE].directions[UP]    = NOWHERE;
    location[TOWER_ENTRANCE].directions[DOWN]  = TOWER_SUBLEVEL;

    location[CLEARING].directions[NORTH] = NOWHERE;
    location[CLEARING].directions[SOUTH] = NOWHERE;
    location[CLEARING].directions[EAST]  = MONOLITH;
    location[CLEARING].directions[WEST]  = NOWHERE;
    location[CLEARING].directions[UP]    = NOWHERE;
    location[CLEARING].directions[DOWN]  = NOWHERE;

    location[CATACOMBS_ENTRANCE].directions[NORTH] = NOWHERE;
    location[CATACOMBS_ENTRANCE].directions[SOUTH] = NOWHERE;
    location[CATACOMBS_ENTRANCE].directions[EAST]  = NOWHERE;
    location[CATACOMBS_ENTRANCE].directions[WEST]  = MONOLITH;
    location[CATACOMBS_ENTRANCE].directions[UP]    = NOWHERE;
    location[CATACOMBS_ENTRANCE].directions[DOWN]  = CATACOMB_DEPTHS;

    location[HILLSIDE].directions[NORTH] = CLIFF;
    location[HILLSIDE].directions[SOUTH] = NOWHERE;
    location[HILLSIDE].directions[EAST]  = NOWHERE;
    location[HILLSIDE].directions[WEST]  = RIVERBANK;
    location[HILLSIDE].directions[UP]    = NOWHERE;
    location[HILLSIDE].directions[DOWN]  = NOWHERE;

    location[TOWER_SUBLEVEL].directions[NORTH] = NOWHERE;
    location[TOWER_SUBLEVEL].directions[SOUTH] = NOWHERE;
    location[TOWER_SUBLEVEL].directions[EAST]  = NOWHERE;
    location[TOWER_SUBLEVEL].directions[WEST]  = NOWHERE;
    location[TOWER_SUBLEVEL].directions[UP]    = TOWER_ENTRANCE;
    location[TOWER_SUBLEVEL].directions[DOWN]  = NOWHERE;

    location[CATACOMB_DEPTHS].directions[NORTH] = NOWHERE;
    location[CATACOMB_DEPTHS].directions[SOUTH] = NOWHERE;
    location[CATACOMB_DEPTHS].directions[EAST]  = TOMB;
    location[CATACOMB_DEPTHS].directions[WEST]  = NOWHERE;
    location[CATACOMB_DEPTHS].directions[UP]    = CATACOMBS_ENTRANCE;
    location[CATACOMB_DEPTHS].directions[DOWN]  = NOWHERE;

    location[CLIFF].directions[NORTH] = NOWHERE;
    location[CLIFF].directions[SOUTH] = HILLSIDE;
    location[CLIFF].directions[EAST]  = HIDDEN_PATH;
    location[CLIFF].directions[WEST]  = NOWHERE;
    location[CLIFF].directions[UP]    = NOWHERE;
    location[CLIFF].directions[DOWN]  = NOWHERE;

    location[TOMB].directions[NORTH] = NOWHERE;
    location[TOMB].directions[SOUTH] = NOWHERE;
    location[TOMB].directions[EAST]  = NOWHERE;
    location[TOMB].directions[WEST]  = CATACOMB_DEPTHS;
    location[TOMB].directions[UP]    = NOWHERE;
    location[TOMB].directions[DOWN]  = NOWHERE;

    location[HIDDEN_PATH].directions[NORTH] = NOWHERE;
    location[HIDDEN_PATH].directions[SOUTH] = NOWHERE;
    location[HIDDEN_PATH].directions[EAST]  = NOWHERE;
    location[HIDDEN_PATH].directions[WEST]  = CLIFF;
    location[HIDDEN_PATH].directions[UP]    = NOWHERE;
    location[HIDDEN_PATH].directions[DOWN]  = NOWHERE;

}

void print_instructions(void)
{
    printf("======================= ODIGIES PAIXNIDIOU =======================\n");
    printf("Entoles Kinhsis: BOREIA, NOTIA, ANATOLIKA, DYTIKA, ANEBA, KATEBA\n");
    printf("Entoles Antikeimenon:\n");
    printf("Prwta anoigoyme ton kataloo gia na paroyme/afhsoyme kati\n");
    printf("    KATALOGO      - Deis ta antikeimena pou exeis\n");
    printf("    PARE <onoma>  - Pare antikeimeno\n");
    printf("    ASE <onoma>   - Afise antikeimeno\n");
    printf("    example: KATALOGO\n");
    printf("             PARE fanari\n");
    printf("Termatismos: dose 9\n");
    printf("=================================================================\n\n");
}

//void printobjloc( TObj a[], int nobjs, int cloc)
//{              int i;
//    for (i=0;i<nobjs;i++)
//    {
//        if (a[i].loc==cloc)
//        {
//            printf("Blepeis: %s\n", a[i].descr);
//        }
//    }
//}

int main (void)
{

    create_map();
    //print_instructions();

    int next_direction, current_position, next_position;
    bool movement;
    bool exit = false;
    char cmd_input[10], obj_input[15];


    current_position = 0;

    do
    {
        printf("You are at the \"%s\"\n", location[current_position].name);
        printf("%s\n", location[current_position].description);

        //printf("\n");
        scanf("%s", cmd_input);

        /* Directions*/
        if(strcmp(cmd_input,"NORTH")==0)      {next_direction=NORTH; movement=true;}
        else if(strcmp(cmd_input,"SOUTH")==0) {next_direction=SOUTH; movement=true;}
        else if(strcmp(cmd_input,"EAST")==0)  {next_direction=EAST;  movement=true;}
        else if(strcmp(cmd_input,"WEST")==0)  {next_direction=WEST;  movement=true;}
        else if(strcmp(cmd_input,"UP")==0)    {next_direction=UP;    movement=true;}
        else if(strcmp(cmd_input,"DOWN")==0)  {next_direction=DOWN;  movement=true;}
        else if(strcmp(cmd_input,"KATALOG")==0) movement =false;

        if (next_direction!=9 && movement)
        {
            next_position = location[current_position].directions[next_direction];

            if(next_position == -1)
                printf("Den mporeis!\n");
            else
                current_position = next_position;
        }

        printf("\n==========================================================\n");
    } while(!exit);

    printf("Telos paixnidiou.\n");

    return 0;
}
//9 = exit
//d = direction of movement (0=boreia) (1=anatolika) (2=notia) (3=dutika)
//obj me loc = -1 briskete sto invetory mou
