import matplotlib.pyplot as plt

X = [100, 1000, 2000]
Y05 = [1.58 / 2, 1.89 / 2, 1.89 / 2]
Y10 = [2.91 / 4, 3.95 / 4, 3.88 / 4]
Y15 = [3.74 / 8, 6.49 / 8, 6.63 / 8]
plt.plot(X, Y05, label='Efficieny with 2 threads', color='black')
plt.plot(X, Y10, label='Efficiency with 4 threads', color='blue')
plt.plot(X, Y15, label='Efficiency with 8 threads', color='pink')
plt.legend()
plt.show()