#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("YeAH/120s/run1/15-03-2022 13-37-02_Yeah_FlowMon.csv")

data = data[['@flowId', '@jitterSum']]

data.plot(kind="scatter", x='@flowId', y='@jitterSum')

plt.xlabel('FlowId')
plt.ylabel('JitterSum (ns)')
plt.show()