import json
import yaml

from flows import Flow

def main():
    
    flows = {}

    with open("meshFlow.json", "r") as fh: #input json file name
        data = json.load(fh)["FlowMonitor"] #load top dict
        
    #combine subdicts
    data = data["Ipv4FlowClassifier"]["Flow"], data["FlowStats"]["Flow"]

    #create nested dicts
    for flow in data: 
        if flow["@flowId"] not in flows.keys():
            flows[flow["@flowId"]] = {
                "packets": [],
                "addresses": []
            }
            
        f = Flow(flow)
        
        #append each dict
        try:
            getattr(f,"sourceaddress")
            flows[flow["@flowId"]]["addresses"].append(dict(f))
        except AttributeError:
            flows[flow["@flowId"]]["packets"].append(dict(f))
    
    with open("flows.yaml","w") as fh: # output file
        yaml.dump(flows,fh,indent=2)
    
if __name__=="__main__":
    main()
