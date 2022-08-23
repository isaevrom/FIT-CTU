#ifndef __PROGTEST__
#include "sample.h"

namespace regexp {

Alternation::Alternation(RegExp left, RegExp right)
    : m_left(std::move(left))
    , m_right(std::move(right))
{
}
Concatenation::Concatenation(RegExp left, RegExp right)
    : m_left(std::move(left))
    , m_right(std::move(right))
{
}
Iteration::Iteration(RegExp node)
    : m_node(std::move(node))
{
}
Symbol::Symbol(alphabet::Symbol symb)
    : m_symbol(std::move(symb))
{
}


#if 1
/** @brief Example usage of std::variant visitor using overloaded struct and lambda functions */
void to_string(const regexp::RegExp& r, std::ostream& os)
{
    std::visit(overloaded{
                   [&os](const std::shared_ptr<regexp::Alternation>& arg) { os << '('; to_string(arg->m_left, os); os << '+'; to_string(arg->m_right, os); os << ')'; },
                   [&os](const std::shared_ptr<regexp::Concatenation>& arg) { os << '('; to_string(arg->m_left, os); os << ' '; to_string(arg->m_right, os); os << ')'; },
                   [&os](const std::shared_ptr<regexp::Iteration>& arg) { os << '('; to_string(arg->m_node, os); os << ")*"; },
                   [&os](const std::shared_ptr<regexp::Symbol>& arg) { os << arg->m_symbol; },
                   [&os](const std::shared_ptr<regexp::Epsilon>& arg) { os << "#E"; },
                   [&os](const std::shared_ptr<regexp::Empty>& arg) { os << "#0"; },
               },
               r);
}
#else
/** @brief Example usage of std::visitor without overloaded struct with lambdas, using std::holds_alternative and std::get instead */
void to_string(const regexp::RegExp& r, std::ostream& os)
{
    if (std::holds_alternative<std::shared_ptr<regexp::Alternation>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Alternation>>(r);
        os << '(';
        to_string(node->m_left, os);
        os << ' ';
        to_string(node->m_right, os);
        os << ')';
    } else if (std::holds_alternative<std::shared_ptr<regexp::Concatenation>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Concatenation>>(r);
        os << '(';
        to_string(node->m_left, os);
        os << ' ';
        to_string(node->m_right, os);
        os << ')';
    } else if (std::holds_alternative<std::shared_ptr<regexp::Iteration>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Iteration>>(r);
        os << '(';
        to_string(node->m_node, os);
        os << ")*";
    } else if (std::holds_alternative<std::shared_ptr<regexp::Symbol>>(r)) {
        os << std::get<std::shared_ptr<regexp::Symbol>>(r)->m_symbol;
    } else if (std::holds_alternative<std::shared_ptr<regexp::Epsilon>>(r)) {
        os << "#E";
    } else if (std::holds_alternative<std::shared_ptr<regexp::Empty>>(r)) {
        os << "#0";
    } else {
        __builtin_unreachable();
    }
}
#endif

/** @brief Convenience function for converting RegExps into the ALT string format (usable in the ALT tool) */
std::ostream& operator<<(std::ostream& os, const regexp::RegExp& regexp)
{
    to_string(regexp, os);
    return os;
}
}

namespace automaton {
/** @brief Convenience function for converting NFAs into the ALT string format (usable in the ALT tool) */
std::ostream& operator<<(std::ostream& os, const automaton::NFA& nfa)
{
    os << "NFA ";
    for (const auto& symb : nfa.m_Alphabet)
        os << symb << " ";
    os << '\n';

    for (const auto& state : nfa.m_States) {
        os << (nfa.m_InitialState == state ? ">" : " ") << (nfa.m_FinalStates.count(state) ? "<" : " ") << state;

        for (const auto& symbol : nfa.m_Alphabet) {
            if (auto iter = nfa.m_Transitions.find({state, symbol}); iter != nfa.m_Transitions.end()) {
                os << " ";
                for (auto i = iter->second.begin(); i != iter->second.end(); ++i) {
                    os << (i != iter->second.begin() ? "|" : "") << *i;
                }
            } else {
                os << " -";
            }
        }
        os << std::endl;
    }
    return os;
}

bool operator==(const automaton::NFA& a, const automaton::NFA& b)
{
    return std::tie(a.m_States, a.m_Alphabet, a.m_Transitions, a.m_InitialState, a.m_FinalStates) == std::tie(b.m_States, b.m_Alphabet, b.m_Transitions, b.m_InitialState, b.m_FinalStates);
}
}
#endif

