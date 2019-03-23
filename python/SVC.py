#!/usr/bin/python3

import numpy as np
import sys
import os

#read data and arrange trainingdata
with open(os.environ["METHOME"] + '/python/trainingsdaten/meteor2.dat') as file1:
    meteore = file1.read()
    with open(os.environ["METHOME"] + '/python/trainingsdaten/plane2.dat') as file2:
        planes = file2.read()
        data = meteore + planes
        lines = data.split('\n')
        points = []
        for point in lines:
            a = point.split(' ')
            for i in range(len(a)):
                a[i] = float(a[i])
            points.append(a)
        X = np.asarray(points)
        Y = [1 for i in range(len(meteore.split('\n')) - 1)] # -1 because there is a '\n' at the end of meteor2.dat
        for j in range(len(planes.split('\n'))):
            Y.append(0)
        Y = np.asarray(Y)

#classify with libary
from sklearn import svm
clf = svm.SVC(C=10000, gamma='scale')
clf.fit(X,Y)


#with open(coefs.txt, 'w') as file:
#    file.write(clf.coef_) #?

#finding error
Y_predict = clf.predict(X)
error = 0
for i in range(len(Y)):
    error += np.abs(Y[i]-Y_predict[i])


