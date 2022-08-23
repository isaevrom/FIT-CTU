#include <stdio.h>
#define MAX 69

int main ( void ) 
{
	int pal_check;
	int n, arr[MAX], save_n, base;  
	int posit = 0, pal_cnt = 0;
	int lo, hi;
	char C;

	printf ( "Vstupni intervaly:\n" );	

   while ( ( scanf ( " %c", &C ) ) != EOF ) 
   {  
	//printf ( "C = %c\n", C );
	if ( (C != 'c' && C != 'l')  ) {
		printf ( "Nespravny vstup.\n" );
		return 1;
	}		
	pal_cnt = 0;

	if ( scanf ( " %d%d%d", &base, &lo, &hi ) != 3 || lo < 0 || lo > hi || base < 2 || base > 36 ) {
		printf ( "Nespravny vstup.\n" );
		return 1;
	}	

 

	for ( int s = lo ; s <= hi ; s++ ) 
	{
		if ( !s ) {		// je-li s == 0
			if ( C == 'l' )	
				printf ( "0 = 0 (%d)\n", base );
			pal_cnt++;
			continue;
		}
		if ( base == 2 && s % 2 == 0 ) { 	// suda cisla v bin. repr. netvori palindrom 	
			continue;
		}		
		/* Nastaveni hodnot pro zpracovani kazdeho cisla z intervalu <lo;hi> */		
		n = s;				// n - dekadicke cislo z intervalu <lo;hi> ke zpracovani
		posit = 0, pal_check = 1;			
		save_n = n;			// Ulozeni hodnoty vstupniho cisla z int. <lo;hi> pro konecny vypis 


		while ( n > 0 ) 
		{
 		        arr[posit] = n % base;
 	 	        n = n / base;
 	 	        posit++;
		}

		for( int q = 0 ; q < posit ; q++ ) {
			if ( arr[q] != arr[posit - q - 1] )	// budou-li se 2 stejne vzdalena cisla lisit -> neni pal. 
  	 		{
  	    			pal_check = 0;
 	     			break;
 	  		}
		}			// 10101 posit = 5, i = 0 K, i = 1 K, i = 2 
					   
		if ( pal_check ) {	// vypisujeme palindrom
		
			//printf ( "Palindrom\n" );
			pal_cnt++;		

			if ( C == 'l' )	
			{	
				printf ( "%d = ", save_n );			
				for ( int j = 0 ; j < posit ; j++ ) 
				{
					if ( arr[j] <= 9  )					
						printf ( "%d", arr[posit - j - 1] );
					else // if arr[j] > 10
						printf ( "%c", arr[posit - j - 1] + 87 ); // vyuzijme ASCII tabulku: a=97, b=98, ..., z=122
				}
				printf ( " (%d)\n", base );
			}
		} 
	}
	if ( C == 'c' ) 
		printf ( "Celkem: %d\n", pal_cnt );		
   }

	return 0;
}
