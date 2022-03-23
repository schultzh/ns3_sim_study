#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("New Reno/120s/thx/NR-allruns.csv")

# remove timestamp data
data.drop(columns=['1.1'], axis=1, inplace=True)
data.columns = ["Run 1", "Run 2"]

std_dev_1 = (round(data["Run 1"].std()))
std_dev_2 = (round(data["Run 2"].std()))

std_devs = std_dev_1, std_dev_2
print(std_dev_1)

rounded_mean_1 = (round(data["Run 1"].mean()))
rounded_mean_2 = (round(data["Run 2"].mean()))

rounded_means = rounded_mean_1, rounded_mean_2
print(rounded_mean_1)

# create plot
data.boxplot(column=["Run 1", "Run 2"] )
plt.xlabel('Simulation Runs')
plt.ylabel('Throughput (Mbit/s)')
plt.show()