#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CSex {
public:
      CSex(int id, const string & name)
    : m_id(id), m_name(name) {}
    virtual ~CSex() = default;

    virtual string   GetName() const { return m_name; }
    virtual int      GetID() const = 0;
    void             SetName(const string & newName) { m_name = newName; }
    friend ostream & operator<<(ostream & os, const CSex & rhs) {
        rhs.PrintPersonInfo(os, rhs);
        return os;
    }
    virtual void     PrintPersonInfo(ostream & os, const CSex & rhs) const = 0;
    void     AddParents( const shared_ptr<CSex> & father, const shared_ptr<CSex> & mother ) {
       if (father)
           m_parents.push_back(father->GetID());
       if (mother)
           m_parents.push_back(mother->GetID());
    }
    virtual void     AddDescendant( const shared_ptr<CSex> & person ) { m_descendants.push_back(person); }
    virtual size_t   CountDescendants () const { return DescendantsCalculator(); } // TODO functionToCalculateDescendants
    // TODO:
    virtual size_t   DescendantsCalculator() const;
    virtual bool     HasPrefix(const string & prefix) const = 0;
    virtual const vector<shared_ptr<CSex>> & getDescendants() const {
        return m_descendants;
    }
    virtual const vector<int> & getParents() const {
        return m_parents;
    }
protected:
    int m_id;
    string m_name;
    vector<shared_ptr<CSex>> m_descendants;
    //shared_ptr<CSex> m_mother, m_father;
    //vector<weak_ptr<CSex>> m_parents;
    vector<int> m_parents;
};
size_t CSex::DescendantsCalculator() const {
    set<int> descendantsIDs;
    queue<shared_ptr<CSex>> q;
    shared_ptr<CSex> curr;
    for ( auto &it : m_descendants ) {
        q.push(it);
        descendantsIDs.insert(it->GetID());
    }

    while ( !q.empty() ) {
        curr = q.front();
        q.pop();
        for ( auto &it : curr->m_descendants ) {
            q.push(it);
            if (! descendantsIDs.count(it->GetID()))
                descendantsIDs.insert(it->GetID());
        }
    }
    return descendantsIDs.size();
}

class CMan : public CSex
{
public:
     CMan(int id, const string & name)
    : CSex(id, name) {}
    ~CMan() override = default;

    virtual int  GetID() const override { return m_id; }
    virtual void PrintPersonInfo(ostream & os, const CSex & rhs) const override {
        os << m_id << ": " << m_name << " (man)";
    }
    virtual bool HasPrefix(const string & prefix) const override {
        if (prefix.size() > m_name.size()) return false;
        for ( size_t i = 0; i < prefix.size();  i++)
            if ( prefix[i] != m_name[i] ) return false;
        return true;
    }
};

class CWoman : public CSex
{
public:
     CWoman(int id, const string & name)
    : CSex(id, name) { isMarried = false; }
    ~CWoman() override = default;

    virtual int  GetID() const override { return m_id; }
    virtual void PrintPersonInfo(ostream & os, const CSex & rhs) const override {
        os << m_id << ": " << m_name;
        isMarried  ?  os << " [born: " << maidenName << "]"
                   :  os << "";
        os << " (woman)";
    }
    virtual bool HasPrefix(const string & prefix) const override {
        bool isPrefixOfMarriedName = true,
             isPrefixOfMaidenName  = true;

        if (prefix.size() > m_name.size())
            isPrefixOfMarriedName = false;
        else
            for ( size_t i = 0; i < prefix.size();  i++)
                if ( prefix[i] != m_name[i] )
                    isPrefixOfMarriedName = false;

        if (prefix.size() > maidenName.size())
            isPrefixOfMaidenName = false;
        else
            for ( size_t i = 0; i < prefix.size();  i++)
                if ( prefix[i] != maidenName[i] )
                    isPrefixOfMaidenName = false;

        return isPrefixOfMarriedName || isPrefixOfMaidenName;
    }
    bool Wedding(const string & marriedName) {
        if (isMarried)
            return false;
        maidenName = m_name;
        m_name     = marriedName;
        isMarried  = true;
        return true;
    }
private:
    // todo
    string maidenName;
    bool isMarried; // set to be false in the constructor body
};

class CRegister
{
public:
    CRegister() = default;
    bool Add (const shared_ptr<CSex> & person, const shared_ptr<CSex> & father, const shared_ptr<CSex> & mother);
    shared_ptr<CSex>         FindByID (const int & id) const;
    vector<shared_ptr<CSex>> FindByName ( const string & prefix ) const;
    list<shared_ptr<CSex>>   FindRelatives ( const int & id1, const int & id2 ) const;
    void ParentsBFS(list<shared_ptr<CSex>> & list, const int & id1, const int & id2) const;
    void DescendantsBFS(list<shared_ptr<CSex>> & list, const int & id1, const int & id2) const;
private:
    // todo
    set<int> recordsOfIDs;
    map<int, shared_ptr<CSex>> personsRecordsByID;
};

