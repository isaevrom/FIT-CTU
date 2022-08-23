#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

/* CCarList BEGIN */
//___ CCarList ___
//___ iterator-class used to iterate through a list of cars of a particular person ___
class CCarList
{
  public:
    CCarList( const vector<string> &personalRZs ) : m_personalRZs(personalRZs) { i = 0; }
    
    const string & RZ      ( void ) const { return m_personalRZs[i]; }
    bool           AtEnd   ( void ) const { return i >= m_personalRZs.size(); }
    void           Next    ( void ) { i++; }
  private:
    const vector<string> m_personalRZs;
    unsigned int i;
};
/* CCarList END */

/* SName BEGIN */
//___ SName ___
//___ is a structure used for storing names of the persons currently present in the database ___
//___ the structure is used in a CPersonList, as it serves for iteration through a list of persons currently present in the database ___
struct SName {
    string name, surname;
    SName( const string & name, const string & surname ) : name(name), surname(surname) {}
    
    void setName   (const string &name)    { this->name = name; }
    void setSurname(const string &surname) { this->surname = surname; }
    
    bool operator<( const SName &r ) {
        bool cmp;
        surname == r.surname ? cmp = name < r.name : cmp = surname < r.surname ;
        return cmp;
    }
    friend ostream & operator<< ( ostream &os, SName & r);
};
ostream & operator<< ( ostream &os, const SName & r) {
    cout << r.name << r.surname << endl;
    return os;
}
/* SName END */

/* CPersonList BEGIN */
//___ CPersonList ___
//___ iterator-class used to iterate through a list of persons ___
//___ which are currently present in the database ___
class CPersonList
{
  public:
    CPersonList( const vector<SName> &NameRecords ) : m_NameRecords(NameRecords) { j = 0; }

    const string & Name    ( void ) const { return m_NameRecords[j].name; }
    const string & Surname ( void ) const { return m_NameRecords[j].surname; }
    bool           AtEnd   ( void ) const { return j >= m_NameRecords.size(); }
    void           Next    ( void ) { j++; }

 public:
    const vector<SName> m_NameRecords;
 private:
    unsigned int j;
};
/* CPersonList END */

/* SRecordRZ BEGIN */
//___ SRecordRZ ___
//___ is a structure used for storing database records ___
//___ in a sorted way (using lower_bound, operator< overload, insert vector method )___
//___ the sorting is based on RZs ___
struct SRecordRZ {
    string rz, name, surname;
    
      SRecordRZ( const string & rz,   const string & name, const string & surname )
    : rz(rz), name(name), surname(surname) {}
    
    bool operator< ( const SRecordRZ &r ) const { return rz < r.rz; }
    friend ostream & operator<< ( ostream &os, const SRecordRZ & r);
};
ostream & operator<< ( ostream &os, const SRecordRZ & r ) {
    cout << r.rz << " " << r.name << " " << r.surname << endl;
    return os;
}
/* SRecordRZ END */

/* SRecordSN BEGIN */
//___ SRecordSN ___
//___ is a structure used for storing database records ___
//___ in a sorted way (using lower_bound, operator< overload, insert vector method ) ___
//___ the sorting is based on their S - Surname and N - Name ___
struct SRecordSN {
    string rz, name, surname;
    
      SRecordSN( const string & rz,   const string & name, const string & surname )
    : rz(rz), name(name), surname(surname) {}
    /**
        @param[in] rz      - rz of a car
        @param[in] name    - name of the person.
        @param[in] surname - surname of the person.
    */
    void setSN( const string & name, const string & surname ) { this->name = name; this->surname = surname; }

    bool operator< ( const SRecordSN &r ) const {
        bool cmp;
        surname == r.surname ? cmp = name < r.name : cmp = surname < r.surname;
        return cmp;
    }
    friend ostream & operator<< ( ostream &os, const SRecordSN &r );
};
ostream & operator<< ( ostream &os, const SRecordSN &r ) {
    cout << r.rz << " " << r.name << " " << r.surname << endl;
    return os;
}
/* SRecordSN END */

