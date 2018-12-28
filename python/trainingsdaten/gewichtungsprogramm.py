import numpy as np

filename = 'plane.dat'
X = np.genfromtxt(filename, delimiter=' ')

xmax=np.amax(X,axis=0)
xmin=np.amin(X,axis=0)
for j in range(len(X[0])):
    for b in range(len(X)):
        X[b][j] = (X[b][j]-xmin[j])/(xmax[j]-xmin[j])

with open(filename[:-4] + '2' + filename[-4:], 'w') as file:
    file.write(str(X))
