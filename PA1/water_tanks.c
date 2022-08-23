#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>
#define ROWS 200000
#define COLS 4
#define MAX_HEIGHTS 800000
bool easy_Case = false; // bool promenna na reseni trivialnich pripadu

/* fce compare_FOO seradi prvky od nejmensiho k nejvetsimu diky q_sortu */
int compare_FOO ( const void *one, const void *two ) {
    int *a = (int*)one;
    int *b = (int*)two;

    if ( *a < *b ) {
        return -1;
    } else if ( *a > *b ) {
        return  1;
    }
    return  0;
}

/* fce read_Bars precte parametry nadrzi */
int read_Bars ( int num_of_bars, int barrels[][4], double *max_height, int *Objem_celkovy )
{
    int Objem_barely  = 1;
    *Objem_celkovy    = 0;  // objem vsech barel dohromady
    
    for ( int i = 0; i < num_of_bars; i++ )
    {
          Objem_barely = 1;
          for ( int j = 0; j < COLS; j++ )
          {
              /*
                  barrels[][j = 0] -> Alt
                  barrels[][j = 1] -> H
                  barrels[][j = 2] -> W
                  barrels[][j = 3] -> D
              */
              /* nečíselné rozměry Alt, H, W, D */
              if ( scanf ( "%d", &barrels[i][j] ) != 1 )
              {
                  return 1;
              }

              // j = 1 --> H | j = 2 --> W | j = 3 --> D
              if ( j )
              {
                  if ( barrels[i][j] <= 0 ) /* nulové nebo záporné rozměry zásobníku H, W, D */
                        return 1;
                  Objem_barely *= barrels[i][j];
              }
                  
        }
        *Objem_celkovy += Objem_barely;
        
        // [i][0] --> Alt, [i][1] --> H, Alt + H == nadmorska vyska horejsku zasobniku
        if ( i == 0 || barrels[i][0] + barrels[i][1] > *max_height )
        {
            // probehne po 1. iteraci || kdyz nadmorska vyska noveho zasobniku bude vyssi nez predchozi
            *max_height = barrels[i][0] + barrels[i][1];
        }
    }
    
    return 0;
}

/* fce find_Height resi trivialni pripady: prazdne, pretece, a objem_vody == objem_nadrzi */
void find_Height ( int bars_volume, int water_volume, double *fmax_Height )
{
    if ( bars_volume < water_volume ) {
        // pretece
        easy_Case = true;
        printf ( "Pretece.\n" );
    } else if ( bars_volume == water_volume ) {
        // vrat nejvetsi nadmorskou vysku horejsku zasobniku
        easy_Case = true;
        printf ( "h = %f\n", *fmax_Height );
        
    } else if ( !water_volume ) {
        // objem vody je nulovy
        easy_Case = true;
        printf ( "Prazdne.\n" );
    }
}

/* fce compute_Height spocita vysku pro netrivialni pripady a vypise ji */
int *delete_Duplicates ( int barelky[][4], int pocet_barelek, int *pocet_altu )
{
    int *duplicity_alts;
    int *arr_of_alts = (int*)malloc( MAX_HEIGHTS * sizeof(int) );
    duplicity_alts   = (int*)malloc( MAX_HEIGHTS * sizeof(int) );
    int tmp, save_j = 0;
    int n = 0;
    for ( n = 0; n < pocet_barelek; n++ )
    {
        arr_of_alts[n] = barelky[n][0]; // ukladani altu
    }
    tmp = n + pocet_barelek;
    int pocet_arr_alts;
    for ( pocet_arr_alts = n; pocet_arr_alts < tmp; pocet_arr_alts++ )
    {
        arr_of_alts[pocet_arr_alts] = barelky[pocet_arr_alts-n][1] + barelky[pocet_arr_alts-n][0]; // ukladani H
    }
    qsort ( arr_of_alts, pocet_arr_alts, sizeof(int), compare_FOO );
    /* ulozeni vsech altu s odstranenim duplicit */
    for ( int j = 0 ; j < pocet_arr_alts; j++ )
    {
        if ( !j )
        {
            duplicity_alts[j] = arr_of_alts[0];
            save_j = 0;
            //printf ( " duplicity[%d] = %d\n", save_j, duplicity_alts[save_j] );
        }
        if ( arr_of_alts[j] > duplicity_alts[save_j] )
        {
            duplicity_alts[++save_j] = arr_of_alts[j];
            //printf ( " duplicity[%d] = %d\n", save_j, duplicity_alts[save_j] );
        }
    }
    *pocet_altu = ++save_j; // pocet prvku v poli duplicity_heights
    free(arr_of_alts); // uz nepotrebujeme, mame vysky v poli duplicity_alts

    return duplicity_alts;
}

