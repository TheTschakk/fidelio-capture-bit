#!/usr/bin/python3

import SVC
from datetime import date, timedelta
import numpy as np
import os
import sys

path = sys.argv[1]
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
<<<<<<< HEAD
                if i+r == len(lines)-1:
=======
                if i+r == len(lines):
>>>>>>> 5c50c79973119fbae34e4e5c43d1fbd8c13a5348
                    break
            for param in params:
                values = param.split(' ')
                for j in range(len(values)):
                    values[j] = float(values[j])
<<<<<<< HEAD
                    if values[j] == float('NaN') or values[j] == float('Inf'):
                        break
                else:
                    values = np.array([values])
                    group = SVC.clf.predict(values)
                    if group == 0:
                        os.system('mv ' + path + line[:-1] + ' ' + path + 'others/')
                    else:
                        os.system('mv ' + path + line[:-1] + ' ' + path + 'meteors/')
                        break
=======
                values = np.array([values])
                group = SVC.clf.predict(values)
                if group == 0:
                    if num == len(params)-1: #if all params are not meteors
                        os.system('mv ' + path + line + ' ' + path + 'other/')
                else:
                    os.system('mv ' + path + line + ' ' + path + 'meteors/')
                    break
>>>>>>> 5c50c79973119fbae34e4e5c43d1fbd8c13a5348
