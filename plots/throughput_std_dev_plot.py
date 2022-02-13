#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("NewReno-throughput.csv")
# label columns
data.columns = [".1", "throughput", "units"]
# calc mean
rounded_mean = (round(data["throughput"].mean()))
# calc standard dev
std_dev = (round(data["throughput"].std()))

# create plot
data.plot(kind='scatter', x='.1', y=['throughput'])
plt.axhline(rounded_mean, color='g', label='Mean', linestyle='--')
plt.axhline(std_dev, color='y', label='Standard Deviation', linestyle='--')
plt.xlabel('Seconds')
plt.ylabel('Throughput (Mbit/s)')
plt.legend()
plt.show()
