import matplotlib.pyplot as plt

X = [100, 1000, 2000]
Y05 = [1.58, 1.89, 1.89]
Y10 = [2.91, 3.95, 3.88]
Y15 = [3.74, 6.49, 6.63]
plt.plot(X, Y05, label='Speedup with 2 threads', color='black')
plt.plot(X, Y10, label='Speedup with 4 threads', color='blue')
plt.plot(X, Y15, label='Speedup with 8 threads', color='pink')
plt.legend()
plt.show()