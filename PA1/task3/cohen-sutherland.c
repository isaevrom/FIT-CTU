#ifndef __PROGTEST__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>


#endif /* __PROGTEST__ */

// O_1[rx1, ry1]   O_2[rx2, ry2]
// U_1[ ax, ay ]   U_2[ bx, by ]
static const int MIDDLE = 0;    // 0 0 0 0
static const int LEFT = 1;    // 0 0 0 1
static const int RIGHT = 2;    // 0 0 1 0
static const int DOWN = 4;    // 0 1 0 0
static const int UP = 8;    // 1 0 0 0

typedef double dbl;

int find_location ( dbl x_u, dbl y_u, dbl xmin, dbl xmax, dbl ymin, dbl ymax )
{
    int posit;

    posit = MIDDLE;

    if (x_u < xmin)
    {           // vlevo od rect.
        posit = posit | LEFT;
    } else if (x_u > xmax) {     // vpravo od rect.
        posit = posit | RIGHT;
    }
    
    if (y_u < ymin) // pod rectanglem
    {
        posit = posit | DOWN;
    } else if (y_u > ymax) {  // nad rectanglem
        posit = posit | UP;
    }
        
    return posit;
}


/* RESENI LINE-CLIPPINGU POMOCI METODY PANU COHENA & SUTHERLANDA viz. wikipedia.org/wiki/Cohen–Sutherland_algorithm */


int                clipLine                                ( double            rx1,
                                                             double            ry1,
                                                             double            rx2,
                                                             double            ry2,
                                                             double          * ax,
                                                             double          * ay,
                                                             double          * bx,
                                                             double          * by )
{
    /* todo */
    double save_bx = *bx, save_by = *by;
    double tmp_x = 0, tmp_y = 0;
    double min_rX, max_rX;
    double min_rY, max_rY;
    int i_am_depressed = 1;
    int match          = 0;
    /* zjistime max/min velikosti rectanglu */
    min_rX = ( rx1 < rx2 ) ? rx1 : rx2 ;
    max_rX = ( rx1 < rx2 ) ? rx2 : rx1 ;
    min_rY = ( ry1 < ry2 ) ? ry1 : ry2 ;
    max_rY = ( ry1 < ry2 ) ? ry2 : ry1 ;
    /*end*/
    
    double Ux_0 = *ax, Ux_1 = *bx;
    double Uy_0 = *ay, Uy_1 = *by;
    
    // zjistujeme lokaci koncovych bodu usecky metodou C.-S.
    int location_A = find_location ( Ux_0, Uy_0, min_rX, max_rX, min_rY, max_rY );
    int location_B = find_location ( Ux_1, Uy_1, min_rX, max_rX, min_rY, max_rY );

    while ( i_am_depressed )
    {
        if ( !( location_A | location_B ) )
        {
            match = 1;
            break;
        } else if ( location_A & location_B ) {
            
            break;
            
        } else {

            int location_outside = location_A ? location_A : location_B;

            if ( location_outside & UP ) {           // nad
                tmp_x = Ux_0 + (Ux_1 - Ux_0) * (max_rY - Uy_0) / (Uy_1 - Uy_0);
                tmp_y = max_rY;
            } else if ( location_outside & DOWN ) { // pod
                tmp_x = Ux_0 + (Ux_1 - Ux_0) * (min_rY - Uy_0) / (Uy_1 - Uy_0);
                tmp_y = min_rY;
            } else if ( location_outside & RIGHT ) {  // vpravo
                tmp_y = Uy_0 + (Uy_1 - Uy_0) * (max_rX - Ux_0) / (Ux_1 - Ux_0);
                tmp_x = max_rX;
            } else if ( location_outside & LEFT ) {   // vlevo
                tmp_y = Uy_0 + (Uy_1 - Uy_0) * (min_rX - Ux_0) / (Ux_1 - Ux_0);
                tmp_x = min_rX;
            }

            if ( location_outside == location_A ) {
                Ux_0 = tmp_x;
                Uy_0 = tmp_y;
                //match = 1;
                //printf ( "Ux_0 = %f, Uy_0 = %f\n", Ux_0, Uy_0 );
                location_A = find_location ( Ux_0, Uy_0, min_rX, max_rX, min_rY, max_rY );
            } else {
                //match = 1;
                Ux_1 = tmp_x;
                Uy_1 = tmp_y;
                //printf ( "Ux_1 = %f, Uy_1 = %f\n", Ux_1, Uy_1 );
                location_B = find_location ( Ux_1, Uy_1, min_rX, max_rX, min_rY, max_rY);
            }
            
        }
    }
//    printf ( "EEE\n" );
    *ax = Ux_0;
    *ay = Uy_0;
    *bx = Ux_1;
    *by = Uy_1;
    
    if ( fabs( *ax - rx1 ) <= DBL_EPSILON * rx1 && fabs( *ay - ry1 ) <= DBL_EPSILON * ry1 && fabs(save_bx - *bx) <=  DBL_EPSILON * save_bx && fabs(save_by - *by) <= DBL_EPSILON * save_by)
    {
        *bx = *ax;
        *by = *ay;
	match = 1;
    }
   // printf ( "*ax = %f , *bx = %f\n", *ax, *bx );
   // printf ( "*ay = %f , *by = %f\n", *ay, *by );
   // printf ( "match = %d\n", match );
 /*   printf ( "x0 = %f\n", *ax );
    printf ( "y0 = %f\n", *ay );
    printf ( "x1 = %f\n", *bx );
    printf ( "y1 = %f\n", *by );
    
    printf ( "x0 = %f\n", x0 );
    printf ( "y0 = %f\n", y0 );
    printf ( "x1 = %f\n", x1 );
    printf ( "y1 = %f\n", y1 );*/
    
       
    
    
    if ( match )
        return 1;
    
    return 0;
}

#ifndef __PROGTEST__
int                almostEqual                             ( double            p,
                                                             double            q )
{
  /* todo */
  if ( p * p - q * q <= DBL_EPSILON * p * p )
    {
       // printf ( "x == y\n" );
        return 1;
        
    }
        
    return 0;
}

int                main                                    ( void )
{
  double x1, y1, x2, y2;    // souradnice usecky

  x1 = 60;  // souradnice usecky
  y1 = 40;
  x2 = 70;
  y2 = 50;
  //  printf ( "" );
  //  printf ( "%d\n", clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 ) );
  //  printf ( "x1 = %f x2 = %f y1 = %f y2 = %f\n", x1, x2, y1, y2 );
 
  
  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 60 )
           && almostEqual ( y1, 40 )
           && almostEqual ( x2, 70 )
           && almostEqual ( y2, 50 ) );

  x1 = 0;
  y1 = 50;
  x2 = 20;
  y2 = 30;
  assert ( clipLine ( 90, 100, 10, 20, &x1, &y1, &x2, &y2 )
           && almostEqual ( x1, 10 )
           && almostEqual ( y1, 40 )
           && almostEqual ( x2, 20 )
           && almostEqual ( y2, 30 ) );




    
  return 0;
}
#endif /* __PROGTEST__ */
