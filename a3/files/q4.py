import sys
import math
from pprint import pprint


f = open('q4_data.txt',"r")


attrs = {
    'type':["lecture","lab","tutorial"],
    'midterm':["t","f"],
    'toc':["early","noon","late"],
    'sleep':["<=6","7", ">=8"],
    'enjoyable':["t","f"],
}

data = []
neg = 0
pos = 0
for line in f:
    line.rstrip('\n')
    words = line.split()
    words = map(lambda x: x.lower(), words)
    a = {
        'type':words[0],
        'midterm':words[1],
        'toc':words[2],
        'sleep':words[3],
        'enjoyable':words[4],
        'goal':words[5]
    };
    if( a['goal'] == 't'):
        pos += 1
    else:
        neg += 1
    data.append(a)


def parse_for_pk_nk(data):
    rs = {}
    for i in attrs:
        rs[i] = {}
        for j in attrs[i]:
            rs[i][j] = {
                'pk': 0 ,
                'nk' : 0
            }

    for row in data:
        for attr in attrs:
            val = row[attr]
            if row['goal'] == 't':
                rs[attr][val]['pk'] += 1
            else:
                rs[attr][val]['nk'] += 1

    return rs

data_pk_nk = parse_for_pk_nk(data)

def B(q):
    if(q == 0):
        return 0
    else:
        return -1*(q*math.log(q,2) + (1-q)*math.log(1-q,2))

def Remainder(attr):
    rs = 0
    for k in attrs[attr]:
        pk = data_pk_nk[attr][k]['pk']
        nk = data_pk_nk[attr][k]['nk']
        rs += (float((pk + nk))/( pos + neg ))*B( float(pk)/(pk + nk))
    return rs

def Gain(attr):
    return B(float(pos)/ ( pos + neg)) - Remainder(attr)


for attr_name in data_pk_nk:
    print(attr_name)
    for attr_type in data_pk_nk[attr_name]:
        print("," + attr_type + "," + str(data_pk_nk[attr_name][attr_type]['pk']) + "," + str(data_pk_nk[attr_name][attr_type]['nk']))

for attr_name in attrs:
    print(str(attr_name) +  "," + str(Gain(attr_name)))