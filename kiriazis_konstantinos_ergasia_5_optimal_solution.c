#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct 
{
    double x, y;
    bool is_covered;
    bool has_school;
    double MeanDistanceFromOther;
} village;

double get_Distance(village a, village b) 
{
    return sqrt((a.x - b.x) * (a.x - b.x) +  (a.y - b.y) * (a.y - b.y));
}

int main() 
{
    int village_count = 0;
    int school_count = 0;
    double max_distance;

    printf("Dose arithmo xorion? ");
    scanf("%d", &village_count);
    printf("Dose tin megisti apostash d: ");
    scanf("%lf", &max_distance);
    while(getchar() != '\n');

    village villages[village_count];
    village ordered_villages[village_count];

    int i;
    for(i = 0; i<village_count; i++)
    {
	villages[i].is_covered = false;
	villages[i].has_school = false;
        ordered_villages[i].is_covered = false;
	ordered_villages[i].has_school = false;
	villages[i].MeanDistanceFromOther = 0;
    }
    
    for(i = 0; i < village_count; i++) 
    {
        printf("\nXorio No%d dose syntetagmenes me morfh x,y : ", i+1);
        scanf("%lf,%lf", &villages[i].x, &villages[i].y);
	while(getchar() != '\n');
    }

    //eyresi twn kaliptomenon xvriwn ana xvriw
    int j;
    for(i = 0; i<village_count; i++)
    {
	for(j=0; j<village_count; j++)
	{
	    if((get_Distance(villages[i], villages[j]) <= max_distance) && (i != j))
	    {
		villages[i].MeanDistanceFromOther = villages[i].MeanDistanceFromOther + 1;
	    }
	}
    }

    //Taxinomisi twn xwriwn me bash thn megisti kalypsi twn geitwnikwn tous
    for(i=0; i<village_count; i=i)
    {
	int max_coverage = 0;
	for(j = 0; j<village_count; j++)
	{
	    if(villages[j].MeanDistanceFromOther > max_coverage)
	    {
		max_coverage = villages[j].MeanDistanceFromOther;
	    }
	}

	for(j=0; j<village_count; j++)
        {
            if(max_coverage == villages[j].MeanDistanceFromOther)
            {
		ordered_villages[i].MeanDistanceFromOther = max_coverage;
		ordered_villages[i].x = villages[j].x;
		ordered_villages[i].y = villages[j].y;
		villages[j].MeanDistanceFromOther = -1;
                i++;
            }
        }
    }
    
    //xekinao apo to proto xorio
    for(i = 0; i < village_count; i++) 
    {
        if (ordered_villages[i].is_covered==false ) // ean den kalyptete to xorio apo sxoleio
	{
                ordered_villages[i].has_school=true;  //xtizo sxoleio sto xorio;
                ordered_villages[i].is_covered=true;  //symeiono oti to xorio kalyptete tora
                school_count++;

            // elegxo kai simeiono poia alla xoria kalyptontai me to neo sxoleio se apostasi ≤ max_distance
            int j;
            for(j = 0; j < village_count; j++) 
	    {
                if (ordered_villages[j].is_covered==false && get_Distance(ordered_villages[i], ordered_villages[j]) <= max_distance) 
		{
                    ordered_villages[j].is_covered=true;
                }
            }
        }
    }

    printf("\n-------------------------------------------------------------------\n");
    printf("O arithmos ton sxoleion einai %d kai briskontai sta parakatv xoria:\n" , school_count);
    for(i = 0; i < village_count; i++) 
    {
        if (ordered_villages[i].has_school==true )
            printf("  Sto xorio me syntetagmenes (%.2f, %.2f)\n", ordered_villages[i].x, ordered_villages[i].y);
    }

    return 0;
}