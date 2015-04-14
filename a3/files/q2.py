import sys
import random
from pprint import pprint,pformat

class Classification:
    def __init__(self,classifier_name,words):
        self.classifier_name = classifier_name
        self.prob_classifier = 0
        self.prob_keyword = {}
        for w in words:
            self.prob_keyword[w] = 0

    def __str__(self):
        return "'{}':{}\n{}\n".format(self.classifier_name,self.prob_classifier,
                                pformat(self.prob_keyword))
    def __repr__(self):
        return self.__str__()


class Bayes:
    def __init__(self):
        self.classifiers = {"classification": ['keywords']}
        self.probs = { 'classification': {'keyword':0.4}}
        self.words = set()

    """
    @param d - dictionary containing the classifications and keywords
        for this naive bayes model.
        Assumption is everything is lower case.
    """
    def setParameters(self, d):
        self.classifiers = d

        # create a set of the keywords
        self.words = set()
        for c in d:
            self.words = self.words.union(d[c])

        # create Classification objects
        self.probs = {}
        for c in self.classifiers:
            self.probs[c] = Classification(c,self.words)

    """
    @param training_data (string) - filename of training data.
        the format is the following
        <classification> <word>,...,<word>
    """
    def train(self, training_data):
        f = open(training_data,"r")

        # temp data structure to hold the number of occurences of each
        # classification as well the count on the number of words
        data = {}
        for x in self.classifiers:
            data[x] = {
                "count":0,
                "words":{}
            }
        num_entries = 0

        # read through the file and fill in the counts
        for line in f:
            line = line.rstrip('\n')
            words = line.split()

            # error checking
            if(len(words) <= 0):
                continue

            class_type = words[0].lower()
            words = words[1:]

            num_entries += 1

            # increment a count on a class_type
            data[class_type]["count"] += 1;

            # go through all the words for this line and keep a count on them
            for w in words:
                w = w.lower()
                if w in data[class_type]["words"]:
                    data[class_type]["words"][w] += 1
                else:
                    data[class_type]["words"][w] = 1
        f.close()

        # parse through data and calculate the probabilties
        alpha = 1
        for k,c in self.probs.items():
            c.prob_classifier = (float(data[k]['count']) + alpha)/(num_entries + alpha*len(self.words))
            for w in c.prob_keyword:
                word_count = 0 if not w in data[k]['words'] else (data[k]['words'][w])
                c.prob_keyword[w] = float(word_count + alpha)/(data[k]['count'] + alpha*len(self.words))

    """
        @param doc(string)- the filename of the document to classify
        @return (string) - string representing the classifcation of the document.
    """
    def predict(self, doc):
        # read in the file and determine the words that are present
        f = open(doc,"r")
        words = set()
        for line in f:
            line = line.rstrip('\n')
            words = words.union(line.split())
        f.close()

        # For all the words present in the document
        # take the product of all the probabilties for each classifcation
        # and take the classification with the highest probability
        max_c = None
        max_prob = 0
        for k,c in self.probs.items():
            prob = c.prob_classifier
            for w in c.prob_keyword:
                if( w in words):
                    # word is present, multiply against the probability
                    prob *= c.prob_keyword[w]

            # if the final probablity is larger than everything, then record it
            if(max_c == None or prob > max_prob):
                max_c = k

        return max_c

    def generate(self,class_type,doc_len=10):
        # extract the array of prob,keyword pairs and sort
        # by from lowest to greatest prob
        vals = self.probs[class_type].prob_keyword.items()
        vals = map(lambda x : x[::-1], vals)
        vals.sort()

        #normalize the probabilities nto the range 0,1
        prob_sum = reduce(lambda x,y : (x[0] + y[0],0),vals)[0]
        vals = map(lambda x : (x[0]/prob_sum,x[1]), vals)

        # construct the string from the keywords based on the prob distribution
        rs = ""
        for i in xrange(doc_len):

            # randomly choose a keyword
            r = random.random()
            for v in vals:
                if r <= v[0]:
                    rs += v[1] + " "
                    break
                else:
                    r -= v[0]
        return rs

b = Bayes();
b.setParameters({
    "school" : ["connex","uvic","assignment","student"],
    "personal" : ["steam","programming","lily","peter"],
    "prof" : ["linkedin","coop","interview","university"]
});

train_file = "q2_training_data.txt"
predict_file = "q2_training_data.txt"
b.train(train_file);
print("predict '{}' => {}\n".format(train_file,b.predict(predict_file)))

def gen(num,type):
    print(type)
    for x in xrange(num):
        print(b.generate(type,10))
    print("")

gen(5,"school")
gen(5,"personal")
gen(5,"prof")