void enumerate(const regexp::RegExp& r, std::ostream & os, std::map<std::shared_ptr<regexp::Symbol>, int> & enumerated, int & i, std::set<char> & alphabet_set) {
    std::visit(overloaded{
                   [&os, &enumerated, &i, &alphabet_set](const std::shared_ptr<regexp::Alternation>& arg) { os << '('; enumerate(arg->m_left, os, enumerated, i, alphabet_set); os << '+'; enumerate(arg->m_right, os, enumerated, i, alphabet_set); os << ')'; },
                   [&os, &enumerated, &i, &alphabet_set](const std::shared_ptr<regexp::Concatenation>& arg) { os << '('; enumerate(arg->m_left, os, enumerated, i, alphabet_set); os << ' '; enumerate(arg->m_right, os, enumerated, i, alphabet_set); os << ')'; },
                   [&os, &enumerated, &i, &alphabet_set](const std::shared_ptr<regexp::Iteration>& arg) { os << '('; enumerate(arg->m_node, os, enumerated, i, alphabet_set); os << ")*"; },
                   [&os, &enumerated, &i, &alphabet_set](const std::shared_ptr<regexp::Symbol>& arg) { os << arg->m_symbol; enumerated[arg] = ++i; alphabet_set.insert(arg->m_symbol); },
                   [&os](const std::shared_ptr<regexp::Epsilon>& arg) { os << "#E"; },
                   [&os](const std::shared_ptr<regexp::Empty>& arg) { os << "#0"; },
               },
               r);
}

bool has_epsilon(const regexp::RegExp& r) {
    if (std::holds_alternative<std::shared_ptr<regexp::Alternation>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Alternation>>(r);
        
        return has_epsilon(node->m_left) || has_epsilon(node->m_right);
    } else if (std::holds_alternative<std::shared_ptr<regexp::Concatenation>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Concatenation>>(r);
        
        return has_epsilon(node->m_left) && has_epsilon(node->m_right);
    } else if (std::holds_alternative<std::shared_ptr<regexp::Iteration>>(r)) {
        return true;
    } else if (std::holds_alternative<std::shared_ptr<regexp::Symbol>>(r)) {
        return false;
    } else if (std::holds_alternative<std::shared_ptr<regexp::Epsilon>>(r)) {
        return true;
    } else if (std::holds_alternative<std::shared_ptr<regexp::Empty>>(r)) {
        return false;
    } else {
        __builtin_unreachable();
    }
}

void construct_z(const regexp::RegExp& r, std::map<std::shared_ptr<regexp::Symbol>, int> & z, const std::map<std::shared_ptr<regexp::Symbol>, int> & enumerated) {
    std::visit(overloaded{
                   [&z, &enumerated](const std::shared_ptr<regexp::Alternation>& arg) {
                       construct_z(arg->m_left, z, enumerated);
                       construct_z(arg->m_right, z, enumerated);
                   },
        
                   [&z, &enumerated](const std::shared_ptr<regexp::Concatenation>& arg) {
                       if (has_epsilon(arg->m_left)) {
                           construct_z(arg->m_left, z, enumerated);
                           construct_z(arg->m_right, z, enumerated);
                       } else {
                           construct_z(arg->m_left, z, enumerated);
                       }
                   },
        
                   [&z, &enumerated](const std::shared_ptr<regexp::Iteration>& arg) {
                       construct_z(arg->m_node, z, enumerated);
                   },
        
                   [&z, &enumerated](const std::shared_ptr<regexp::Symbol>& arg) {
                       z[arg] = enumerated.at(arg);
                   },
        
                   [](const std::shared_ptr<regexp::Epsilon>& arg) {
                   },
        
                   [](const std::shared_ptr<regexp::Empty>& arg) {
                   },
               },
               r);
}

