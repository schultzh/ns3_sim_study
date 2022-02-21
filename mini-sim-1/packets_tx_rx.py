#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("BBR/run5/WAP-BBR_FlowMon.csv")

data = data[['@flowId', '@txPackets', '@rxPackets']]

sentSum = data['@txPackets'].sum()
deliveredSum = data['@rxPackets'].sum()

ratio = 100 * (deliveredSum/sentSum)
#print(round(ratio))

data.plot(kind='bar', width=0.9, x='@flowId', y=['@txPackets', '@rxPackets'])

plt.xlabel('FlowId')
plt.ylabel('Packets')
plt.legend()
plt.show()