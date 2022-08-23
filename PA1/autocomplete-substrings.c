#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    double x;
    char *voyage;
} String;

/*
 fce ktera uvolni "stringy" v kazdem structu v ramci pole
 pote uvolni pole structu samotne
 */
void free_Me( String *st_flights, int amount ) {
    for ( int t = 0; t < amount ; t++ )
    {
        free( st_flights[t].voyage );
    }
    free(st_flights);
}

/*
 fce pomahajici quicksortu seradit structy dle velikosti zadane cetnosti
*/
int compare_Structs (const void * a, const void * b)
{

  String *StringA = (String *)a;
  String *StringB = (String *)b;

  return ( StringB->x - StringA->x );
}

void sort_Strings ( String *struct_to_sort, int num ) {
    qsort ( struct_to_sort , num, sizeof( String ), compare_Structs );
}

/*
 - implementace vlastni fce resici substringy
 - pokud pokud str_2 (substring) neni nalezen v str_1 (puvodni string), tak se vrati hodnota 0, jinak hodnota 1
 - pokud hodnota promenne print je 1, tak se provede vypis nalezenych subtringu, jinak ne
*/
int is_Sub( char *str_1, char *str_2, int print )
{
    int flag;
    int len_1 = strlen(str_1);
    int len_2 = strlen(str_2);
    
    for ( int i = 0; str_1[i] != '\0' ; i++ )
    {
        flag = 1;
        if ( len_1 - i < len_2 - 1 )
        {
            //printf ( "TOO SHORT.\n" );
            break;
        }
            
        for ( int j = 0; str_2[j] != '\0'; j++ )
        {
            if ( tolower(str_2[j]) != tolower(str_1[i+j]) )
            {
                flag = 0;
                break;
            }
        }
        /* je-li flag 1 --> jedna se o substring a tedy vracime hodnotu 1 */
        if ( flag )
        {
            if ( print ) printf ( "> %s\n", str_1 );
            return 1;
        }
    }
    
    return 0;
}

void sub_String ( String *my_strings, int amount_of_strings ) {
    if ( amount_of_strings != 1 ) // chceme vice jak jeden struct
        sort_Strings( my_strings, amount_of_strings );
    int nalezeno = 0;
    
    char *input;
    size_t buf = 32;
    
    printf ( "Hledani:\n" );
    while ( 1 )
    {
        nalezeno = 0;
        input = (char*)malloc( sizeof(char) * buf );
        if ( getline ( &input, &buf, stdin ) == EOF )
        {
            // zde program konci
            free_Me( my_strings , amount_of_strings );
            free(input);
            break;
        }
        
        if ( input[strlen(input)-1] == '\n' )
        {
            input[strlen(input)-1] = '\0';
        }
        
        for ( int t = 0; t < amount_of_strings; t++ )
        {
            nalezeno += is_Sub( my_strings[t].voyage, input, 0 );
        }
        printf ( "Nalezeno: %d\n", nalezeno );
        for ( int t = 0; t < amount_of_strings; t++ )
        {
            /* tento for loop resi vypis */
            nalezeno += is_Sub( my_strings[t].voyage, input, 1 );
        }
        free(input);
    }
    
}

int main ( void )
{
    char *user_input;
    size_t buffer = 32;
    int length = 0;
    int cnt = 0;
    int flag;
    
    printf ("Casto hledane fraze:\n" );
    
    String _string, *the_string;
    the_string = &_string;
    the_string = ( String * )malloc( sizeof( String ) );
    
    while (  1  )
    {
        if ( cnt )
        {
            the_string = ( String* )realloc( the_string , sizeof( String ) * (cnt+1) );
        }
        user_input = (char*)malloc( sizeof(char) * buffer );

        
        if ( ( length = getline( &user_input, &buffer, stdin ) ) == EOF )
        {
            printf ( "Nespravny vstup.\n" );
            free   ( user_input );
            free_Me( the_string, cnt );
            return 1;
        }
        
        if ( strlen ( user_input ) == 1 && user_input[0] == '\n' )
        {
            if ( cnt == 0 )   // pokud se to stane hned na zacatku
            {
                printf ( "Nespravny vstup.\n" );
                free( user_input );
                free( the_string );
                return 1;
            }
            // jinak spust zbytek programu
            free      ( user_input );
            sub_String( the_string, cnt );
            return 0;
        }
        
        the_string[cnt].voyage = (char*)malloc( sizeof(char) * length );

        the_string[cnt].x = -1.0;
        
        if ( ( flag = sscanf ( user_input, "%lf:%[^\n]s", &the_string[cnt].x, the_string[cnt].voyage )) < 2 )
        {
            printf ( "Nespravny vstup.\n" );
            free_Me( the_string, cnt + 1 );
            free   ( user_input );
         
            return 1;
        }
        
      
        cnt++;
        free( user_input );
    }

    return 0;
}