void construct_f(const regexp::RegExp& r, std::map<std::shared_ptr<regexp::Symbol>, int> & f, const std::map<std::shared_ptr<regexp::Symbol>, int> & enumerated) {
    std::visit(overloaded{
                   [&f, &enumerated](const std::shared_ptr<regexp::Alternation>& arg) {
                       construct_f(arg->m_left, f, enumerated);
                       construct_f(arg->m_right, f, enumerated);
                   },
        
                   [&f, &enumerated](const std::shared_ptr<regexp::Concatenation>& arg) {
                       if (has_epsilon(arg->m_right)) {
                           construct_f(arg->m_left, f, enumerated);
                           construct_f(arg->m_right, f, enumerated);
                       } else {
                           construct_f(arg->m_right, f, enumerated);
                       }
                   },
        
                   [&f, &enumerated](const std::shared_ptr<regexp::Iteration>& arg) {
                       construct_f(arg->m_node, f, enumerated);
                   },
        
                   [&f, &enumerated](const std::shared_ptr<regexp::Symbol>& arg) {
                       f[arg] = enumerated.at(arg);
                   },
        
                   [](const std::shared_ptr<regexp::Epsilon>& arg) {
                   },
        
                   [](const std::shared_ptr<regexp::Empty>& arg) {
                   },
               },
               r);
}

void construct_p(const regexp::RegExp& r,
                 std::map< std::pair<std::shared_ptr<regexp::Symbol>, int>, std::set< std::pair< std::shared_ptr<regexp::Symbol>, int> > > & p,
                 const std::map<std::shared_ptr<regexp::Symbol>, int> & enumerated) {
    std::visit(overloaded{
                   [&p, &enumerated](const std::shared_ptr<regexp::Alternation>& arg) {
                       construct_p(arg->m_left, p, enumerated);
                       construct_p(arg->m_right, p, enumerated);
                   },
        
                   [&p, &enumerated](const std::shared_ptr<regexp::Concatenation>& arg) {
                       construct_p(arg->m_left, p, enumerated);
                       construct_p(arg->m_right, p, enumerated);
                       // f(v1) x z(v2)
                       // 1. construct f
                       // 2. construct z
                       // if both sizes != 0, then insert x into map
                       std::map<std::shared_ptr<regexp::Symbol>, int> tmp_f;
                       std::map<std::shared_ptr<regexp::Symbol>, int> tmp_z;
                       construct_f(arg->m_left, tmp_f, enumerated);
                       construct_z(arg->m_right, tmp_z, enumerated);
                       if (tmp_f.size() > 0 && tmp_z.size() > 0) {
                           for (auto v1 = tmp_f.begin(); v1 != tmp_f.end(); v1++) {
                               for (auto v2 = tmp_z.begin(); v2 != tmp_z.end(); v2++) {
                                   p[std::make_pair(v1->first, v1->second)].insert(std::make_pair(v2->first, v2->second));
                               }
                           }
                       }
                   },
        
                   [&p, &enumerated](const std::shared_ptr<regexp::Iteration>& arg) {
                       construct_p(arg->m_node, p, enumerated);
                       // f(v1) x z(v1)
                       std::map<std::shared_ptr<regexp::Symbol>, int> tmp_f;
                       std::map<std::shared_ptr<regexp::Symbol>, int> tmp_z;
                       construct_f(arg->m_node, tmp_f, enumerated);
                       construct_z(arg->m_node, tmp_z, enumerated);
                       if (tmp_f.size() > 0 && tmp_z.size() > 0) {
                           for (auto fv1 = tmp_f.begin(); fv1 != tmp_f.end(); fv1++) {
                               for (auto zv1 = tmp_z.begin(); zv1 != tmp_z.end(); zv1++) {
                                   p[std::make_pair(fv1->first, fv1->second)].insert(std::make_pair(zv1->first, zv1->second));
                               }
                           }
                       }
                   },
        
                   [](const std::shared_ptr<regexp::Symbol>& arg) {
                   },
        
                   [](const std::shared_ptr<regexp::Epsilon>& arg) {
                   },
        
                   [](const std::shared_ptr<regexp::Empty>& arg) {
                   },
               },
               r);
}

