import matplotlib.pyplot as plt

X = [1, 2, 4, 8]
Y100 = [14300, 11705, 5816, 5470]
Y1000 = [24568124, 11890829, 8619521, 8200821]
Y2000 = [275040044, 166470332, 111113505, 109881098]
plt.plot(X, Y100, label='100 x 100 with 100 x 100', color='blue')
# plt.plot(X, Y1000, label='1000 x 1000 with 1000 x 1000', color='red')
# plt.plot(X, Y2000, label='2000 x 2000 with 2000 x 2000', color='pink')

plt.legend()
plt.show()