import json

import pandas as pd
from glom import flatten

with open("HighSpeed/120s/run1/14-03-2022 21-26-39_HS_FlowMon.json", "r") as fh: #input json file name
        data = json.load(fh)["FlowMonitor"] #load top dict

#list objects 
data2 = data["FlowStats"]["Flow"] 
flatten(data2)

# transform into pandas dataframe
dd = pd.DataFrame(data2)

# remove extraneous columns
dd.drop(columns=['@timeFirstTxPacket', '@timeFirstRxPacket', '@timeLastTxPacket', '@timeLastRxPacket', '@delaySum', '@lastDelay', 'delayHistogram', 'packetSizeHistogram', 'flowInterruptionsHistogram', '@timesForwarded', 'jitterHistogram'], axis=1, inplace=True)

# convert to csv file 
dd.to_csv('HighSpeed/120s/run1/14-03-2022 21-26-39_HS_FlowMon.csv')