int main ( void )
{
    int bars[ROWS][COLS], pocet_nadrzi, objem_vody, save_objem_vody, flag;
    int *alts_bez_duplicity = NULL, num_of_alts;//, size_alts_bez_duplicity;
    int celkovy_objem, big_tank_Surface = 0, big_tank_Volume, sum_of_V_big_tanks_filled = 0;
    double highest_level, water_height, sea_height;
    /*
       bars[r][c]      ~  nadrze[rows] + jejich popis[columns]
       bars[][0] (Alt) ~  nadmořská výška dna zásobníku
       bars[][1] (H)   ~  výška zásobníku
       bars[][2] (W)   ~  šířka zásobníku
       bars[][3] (D)   ~  hloubka zásobníku
       objem_vody      ~  objem nalevane vody
       flag            ~  reseni EOFu
       celkovy_objem   ~  celkovy objem nadrzi
       highest_level   ~  nejvyssi uroven vysky nadrze nad morem
    */
    
    /* [ 1 ] Pocet nadrzi: */
    printf ( "Zadejte pocet nadrzi:\n" );
    
    if ( scanf ( "%d", &pocet_nadrzi )
                != 1
                || pocet_nadrzi <= 0
                || pocet_nadrzi > 200000
    )
    {
        printf ( "Nespravny vstup.\n" );
        return 1;
    }
    /*solved*/
    
    
    /* [ 2 ] Cteni parametru nadrzi: */
    printf ( "Zadejte parametry nadrzi:\n" );
    
    if ( read_Bars( pocet_nadrzi, bars, &highest_level, &celkovy_objem ) )
    {
        /* Neuspesne cteni ve fci read_Bars:  */
        printf ( "Nespravny vstup.\n" );
        return 1;
    }
    /*solved*/
    qsort ( bars , pocet_nadrzi , sizeof bars[0] , compare_FOO ); // serazeni podle altu
 
    alts_bez_duplicity = delete_Duplicates( bars, pocet_nadrzi, &num_of_alts );
    /* [ 3 ] Reseni vysky: */
    printf ( "Zadejte objem vody:\n" );
    while ( ( flag = scanf ( "%d", &objem_vody ) ) != EOF  )
    {
        save_objem_vody = objem_vody;
        if ( !flag || objem_vody < 0 )
        {
            /* neciselne cteni objemu */
            printf ( "Nespravny vstup.\n" );
            return 1;
        }
        
        easy_Case = false;  // vzdy nastavim na false, jelikoz v find_Height se MUZE hodnota zmenit
        find_Height ( celkovy_objem, objem_vody, &highest_level );

        if ( easy_Case == false ) // kdyz to nejde trivialne vyresit
        {
            /* pocitani */
            big_tank_Surface = 0;
            sum_of_V_big_tanks_filled = 0;
            big_tank_Volume = 0;
            
            for ( int q = 0; q < num_of_alts; q++ )
            {
                big_tank_Surface = 0;
                big_tank_Volume  = 0;
                for ( int p = 0; p < pocet_nadrzi; p++ )
                {
                    if ( bars[p][0] <= alts_bez_duplicity[q] && bars[p][0] + bars[p][1] >= alts_bez_duplicity[q+1] )
                    {
                        big_tank_Surface += bars[p][2] * bars[p][3];
                    }
                }
                big_tank_Volume = big_tank_Surface * abs( alts_bez_duplicity[q] - alts_bez_duplicity[q+1] );
                objem_vody = objem_vody - big_tank_Volume;
                if ( objem_vody > 0 )
                    sum_of_V_big_tanks_filled += big_tank_Volume;
                if ( objem_vody <= 0 )
                {
                    water_height = ( save_objem_vody - sum_of_V_big_tanks_filled ) / (double)big_tank_Surface;
                    
                    sea_height = water_height + alts_bez_duplicity[q];
                    printf ( "h = %f\n", sea_height );
                    break;
                }
            }
        }
        
       
        
    }
   
    free(alts_bez_duplicity);
    /*solved*/
    

    return 0;
}
