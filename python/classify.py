import SVC
from datetime import date
import numpy as np
import os

today = date.today()
today = today.isoformat()

path = '/home/pi/videos/' + today + '/'

with open(path + 'log.txt') as f:
    f = f.read()
    lines = f.split('\n')
    for i, line in enumerate(lines):
        if i%2 == 0: #if index is not odd (eg. first line, thrid line ...)
            params = lines[i+1]
            params = np.genfromtxt(params)
            group = clf.predict(params)
            if group == 0:
                os.system('mv ' + path + line + ' ' + path + 'planes/')
            else:
                os.system('mv ' + path + line + ' ' + path + 'meteors/')

