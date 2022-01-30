/* attempt chunk by chunk  11/15 */ 

// streams
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sstream>

// ns3 modules
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/mesh-module.h"
#include "ns3/mesh-helper.h"
#include "ns3/wifi-module.h"

// flow monitor modules
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"

// annimation module
#include "ns3/netanim-module.h"


NS_LOG_COMPONENT_DEFINE ("ChunkExample");

using namespace ns3;

// main function
int
main (int argc, char *argv[])
{

// define variables 
std::string dataRate = "100Mbps";


// mesh variables
int       m_xSize = (5); ///< X size
int       m_ySize = (5); ///< Y size
double    m_step = (100.0); ///< step
double    m_randomStart = (0.1); ///< random start
double    m_totalTime = (100.0); ///< total time
// double    m_packetInterval = (0.1); ///< packet interval
uint16_t  m_packetSize = (1024); ///< packet size
uint32_t  m_nIfaces = (1); ///< number interfaces
bool      m_chan = (true); ///< channel
std::string m_stack = ("ns3::Dot11sStack"); ///< stack
std::string m_root = ("ff:ff:ff:ff:ff:ff"); ///< root

// Create Mesh Nodes
NodeContainer genMesh;
genMesh.Create(m_ySize*m_xSize);
NetDeviceContainer genMeshDevice;

// Wifi Helpers
WifiMacHelper wifiMac;
WifiHelper wifiHelper;
wifiHelper.SetStandard (WIFI_STANDARD_80211b);

// Physical Layer
YansWifiPhyHelper wifiPhy;
YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
wifiPhy.SetChannel (wifiChannel.Create ());

// Configure Nodes with Wifi
genMeshDevice = wifiHelper.Install(wifiPhy, wifiMac, genMesh);

// create mesh helper
MeshHelper mesh;
mesh = MeshHelper::Default ();
if (!Mac48Address (m_root.c_str ()).IsBroadcast ())
{
    mesh.SetStackInstaller (m_stack, "Root", Mac48AddressValue (Mac48Address (m_root.c_str ())));
}
else
{
    //If root is not set, we do not use "Root" attribute, because it
    //is specified only for 11s
    mesh.SetStackInstaller (m_stack);
}
if (m_chan)
{
    mesh.SetSpreadInterfaceChannels (MeshHelper::SPREAD_CHANNELS);
}
else
{
    mesh.SetSpreadInterfaceChannels (MeshHelper::ZERO_CHANNEL);
}
mesh.SetMacType ("RandomStart", TimeValue (Seconds (m_randomStart)));
// Set number of interfaces - default is single-interface mesh point
mesh.SetNumberOfInterfaces (m_nIfaces);
// Install protocols and return container if MeshPointDevices
genMeshDevice = mesh.Install (wifiPhy, genMesh);

// Mobility model
MobilityHelper mobility;
	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
								   "MinX", DoubleValue (0.0),
								   "MinY", DoubleValue (0.0),
								   "DeltaX", DoubleValue (m_step),
								   "DeltaY", DoubleValue (m_step),
								   "GridWidth", UintegerValue (m_xSize),
								   "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel"); 
    //mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
        //"Bounds", RectangleValue (Rectangle (50, 200, 50, 200)));                                  
	mobility.Install (genMesh);

// Internet Stack
InternetStackHelper stack;
stack.Install (genMesh);

// Assign addresses
Ipv4AddressHelper address;
address.SetBase ("10.0.0.0", "255.255.255.0");
Ipv4InterfaceContainer ic; 
ic = address.Assign (genMeshDevice);

// Create TCP socket
std::string tcpVariant = "TcpNewReno"; 
tcpVariant = std::string ("ns3::") + tcpVariant;
Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (genMesh.Get (0), TcpSocketFactory::GetTypeId ());

// OnOffHelper
OnOffHelper onOffHelper ("ns3::TcpSocketFactory", Address ());
onOffHelper.SetAttribute ("PacketSize", UintegerValue (m_packetSize));
onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
onOffHelper.SetAttribute ("DataRate", DataRateValue (DataRate (dataRate)));
// onOffHelper.SetAttribute ("Interval", TimeValue (Seconds (m_packetInterval)));
 

// Install application
ApplicationContainer meshApp;
uint16_t port = 8080;

//Address node1 (InetSocketAddress (Ipv4Address::GetAny (), port));
//PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", node1);

// star TCP example logic
// each node sends packets to the next node
for (uint32_t i = 0; i < (m_ySize*m_xSize); ++i)
    {
        if (i < ((m_ySize*m_xSize)-1)) {
            AddressValue remoteAddress (InetSocketAddress (ic.GetAddress(i + 1), port)); 
            onOffHelper.SetAttribute ("Remote", remoteAddress);
            meshApp.Add (onOffHelper.Install (genMesh.Get (i)));
        }
        
    }

    meshApp.Start (Seconds (0));
    meshApp.Stop (Seconds (m_totalTime));

// Install FlowMonitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();   

Simulator::Stop (Seconds (m_totalTime));
Simulator:: Run ();

// Collect FlowMonitor data
monitor->SerializeToXmlFile("chunk_FlowMon.xml", true, true);
// Collect NetAnim data
AnimationInterface anim ("chunk_animation_A.xml");

Simulator:: Destroy ();
return 0;
}
