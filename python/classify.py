#!/usr/bin/python3

import SVC
from datetime import date, timedelta
import numpy as np
import os

today = date.today()

for round in [0,1]:
    day = today - timedelta(days=round)
    day = day.isoformat()
    path = '/home/pi/videos/' + day + '/'
    with open(path + 'log.txt') as f:
        f = f.read()
        lines = f.split('\n')
        for i, line in enumerate(lines):
            if i%2 == 0: #if index is not odd (eg. first line, thrid line ...)
                params = lines[i+1]
                params = np.genfromtxt(params, delimeter=' ')
                group = clf.predict(params)
                if group == 0:
                    os.system('mv ' + path + line + ' ' + path + 'planes/')
                else:
                    os.system('mv ' + path + line + ' ' + path + 'meteors/')

