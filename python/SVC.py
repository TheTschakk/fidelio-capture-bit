# imports
import numpy as np
import sys

# some paths (individual for every device)
meteor2_path_davidh = 'C:/Users/Davanchama/Desktop/fidelio-capture-bit/python/trainingsdaten/meteor2.dat'
meteor3_path_davidh = 'C:/Users/Davanchama/Desktop/fidelio-capture-bit/python/trainingsdaten/meteor3.dat'
plane2_path_davidh = 'C:/Users/Davanchama/Desktop/fidelio-capture-bit/python/trainingsdaten/plane2.dat'
plane3_path_davidh = 'C:/Users/Davanchama/Desktop/fidelio-capture-bit/python/trainingsdaten/plane3.dat'

# read data
with open(meteor2_path_davidh) as meteor_file2:
    meteor_train = meteor_file2.read()
with open(meteor3_path_davidh) as meteor_file3:
    meteor_test = meteor_file3.read()
with open(plane2_path_davidh) as plane_file2:
    planes_train = plane_file2.read()
with open(plane3_path_davidh) as plane_file3:
    planes_test = plane_file3.read()


# arrange data
def arr_data(data):  # in: list of Strings, out: array of floats
    lines = data.split('\n')  # list with strings, each element is one line
    points = []
    for point in lines:
        a = point.split(' ')  # each number in a line is divided by a space, one 'a' for one line
        for i in range(len(a)):
            print(a)
            a[i] = float(a[i])
        points.append(a)
    return np.asarray(points)


train_data = meteor_train + planes_train
print(train_data)
test_data = meteor_test + planes_test
print(test_data)
train_data = arr_data(train_data)
test_data = arr_data(test_data)

truth_of_data = [1 for i in range(len(meteor_train.split(
    '\n')) - 1)]  # -1 because there is a '\n' at the end of meteor2.dat; Y[i] = 1 ==> X[i] is true/meteor
for j in range(len(planes_train.split('\n'))):
    truth_of_data.append(0)
truth_of_data = np.asarray(truth_of_data)

# classify with libary (machine learning used)
from sklearn import svm

clf = svm.SVC(C=10000, gamma=0.2)  # define the 'ai'
clf.fit(train_data, truth_of_data)  # train

# finding error
classification = clf.predict(train_data)
error = 0
for i in range(len(truth_of_data)):
    error += np.abs(truth_of_data[i] - classification[i])
print(error)