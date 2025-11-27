/*
adventure game
*/
#include <stdio.h>
#include <string.h>

typedef struct
{
    char descr[100];
    int c[6];
} TLoc;

typedef struct
{   char descr[50];
    int loc;
} TObj;


void printobjloc( TObj a[], int nobjs, int cloc)
{              int i;
    for (i=0;i<nobjs;i++)
    {
        if (a[i].loc==cloc)
        {
            printf("Blepeis: %s\n", a[i].descr);
        }
    }
}

int main (void)
{
    TLoc a[6];
    TObj objs[3];
    int x, xx, d;
    bool movement;
    char cmd_input[10], obj_input[15];

    strcpy(a[0].descr, "Eisai sto dasos. Blepeis ena monopati boreia.");
    strcpy(a[1].descr, "Eisai sto ksefwto. Blepeis monopatia notia, anatolika kai dytika.");
    strcpy(a[2].descr, "Eisai sth pediada.");
    strcpy(a[3].descr, "Eisai sthn oxthh enos potamou. Ena monopati sta anatolika odhgei sto ksefwto.");

    /*   Dimiourgia xarth me dinatothta pano kato (6 kateuthinseis)   */
    a[0].c[0]=1;
    a[0].c[1]=-1;
    a[0].c[2]=-1;
    a[0].c[3]=-1;
    a[0].c[4]=-1;
    a[0].c[5]=-1;


    a[1].c[0]=-1;
    a[1].c[1]=2;
    a[1].c[2]=0;
    a[1].c[3]=3;
    a[1].c[4]=-1;
    a[1].c[5]=-1;

    a[2].c[0]=-1;
    a[2].c[1]=-1;
    a[2].c[2]=-1;
    a[2].c[3]=1;
    a[2].c[4]=-1;
    a[2].c[5]=-1;

    a[3].c[0]=-1;
    a[3].c[1]=1;
    a[3].c[2]=-1;
    a[3].c[3]=-1;
    a[3].c[4]=-1;
    a[3].c[5]=-1;
    /*  Telos dhmioyrgias xarth */


    strcpy(objs[0].descr, "Ena fanari.");
    objs[0].loc=2;

    strcpy(objs[1].descr, "Ena kibwtio.");
    objs[1].loc=2;

    strcpy(objs[2].descr, "Ena sako.");
    objs[2].loc=3;

    x=0;

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


    do
    {
        printf("%s\n",a[x].descr);
        printobjloc(objs, 3, x);

        printf("Pou phgaineis? (BOREIA/NOTIA/ANATOLIKA/DYTIKA/ANEBA/KATEBA/KATALOGO)\n");
        scanf("%s", cmd_input);

        /* Directions*/
        if(strcmp(cmd_input,"BOREIA")==0) {d=0; movement=true;}
        else if(strcmp(cmd_input,"ANATOLIKA")==0) {d=1; movement=true;}
        else if(strcmp(cmd_input,"NOTIA")==0) {d=2; movement=true;}
        else if(strcmp(cmd_input,"DYTIKA")==0) {d=3; movement=true;}
        else if(strcmp(cmd_input,"ANEBA")==0) {d=4; movement=true;} //pros to paron mi diathesimo logo 4 kateuthinsewn se auton ton xarth
        else if(strcmp(cmd_input,"KATEBA")==0) {d=5; movement=true;} //pros to paron mi diathesimo logo 4 kateuthinsewn se auton ton xarth
        else if(strcmp(cmd_input,"KATALOGO")==0) movement =false;

        //entoles kinishw
        if (d!=9 && movement)
        {
            xx=a[x].c[d];
            if(xx==-1)
                printf("Den mporeis!\n");
            else
                x=xx;
        }

        //Katalogos antikeimenwn sto inventory kai entoles lipseis kai aporhceis antikeimenwn
        else if (!movement)
        {
            printf("Exeis epano sou: \n");
            int i = 0;
            for (i = 0; i < 3; i++)
            {
                if (objs[i].loc == -1) // obj me loc = -1 simenei oti briskete sto invetory mou
                {
                    printf("    %s\n", objs[i].descr);
                }
            }
            printf("\nTi theleis na keneis? (ASE obj, PARE obj)\n");
            scanf("%s %s", cmd_input, obj_input);
            int j = 0;
            for (j = 0; j < 3; j++)
            {
                    if (strstr(objs[j].descr, obj_input) != NULL)
                    {
                        if(strcmp(cmd_input,"PARE")==0) objs[j].loc = -1;
                        else if (strcmp(cmd_input,"ASE")==0) objs[j].loc = x;
                    }
            }
        }
        printf("\n==========================================================\n");
    } while( d!=9);

    printf("Telos paixnidiou.\n");

    return 0;
}
//9 = exit
//d = direction of movement (0=boreia) (1=anatolika) (2=notia) (3=dutika)
//obj me loc = -1 briskete sto invetory mou
