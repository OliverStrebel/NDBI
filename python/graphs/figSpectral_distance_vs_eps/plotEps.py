import numpy
import matplotlib.pyplot as plt

dist_vs_eps = numpy.loadtxt(open("data_distance_vs_eps.txt"), delimiter=' ')

plt.plot(dist_vs_eps[0:200,0], dist_vs_eps[0:200,1],  color = 'black')
plt.xlabel("$\epsilon$")
plt.ylabel("euclidean norm")

plt.savefig('fig_distance_vs_eps.jpeg')
plt.show()



print("End program")
