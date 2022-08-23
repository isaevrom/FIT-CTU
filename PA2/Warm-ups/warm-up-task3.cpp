#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <algorithm>
#include <functional>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cctype>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */
// template <class T> T CSet<T>
template <class T> class CSet {
private:
    // TODO
    //string ** m_arr;
    T ** m_arr;
    size_t m_elements = 0;
    size_t m_capacity  = 0;
    size_t firstAlloc = 16;
public:
    // default constructor
    CSet() = default;
    // copy constructor
    CSet( const CSet & c ) {
        // TBD
        cout << "$ Copy constructor called!" << endl;
        m_elements = c . m_elements;
        m_capacity = c . m_capacity;
        firstAlloc = c . firstAlloc; // 0 in 80% of cases
        //m_arr = new string* [ c . m_capacity ];
        m_arr = new T* [ c . m_capacity ];
        for ( unsigned int i = 0; i < c . m_capacity; i++ ) {
            if ( c . m_arr[i] == nullptr ) {
                m_arr [i] = nullptr;
                continue;
            }
            //m_arr[i] = new string( *c . m_arr[i] );
            m_arr[i] = new T( *c . m_arr[i] );
        }
    }
    // operator=
    //void operator=( const CSet & rhs );
    CSet<T> & operator=(  CSet rhs );

    // destructor
    ~CSet() {
        // TBD
        cout << "$ Destructor called!" << endl;
        for ( unsigned int i = 0; i < m_capacity; i++ ) {
            if ( m_arr[i] == nullptr )
                continue;
            delete m_arr[i];
        }
        if ( m_capacity != 0 )
            delete [] m_arr;
    }
    // Insert
    //bool Insert  ( const string& s );
    bool Insert  ( const T& s );
    // Remove
    //bool Remove  ( const string& s );
    bool Remove  ( const T& s );
    // Contains
    //bool Contains( const string& s ) const;
    bool Contains( const T& s ) const;
    // Size
    size_t Size() const { cout << "Elements: " << m_elements << endl; return m_elements; }
};

// Vloží do množiny objekt typu Type, vrací true pokud se vložení povede. Pokud již v množině objekt je, vrátí funkce false a množinu nemění.
//bool CSet::Insert  ( const string& s ) {
template <class T> bool CSet<T>::Insert  ( const T& s ) {
    cout << "@ INSERT" << endl;
    if ( firstAlloc ) {
        //m_arr  = new string*[firstAlloc];
        m_arr = new T*[firstAlloc];
        m_capacity = firstAlloc;
        for ( unsigned int i = 0; i < m_capacity; i++ ) {
            m_arr[i] = nullptr;
        }
        firstAlloc = 0;
    }
    
    for ( unsigned int i = 0; i < m_capacity; i++ ) {
        if ( m_arr[i] == nullptr )
            continue;
        if ( !(s < *m_arr[i]) && !(*m_arr[i] < s) ) {
            cout << s << " alredy exists in the array!" << endl;
            return false;
        }
    }
    
    for ( unsigned int i = 0; i < m_capacity; i++ ) {
        // after first occurence of nullptr modify it (put s into it)
        if ( m_arr[i] == nullptr ) {
            cout << i << " FOO BAR" << endl;
            //m_arr[i] = new string( s );
            m_arr[i] = new T( s );
            cout << *m_arr[i] << " - added" << endl;
            m_elements++;
            return true;
        }
    }
    

    cout << "Gotta resize the array and add: " << s << endl;
    //string ** tmpArr = new string* [m_capacity * 2];
    T ** tmpArr = new T* [m_capacity * 2];
    
    for ( unsigned int i = 0; i < m_capacity; i++ ) {
        if ( m_arr[i] == nullptr ) {
            tmpArr[i] = nullptr;
            continue;
        }
        //tmpArr[i] = new string( *m_arr[i] );
        tmpArr[i] = new T( *m_arr[i] );
        delete m_arr[i];
    }
    for ( unsigned int i = m_capacity; i < m_capacity * 2; i++  ) {
        tmpArr[i] = nullptr;
    }
    //tmpArr[m_capacity] = new string( s );
    tmpArr[m_capacity] = new T( s );
    cout << *tmpArr[m_capacity] << " - added" << endl;
    m_elements++;
    if ( m_capacity != 0 )
        delete [] m_arr;
    m_arr = tmpArr;
    m_capacity = m_capacity * 2;
    
    return true;
}

// Odstraní z množiny objekt typu Type, vrací true pokud se smazání povede. Pokud v množině objekt není, vrátí funkce false a množinu nemění.
//bool CSet::Remove  ( const string& s ) {
template <class T> bool CSet<T>::Remove  ( const T& s ) {
    cout << "@ REMOVE: " << s << endl;
    cout << "CAPACITY = " << m_capacity << endl;
    for ( unsigned int i = 0; i < m_capacity; i++ ) {
        if ( m_arr[i] == nullptr )
            continue;
        if ( !(s < *m_arr[i]) && !(*m_arr[i] < s) ) {
            delete m_arr[i];
            m_arr[i] = nullptr;
            m_elements--;
            return true;
        }
    }
    cout << "WHAT???" << endl;
    return false;
}

