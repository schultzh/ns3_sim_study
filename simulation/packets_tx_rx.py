#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

# reading the file
data = pd.read_csv("YeAH/30s/run1/15-03-2022 18-36-47_Yeah_FlowMon.csv")

data = data[['@flowId', '@txPackets', '@rxPackets']]

# calulate ratio of sent packets to delivered packets
sentSum = data['@txPackets'].sum()
deliveredSum = data['@rxPackets'].sum()
ratio = 100 * (deliveredSum/sentSum)
#print(round(ratio))

data.plot(kind='bar', width=0.9, x='@flowId', y=['@txPackets', '@rxPackets'], title="New Reno, HighSpeed, and YeAH")

plt.xlabel('FlowId')
plt.ylabel('Packets')
plt.legend()
plt.show()