void print_map(std::ostream & os, const std::string & name, std::map<std::shared_ptr<regexp::Symbol>, int> my_map) {
    std::cout << name << ":" << my_map.size() << std::endl;
    std::cout << "{" << std::endl;

    for (auto it = my_map.begin(); it != my_map.end(); it++)
        std::cout << "\t" << it->first->m_symbol << it->second << ";" << std::endl;
    
    std::cout << "}" << std::endl;
}

void print_pairs(std::ostream & os, const std::string & name,
                 std::map< std::pair<std::shared_ptr<regexp::Symbol>, int>, std::set< std::pair< std::shared_ptr<regexp::Symbol>, int> > > & my_map) {
    std::cout << name << ":" << my_map.size() << std::endl;
    std::cout << "{" << std::endl;

    for (auto it = my_map.begin(); it != my_map.end(); it++) {
        std::cout << "\t" << it->first.first->m_symbol << it->first.second << "\t->";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            std::cout << "\t" << it2->first->m_symbol << it2->second << ";";
        }
        std::cout << std::endl;
    }
    
    std::cout << "}" << std::endl;
}

automaton::NFA convert(const regexp::RegExp& regexp)
{
    // TODO: implement
    std::set<char> alphabet_set;

    /* ENUMERATION */
    int i = 0;
    std::map<std::shared_ptr<regexp::Symbol>, int> enumerated;
    enumerate(regexp, std::cout, enumerated, i, alphabet_set);
    std::cout << "\n";
    print_map( std::cout, "ENUMERATED", enumerated);

    /* Z */
    std::map<std::shared_ptr<regexp::Symbol>, int> z;
    construct_z(regexp, z, enumerated);
    print_map( std::cout, "Z", z);
    
    /* F */
    std::map<std::shared_ptr<regexp::Symbol>, int> f;
    construct_f(regexp, f, enumerated);
    print_map( std::cout, "F", f);
    
    /* P */
    std::map< std::pair<std::shared_ptr<regexp::Symbol>, int>, std::set< std::pair< std::shared_ptr<regexp::Symbol>, int> > > p;
    construct_p(regexp, p, enumerated);
    print_pairs( std::cout, "P", p);
    
    
    /* ALPHABET */
    std::cout << "ALPHABET:" << std::endl;
    for (auto it = alphabet_set.begin(); it != alphabet_set.end(); it++) {
        std::cout << *it << "; ";
    }
    std::cout << std::endl;
    
    /* # # # # # # #  CONVERSION  # # # # # # # # # */
    // NKA = (Q, \Sigma, \delta, q0, F)
    automaton::NFA nfa;
    //1
    for (unsigned int i = 0; i < enumerated.size() + 1; i++)
        nfa.m_States.insert(i);
    
    //2
    nfa.m_Alphabet = alphabet_set;
    
    //3 --     std::map< std::pair<State, alphabet::Symbol>, std::set<State> > m_Transitions;
    for (auto it = z.begin(); it != z.end(); it++) {
        nfa.m_Transitions[std::make_pair(0, it->first->m_symbol)].insert(it->second);
    }
    
    for (auto it = p.begin(); it != p.end(); it++) {
        for (auto set_it = it->second.begin(); set_it != it->second.end(); set_it++) {
            nfa.m_Transitions[std::make_pair(it->first.second, set_it->first->m_symbol)].insert(set_it->second);
        }
    }
    //4
    nfa.m_InitialState = 0;
    
    //5
    for (auto it = f.begin(); it != f.end(); it++) {
        nfa.m_FinalStates.insert(it->second);
    }
    if (has_epsilon(regexp))
        nfa.m_FinalStates.insert(0);
    
    std::cout << nfa;
    
    return nfa;
}