/* CRegister BEGIN */
//___ CRegister is a class which is used to store ___
//___ records in a database ___
//___ which are currently present in the database ___
class CRegister {
  public:
                CRegister  ( void ) {}
               ~CRegister  ( void ) {}
                CRegister  ( const CRegister & src ) = delete;
    CRegister & operator = ( const CRegister & src ) = delete;
    /** AddCar method is used to add new records into the database
       @param[in] rz      - rz of a car
       @param[in] name    - name of the person.
       @param[in] surname - surname of the person.
       Return: true if a whole new car is added, false otherwise
     */
    bool AddCar ( const string & rz,   const string & name, const string & surname );
    /** DelCar method is used to delete a record from the database
        @param[in] rz      - rz of a car
        Return: false if no such car exists in the database and hence can't be deleted & true otherwise
    */
    bool DelCar ( const string & rz );
    /** Transfer
        @param[in] rz       - rz of a car
        @param[in] nName    - name of a new owner.
        @param[in] nSurname - surname of a new owner.
        Return: false if 1) no such car exists 2) owner is trying to transfer a car to himself, true otherwise
    */
    bool Transfer ( const string & rz,   const string & nName, const string & nSurname);
    /** CCarList
        @param[in] name    - name of    a person whose cars we want to see via list
        @param[in] surname - surname of a person whose cars we want to see via list
        Return: either empty CCarList instance (if no such person exists in the database)
                or a CCarList instance with all the cars of an existing person
    */
    CCarList ListCars ( const string & name, const string & surname ) const;
    /** CountCars
        @param[in] name    - name of    a person whose cars we want to see via list
        @param[in] surname - surname of a person whose cars we want to see via list
        Return: 0 ( if no such person exists in the database) or the amount of the cars he owns
    */
    int CountCars ( const string & name, const string & surname ) const;
    /** CPersonList
        @param[in] - no parameters
        Return: list of the persons currently present in the database
    */
    CPersonList ListPersons( void ) const;
    /* = = = = = = = = = = = = = = = = = = = = = = = */
    /*
     4 simple getters follow.
     Const version used in order to access elements in a way which does not modify the object. E.g.: iterating a vector with a const_iterator.
    */
    vector<SRecordRZ> & get_m_byRZRecords()   {   return m_byRZRecords;   }
    vector<SRecordSN> & get_m_byNameRecords() {   return m_byNameRecords; }
    const vector<SRecordRZ> & get_m_byRZRecords()  const  {   return m_byRZRecords;   }
    const vector<SRecordSN> & get_m_byNameRecords() const {   return m_byNameRecords; }
 private:
    // m_byRZRecords   -- contains records insert-sorted by RZs of the cars
    vector<SRecordRZ> m_byRZRecords;
    // m_byNameRecords -- contains records insert-sorted by names of the car owners
    vector<SRecordSN> m_byNameRecords;
 };

