#!/bin/python

serial = 356.00
data = [ 384.40, 227.50, 127.00, 182.50, 106.50, 59.58, 98.10, 48.40, 24.50, 51.00, 7.10, 16.10, 42.00, 27.40, 9.32, 29.00, 12.38, 6.10]

i = 0
for x in data:
	print(str(serial / x) + ',', end='')
	if i % 3 == 2:
		print()
	i += 1