#ifndef __PROGTEST__
regexp::RegExp tests[] = {
    std::make_shared<regexp::Iteration>(
        std::make_shared<regexp::Concatenation>(
                                                
            std::make_shared<regexp::Iteration>(
                std::make_shared<regexp::Alternation>(
                    std::make_shared<regexp::Symbol>('a'),
                    std::make_shared<regexp::Symbol>('b'))),
                                                
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Symbol>('a'),
                std::make_shared<regexp::Concatenation>(
                    std::make_shared<regexp::Symbol>('b'),
                    std::make_shared<regexp::Iteration>(
                        std::make_shared<regexp::Alternation>(
                            std::make_shared<regexp::Symbol>('a'),
                            std::make_shared<regexp::Symbol>('b')))))))

    ,

    std::make_shared<regexp::Iteration>(
        std::make_shared<regexp::Alternation>(
            std::make_shared<regexp::Alternation>(
                std::make_shared<regexp::Symbol>('a'),
                std::make_shared<regexp::Epsilon>()),
            std::make_shared<regexp::Alternation>(
                std::make_shared<regexp::Symbol>('b'),
                std::make_shared<regexp::Empty>()))),

    std::make_shared<regexp::Concatenation>(
        std::make_shared<regexp::Iteration>(
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Symbol>('a'),
                std::make_shared<regexp::Concatenation>(
                    std::make_shared<regexp::Alternation>(
                        std::make_shared<regexp::Empty>(),
                        std::make_shared<regexp::Epsilon>()),
                    std::make_shared<regexp::Concatenation>(
                        std::make_shared<regexp::Iteration>(
                            std::make_shared<regexp::Symbol>('b')),
                        std::make_shared<regexp::Symbol>('a'))))),
        std::make_shared<regexp::Alternation>(
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Symbol>('b'),
                std::make_shared<regexp::Iteration>(
                    std::make_shared<regexp::Symbol>('c'))),
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Iteration>(
                    std::make_shared<regexp::Symbol>('a')),
                std::make_shared<regexp::Alternation>(
                    std::make_shared<regexp::Epsilon>(),
                    std::make_shared<regexp::Concatenation>(
                        std::make_shared<regexp::Symbol>('b'),
                        std::make_shared<regexp::Empty>()))))),
 
};

// Your solution might give another (equivalent) answers. See task details.
automaton::NFA results[] = {
    {{0, 1, 2, 3, 4, 5, 6},
     {'a', 'b'},
     {
         {{0, 'a'}, {1, 3}},
         {{0, 'b'}, {2}},
         {{1, 'a'}, {1, 3}},
         {{1, 'b'}, {2}},
         {{2, 'a'}, {1, 3}},
         {{2, 'b'}, {2}},
         {{3, 'b'}, {4}},
         {{4, 'a'}, {1, 3, 5}},
         {{4, 'b'}, {2, 6}},
         {{5, 'a'}, {1, 3, 5}},
         {{5, 'b'}, {2, 6}},
         {{6, 'a'}, {1, 3, 5}},
         {{6, 'b'}, {2, 6}},
     },
     0,
     {0, 4, 5, 6}}

    ,

    {{0, 1, 2},
     {'a', 'b'},
     {
         {{0, 'a'}, {1}},
         {{0, 'b'}, {2}},
         {{1, 'a'}, {1}},
         {{1, 'b'}, {2}},
         {{2, 'a'}, {1}},
         {{2, 'b'}, {2}},
     },
     0,
     {0, 1, 2}},

    {{0, 1, 2, 3, 4, 5, 6, 7},
     {'a', 'b', 'c'},
     {
         {{0, 'a'}, {1, 6}},
         {{0, 'b'}, {4, 7}},
         {{1, 'a'}, {3}},
         {{1, 'b'}, {2}},
         {{2, 'a'}, {3}},
         {{2, 'b'}, {2}},
         {{3, 'a'}, {1, 6}},
         {{3, 'b'}, {4, 7}},
         {{4, 'c'}, {5}},
         {{5, 'c'}, {5}},
         {{6, 'a'}, {6}},
         {{6, 'b'}, {7}},
     },
     0,
     {0, 3, 4, 5, 6}},
         
};

int main()
{
    std::cout << "RV:\n";
    to_string(tests[0], std::cout);
    std::cout << "\n";

    assert(convert(tests[0]) == results[0]);
    
    assert(convert(tests[1]) == results[1]);
    assert(convert(tests[2]) == results[2]);
     
}
#endif