bool CRegister::AddCar ( const string & rz,  const string & name,  const string & surname ) {
 /* Insert Sort by RZ */
    SRecordRZ tmpRecordRZ( rz, name, surname );
    auto rzPlace  = lower_bound( get_m_byRZRecords() . begin(), get_m_byRZRecords() . end(), tmpRecordRZ );
    if ( rzPlace != get_m_byRZRecords() . end() ) {
        /* This could be a duplicity, hence we need to check it */
        if ( rzPlace->rz == rz ) {
            //cout << "Trying to add an existing car!" << endl;
            return false;
        }
        get_m_byRZRecords() . insert( rzPlace, tmpRecordRZ );
    } else {
        // Append a whole new car
        get_m_byRZRecords() . push_back( tmpRecordRZ );
    }
    
 /* Insert Sort by Surname-Name */
    SRecordSN tmpRecordSN( rz, name, surname );
    auto namePlace  = lower_bound( get_m_byNameRecords() . begin(), get_m_byNameRecords() . end(), tmpRecordSN );
    if ( namePlace != get_m_byNameRecords() . end() ) {
        /* Insert into the middle, as tmpRecordSN has some predecessor */
        get_m_byNameRecords() . insert( namePlace, tmpRecordSN );
    } else {
        /* Append the record, as either the vector is empty, or tmpRecordSN has no predecessor */
        get_m_byNameRecords() . push_back( tmpRecordSN );
    }
    
    return true;
}
bool CRegister::DelCar     ( const string & rz ) {  // Erase by RZ
    string delName, delSurname;
    SRecordRZ tmpRecordRZ( rz, "Luciano", "Pavarotti" );
    auto rzPlace  = lower_bound( get_m_byRZRecords() . begin(), get_m_byRZRecords() . end(), tmpRecordRZ );
    if ( rzPlace != get_m_byRZRecords() . end() ) {
        /* Found such an rz among m_byRZRecords (vector of records sorted by RZs in the ascending order) */
        if ( rzPlace->rz != rz ) {
            //cout << "No such car to delete..." << endl;
            return false;
        }
        /* I need to save the name of the current owner, as in the next step I am going to delete it by Surname-Name from m_byNameRecords */
        delName     = rzPlace->name;
        delSurname  = rzPlace->surname;
        /* In case I found it, I need to erase that record, as I am gonna add a new owner */
        get_m_byRZRecords() . erase( rzPlace );
    } else {
        //cout << "No such car to delete..." << endl;
        return false;
    }
    // Erase by SN
    SRecordSN tmpRecordSN( "", delName, delSurname );
    auto namePlace  = lower_bound( get_m_byNameRecords() . begin(), get_m_byNameRecords() . end(), tmpRecordSN );
    while ( namePlace != get_m_byNameRecords() . end() ) {
        if ( namePlace->name == delName && namePlace->surname == delSurname ) {
            if ( namePlace->rz == rz ) {
                get_m_byNameRecords().erase( namePlace );
                return true;
            } else {
                namePlace++;
            }
        } else {
            //cout << "No such a record found by SN..." << endl;
            return false;
        }
    }

    return false;
}
bool CRegister::Transfer   ( const string & rz,  const string & nName, const string & nSurname) {
    SRecordRZ tmpOwnerToHimself( rz, "Placido", "Domingo" );
    auto ownerToHimself = lower_bound( get_m_byRZRecords() . begin(), get_m_byRZRecords() . end(), tmpOwnerToHimself );
    if ( ownerToHimself != get_m_byRZRecords() . end() ) {
        if ( ownerToHimself->name == nName && ownerToHimself->surname == nSurname ) {
            //cout << "The person is trying to transfer a car to himself..." << endl;
            return false;
        }
    }
    if ( ! DelCar( rz ) ) {
        //cout << "Transfer did not happen..." << endl;
        return false;
    }
    // Add a new record, in other words -- change the owner of the car
    AddCar( rz, nName, nSurname );
    
    
    return true;
}
CCarList CRegister::ListCars ( const string & name, const string & surname ) const {
    vector<string> personalRZs;
    SRecordSN tmpRecordSN( "", name, surname );
    
    auto namePlace  = lower_bound( get_m_byNameRecords() . begin(), get_m_byNameRecords() . end(), tmpRecordSN );
    while ( namePlace != get_m_byNameRecords() . end() ) {
        /* If I found such an owner, I need to add all of his cars into the personalRZs */
        if ( namePlace->name == name && namePlace->surname == surname ) {
            personalRZs.push_back( namePlace->rz );
            ++namePlace;
        } else {
            /* All the cars added */
            break;
        }
    }

    return CCarList( personalRZs );
}