bool CRegister::Add (const shared_ptr<CSex> & person, const shared_ptr<CSex> & father, const shared_ptr<CSex> & mother) {
    if (personsRecordsByID.count(person->GetID()))
        return false;
    person->AddParents(father, mother); // either a legit f/m or nullptr
    personsRecordsByID[ person->GetID() ] = person;
    if (father) father->AddDescendant(person);
    if (mother) mother->AddDescendant(person);
    return true;
}

shared_ptr<CSex> CRegister::FindByID (const int & id) const {
    try {
        return personsRecordsByID.at(id);
    } catch( out_of_range & ofr ) {
        return nullptr;
    }
}

vector<shared_ptr<CSex>> CRegister::FindByName ( const string & prefix ) const {
    vector<shared_ptr<CSex>> tmpPersonsByName;
    for ( auto & it : personsRecordsByID )
        if (it.second->HasPrefix(prefix)) // TODO: HasPrefix
            tmpPersonsByName.push_back(it.second);
    sort(tmpPersonsByName.begin(), tmpPersonsByName.end(),
         [](const shared_ptr<CSex> & a, const shared_ptr<CSex> & b) {
                     return a->GetID() < b->GetID();
                }
    );
    return tmpPersonsByName;
}

void CRegister::DescendantsBFS(list<shared_ptr<CSex>> & list, const int & id1, const int & id2) const {
    //queue<shared_ptr<CSex>> q;
    set<int> descendantsIDs;
    queue<int> q;
    shared_ptr<CSex> curr;
    map<int,int> isSonOf;

    q.push(id1);
    while (!q.empty()) {
        curr = personsRecordsByID.at(q.front());
        q.pop();
        for ( auto parent_id: curr->getParents() ) {
            if (! descendantsIDs.count(parent_id)) {
                q.push(parent_id);
                descendantsIDs.insert(parent_id);
                isSonOf[parent_id] = curr->GetID();
                if ( parent_id == id2 ) {
                    // TODO: restore path and return list
                    list.push_front(personsRecordsByID.at(id2));
                    int tmp = id2;
                    while(true) {
                        if (tmp == id1)
                            return;
                        list.push_front(personsRecordsByID.at(isSonOf[tmp]));
                        tmp = isSonOf[tmp];
                    }
                }
            }
        }

        for ( auto &it: curr->getDescendants() ) {
            if (! descendantsIDs.count(it->GetID())) {
                q.push(it->GetID());
                descendantsIDs.insert(it->GetID());
                isSonOf[it->GetID()] = curr->GetID();
                if ( it->GetID() == id2 ) {
                    // TODO: restore path and return list
                    list.push_front(personsRecordsByID.at(id2));
                    int tmp = id2;
                    while(true) {
                        if (tmp == id1)
                            return;
                        list.push_front(personsRecordsByID.at(isSonOf[tmp]));
                        tmp = isSonOf[tmp];
                    }
                }
            }
        }
    }
}
list<shared_ptr<CSex>>   CRegister::FindRelatives ( const int & id1, const int & id2 ) const {
    // list := id1, common_relatives, id2
    list<shared_ptr<CSex>> listViaDescendants;
    if ( personsRecordsByID.count(id1) == 0 || personsRecordsByID.count(id2) == 0 ) {
        throw invalid_argument("New Accounts should have a positive balance.");
    }
    // common_relatives
    DescendantsBFS(listViaDescendants, id1, id2);

    return listViaDescendants;
}

