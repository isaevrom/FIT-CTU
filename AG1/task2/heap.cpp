#ifndef __PROGTEST__
#include "flib.h"
#endif //__PROGTEST__

int cmp_func (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void swapTwo(int &a, int &b)
{
   a = a + b;
   b = a - b;
   a = a - b;
}

void heapify(int array[], int amount, int i)
{
    int max = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    if (l < amount && array[l] > array[max])
        max = l;
    if (r < amount && array[r] > array[max])
        max = r;
    if (max != i)
    {
        swapTwo(array[i], array[max]);
          heapify(array, amount, max);
    }
}
  
void heapSort(int array[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(array, n, i);
  
    for (int i=n-1; i>0; i--)
    {
        swapTwo(array[0], array[i]);
          heapify(array, i, 0);
    }
}
/*
      |----------------|
 ----  FLIB.H FUNCTIONS  ----
      |----------------|
 void flib_init_files ( uint16_t number_of_files );
 void flib_free_files ( );
 void flib_open ( uint16_t file_ID, FLIB_Mode mode );
 void flib_close ( uint16_t file_ID );
 int64_t flib_read  ( uint16_t file_ID, int32_t * buffer, int64_t count );
 int64_t flib_write ( uint16_t file_ID, int32_t * buffer, int64_t count );
 void flib_remove ( uint16_t file_ID );
*/

// h p m
//      ID souboru obsahujícího data k seřazení,
//      ID souboru, ve kterém mají být uložena vzestupně seřazená čísla,
//      velikost paměťi (haldy) v bytech, kterou máme dostupnou k výpočtu
// ID vstupního a výstupního souboru budou 0 a 1 nebo 1 a 0
// ze souboru H je možné pouze sekvenčně číst a do souboru P je možné pouze sekvenčně zapisovat. Soubor H není možné smazat.
void tarant_allegra ( int32_t in_file, int32_t out_file, int32_t bytes )
{
    // open max. 3 files at a time ... (i), (i+1) --> (j+1)
    // need to substract 3*12b from bytes (price paid for opening 1 file: 12b)
    int64_t amountRead, amountWritten;
    uint16_t fileNameIterator = 1;
    bytes -= 36;
    bytes /=  4;
    bytes = bytes + 8135;

    int32_t *buffer = new int32_t[bytes];
    
    flib_open( in_file, READ );
    //  amountRead != bytes   =>   EOF or mistake
    //  (1) Mistake? Find problem.
    //  (2) EOF?  =>  stop reading, start merging


    while ( 1 )
    {
        amountRead    = flib_read ( in_file, buffer, bytes );

        if ( amountRead == 0 ) // there is nothing left to be read in the in_file
            break;

        heapSort(buffer, amountRead);
        flib_open ( ++fileNameIterator, WRITE );       // ??? do i need to create file before opening it
                                                       // initially fileNameIt. is eq. to 1, so it becomes 2 after "++" operation
                                                       // fileNameIt. <-- buffer
        amountWritten = flib_write( fileNameIterator, buffer, amountRead );
        flib_close( fileNameIterator );
    }
    // all read => file can be closed
    flib_close( in_file );
    // more files than 1
    if ( fileNameIterator != 2 )
        delete [] buffer;
    
    //            M  E  R  G  E    S  O  R  T
    /*
     - if fileNameIt. == 1, then no file was created in the previous step
     - create 3 buffers... e.g.: buf1 for file2, buf2 for file3, buf4 for file ++fileNameIterator = {file2 + file3 content}
     - iterators: j, l
     - if ( b1[j] <= b2[l] )  =>  b3 <- b1[j++]
     */
   /* int newSize     = bytes/3;

    int *bufLeft    = new int[newSize],
        *bufRight   = new int[newSize],
        *bufOut     = new int[newSize];
    uint16_t i = 2,
    save_fileNameIterator = fileNameIterator;*/
   
    // TWO CASES: 1) fileNameIterator > 2 && fileNameIterator % 2 == 0 => even # of files to be merged
    //   else     2) fileNameIterator % 2 != 1                         => odd  # of files to be merged
 if ( fileNameIterator != 2 )
 {
    // if there are more than 1 files
     int newSize     = bytes/3;

      int *bufLeft    = new int[newSize],
          *bufRight   = new int[newSize],
          *bufOut     = new int[newSize];
      uint16_t i = 2, save_fileNameIterator = fileNameIterator;
    for ( int y = 1; y < save_fileNameIterator - 1; y++ ) // there is still information in both files to be read
    {
        flib_open( i,   READ );
        flib_open( i+1, READ );
            
        if ( y + 2 == save_fileNameIterator ) { // merge into output file
            fileNameIterator = out_file - 1; // because later we ++ it
        }
        flib_open( ++fileNameIterator, WRITE ); // fileNameIterator <--- two merged files


        // l - leftWrittenAmount, r - rightWrittenAmount (into bufOut)
        // L - leftReadAmount   , R - rightReadAmount (from files)
        int r = 0, l = 0, o = 0;
        int L = 0, R = 0;

        // while there is something to write
        while ( 1 )
        {
            if ( l == L )
            {

                L = flib_read( i,   bufLeft,  newSize );
                l = 0;
            }
            if ( r == R )
            {

                R = flib_read( i+1, bufRight, newSize );
                r = 0;
            }
            if ( L == 0 || R == 0 )
                break;

            if ( o == newSize ) {
                flib_write( fileNameIterator, bufOut, o );
                o = 0;
            }
            if ( bufLeft[l] < bufRight[r] ) {
                bufOut[o++] = bufLeft[l++];
            } else {
                bufOut[o++] = bufRight[r++];
            }
        }

        bool flag = false;
        // need to read the rest of the bufLeft
        if ( R == 0 )
        {
            flag = true;

            while ( 1 )
            {
                if ( o == newSize )
                {
                    flib_write( fileNameIterator, bufOut, o );
                    o = 0;
                }
                if ( l == L )
                {
                    L = flib_read( i,   bufLeft,  newSize );
                    l = 0;
                }
                if ( L == 0 )
                    break;
                bufOut[o++] = bufLeft[l++];
            }
        }

        // need to read the rest of the bufRight
        if ( L == 0 && !flag )
        {

            while ( 1 )
            {
                if ( o == newSize )
                {
                    flib_write( fileNameIterator, bufOut, o );
                    o = 0;
                }
                if ( r == R )
                {
                    R = flib_read( i+1,   bufRight,  newSize );
                    r = 0;
                }
                if ( R == 0 )
                    break;
                bufOut[o++] = bufRight[r++];
            }
        }
        if ( o > 0 && o < newSize )
        {
            flib_write( fileNameIterator, bufOut, o );
            o = 0;
        }
        flib_close( fileNameIterator );
        flib_close( i );
        flib_close( i+1 );
        flib_remove( i ); // ??? do i need to close file before removing it
        flib_remove( i+1 );
        i += 2;
    }

    delete [] bufRight;
    delete [] bufLeft;
    delete [] bufOut;
    
 } else {
        if ( fileNameIterator == 2 )
        {
            flib_open( out_file, WRITE );
            flib_open( fileNameIterator, READ );
            amountRead = flib_read ( fileNameIterator, buffer, bytes );
            flib_write( out_file, buffer, amountRead );
            
            flib_close(fileNameIterator);
            flib_remove(fileNameIterator);
            flib_close(out_file);
            delete [] buffer;
        }
 }
}


#ifndef __PROGTEST__

uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
    /* srand(time(NULL)); */
#define STEP 100ll
    int val[STEP];
    for(int i=0; i<size; i+=STEP){
        for(int j=0; j<STEP && i+j < size; ++j){
            val[j]=-1000 + (rand()%(2*1000+1));
            total_sum_mod += val[j];
        }
        flib_write(output, val, (STEP < size-i) ? STEP : size-i);
    }
    flib_close(output);
}

void tarant_allegra ( int in_file, int out_file, int bytes );

void check_result ( int out_file, int SIZE ){
    flib_open(out_file, READ);
    int q[30], loaded, last=-(1<<30), total=0;
    uint64_t current_sum_mod=0;
    while(loaded = flib_read(out_file, q, 30), loaded != 0){
        total += loaded;
        for(int i=0; i<loaded; ++i){
            if(last > q[i]){
                printf("the result file contains numbers %d and %d on position %d in the wrong order!\n", last, q[i], i-1);
                exit(1);
            }
            last=q[i];
            current_sum_mod += q[i];
        }
    }
    if(total != SIZE){
        printf("the output contains %d but the input had %d numbers\n", total, SIZE); exit(1);
    }
    if(current_sum_mod != total_sum_mod){
        printf("the output numbers are not the same as the input numbers\n");
        exit(1);
    }
    flib_close(out_file);
}

int main(int argc, char **argv){
    const uint16_t MAX_FILES = 65535;
    flib_init_files(MAX_FILES);
    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 240;

    create_random(INPUT, SIZE);

    tarant_allegra(INPUT, RESULT, 1000);

    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