int CRegister::CountCars ( const string & name, const string & surname ) const {
    int carsCnt = 0;
    SRecordSN tmpRecordSN( "", name, surname );
    
    auto namePlace  = lower_bound( get_m_byNameRecords() . begin(), get_m_byNameRecords() . end(), tmpRecordSN );
    while ( namePlace != get_m_byNameRecords() . end() ) {
        /* If I found such an owner, I need to count all of his cars */
        if ( namePlace->name == name && namePlace->surname == surname ) {
            ++carsCnt;
            ++namePlace;
        } else {
            /* All cars counted */
            break;
        }
    }
    /* carsCnt == 0 if owner has no cars at all */
    return carsCnt;
}

 CPersonList CRegister::ListPersons( void ) const {
     vector<SName> NameRecords;
     // If there are no records, why bother trying to check them...
     if ( m_byNameRecords.size() == 0 ) {
         return CPersonList( NameRecords );
     }
     string name, surname;
     SRecordSN tmpRecordSN( "", "", "" );
     SName     tmpName( "", "" );
     vector<SRecordSN>::const_iterator namePlace = get_m_byNameRecords() . begin();

     while ( namePlace != get_m_byNameRecords() . end() ) {
             name    = namePlace->name;
             surname = namePlace->surname;
         
             tmpName.setName( name );
             tmpName.setSurname( surname );
             NameRecords.push_back( tmpName );
         
             tmpRecordSN.setSN( name, surname );
             // Keep searching for a greater ( "<", not "<=" ) name using upper_bound until the end of m_byNameRecords
             namePlace  = upper_bound( get_m_byNameRecords() . begin(), get_m_byNameRecords() . end(), tmpRecordSN );
     }
     
     return CPersonList( NameRecords );
 }
/* CRegister END */


#ifndef __PROGTEST__

bool checkPerson ( CRegister & r, const string & name, const string & surname, vector<string> result )
{
  for ( CCarList l = r . ListCars ( name, surname ); ! l . AtEnd (); l . Next () )
  {
    auto pos = find ( result . begin (), result . end (), l . RZ () ); 
    if ( pos == result . end () )
      return false;
    result . erase ( pos );
  }
  return result . size () == 0;
}

