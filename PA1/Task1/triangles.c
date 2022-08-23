#include <stdio.h>
#include <math.h>
#include <float.h>
#define Pi 3.14159265358979323846

double max( double L, double P ) {
	double max = L;
	if ( P > max )
		max = P;
	return max;
}


void sides_of_SUS ( double *b, double *U, double *c ) {
	double a;

        *U = *U * (Pi/180);

        a = sqrt ( pow (*b,2) + pow (*c,2) - 2*(*b)*(*c)*cos(*U) );
	*U = a;	// ulozime do U vypocitanou hodnotu --> ulozime ji do puvodni promenne z mainu
}

int sides_of_USU ( double *U_A, double *c, double *U_B ) {
	double a, b, U_C;     
        if ( *U_A + *U_B >= 180 ) {	// netvori trojuhelnik
                return 1;
        }
        U_C = 180 - *U_A - *U_B;
        a = ( *c/sin(U_C * Pi/180) ) * sin (*U_A * Pi/180);
	*U_A = a;						// uloz nalez. hodnotu do puvodni promenne
        b = ( *c/sin(U_C * Pi/180) ) * sin (*U_B * Pi/180);
	*U_B = b;						// uloz nalez. hodnotu do puvodni promenne
	
	return 0;
}	

int main (void)
{
	double A_o, B_o, C_p; // 2 odv. A, B a prepona C 1. troj. ABC
	double X_o, Y_o, Z_p; // 2 odv. X, Y a prepona Z 2. troj. XYZ
	double shoda, tmp;	

	char C1, C2, C3;	
	double val_1, val_2, val_3;	
	
	printf ( "Trojuhelnik #1:\n" );
	
	 if ( scanf ( " %c%c%c %lf %lf %lf", &C1, &C2, &C3, &val_1, &val_2, &val_3) !=6 || val_1 <= 0 || val_2 <= 0 || val_3 <= 0 || (C1 == 'U' && val_1 >= 180) || (C2 == 'U' && val_2 >= 180) || (C3 == 'U' && val_3 >= 180 ) ) {
                printf ( "Nespravny vstup.\n" );        
                return 1;
        }

	switch (C1) {		// C1 == 'S' v 'U'
		case 'S':				// SSS, SUS
			if ( C2 == 'S' && C3 == 'S' ) {		// veta SSS
				A_o = val_1;				
				B_o = val_2;
				C_p = val_3;
				break;				// opust switch a pokracuj
			} else if ( C2 == 'U' && C3 == 'S' ) { 	// veta SUS
				// SUS, fce?
				sides_of_SUS ( &val_1, &val_2, &val_3 );
				A_o = val_1;
				B_o = val_2;
				C_p = val_3;
			} else {				// !SSS, !SUS	
				printf ( "Nespravny vstup.\n" );				
				return 1;
			} 
			
			break;	//case break
		case 'U':				// USU	
			if ( C2 == 'S' && C3 == 'U' ) {
				// USU, fce?
				if ( sides_of_USU ( &val_1, &val_2, &val_3) == 1 ) {
					printf ( "Vstup netvori trojuhelnik.\n" );					
					return 1;
				} else {
					A_o = val_1;
					B_o = val_2;
					C_p = val_3;
				}
			} else {
				printf ( "Nespravny vstup.\n" );
				return 1;
			}

			break;	//case break;
		default:				// NEJAKY NESMYSL (LKE, JSC, 1Q3, ...)
			printf ( "Nespravny vstup.\n" );
			return 1;
	}
		
	/* Prohozeni hodnot stran tr. ABC dle velikosti, "a <= b <= c" */		
	if ( C_p < A_o ) {
		tmp = A_o;
		A_o = C_p;
		C_p = tmp;
	}
	if ( C_p < B_o ) {
		tmp = B_o;
		B_o = C_p;
		C_p = tmp;
	}
	if ( A_o > B_o ) {
		tmp = A_o;
		A_o = B_o;
		B_o = tmp;
	}
	/* solved */	
	
	if ( A_o + B_o - C_p <= 1024 * DBL_EPSILON * C_p ) {
		printf ( "Vstup netvori trojuhelnik.\n" );
		return 0;
	}
	
	printf ( "Trojuhelnik #2:\n" );	

	if ( scanf ( " %c%c%c %lf %lf %lf", &C1, &C2, &C3, &val_1, &val_2, &val_3) !=6 || val_1 <= 0 || val_2 <= 0 || val_3 <= 0 || (C1 == 'U' && val_1 >= 180) || (C2 == 'U' && val_2 >= 180) || (C3 == 'U' && val_3 >= 180 ) ) {
                printf ( "Nespravny vstup.\n" );        
                return 1;
        }

	switch (C1) {		// C1 == 'S' v 'U'
		case 'S':				// SSS, SUS
			if ( C2 == 'S' && C3 == 'S' ) {		// veta SSS
				X_o = val_1;				
				Y_o = val_2;
				Z_p = val_3;
				break;				// opust switch a pokracuj
			} else if ( C2 == 'U' && C3 == 'S' ) { 	// veta SUS
				// SUS, fce?
				sides_of_SUS ( &val_1, &val_2, &val_3 );
				X_o = val_1;
				Y_o = val_2;
				Z_p = val_3;
			} else {				// !SSS, !SUS	
				printf ( "Nespravny vstup.\n" );				
				return 1;
			} 
			
			break;	//case break
		case 'U':				// USU	
			if ( C2 == 'S' && C3 == 'U' ) {
				// USU, fce?
				if ( sides_of_USU ( &val_1, &val_2, &val_3) == 1 ) {
					printf ( "Vstup netvori trojuhelnik.\n" );					
					return 1;
				} else {
					X_o = val_1;
					Y_o = val_2;
					Z_p = val_3;
				}
			} else {
				printf ( "Nespravny vstup.\n" );
				return 1;
			}

			break;	//case break;
		default:				// NEJAKY NESMYSL (LKE, JSC, 1Q3, ...)
			printf ( "Nespravny vstup.\n" );
			return 1;
	}

	/* Prohozeni hodnot stran tr. XYZ dle velikosti, "x <= y <= z" */
	if ( Z_p < X_o ) {
		tmp = X_o;
		X_o = Z_p;
		Z_p = tmp;
	}
	if ( Z_p < Y_o ) {
		tmp = Y_o;
		Y_o = Z_p;
		Z_p = tmp;
	}
	if ( X_o > Y_o ) {
		tmp = X_o;
		X_o = Y_o;
		Y_o = tmp;
	}
	/* solved */


	if ( X_o + Y_o - Z_p <= 1024 * DBL_EPSILON * Z_p) {
		printf ( "Vstup netvori trojuhelnik.\n" );
		return 0;	
	}

	/* SHODNE, PODOBNE, ROZLISNE */
	shoda = (X_o - A_o) + (Y_o - B_o) + (Z_p - C_p);	// if tr.ABC = tr.XYZ --> shoda = 0	
	if ( fabs ( shoda ) <= DBL_EPSILON * 1024 * C_p )		
		shoda = 0;

	if (!shoda) {
		printf ( "Trojuhelniky jsou shodne.\n" );
		shoda = 1;
	} else if ( fabs ( A_o/X_o - B_o/Y_o ) < 10000 * DBL_EPSILON * max( fabs(A_o/X_o), fabs(B_o/Y_o) ) && fabs ( A_o/X_o - C_p/Z_p ) < DBL_EPSILON * 10000 * max( fabs(A_o/X_o), fabs(C_p/Z_p) ) && fabs ( B_o/Y_o - C_p/Z_p ) < DBL_EPSILON * 10000 * max( fabs(B_o/Y_o), fabs(C_p/Z_p) ) ) {
		printf ( "Trojuhelniky nejsou shodne, ale jsou podobne.\n" );
	} else {
		printf ( "Trojuhelniky nejsou shodne ani podobne.\n" );
	}
	/* solved */



	return 0;
}
