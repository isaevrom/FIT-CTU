#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <memory>
#endif /* C++ 11 */
using namespace std;
#endif /* __PROGTEST__ */
bool smallDiff ( double a, double b );
const double PI = acos(-1);

class CComplex {
  public:
    CComplex (void) = default;
    CComplex ( double real ) : m_real(real) {}
    CComplex ( double real, double imag ) {
        m_real = real;
        m_imag = imag;
    }
    double real(void) const {
        return m_real;
    }
    CComplex operator- ( void ) const {
         return CComplex( (-1) * m_real,
                          (-1) * m_imag );
    }
    CComplex operator+ ( const CComplex & right ) const {
        return CComplex( m_real + right.m_real,
                         m_imag + right.m_imag );
    }
    CComplex operator- ( const CComplex & rhs ) {
        return (*this) + ( - (rhs));
    }
    CComplex operator* ( const CComplex & rhs ) {
        return CComplex( m_real * rhs.m_real - m_imag * rhs.m_imag,
                         m_real * rhs.m_imag + m_imag * rhs.m_real );
    }
    CComplex operator/ ( const CComplex & c ) {
        double DN;
        DN = (c.m_real*c.m_real) + (c.m_imag*c.m_imag);
        CComplex Complex;
        Complex.m_real = ((m_real*c.m_real)+(m_imag*c.m_imag)) / DN;
        Complex.m_imag = ((m_imag*c.m_real)-(m_real*c.m_imag)) / DN;
        return Complex;
    }
  public:
    double m_real,
           m_imag = 0;
};

/*
 Partie kódu pro implementaci FFT byla zapůjčena a upravena
 z tohoto zdroje:
 https://cp-algorithms.com/algebra/fft.html
 */
class CPolynomial
{
  public:
    void fft(vector<CComplex> & A, bool invert) const {
        int n = A.size();
        if (n == 1)
            return;
        vector<CComplex> a0(n / 2), a1(n / 2);
        for (int i = 0; 2 * i < n; i++) {
            a0[i] = A[2*i];
            a1[i] = A[2*i+1];
        }
        fft(a0, invert);
        fft(a1, invert);

        double ang = 2 * PI / n * (invert ? -1 : 1);
        CComplex w(1), wn(cos(ang), sin(ang));
        //cd w(1), wn(cos(ang), sin(ang));
        
        for (int i = 0; 2 * i < n; i++) {
            A[i] = a0[i] + w * a1[i];
            A[i + n/2] = a0[i] - w * a1[i];
            if ( invert ) {
                A[i] = A[i] / 2;
                A[i + n/2] =  A[i + n/2] / 2;
            }
            w = w * wn;
        }
    }

    CPolynomial multiply(vector<double> const& a, vector<double> const& b) const {
        vector<CComplex> vecA, vecB;
        cout << "vecA: " << endl;
        for ( int i = 0; i < static_cast<int>(a.size()); i++ ) {
            CComplex tmp( a[i] );
            vecA . push_back( tmp );
        }
        cout << "vecA. " << endl;

        cout << endl;
        cout << "vecB: " << endl;
        for ( int i = 0; i < static_cast<int>(b.size()); i++ ) {
            CComplex tmp( b[i] );
            vecB . push_back( tmp );
        }
        cout << "vecB. " << endl;

        cout << endl;
        
        int n = 1;
        while (n < static_cast<int>(a.size() + b.size()))
            n <<= 1;
        vecA.resize(n);
        vecB.resize(n);
  

        fft(vecA, false);
        fft(vecB, false);

        for (int i = 0; i < n; i++)
            vecA[i] = vecA[i] * vecB[i];

        fft(vecA, true);

        CPolynomial retPol;
        vector<double> result(n);
        double tmp; double zero = 0.0;
        for (int i = 0; i < n; i++) {
            tmp = vecA[i].real();
            if ( smallDiff( zero, tmp ) )
                result[i] = 0;
            else
                result[i] = vecA[i].real();
        }
  
        retPol.m_Coefficients = result;
        
        return retPol;
    }
    
    
    
    
    friend ostream & operator<< ( ostream &os, const CPolynomial & pol );
    // default constructor
    CPolynomial() = default;
    // operator +
    CPolynomial operator+ ( const CPolynomial &rhs ) const {
        CPolynomial tmp;
        cout << endl << "a degree" << this->Degree() << endl;
        cout << endl << "b degree" << rhs.Degree()   << endl;

        if ( this->Degree() > rhs.Degree() ) { // left polynomial has got a higher degree
            tmp = ( *this );
        /* cout << endl << "### BEFORE CHANGE" << endl;
            for ( int i = 0; i <= tmp.Degree(); i++ ) {
                cout << "tmp[" << i << "]" << " = " << tmp.m_Coefficients[i] << " ";
            }
            cout << endl << "### BEFORE CHANGE" << endl;
        */
            for ( int i = 0; i <= rhs.Degree(); i++ ) {
                tmp[i] += rhs[i];
            }
        } else { // LDegree <= RDegree
            tmp = ( rhs );
        /*    cout << endl << "### BEFORE CHANGE" << endl;
            for ( int i = 0; i < (int)tmp.m_Coefficients.size(); i++ ) {
                cout << "tmp[" << i << "]" << " = " << tmp.m_Coefficients[i] << " ";
            }
            cout << endl << "### BEFORE CHANGE" << endl;
         */
        //    cout << endl << "@" << tmp  << "@" << endl;
            for ( int i = 0; i <= this->Degree(); i++ ) {
                tmp[i] += this->m_Coefficients[i];
                //cout <<
            }
        }

        return tmp;
    }
    // operatnr -
    CPolynomial operator- ( const CPolynomial &rhs ) const {
        CPolynomial tmp = *this + ( rhs * (-1.0) );
        return tmp;
    }
    // operator * (polynomial, double)
    friend  CPolynomial operator* ( const CPolynomial & lhs, double mul );
    // operator * (polynomial, polynomial)
    CPolynomial operator* ( const CPolynomial & rhs ) const {
        
        return multiply( m_Coefficients, rhs.m_Coefficients );
    }
    
