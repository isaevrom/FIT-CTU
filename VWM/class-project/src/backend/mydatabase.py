import os


class Database:
    def __init__(self, data_source):
        # print("Constructing Database...")
        # Array for storing documents
        self.documents = []
        # Array for storing names of files
        self.documents_file_names = []
        # Path to the data source
        # self.data_source = os.getcwd() + '/static/data'
        # self.data_source = os.getcwd() + '/static/generated_data/dir_2200'
        self.data_source = data_source
        self.numberOfDocuments = 0

    # read all the data in corresponding files
    def read_data(self):
        for filename in os.listdir(self.data_source):
            # print(filename)
            with open(os.path.join(self.data_source, filename), 'r', encoding="utf8", errors="ignore") as file:
                if ".txt" in filename:
                    doc = file.read().replace('\n', ' ')
                    self.documents.append(doc)
                    self.documents_file_names.append(filename)
        self.numberOfDocuments = len(self.documents)