//bool CSet::Contains( const string& s ) const {
template <class T> bool CSet<T>::Contains  ( const T& s ) const {
    cout << "@ CONTAINS" << endl;
    for ( unsigned int i = 0; i < m_capacity; i++ ) {
        if ( m_arr[i] == nullptr )
            continue;
        if ( !(s < *m_arr[i]) && !(*m_arr[i] < s) ) {
            cout << s << " - found" << endl;
            return true;
        }
    }
    
    return false;
}

//void CSet::operator=( const CSet & rhs ) {
template <class T>
CSet<T> & CSet<T>::operator=  (  CSet rhs ) {
    cout << "@ Operator=" << endl;
    swap( m_arr , rhs . m_arr );
    swap( m_elements , rhs . m_elements );
    swap( m_capacity , rhs . m_capacity );
    swap( firstAlloc , rhs . firstAlloc );
    
/*
    // (1) delete array elements
    cout << "1st for?" << endl;
    cout << m_capacity << endl;
    for ( unsigned int i = 0; i < m_capacity; i++ ) {
        cout << "F" << endl;
        if ( m_arr[i] == nullptr )
            continue;
        cout << "Here?" << endl;
        delete m_arr[i];
    }
    cout << "DED" << endl;
    if ( m_capacity != 0 )
        delete [] m_arr;
    // (2) setup a new array
    m_elements = rhs . m_elements;
    m_capacity = rhs . m_capacity;
    firstAlloc = rhs . firstAlloc; // 0 in 80% of cases
    //m_arr = new string* [ rhs . m_capacity ];
    if ( rhs . m_capacity == 0 )
        return *this;
    m_arr = new T* [ rhs . m_capacity ];

    for ( unsigned int i = 0; i < rhs . m_capacity; i++ ) {
        if ( rhs . m_arr[i] == nullptr ) {
            m_arr [i] = nullptr;
            continue;
        }
        //m_arr[i] = new string( *rhs . m_arr[i] );
        m_arr[i] = new T( *rhs . m_arr[i] );
    }
 */
    return *this;
}



#ifndef __PROGTEST__
#include <cassert>

struct CSetTester
{
    /*
    static void my_test()
    {
        //CSet<string> x0;
        CSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
    
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        //assert( ! x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Insert( "Endu P" ) );
        assert( x0 . Insert( "Etyo W" ) );
        assert( x0 . Insert( "Wer M" ) );
        assert( x0 . Insert( "Opoi Ert" ) );
        assert( x0 . Insert( "Keko Sata" ) );
        assert( x0 . Insert( "Wito No" ) );
        assert( x0 . Insert( "Wu Tan" ) );
        assert( x0 . Insert( "Tranh E" ) );
        assert( x0 . Insert( "Kuw Weit" ) );
        assert( x0 . Insert( "Rus Sia" ) );
        assert( x0 . Insert( "Cz Ech" ) );
        assert( x0 . Insert( "O Maro" ) );
        assert( x0 . Insert( "Kal Mar" ) );
        
        assert( x0 . Insert( "Pavlik T" ) ); // 17
        assert( x0 . Insert( "Pavlik E" ) ); // 18
        assert( x0 . Insert( "Pavlik W" ) ); // 19
        assert( x0 . Insert( "Pavlik Q" ) ); // 20
        
        assert ( x0 . Size() == 20 );
        
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( x0 . Contains( "Jerabek Michal" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Contains( "Jerabek Michal" ) );
        
        assert ( x0 . Size() == 19 );

        
        assert( x0 . Remove( "Wu Tan" ) );
        assert( x0 . Remove( "Tranh E" ) );
        assert( ! x0 . Remove( "Kal Maro" ) );

        
        assert ( x0 . Size() == 17 );
    }
    
    static void test_copy_const()
    {
        CSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( x0 . Size() == 2 );
        

        CSet x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }
    */
    /*
    static void test_op()
    {
        CSet x0;
        CSet x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }
    */

    static void test_ints()
    {
        CSet<int> x0;
        assert( x0 . Insert( 4 ) );
        assert( x0 . Insert( 8 ) );
        assert( x0 . Insert( 1 ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( 4 ) );
        assert( !x0 . Contains( 5 ) );
        assert( !x0 . Remove( 5 ) );
        assert( x0 . Remove( 4 ) );
    }
    

    static void test0()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
    
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
         
    }

    static void test1()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CSet<string> x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CSet<string> x0;
        CSet<string> x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test4()
    {
        CSet<int> x0;
        assert( x0 . Insert( 4 ) );
        assert( x0 . Insert( 8 ) );
        assert( x0 . Insert( 1 ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( 4 ) );
        assert( !x0 . Contains( 5 ) );
        assert( !x0 . Remove( 5 ) );
        assert( x0 . Remove( 4 ) );
    }

    static void ContainsTest()
    {
        CSet<string> x0;
        assert( x0 . Insert( "ebjkpjkmjvi" ) );
        CSet<string> x1;
        assert ( x1 . Insert ( "keko3e" ) );
        x1 = x0;
        assert( x1 . Contains( "ebjkpjkmjvi" )  );
        assert( ! x1 . Remove( "keko3e" ) );
        assert( x1 . Remove( "ebjkpjkmjvi" ) );
    }
    
};

int main ()
{
    //CSetTester::my_test();
    //CSetTester::test_copy_const();
    //CSetTester::test_op();
    //CSetTester::test_ints();
    
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();
     
    CSetTester::ContainsTest();
    
    return 0;
}
#endif /* __PROGTEST__ */
