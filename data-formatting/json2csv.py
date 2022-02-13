import json

import pandas as pd
from glom import flatten

with open("WAP-NewReno_FlowMon.json", "r") as fh: #input json file name
        data = json.load(fh)["FlowMonitor"] #load top dict

#list objects 
data1 = data["Ipv4FlowClassifier"]["Flow"]
data2 = data["FlowStats"]["Flow"] 
flatten(data1)
flatten(data2)
data = data1 + data2

# transform into pandas dataframe
df = pd.DataFrame(data)

# remove extraneous columns
df.drop(columns=['Dscp', '@timeFirstTxPacket', '@timeFirstRxPacket', '@timeLastTxPacket', '@timeLastRxPacket', '@delaySum', '@jitterSum', '@lastDelay', 'delayHistogram', 'jitterHistogram', 'packetSizeHistogram', 'flowInterruptionsHistogram'], axis=1, inplace=True)

# group by flowId
df.groupby("@flowId").groups
print(df.groupby("@flowId").groups) # prints correctly, does not merge in CSV

# convert to csv file 
df.to_csv('data.csv')
