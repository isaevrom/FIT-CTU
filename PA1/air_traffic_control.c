#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

typedef struct {
    double x;
    double y;
    char* voyage;
} Flight;

void free_Me( Flight *st_flights, int amount ) {
    for ( int t = 0; t < amount ; t++ )
    {
        //printf ( "FKEK\n" );
        //printf ( "str[%d] = %s\n", t, st_flights[t].voyage );
        free( st_flights[t].voyage );
    }
    free(st_flights);
}

void free_Special( Flight *strct_flights, int _amount ) {
    //printf ( "1st _amount = %d\n", _amount );
    for ( int e = 0; e < _amount + 1; e++ )
    {
        //printf ( "FKEK\n" );
        //printf ( "str[%d] = %s\n", t, st_flights[t].voyage );
        //printf ( "_amount = %d\n", _amount );
        free( strct_flights[e].voyage );
    }
    free(strct_flights);
}

double min_Dist ( Flight *strct, int num_of_structs ) {
    double distance = -1, calc_dist;
    for ( int p = 0; p < num_of_structs ; p++ )
    {
        for ( int q = 0; q < num_of_structs; q++ )
        {
            if ( p == q ) continue; // nechceme pocitat [0] a [0], nebo [1] a [1]
                
            calc_dist = sqrt( pow( strct[p].x - strct[q].x , 2 ) + pow( strct[p].y - strct[q].y, 2 ) );
            if ( distance == -1 || calc_dist <= distance )
                distance = calc_dist;
        }
    }
    
    return distance;
}



void calc_Dist ( Flight *the_struct, int num_structs, double the_minimum ) {
    double tmp;
    //int *arr = (int*)malloc( sizeof num_structs );
    // uloz j ktere se shodovalo do promenne a pak vypis po for (j) ten string ve kterem byl stejny distance
    //printf ( "num of structs: %d\n", num_structs );
    
    printf ( "Nejmensi vzdalenost: %f\n", the_minimum );
    
    for ( int i = 0; i < num_structs; i++ )
    {
        for ( int j = i ; j < num_structs ; j++ )
        {
            if ( i == j ) continue;
            tmp = sqrt( pow( the_struct[i].x - the_struct[j].x , 2 ) + pow( the_struct[i].y - the_struct[j].y, 2 ) );
            if ( fabs(the_minimum - tmp ) <= the_minimum * DBL_EPSILON * 10e4 )
            {
                printf ( "%s - %s\n", the_struct[i].voyage, the_struct[j].voyage );
            }
        }
    }
    
    for ( int k = 0 ; k < num_structs; k++ )
    {
        /*if ( strlen(the_struct[k].voyage) == 0 )
            continue;
        else*/
        free(the_struct[k].voyage);
    }
    free(the_struct);
    
    //printf ( "calc_Dist THE END !\n" );
}


int main ( void ) 
{
    char *user_input, end_parenth;
    size_t buffer = 32;
    //size_t length = 0;
    int length = 0;
    int cnt = 0;
    int flag;
    double min_distance;
    
    printf ( "Zadejte lety:\n" );
    
    Flight the_flight, *fly_emirates;
    fly_emirates = &the_flight;
    fly_emirates = ( Flight * )malloc( sizeof(Flight) );
    
    while (  1  )
    {
        //fly_emirates = ( Flight * )malloc( sizeof(Flight));
        if ( cnt )
        {
            fly_emirates = ( Flight* )realloc( fly_emirates , sizeof(Flight) * (cnt+1) );
        }
        user_input = (char*)malloc( sizeof(char) * buffer );

        
        if ( ( length = getline( &user_input, &buffer, stdin ) ) == EOF )
        {
            if ( cnt >= 2 )
            {
                free ( user_input );
                min_distance = min_Dist( fly_emirates, cnt );
                calc_Dist( fly_emirates, cnt, min_distance );   // pokud vse probehne ok tak se vse uvolni v calc_Dist
                return 0;
            }
            //printf ( "EOF\n" ); // delete
            // pokud cnt = 0 nebo cnt = 1 //
            free_Me ( fly_emirates, cnt );
            free(user_input);
            break;
        }
        
        fly_emirates[cnt].voyage = (char*)malloc( sizeof(char) * length );
        
        if ( (flag = sscanf ( user_input, " [ %lf , %lf %c %[^\n]s", &fly_emirates[cnt].x, &fly_emirates[cnt].y, &end_parenth, fly_emirates[cnt].voyage )) < 3 || end_parenth != ']' )
        {
            //printf ( "flag = %d c = %c\n", flag, end_parenth );
            printf ( "Nespravny vstup.\n" );
            
            free_Special( fly_emirates, cnt );
            free   ( user_input );
                        
            
            return 1;
        }
        
        if ( flag == 3 )    // do fly_emirates[cnt].voyage se dostal '\n'
        {
            //printf ( "fly = NEW_LINE!" );
            //fly_emirates[cnt].voyage[0] = '\0';
            fly_emirates[cnt].voyage[0] = '\0';
            //printf ( "%lu\n", strlen(fly_emirates[cnt].voyage) );
        }
      
        cnt++;
        free( user_input );
    }
    
    // sem se dostaneme pouze s cnt = 0 nebo cnt = 1 //
    if ( cnt < 2 )
        printf ( "Nespravny vstup.\n" );    // LESS THAN 2 FLIGHTS
    
    return 0;
}
