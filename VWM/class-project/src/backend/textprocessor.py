import re
import nltk
from nltk.corpus import wordnet
from nltk.stem import WordNetLemmatizer
from nltk.stem import PorterStemmer
from nltk.corpus import stopwords

# Define language nuances we want to get rid of
cleanStepTable = [
    ["won't", "will not"],
    ["can't", "cannot"],
    ["n't", " not"],
    ["'re", " are"],
    ["'s", " is"],
    ["'d", " would"],
    ["'ll", " will"],
    ["'t", " not"],
    ["'ve", " have"],
    ["'m", " am"]
]

# Get all the stop words for english
stopWords = set(stopwords.words('english'))


def is_a_stop_word(word):
    return word in stopWords


# Clean the document of such nuances + other bits and pieces here and there
def clean_document(document):
    for pair in cleanStepTable:
        document = document.replace(pair[0], pair[1]).lower()
    document = re.sub('[0-9]+', '', document)
    document = re.sub('[^\w\s]', ' ', document)
    document = re.sub(' +', ' ', document)
    return document


# Helper function for the lemmatize method of TextProcessor
def get_wordnet_pos(word):
    # Map POS tag to first character lemmatize() accepts
    tag = nltk.pos_tag([word])[0][1][0].upper()
    tag_dict = {"J": wordnet.ADJ,
                "N": wordnet.NOUN,
                "V": wordnet.VERB,
                "R": wordnet.ADV}
    return tag_dict.get(tag, wordnet.NOUN)


class TextProcessor:
    def __init__(self):
        # print("Constructing TextProcessor...")
        # Define lemmatizer
        self.lemmatizer = WordNetLemmatizer()
        # Define stemmer
        self.stemmer = PorterStemmer()

    @staticmethod
    def clean_documents(documents):
        for i in range(len(documents)):
            documents[i] = clean_document(documents[i])
        return documents

    def lemmatize(self, word):
        word = self.lemmatizer.lemmatize(word, get_wordnet_pos(word))
        return word

    def adjust_term_frequency_dicts(self, word_list, dict_global):
        dict_tmp = {}
        # Iterate over all tokens
        for word in word_list:
            if is_a_stop_word(word):
                continue
            # Lemmatize the word
            word = self.lemmatize(word)
            # If it already is in dictionary, increase the count
            # If not, add it to the dictionary and set the count to 1
            if word in dict_tmp.keys():
                dict_tmp[word] += 1
            else:
                dict_tmp[word] = 1

            # Same for global dictionary
            if word in dict_global.keys():
                dict_global[word] += 1
            else:
                dict_global[word] = 1

        return dict_tmp, dict_global
