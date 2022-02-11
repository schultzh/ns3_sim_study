import re

class Flow:
    
    accepts = [
        "@destinationAddress",
        "@destinationPort",
        "@protocol",
        "@sourceAddress",
        "@sourcePort",
        "@lostPackets",
        "@rxPackets",
        "@txPackets"
    ]
    
    def __init__(self, data):
        for field in data:
            if not field == "@flowId" and field in self.accepts:
                prop = re.sub(r"[@:]","",field).lower()
                setattr(self,prop, data[field])
                
    def __iter__(self):
        for prop, val in vars(self).items():
            if val:
                yield prop, getattr(self,prop)