#ifndef __PROGTEST__
template <typename T_>
static bool        vectorMatch ( const vector<T_>     & res,
                                 const vector<string> & ref )
{
    vector<string> tmp;
    for ( const auto & x : res )
    {
        ostringstream oss;
        oss << *x;
        tmp . push_back ( oss . str () );
    }
    return tmp == ref;
}
template <typename T_>
static bool        listMatch ( const list<T_>     & res,
                               const list<string> & ref )
{
    list<string> tmp;
    for ( const auto & x : res )
    {
        ostringstream oss;
        oss << *x;
        tmp . push_back ( oss . str () );
    }
    return tmp == ref;
}
int main ( void )
{
    ostringstream oss;
    CRegister r;
    assert ( r . Add ( make_shared<CMan> ( 1, "Peterson George" ),
                       nullptr, nullptr ) == true );
    assert ( r . Add ( make_shared<CMan> ( 2, "Watson Paul" ),
                       nullptr, nullptr ) == true );
    assert ( r . Add ( make_shared<CMan> ( 10, "Smith Samuel" ),
                     nullptr, nullptr ) == true );


    assert ( r . Add ( make_shared<CWoman> ( 11, "Peterson Jane" ),
                     r . FindByID ( 1 ), nullptr ) == true );


  assert ( r . Add ( make_shared<CWoman> ( 12, "Peterson Sue" ),
                     r . FindByID ( 1 ), nullptr ) == true );
  assert ( r . Add ( make_shared<CMan> ( 13, "Pershing John" ),
                     nullptr, nullptr ) == true );

  assert ( dynamic_cast<CWoman &> ( *r . FindByID ( 12 ) ) . Wedding ( "Smith Sue" ) == true );
  assert ( r . Add ( make_shared<CMan> ( 100, "Smith John" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CMan> ( 101, "Smith Roger" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CMan> ( 102, "Smith Daniel" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 103, "Smith Eve" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 103, "Smith Jane" ),
                     r . FindByID ( 10 ), r . FindByID ( 11 ) ) == false );
  assert ( r . Add ( make_shared<CMan> ( 150, "Pershing Joe" ),
                     r . FindByID ( 13 ), r . FindByID ( 12 ) ) == true );
  assert ( r . Add ( make_shared<CMan> ( 200, "Pershing Peter" ),
                     r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 201, "Pershing Julia" ),
                     r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
  assert ( r . Add ( make_shared<CWoman> ( 202, "Pershing Anne" ),
                     r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
/*
  for ( auto it : r . FindByName ( "Peterson" ) )
      cout << *it << endl;
*/
  assert ( vectorMatch ( r . FindByName ( "Peterson" ), vector<string>
           {
             "1: Peterson George (man)",
             "11: Peterson Jane (woman)",
             "12: Smith Sue [born: Peterson Sue] (woman)"
           } ) );
/*
  for ( auto it : r . FindByName ( "Pe" ) )
      cout << *it << endl;
*/

  assert ( vectorMatch ( r . FindByName ( "Pe" ), vector<string>
           {
             "1: Peterson George (man)",
             "11: Peterson Jane (woman)",
             "12: Smith Sue [born: Peterson Sue] (woman)",
             "13: Pershing John (man)",
             "150: Pershing Joe (man)",
             "200: Pershing Peter (man)",
             "201: Pershing Julia (woman)",
             "202: Pershing Anne (woman)"
           } ) );

    assert ( vectorMatch ( r . FindByName ( "Smith" ), vector<string>
             {
               "10: Smith Samuel (man)",
               "12: Smith Sue [born: Peterson Sue] (woman)",
               "100: Smith John (man)",
               "101: Smith Roger (man)",
               "102: Smith Daniel (man)",
               "103: Smith Eve (woman)"
             } ) );

    assert ( r . FindByID ( 1 ) -> GetID () == 1 );
    oss . str ( "" );
    oss << * r . FindByID ( 1 );
    assert ( oss . str () == "1: Peterson George (man)" );

    assert ( r . FindByID ( 1 ) -> CountDescendants () == 10 );

    assert ( r . FindByID ( 2 ) -> GetID () == 2 );
    oss . str ( "" );
    oss << * r . FindByID ( 2 );
    assert ( oss . str () == "2: Watson Paul (man)" );
    assert ( r . FindByID ( 2 ) -> CountDescendants () == 0 );

    for ( auto it : r . FindRelatives( 100, 1 ) )
        cout << *it << endl;

    assert ( listMatch ( r . FindRelatives ( 100, 1 ), list<string>
             {
               "100: Smith John (man)",
               "11: Peterson Jane (woman)",
               "1: Peterson George (man)"
             } ) );

    assert ( listMatch ( r . FindRelatives ( 100, 13 ), list<string>
             {
               "100: Smith John (man)",
               "10: Smith Samuel (man)",
               "103: Smith Eve (woman)",
               "200: Pershing Peter (man)",
               "150: Pershing Joe (man)",
               "13: Pershing John (man)"
             } ) );
    assert ( listMatch ( r . FindRelatives ( 100, 2 ), list<string>
             {

             } ) );
    assert ( listMatch ( r . FindRelatives ( 100, 100 ), list<string>
             {
               "100: Smith John (man)"
             } ) );

    try
    {
      r . FindRelatives ( 100, 3 );
      assert ( "Missing an exception" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
    }
    catch ( ... )
    {
      assert ( "An unexpected exception thrown" );
    }

    return 0;
}
#endif /* __PROGTEST__ */
