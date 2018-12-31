#!/usr/bin/python3

import SVC
from datetime import date, timedelta
import numpy as np
import os

today = date.today()

day = today - timedelta(days=1)
day = day.isoformat()
path = '/home/pi/videos/' + day + '/'
with open(path + 'log.txt') as f:
    f = f.read()
    lines = f.split('\n')
    for i, line in enumerate(lines):
        if ':' in line:
            params = lines[i+1]
            params = np.genfromtxt(params, delimiter=' ')
            group = clf.predict(params)
            if group == 0:
                os.system('mv ' + path + line + ' ' + path + 'planes/')
            else:
                os.system('mv ' + path + line + ' ' + path + 'meteors/')
