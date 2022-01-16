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
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/mesh-module.h"
#include "ns3/mesh-helper.h"


// flow monitor modules
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"


NS_LOG_COMPONENT_DEFINE ("ChunkExample");

using namespace ns3;

// calculate throughput
Ptr<PacketSink> sink;                         /* Pointer to the packet sink application */
uint64_t lastTotalRx = 0;                     /* The value of the last total received bytes */

void
CalculateThroughput ()
{
  Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
  double cur = (sink->GetTotalRx () - lastTotalRx) * (double) 8 / 1e5;     /* Convert Application RX Packets to MBits. */
  std::cout << now.GetSeconds () << "s: \t" << cur << " Mbit/s" << std::endl;
  lastTotalRx = sink->GetTotalRx ();
  Simulator::Schedule (MilliSeconds (100), &CalculateThroughput);
}

// main function
int
main (int argc, char *argv[])
{

// define variables 
uint32_t payloadSize = 1472; // Transport layer payload size in bytes

// mesh variables
int       m_xSize = (3); ///< X size
int       m_ySize = (3); ///< Y size
double    m_step = (100.0); ///< step
double    m_randomStart = (0.1); ///< random start
double    m_totalTime = (100.0); ///< total time
double    m_packetInterval = (0.1); ///< packet interval
uint16_t  m_packetSize = (1024); ///< packet size
uint32_t  m_nIfaces = (1); ///< number interfaces
bool      m_chan = (true); ///< channel
bool      m_pcap = (false); ///< PCAP
bool      m_ascii = (true); ///< ASCII
std::string m_stack = ("ns3::Dot11sStack"); ///< stack
std::string m_root = ("ff:ff:ff:ff:ff:ff"); ///< root

//std::string tcpVariant = "TcpNewReno"; 
//tcpVariant = std::string ("ns3::") + tcpVariant;

// TCP options
Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));

WifiMacHelper wifiMac;
WifiHelper wifiHelper;
wifiHelper.SetStandard (WIFI_STANDARD_80211n_5GHZ);

// Static Routing
Ipv4StaticRoutingHelper staticRoutingHelper;
Ipv4ListRoutingHelper listRouting;
listRouting.Add (staticRoutingHelper, 0);

// Create Mesh Nodes
NodeContainer genMesh;
NetDeviceContainer genMeshDevice;
genMesh.Create(m_ySize*m_xSize);

// Configure YansWifiChannel
YansWifiPhyHelper wifiPhy;
YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
wifiPhy.SetChannel (wifiChannel.Create ());

// create mesh helper
MeshHelper mesh;
mesh = MeshHelper::Default (); //
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
	mobility.Install (genMesh);

// if tracing enabled
if (m_pcap)
wifiPhy.EnablePcapAll (std::string ("mp-"));
if (m_ascii)
{
    AsciiTraceHelper ascii;
    wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("mesh.tr"));
}

// Internet Stack
InternetStackHelper stack;
stack.Install (genMesh);

// Assign addresses
Ipv4AddressHelper address;
address.SetBase ("10.0.0.0", "255.255.255.0");
Ipv4InterfaceContainer ic; 
ic = address.Assign (genMeshDevice);

// application 

// Populate routing table
Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

// Print routing table
Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>("chunk.routes",std::ios::out); 
	for (int i = 0; i < (m_xSize*m_ySize); i++)
  	{
    	listRouting.PrintRoutingTableAt (Seconds (1.0), genMesh.Get(i), routingStream);
  	}

// Install FlowMonitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();   

Simulator::Stop (Seconds (100));
Simulator:: Run ();

// Collect FlowMonitor data
monitor->SerializeToXmlFile("chunk_FlowMon.xml", true, true);

Simulator:: Destroy ();
return 0;
}
