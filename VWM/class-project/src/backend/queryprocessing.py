import pyeda.boolalg.expr
import re
from backend import textprocessor

tests = [
    '(((x AND y) OR (y OR z)) AND w) OR x',
    '(a OR b)',
    '(NOT (a OR b))',
    '(((x AND y) OR ((NOT y) OR z)) AND (NOT w))',
    '(x AND y OR z AND d) OR u',
    'o AND e',
    'swimming AND mouse AND NOT being OR (u OR v)',
    'trump',
    'oil OR gas',
    'arabic AND wonderful AND (pink OR red) AND violin',
    'boiler OR mix OR purple',
    "digest AND armory",
    "fdr AND mob AND madness",
    "NOT biden AND NOT predator AND trump AND wad",
    "~biden AND ~biden",
    "~kistelbunkez AND biden AND ~biden",
    '~y AND ~z',
    'y AND ~z',
    'enact',
    'trump OR trump',
    'trump OR biden',
    'trump AND biden',
    '~trump AND biden',
    'trump AND ~biden',
    'black',
    'pink AND yellow OR (white AND black AND NOT orange AND good) OR yes',
    'trump OR black AND yellow',
    'take AND hot OR fast AND ~slow',
    'orange AND blue AND map AND ~skyline AND sky AND line AND ~weather',
    '~(x AND z)',
    '~((x OR y) AND ~w) OR ~(~y AND z)',
    'green',
    'biden',
    'democrat',
    'democrat AND ~everywhere',
    'democrat OR ~read',
    'democrat AND read AND ~everywhere OR ~democrat',
    '~lgleg AND ~wdje AND ~wiier',
    '~democrat',
    '~democrat AND democrat OR well AND u',
    'direction AND ~dump OR white',
    'farmer',
    '~farmer and sum',
    '~safety AND three OR negotiate AND ~common',
    'safety AND ~safety',
    'safety OR ~safety',
    'going OR swimming',
    '~going AND ~swimming'
]

tests2 = [
    "NOT biden AND NOT predator AND trump AND wad",
    'trump AND ~biden',
    'trump OR black AND yellow',
]


def simplify_query(query):
    """Generate parenthesized contents in string as pairs (level, contents)."""
    # return list(self.__parenthetic_contents(query))
    query = query.lower()
    query = re.sub(" and ", " & ", query)
    query = re.sub(" or ", " | ", query)
    # query = re.sub("not ", "~", query)
    query = re.sub(r"\bnot([( ])", "~", query)
    try:
        query = pyeda.boolalg.expr.expr(query)
    except:
        raise ValueError("Can't process query: >" + query + "<")
    query.simplify()
    query = query.to_dnf()
    # print("DNF:\t\t\t", query)
    """
    Append ';' after each subquery.
    e.g: Or(x, And(y, z), ~w) ---> Or(x; And(y, z); ~w)
    """
    query = str(query)
    i = 0
    while True:
        if i >= len(query):
            break
        if query[i] == 'A':
            while True:
                i += 1
                if query[i] == ')':
                    break
            continue
        if query[i] == ',':
            query = query[:i] + ';' + query[i + 1:]
        i += 1
    """ 
    Getting rid of "Or(" and ")" parts if they are present
    e.g: Or(x, And(y, z), ~w) ---> x; And(y, z); ~w
    """
    if len(query) >= 3:
        if query[:3] == "Or(":
            query = query[3:-1]
    # print("Simplified:\t\t", query)
    return query


class QueryProcessor:
    """QueryProcessor parses queries and returns sub-queries in a proper order"""

    def __init__(self):
        # print("Constructing QueryProcessor...")
        self.textProcessor = textprocessor.TextProcessor()

    def extract_and_terms(self, query):
        """
        Getting rid of "And(" and ")" parts if they are present
        e.g: And(y, z) ---> y, z
        """
        query = query[4:-1]
        terms = list(query.split(','))
        for i in range(len(terms)):
            tmp = terms[i]
            tmp = tmp.replace(' ', '')
            if tmp[0] == '~':
                tmp = '~' + self.textProcessor.lemmatize(tmp[1:])
            else:
                tmp = self.textProcessor.lemmatize(tmp)
            terms[i] = tmp
            # # print("and subquery: " + terms[i])
        return terms

    def parse_query(self, query):
        # print("TODO parsing: " + query)
        terms_list = []
        not_terms_list = []
        and_queries_terms_list = []

        try:
            query = simplify_query(query)
        except ValueError as e:
            raise ValueError(e)

        # # print(query.split(';'))
        subqueries = list(query.split(';'))
        # # print(subqueries)

        for subquery in subqueries:
            subquery = subquery.replace(' ', '')
            # # print("subquery: " + subquery)
            if subquery[0] == '~':
                subquery = '~' + self.textProcessor.lemmatize(subquery[1:])
                not_terms_list.append(subquery)
            elif subquery[0] == 'A':
                and_queries_terms_list.append(self.extract_and_terms(subquery))
            else:
                subquery = self.textProcessor.lemmatize(subquery)
                terms_list.append(subquery)

        return terms_list, not_terms_list, and_queries_terms_list
