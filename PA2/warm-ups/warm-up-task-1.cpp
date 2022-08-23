#ifndef __PROGTEST__
#include <cstddef>
#include <stddef.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class ACThing {
  public:
    virtual ~ACThing() = default;
    virtual ACThing* clone() const = 0;
    virtual unsigned long   Size()  const = 0;
    virtual ostream& Print(ostream &os) const = 0;
};

class CFile : public ACThing {
    string m_hash;
    unsigned long m_filesize;
 public:
    // destructor
    ~CFile() = default;
    // constructor
    CFile( const string & hash, const size_t & filesize )
    : m_hash(hash), m_filesize(filesize) { }
    // Size
    unsigned long Size() const override { //cout << "Size_f: " << m_filesize << endl;
        return m_filesize; }
    // Change
    CFile & Change( const string & hash, const size_t & filesize );
    ostream & Print( ostream &os ) const override {
        return os << " " << m_hash;
    }
    CFile* clone() const override { return new CFile(*this); };
};
CFile & CFile::Change( const string & hash, const size_t & filesize ) {
     m_hash     = hash;
     m_filesize = filesize;
     return *this;
}

class CLink : public ACThing {
    string m_path;
  public:
    // destructor
    ~CLink() = default;
    // constructor
    CLink( const string & path )
    : m_path(path) {}
    // Size
    unsigned long Size() const override { //cout << "Size_l: " << m_path.size() + 1 << endl;
        return m_path.size() + 1; }
    // Change
    CLink & Change( const string & path );
    // clone
    CLink* clone() const override { return new CLink(*this); }
    
    ostream & Print( ostream &os ) const override {
        return os << " -> " << m_path;
    }
};
CLink & CLink::Change( const string & path ) {
    m_path = path;
    return  *this;
}

/*
    CDirectory::Get( filename ) - vrátí z adresáře referenci na soubor s názvem filename (vyžaduje i variantu s const, neprohledává rekurzivně), pokud soubor   filename v adresáři neexistuje, vyhoďte výjimku std::out_of_range
    operator<<( ostream &, directory ) - operátor pro výpis (formát výpisu je velikost souboru, tabulátor, název souboru a další parametry dle druhu souboru, viz ukázka)
*/

class CDirectory : public ACThing {
    //map<string, ACThing* > mp;
    map<string, shared_ptr<ACThing>> mp;
  public:
    // destructor
    ~CDirectory() {}
    // constructor
    CDirectory() = default;
    // Size
    unsigned long Size() const override;
    // Change
    CDirectory & Change( const string & filename, const ACThing & ac );
    CDirectory & Change( const string & filename, std::nullptr_t t );
    // Get
    ACThing & Get ( const string & filename ) {
        auto finder = mp . find ( filename );
        if ( finder != mp . end () )
            cout << "FOUND!" << endl;
        else
            throw std::out_of_range ( "Out of range!" );
        //return * ( finder -> second -> clone() );
        return * ( finder -> second );
    }
    
    ACThing & Get ( const string & filename ) const {
        auto finder = mp . find ( filename );
        if ( finder != mp . end () )
            cout << "FOUND!" << endl;
        else
            throw std::out_of_range ( "Out of range!" );
        return * ( finder -> second );
    }

     //"9\tfile.ln -> file.txt\n"
        
     //"1623\tfile.txt jhwadkhawkdhajwdhawhdaw=\n"
        
     //"81246\tfolder/\n"
 
    
    ostream & Print( ostream &os ) const override {
        return os << "/";
    }
    // operator<<
    // velikost souboru |  \t   | název souboru | další parametry dle druhu souboru, viz ukázka
    friend ostream& operator<<( ostream & os, const CDirectory & dir ) {
        for ( const auto & it: dir . mp ) {
            os << it . second -> Size();
            os << "\t";
            os << it . first;
            it . second -> Print( os );
            os << "\n";
        }
        return os;
    }

    // clone
  //protected:
    CDirectory* clone() const override { return new CDirectory(*this); }
};

unsigned long CDirectory::Size() const {
    unsigned long _size = 0;
    for ( const auto & it: mp ) {
        _size += it . second -> Size() + it . first . size();
    }
    //cout << "Size is: " << _size << endl;
    return _size;
}
/*
CDirectory & CDirectory::Change( const string & filename, const CDirectory & ac ) {
    auto it = mp . find(filename);
    if ( it != mp.end() ) {
        cout << "Found" << endl;
        mp.erase( it );
    } else {
        cout << "Not found" << endl;
    }
    mp.insert( { filename , shared_ptr<ACThing>( ac . clone() ) } );
    
    return *this;
}
*/
CDirectory & CDirectory::Change( const string & filename, const ACThing & ac ) {
    /*
    auto it = mp . find(filename);
    if ( it != mp.end() ) {
        //cout << "Found" << endl;
        mp.erase( it );
    } else {
        //cout << "Not found" << endl;
    }
    mp.insert( { filename , shared_ptr<ACThing>( ac . clone() ) } );
    */
    mp [ filename ] = shared_ptr<ACThing>( ac . clone() );
    
    return *this;
}
CDirectory & CDirectory::Change( const string & filename, std::nullptr_t t ) {
    cout << "DELETING A FILE" << endl;
    
    auto it = mp . find( filename );
    
    if ( it != mp.end() ) {
        //cout << "Found" << endl;
        mp.erase( it );
    } else {
        //cout << "Not found" << endl;
    }
    
    
    return *this;
}


#ifndef __PROGTEST__
int main ()
{
    //CDirectory root;
    //stringstream sout;
    CDirectory dir;
    dir.Change("kek", CFile("foo", 3));
    dir.Change("directory", CDirectory());
    cout << dir;
    dir.Size();
    cout << "#" << endl;
    dir.Change( "b", dir ).Size();
    cout << "#" << endl;
    cout << dir;
    dir.Size();

    /*
        CFile("jhwadkhawkdhajwdhawhdaw=", 1623).Size();
        CLink("").Change("Kik").Change("").Size();
    */
/*
        root . Change(   "file.txt"    ,    CFile("jhwadkhawkdhajwdhawhdaw=", 1623)    );
        root . Change(   "file.txt"    ,    nullptr    );
        root . Change(   "file.txt"    ,    CFile("jhwadkhawkdhajwdhawhdaw=", 1623)    );
        root . Change(   "file.txt"    ,    CFile("jhwadkhawkdhajwdhawhdaw=", 1623)    );
*/
/*
    root.Change("file.txt", CFile("jhwadkhawkdhajwdhawhdaw=", 1623))
        .Change("file.ln", CLink("").Change("file.txt"))
        .Change("folder", CDirectory()
            .Change("fileA.txt", CFile("", 0).Change("skjdajdakljdljadkjwaljdlaw=", 1713))
            .Change("fileB.txt", CFile("kadwjkwajdwhoiwhduqwdqwuhd=", 71313))
            .Change("fileC.txt", CFile("aihdqhdqudqdiuwqhdquwdqhdi=", 8193))
        );


    sout.str( "" );
    sout << root;
    cout << root;
    assert( sout.str() == 
        "9\tfile.ln -> file.txt\n"
        "1623\tfile.txt jhwadkhawkdhajwdhawhdaw=\n"
        "81246\tfolder/\n" );
    assert( root.Size() == 82899 );

    string filename = "folder";
    const CDirectory & inner = dynamic_cast<const CDirectory &>( root.Get( filename ) );

    sout.str( "" );
    sout << inner;
    assert( sout.str() ==
        "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
        "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
        "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n" );
    assert( inner.Size() == 81246 );
*/
    return 0;
}
#endif /* __PROGTEST__ */