int main ( void )
{
    
    CRegister b1;
    assert ( b1 . AddCar ( "ABC-12-34", "John", "Smith"  ) == true );
    assert ( b1 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
    assert ( b1 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
    assert ( b1 . CountCars ( "John", "Hacker" ) == 1 );
    assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22" } ) );
    assert ( b1 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
 

    assert ( b1 . AddCar ( "XYZ-99-88", "John", "Smith" ) == true );
    assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
    assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
    assert ( b1 . CountCars ( "John", "Hacker" ) == 2 );
    assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22", "XYZ-11-22" } ) );
    assert ( b1 . CountCars ( "Peter", "Smith" ) == 0 );
    assert ( checkPerson ( b1, "Peter", "Smith", {  } ) );
    assert ( b1 . Transfer ( "XYZ-11-22", "Jane", "Black" ) == true );
    assert ( b1 . CountCars ( "Jane", "Black" ) == 1 );
    assert ( checkPerson ( b1, "Jane", "Black", { "XYZ-11-22" } ) );
    assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
    assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
 

    assert ( b1 . DelCar ( "XYZ-11-22" ) == true );
    assert ( b1 . CountCars ( "Jane", "Black" ) == 0 );
    assert ( checkPerson ( b1, "Jane", "Black", {  } ) );
    assert ( b1 . AddCar ( "XYZ-11-22", "George", "White" ) == true );
 
    
    CPersonList i1 = b1 . ListPersons ();
    /*
    for ( int i = 0; i < i1.m_NameRecords.size(); i++ ) {
        cout << i1.m_NameRecords[i];
    }
    */
    assert ( ! i1 . AtEnd () && i1 . Surname () == "Hacker" && i1 . Name () == "John" );
  
    assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22" } ) );
    i1 . Next ();
    assert ( ! i1 . AtEnd () && i1 . Surname () == "Smith" && i1 . Name () == "John" );
    assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
    i1 . Next ();
    assert ( ! i1 . AtEnd () && i1 . Surname () == "White" && i1 . Name () == "George" );
    assert ( checkPerson ( b1, "George", "White", { "XYZ-11-22" } ) );
    i1 . Next ();
    assert ( i1 . AtEnd () );

    CRegister b2;
    assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
    assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
    assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
    assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
   
    /*
    cout << "PART 1: " << endl;
   
    for ( int i = 0; i < b2 . m_byNameRecords.size(); i++ ) {
        cout << b2 . m_byNameRecords[i];
    }
            
    cout << endl << "Listing records by RZ --------" << endl;
    for ( int i = 0; i < b2 . m_byRZRecords.size(); i++ ) {
        cout << b2 . m_byRZRecords[i];
    }
    cout << "PART 1 END. " << endl;
    cin.get();
     */
    assert ( b2 . DelCar ( "AAA-11-11" ) == false );
    /*
    cout << "PART 2: " << endl;
    
    for ( int i = 0; i < b2 . m_byNameRecords.size(); i++ ) {
        cout << b2 . m_byNameRecords[i];
    }
             
    cout << endl << "Listing records by RZ --------" << endl;
    for ( int i = 0; i < b2 . m_byRZRecords.size(); i++ ) {
        cout << b2 . m_byRZRecords[i];
    }
    cout << "PART 2 END. " << endl;
    cin.get();
    
    */
    assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
    /*
    cout << "PART 3: " << endl;
    
    for ( int i = 0; i < b2 . m_byNameRecords.size(); i++ ) {
        cout << b2 . m_byNameRecords[i];
    }
             
    cout << endl << "Listing records by RZ --------" << endl;
    for ( int i = 0; i < b2 . m_byRZRecords.size(); i++ ) {
        cout << b2 . m_byRZRecords[i];
    }
    cout << "PART 3 END. " << endl;
    cin.get();
    */
    
    
    assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
   /*
    cout << "PART 4: " << endl;
     
     for ( int i = 0; i < b2 . m_byNameRecords.size(); i++ ) {
         cout << b2 . m_byNameRecords[i];
     }
              
     cout << endl << "Listing records by RZ --------" << endl;
     for ( int i = 0; i < b2 . m_byRZRecords.size(); i++ ) {
         cout << b2 . m_byRZRecords[i];
     }
     cout << "PART 4 END. " << endl;
    cin.get();
    */
    
    assert ( b2 . CountCars ( "George", "White" ) == 0 );
    assert ( checkPerson ( b2, "George", "White", {  } ) );
    /*
    cout << "PART 5: " << endl;
        
        for ( int i = 0; i < b2 . m_byNameRecords.size(); i++ ) {
            cout << b2 . m_byNameRecords[i];
        }
                 
        cout << endl << "Listing records by RZ --------" << endl;
        for ( int i = 0; i < b2 . m_byRZRecords.size(); i++ ) {
            cout << b2 . m_byRZRecords[i];
        }
        cout << "PART 5 END. " << endl;
       cin.get();
    */
    
    CPersonList i2 = b2 . ListPersons ();
    /*
    cout << "LMAO" << endl;
    for ( int i = 0; i < i2.m_NameRecords.size(); i++ ) {
        cout << i2.m_NameRecords[i];
    }
    */
    assert ( ! i2 . AtEnd () && i2 . Surname () == "Hacker" && i2 . Name () == "John" );
    assert ( checkPerson ( b2, "John", "Hacker", { "ABC-32-22" } ) );
    i2 . Next ();
    assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "John" );
    assert ( checkPerson ( b2, "John", "Smith", { "ABC-12-34" } ) );
    i2 . Next ();
    assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "Peter" );
    assert ( checkPerson ( b2, "Peter", "Smith", { "XYZ-11-22" } ) );
    i2 . Next ();
    assert ( i2 . AtEnd () );
    
   
    return 0;
}
#endif /* __PROGTEST__ */
