#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("NewReno/run3/NR-throughput.csv")
# label columns
data.columns = [".1", "throughput", "units"]
# calc mean
rounded_mean = (round(data["throughput"].mean()))
# calc median
rounded_med = (round(data["throughput"].median()))

# create plot
data.plot(kind='scatter', x='.1', y=['throughput'])
plt.axhline(rounded_mean, color='r', label='Mean', linestyle='--')
plt.axhline(rounded_med, color='g', label='Median', linestyle='--')
plt.xlabel('Seconds')
plt.ylabel('Throughput (Mbit/s)')
plt.legend()
plt.show()