    // operator ==
    bool operator== ( const CPolynomial & rhs ) const {
        int maxDegree;
        this->Degree() > rhs.Degree() ? maxDegree = this->Degree() : maxDegree = rhs.Degree();
        for ( int i = 0; i <= maxDegree; i++ ) {
            if ( ! ( fabs( this->getVal(i) - rhs.getVal(i) ) <= DBL_EPSILON * ( fabs( this->getVal(i) - rhs.getVal(i) ) ) ) )
                return false;
        }
        return true;
    }
    // operator !=
    bool operator!= ( const CPolynomial & rhs ) const {
        return !(*this == rhs);
    }
    // operator []
    double & operator[] ( int pos ) {
        //cout << "Write[]" << endl;
        //cout << "pos = " << pos << "; size = " << m_Coefficients.size() << endl;
        for ( int i = m_Coefficients.size(); i <= pos; i++ ) // if user is trying to access an out-of-range value, we need to make the vector bigger
            m_Coefficients.push_back(0.0);
        return m_Coefficients[pos];
    }
    double   operator[] ( int pos ) const { // i don't want to return it by reference, as I don't need to change a value of the class internal data
        //cout << "Read[]" << endl;
        if ( pos >= (int)m_Coefficients.size() ) //return false; // an attempt to read an out-of-range value, which should be 0 for a polynomial
            return 0;
        return m_Coefficients[pos];
    }
    // operator ()
    double operator() ( double xVal ) const {
        return Sum(xVal);
    }
    // Degree (), returns unsigned value
    int Degree (void) const {
        // !!! return an index of a highest non-zero element of the vector
        //return ( m_Coefficients.size() - 1 );
        //cout << "Degree is " << FindDegree() << endl;
        return FindDegree();
    }
    /* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
    double Sum ( double & xVal ) const {
        double sum = 0;
        for ( int i = 0; i <= Degree(); i++ ) // i'm considering only the non-zero coefficients, hence why the "i < Degree()" part is written
            sum += m_Coefficients[i] * pow( xVal, i ); // pow( what , what_power )
        return sum;
    }
    int FindDegree ( void ) const {
        int tmpDegree = 0;

        for ( int i = static_cast<int>(m_Coefficients.size()) - 1; i >= 0; i-- ) {
            // if ( ! ( fabs( m_Coefficients[i] ) < m_Coefficients[i] * DBL_EPSILON ) )
            if ( ! ( fabs( m_Coefficients[i] ) <= DBL_EPSILON * m_Coefficients[i] ) ) {
                tmpDegree = i;
                break;
            }
        }
        return tmpDegree;
    }
    double getVal ( int pos ) const { // i don't want to return it by reference, as I don't need to change a value of the class internal data
        cout << "getVal" << endl;
        if ( pos >= (int)m_Coefficients.size() ) // return false; // an attempt to read an out-of-range value, which should be 0 for a polynomial
            return 0;
        return m_Coefficients[pos];
    }
  //private:
  public:
    // todo
    vector<double> m_Coefficients;
    int degree;
};


CPolynomial operator* ( const CPolynomial & lhs, double mul ) {
    CPolynomial pol( lhs );
    for ( int i = pol.m_Coefficients.size() - 1; i >= 0; i-- ) {
        pol.m_Coefficients[i] *= mul;
    }
    return pol;
}


ostream & operator<< ( ostream &os, const CPolynomial & pol ) {
    bool First = true;
    for ( int i = static_cast<int>(pol.m_Coefficients.size()) - 1; i >= 0; i-- ) {
        // if ( ! ( fabs( m_Coefficients[i] ) < m_Coefficients[i] * DBL_EPSILON ) )
        if ( pol.m_Coefficients[i] != 0 ) {
            if ( pol.m_Coefficients[i] + DBL_EPSILON > DBL_EPSILON ) { // if the coefficient is positive
                if ( ! First ) {  // if First == true we don't print the "+" sign
                    os   << " + ";
                    //cout << " + ";
                }
                if ( fabs(pol.m_Coefficients[i]) != 1 ) {
                    os   << fabs( pol.m_Coefficients[i] );
                    //cout << fabs( pol.m_Coefficients[i] );
                }
            } else {    // if the coefficient is not positive
                if ( ! First ) {
                    os   << " - ";
                    //cout << " - ";
                } else { // if First == true
                    os   << "- ";
                    //cout << "- ";
                }
                if ( fabs(pol.m_Coefficients[i]) != 1 ) {
                    os   << fabs( pol.m_Coefficients[i] );
                    //cout << fabs( pol.m_Coefficients[i] );
                }
            }
            if ( i != 0 ) {
                if ( fabs(pol.m_Coefficients[i]) != 1 )
                    os << "*";
                os   << "x" << "^" << i;
                //if ( fabs(pol.m_Coefficients[i]) != 1 )
                  //  cout << "*";
                //cout << "x" << "^" << i;
            }
            if ( i == 0 && fabs(pol.m_Coefficients[i]) == 1  ) {
                os   << fabs( pol.m_Coefficients[i] );
                //cout << fabs( pol.m_Coefficients[i] );
            }
            First = false;
        }
    }
    // zeroPolynomial:
    if ( First ) {
        os   << "0";
        //cout << "0";
    }
    
    return os;
}
    
bool smallDiff ( double a, double b ) {
    return fabs(a - b) < 1e-4;
}

#ifndef __PROGTEST__

bool dumpMatch ( const CPolynomial & x, const vector<double> & ref ) {
  // DIY
    for ( int i = ref.size() - 1; i >= 0; i-- ) {
        if ( ! smallDiff( x.m_Coefficients[i], ref[i] ) )
            return false;
    }
    return true;
}

int main( void ) {
    /*
    CPolynomial a, b, c;
    a[3] = -7.0;
    a[0] =  2.0;
    b[0] =  2;
    cout << endl << a << endl;
    cout << endl << b << endl;
    c = multiply( a.m_Coefficients, b.m_Coefficients );
    double zero = 0.0;
    for( int i = 0; i <= c.Degree(); i++ ) {
        if ( smallDiff( zero, c.m_Coefficients[i] ) ) continue;
        cout << c.m_Coefficients[i] << "^" << i << "  ";
    }
    cout << endl << "# # # # # # # # # #" << endl;
    cout << c << endl;
  */
    
    CPolynomial a, b, c;
    ostringstream out;

    a[0] = -10;
    a[1] = 3.5;
    a[3] = 1;
    assert ( smallDiff ( a ( 2 ), 5 ) );
    out . str ("");
    out << a;
    assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
    a = a * -2;
    assert ( a . Degree () == 3
             && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

    out . str ("");
    out << a;
    assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
    out . str ("");
    out << b;
    assert ( out . str () == "0" );
    b[5] = -1;
    out . str ("");
    out << b;
    assert ( out . str () == "- x^5" );
    c = a + b;
    assert ( c . Degree () == 5
             && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
    c = a - b;
    assert ( c . Degree () == 5
             && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
    c = a * b;
    assert ( c . Degree () == 8
             && dumpMatch ( c, vector<double>{ -0.0, -0.0, 0.0, -0.0, 0.0, -20.0, 7.0, -0.0, 2.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
    assert ( a != b );
    b[5] = 0;
    assert ( !(a == b) );
    a = a * 0;
    assert ( a . Degree () == 0
             && dumpMatch ( a, vector<double>{ 0.0 } ) );

    assert ( a == b );
    
    
    
    return 0;
}
#endif /* __PROGTEST__ */
