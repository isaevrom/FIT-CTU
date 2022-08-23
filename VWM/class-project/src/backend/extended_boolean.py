import time
import math
import nltk
from backend import textprocessor
from backend import mydatabase
from backend import queryprocessing

MY_EPSILON = 0.000000000000001


def is_a_tautology(terms_list, not_terms_list, and_terms_list) -> bool:
    if len(terms_list) == 0:
        return False
    return terms_list[0] == '1' and len(terms_list) + len(not_terms_list) + len(and_terms_list) == 1


def is_a_contradiction(terms_list, not_terms_list, and_terms_list) -> bool:
    if len(terms_list) == 0:
        return False
    return terms_list[0] == '0' and len(terms_list) + len(not_terms_list) + len(and_terms_list) == 1


class ExtendedBooleanModel:
    def __init__(self, is_matrix_used, data_source):
        # Indicates, whether matrix should be used. If false, inverted index list is used instead.
        self.isMatrixUsed = is_matrix_used
        # print("Constructing ExtendedBooleanModel...")
        # [ { term_x (str) : frequency_x (int), ..., term_y : frequency_y },
        #   { ... }, ... ]
        # One dict per document from the database
        self.termFrequencyDicts = []
        # { term_w (str) : frequency_w (int), ... }
        self.globalFrequencyDict = {}
        # Create a simple database for storing documents and their names
        self.database = mydatabase.Database(data_source)
        # Create an instance of a helper class for text processing
        self.textProcessor = textprocessor.TextProcessor()
        # Create a dictionary of inverted lists
        self.invertedListsDict = {}
        # Create an instance of a helper class for query processing
        self.queryProcessor = queryprocessing.QueryProcessor()
        # Indicates whether preprocessing is done
        self.run_preprocessing()

    def calculate_term_in_document_weight(self, _id, term) -> float:
        # Weight formula: w_ij = tf_ij  *  log_2( n / df_i )
        # Amount of all documents
        n = self.database.numberOfDocuments

        # Frequency of term occurrence in document with document_id == id
        f_ij = self.termFrequencyDicts[_id][term]

        # Amount of documents containing term t
        df_i = 0

        # Maximum frequency of term t_i across the entire collection
        max_f_ij = 0

        for document_dict in self.termFrequencyDicts:
            if term in document_dict.keys():
                df_i += 1
                if document_dict[term] > max_f_ij:
                    max_f_ij = document_dict[term]

        # Normalized term frequency of term t_i in document d_j
        tf_ij = float(f_ij) / max_f_ij

        # tf-idf weight
        weight = tf_ij * math.log((n / df_i), 2)
        if (n/df_i) == 1:
            weight += MY_EPSILON
        """
        # print("--- tf_ij ==", tf_ij)
        # print("--- n == ", n)
        # print("--- df_i == ", df_i)
        # print("--- math.log((n / df_i), 2) == {:.50f}".format(math.log((n / df_i), 2)))
        # print("final weight == ", weight)
        """

        return weight

    # Creates inverted list for a particular term in the vocabulary
    def create_inverted_list(self, term) -> list[(int, float)]:
        inv_list = []
        doc_id = 0
        for documentDict in self.termFrequencyDicts:
            if term in documentDict.keys():
                weight = self.calculate_term_in_document_weight(doc_id, term)
                if weight == 0.0 and self.isMatrixUsed is False:
                    doc_id += 1
                    continue
                inv_list.append((doc_id, weight))
            elif self.isMatrixUsed is True:
                inv_list.append((doc_id, 0.0))
            doc_id += 1
        return inv_list

    def add_complement_of_term(self, not_term):
        """
        not_term examples: ~trump, ~blue, ~sky, ...

        In case that there is a, say, word "trump" in the document,
        it's expected that a user query may have "~trump" (NOT trump) as a part of it
        quite frequently.
        Hence it's a good idea to construct a complement of the term, consisting
        of ids of the documents in which the term is not present.
        """
        t = not_term
        if self.isMatrixUsed is False:
            if t not in self.invertedListsDict.keys():
                doc_id = 0
                pos = 0
                self.invertedListsDict[t] = list()
                while pos < len(self.invertedListsDict[t[1:]]):
                    while doc_id < self.invertedListsDict[t[1:]][pos][0]:
                        self.invertedListsDict[t].append((doc_id, None))
                        doc_id += 1
                    doc_id += 1
                    pos += 1
                while doc_id < self.database.numberOfDocuments:
                    self.invertedListsDict[t].append((doc_id, None))
                    doc_id += 1
        else:
            if t not in self.invertedListsDict.keys():
                doc_id = 0
                self.invertedListsDict[t] = list()
                for pair in self.invertedListsDict[t[1:]]:
                    weight = pair[1]
                    if weight == 0.0:
                        self.invertedListsDict[t].append((doc_id, None))
                    doc_id += 1

    def get_term_in_document_relevancy(self, term, doc_id) -> float:
        relevancy = 0.0
        if term[0] == '~':
            """
            Calculated using formula:
            relevancy(~t, d_j) = 1 - weight(t, d_j)  
            """
            relevancy = 1.0
            if term[1:] in self.invertedListsDict.keys():
                for _pair in self.invertedListsDict[term[1:]]:
                    if _pair[0] == doc_id:
                        relevancy -= _pair[1]
                        break
        else:
            if term in self.invertedListsDict.keys():
                for _pair in self.invertedListsDict[term]:
                    if _pair[0] == doc_id:
                        relevancy = _pair[1]
                        break
        return relevancy

    def calculate_and_operation_relevancy(self, and_terms, doc_id) -> float:
        """
        Using AND relevance formula for document d_j as follows:
        q1 AND q2 AND ... AND qt  ---> 1 - sqrt( [(1 - w__1,j)^2 + ... + (1 - w__t,j)^2] / t )
        """
        numerator = 0
        denominator = 0

        for t in and_terms:
            denominator += 1
            numerator += pow((1 - self.get_term_in_document_relevancy(t, doc_id)), 2)
        relevancy = 1 - math.sqrt(numerator / denominator)
        return relevancy

    def sort_ids_by_relevance(self, terms_list, not_terms_list,
                              and_terms_list, set_of_ids) -> list[(int, float)]:
        """
        Using OR relevance formula for document d_j as follows:
        q1 OR q2 OR ... OR qt  ---> sqrt( [(w__1,j)^2 + ... + (w__t,j)^2] / t )
        """

        id_relevance_dict = dict()
        for doc_id in set_of_ids:
            id_relevance_dict[doc_id] = 0.0

        for t in terms_list:
            for doc_id in set_of_ids:
                id_relevance_dict[doc_id] += pow(self.get_term_in_document_relevancy(t, doc_id), 2)

        for nt in not_terms_list:
            for doc_id in set_of_ids:
                id_relevance_dict[doc_id] += pow(self.get_term_in_document_relevancy(nt, doc_id), 2)

        for and_terms in and_terms_list:
            for doc_id in set_of_ids:
                id_relevance_dict[doc_id] += pow(self.calculate_and_operation_relevancy(and_terms, doc_id), 2)

        for doc_id in set_of_ids:
            id_relevance_dict[doc_id] /= len(set_of_ids)
            id_relevance_dict[doc_id] = math.sqrt(id_relevance_dict[doc_id])

        """
        Turning id_numerator_dict which has document d_j id as its key
        and relevance of a document d_j as its value
        into a list of pairs (document_id, relevance) sorted by the relevance
        """
        id_relevance_list = sorted(id_relevance_dict.items(), key=lambda x: x[1], reverse=True)
        # # print("--- id_relevance_list:")
        # # print(id_relevance_list)

        return id_relevance_list

    def and_query_processing(self, and_terms) -> list[int]:
        # print("and_terms:\n", and_terms)
        terms = []
        for t in and_terms:
            if t not in self.invertedListsDict.keys():
                if t[0] == '~':
                    if t[1:] in self.invertedListsDict.keys():
                        self.add_complement_of_term(t)
                        terms.append(t)
                else:
                    """
                    AND-ing something out of range with the rest 
                    of the query gives us an empty document set.
                    """
                    return []
            else:
                terms.append(t)
        # print("and terms to process:\n", terms)
        """
        e.g.: ~trump AND ~biden, while both trump and biden are 
               not present in any document.
               Hence all the ids are returned. 
        """
        if len(terms) == 0:
            return list(range(self.database.numberOfDocuments))

        found_ids = []
        pos_dict = dict()
        for t in terms:
            pos_dict[t] = 0

        while True:
            flag_match = True
            flag_end = False

            for t in terms:
                """
                Breaking in case that any of the inverted index lists
                reached its end 
                """
                if pos_dict[t] == len(self.invertedListsDict[t]):
                    flag_end = True
                    break
            if flag_end is True:
                break
            prev_id = self.invertedListsDict[terms[0]][pos_dict[terms[0]]][0]
            for t in terms:
                # # print(prev_id)
                if self.invertedListsDict[t][pos_dict[t]][0] != prev_id or \
                        self.invertedListsDict[t][pos_dict[t]][1] == 0.0:
                    flag_match = False
                    break
                prev_id = self.invertedListsDict[t][pos_dict[t]][0]
            if flag_match is True:
                # # print("Match!", prev_id)
                found_ids.append(prev_id)
                for t in terms:
                    pos_dict[t] += 1
            else:
                min_t = terms[0]
                for t in terms:
                    if self.invertedListsDict[t][pos_dict[t]][0] \
                            < \
                            self.invertedListsDict[min_t][pos_dict[min_t]][0]:
                        min_t = t
                pos_dict[min_t] += 1
                # # print(min_t, pos_dict[min_t])
        # print("and returning found_ids: ", found_ids)
        return found_ids

    def inverted_list_query(self, terms_list, not_terms_list, and_terms_list) -> list[int]:
        set_of_ids = set()

        for and_query_terms in and_terms_list:
            list_of_ids = self.and_query_processing(and_query_terms)
            for doc_id in list_of_ids:
                set_of_ids.add(doc_id)

        for t in terms_list:
            if t in self.invertedListsDict.keys():
                for pair in self.invertedListsDict[t]:
                    doc_id = pair[0]
                    set_of_ids.add(doc_id)

        for nt in not_terms_list:
            if nt[1:] in self.invertedListsDict.keys():
                self.add_complement_of_term(nt)
                for pair in self.invertedListsDict[nt]:
                    doc_id = pair[0]
                    set_of_ids.add(doc_id)
            else:
                """
                If, say, term "red" is not present in any document,
                then "~red" (NOT red) should return all the document ids,
                as red is not present in all of them.
                """
                return list(range(self.database.numberOfDocuments))

        return list(set_of_ids)

    def matrix_query(self, terms_list, not_terms_list, and_terms_list) -> list[int]:
        set_of_ids = set()

        for and_query_terms in and_terms_list:
            list_of_ids = self.and_query_processing(and_query_terms)
            for doc_id in list_of_ids:
                set_of_ids.add(doc_id)

        for t in terms_list:
            if t in self.invertedListsDict.keys():
                for pair in self.invertedListsDict[t]:
                    weight = pair[1]
                    if weight == 0.0:
                        continue
                    doc_id = pair[0]
                    set_of_ids.add(doc_id)

        for nt in not_terms_list:
            if nt[1:] in self.invertedListsDict.keys():
                self.add_complement_of_term(nt)
                for pair in self.invertedListsDict[nt]:
                    doc_id = pair[0]
                    set_of_ids.add(doc_id)
            else:
                """
                If, say, term "red" is not present in any document,
                then "~red" (NOT red) should return all the document ids,
                as red is not present in all of them.
                """
                return list(range(self.database.numberOfDocuments))

        return list(set_of_ids)

    def process_query(self, query) -> list[list[int, float]]:
        try:
            # [],  [],  [ [], [], ... ]
            terms_list, not_terms_list, and_queries_terms_list = self.queryProcessor.parse_query(query)
        except ValueError as e:
            # TODO: display this message via frontend
            raise e
        """
        # print('\n')
        # print("Terms:\n", terms_list)
        # print("~Terms:\n", not_terms_list)
        # print("And terms:\n", and_queries_terms_list)
        # print('\n')
        """

        if is_a_tautology(terms_list, not_terms_list, and_queries_terms_list):
            id_relevance_list = []
            for doc_id in range(self.database.numberOfDocuments):
                id_relevance_list.append(list((doc_id, 1.0)))
            #return list(range(self.database.numberOfDocuments))
            return id_relevance_list
        elif is_a_contradiction(terms_list, not_terms_list, and_queries_terms_list):
            return []

        ids_set = []
        if self.isMatrixUsed is True:
            ids_set = self.matrix_query(terms_list, not_terms_list, and_queries_terms_list)
        else:
            ids_set = self.inverted_list_query(terms_list, not_terms_list, and_queries_terms_list)

        id_relevance_list = self.sort_ids_by_relevance(terms_list, not_terms_list,
                                                       and_queries_terms_list, ids_set)
        # ids_sorted_by_relevance = []
        # for pair in id_relevance_list:
        #    doc_id = pair[0]
        #    ids_sorted_by_relevance.append(doc_id)

        # return ids_sorted_by_relevance

        if len(id_relevance_list) > 0:
            num_docs = len(id_relevance_list)
            max_rel = id_relevance_list[0][1]
            min_rel = id_relevance_list[num_docs-1][1]

            if max_rel == min_rel:
                for i in range(num_docs):
                    id_relevance_list[i] = list(id_relevance_list[i])
                    id_relevance_list[i][1] = 1.0
            else:
                for i in range(num_docs):
                    id_relevance_list[i] = list(id_relevance_list[i])
                    normalized_rel = (id_relevance_list[i][1] - min_rel) / (max_rel - min_rel)
                    id_relevance_list[i][1] = round(normalized_rel, 2)

        return id_relevance_list

    def get_query_results(self, query):
        id_relevance_list = []
        start = time.time()
        try:
            id_relevance_list = self.process_query(query)
        except ValueError as e:
            # TODO: display this message on frontend
            # print(e)
            return None, []
        end = time.time()
        doc_name_relevance_list = []
        # print("--- id_relevance_list type is:", type(id_relevance_list))
        # print("--- id_relevance_list content is:", id_relevance_list)

        for pair in id_relevance_list:
            pair = list(pair)
            doc_id = pair[0]
            doc_relevance = pair[1]
            doc_name = self.database.documents_file_names[doc_id]
            if len(doc_name) >= 4:
                if doc_name[-4:] == '.txt':
                    doc_name_relevance_list.append((doc_name[:-4], doc_relevance))
                    continue
            doc_name_relevance_list.append((doc_name, doc_relevance))
        # print(query)
        # print(doc_name_relevance_list)

        """
        ids_sorted_by_relevance = []
        try:
            ids_sorted_by_relevance = self.process_query(query)
        except ValueError as e:
            # TODO: display this message on frontend
            # print(e)
        # # print("\n---------------------------------------")
        # # print("IDs sorted by relevance:", ids_sorted_by_relevance)
        # # print('\n\n')
        doc_names_sorted_by_relevance = []
        for doc_id in ids_sorted_by_relevance:
            doc_name = self.database.documents_file_names[doc_id]
            if len(doc_name) >= 4:
                if doc_name[-4:] == '.txt':
                    doc_names_sorted_by_relevance.append(doc_name[:-4])
                    continue
            doc_names_sorted_by_relevance.append(doc_name)
        # print(query)
        # print(doc_names_sorted_by_relevance)
        """
        time_spent = round(end - start, 8)
        return time_spent, doc_name_relevance_list

    def run_preprocessing(self):
        # Read documents into the database
        self.database.read_data()
        # Clean database documents
        self.database.documents = self.textProcessor.clean_documents(self.database.documents)

        # Construct a global frequency dictionary and
        # one frequency dictionary for each document
        for document in self.database.documents:
            word_list = nltk.word_tokenize(document)
            tmp_dict, self.globalFrequencyDict = self \
                .textProcessor \
                .adjust_term_frequency_dicts(word_list, self.globalFrequencyDict)
            self.termFrequencyDicts.append(tmp_dict)

        # Construct an inverted list for each term
        for term in self.globalFrequencyDict.keys():
            tmp_inverted_list = self.create_inverted_list(term)
            # print(term + ":", tmp_inverted_list)
            if len(tmp_inverted_list) > 0:
                self.invertedListsDict[term] = tmp_inverted_list


def main():
    is_matrix_used = False
    extended_boolean_model = ExtendedBooleanModel(is_matrix_used)
    extended_boolean_model.get_query_results("biden")
    extended_boolean_model.get_query_results("bidene")
    extended_boolean_model.get_query_results("trump OR NOT trump")


if __name__ == '__main__':
    main()
