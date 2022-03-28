import json
import requests
from os import getcwd
import os

f = open('dramsim3_forks.json', 'r')

data = json.loads(f.read())

curdir = getcwd()
urlbase1 = 'https://raw.github.com/'
urlbase2 = '/master/src/controller.cc'

for i in data:
    print(i['full_name'])
    ghpath =  urlbase1 + i['full_name'] + urlbase2 #.replace('/', '%2F')
    r = requests.get(ghpath)
    outputfilename  = curdir + '/' + i['owner']['login'] + '.cc'
    with open(outputfilename, 'w') as f2:
        f2.write(r.text)

f.close()