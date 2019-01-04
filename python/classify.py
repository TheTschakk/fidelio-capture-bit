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
            params = []
            r = 1
            while ':' not in lines[i+r]:
                params.append(lines[i+r])
                r += 1
                if i+r == len(lines):
                    break
            for param in params:
                values = param.split(' ')
                for j in range(len(values)):
                    values[j] = float(values[j])
                values = np.array([values])
                group = SVC.clf.predict(values)
                if group == 0:
                    if num == len(params)-1: #if all params are not meteors
                        os.system('mv ' + path + line + ' ' + path + 'other/')
                else:
                    os.system('mv ' + path + line + ' ' + path + 'meteors/')
                    break
