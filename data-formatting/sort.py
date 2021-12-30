import json
import yaml

from flows import Flow

def main():
    
    flows = {}

    with open("meshFlow.json", "r") as fh:
        data = json.load(fh)["FlowMonitor"]
    
    data = data["Ipv4FlowClassifier"]["Flow"], data["FlowStats"]["Flow"]

    for flow in data: # <-- P2
        if flow["@flowId"] not in flows.keys():
            flows[flow["@flowId"]] = {
                "packets": [],
                "addresses": []
            }
            
        f = Flow(flow)
        
        try:
            getattr(f,"sourceaddress")
            flows[flow["@flowId"]]["addresses"].append(dict(f))
        except AttributeError:
            flows[flow["@flowId"]]["packets"].append(dict(f))
    
    with open("flows.yaml","w") as fh:
        yaml.dump(flows,fh,indent=2)
    
if __name__=="__main__":
    main()