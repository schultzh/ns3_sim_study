#!/usr/bin/env python

import csv
import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("NewReno-throughput.csv")
data.columns = [".1", "throughput", "units"]
rounded_average = (round(data["throughput"].mean()))

print(data.to_string())
print(rounded_average)

x_cord = [0,12]
y_cord = [rounded_average, rounded_average]

data.plot(kind='scatter', x='.1', y=['throughput'])
plt.axhline(rounded_average, color='g', linestyle='--')
plt.xlabel('Seconds')
plt.ylabel('Throughput (Mbit/s)')
plt.show()
