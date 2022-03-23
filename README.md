# An ns3 Simulation Study of End-to-End TCP Variants

The transition from wired to wireless networks did not account for
the original design of Transmission Control Protocols (TCP/IP) where wireless behavior
triggers congestion control, slowing down the network because it is unable to distinguish between wired data loss
and wireless mobility. This research implements, tests, and evaluates the TCP variants New Reno, YeAH, and Highspeed,
each equipped to handle wireless behavior. By utilizing the ns3 discrete event simulator, the three variants
will be compared, allowing for standardized simulations across all three approaches. The results showcase that no one variant
outperforms the other by using metrics such as throughput, packet loss, and jitter. Originally, instead of using HighSpeed, BBR was going to be tested. However, initial testing
showed its mean throughput values to be zero, and was swapped out for HighSpeed. Variants New Reno and HighSpeed were both built off
the same original variant of Reno, causing their behavior in simulation to be exactly the same while YeAH could outperform them with a lower
jitter measure, the amount by which the measure is less is not significant enough to be confident. This means that the results
of the testing in this research are inconclusive, where New Reno, YeAH, and HighSpeed do not outperform each other based on the performance metrics used and the simulations performed.
Future work includes running these tests on a real network in order to compare the performance to the ns3 simulations.


**This repository contains the materials that go alongside my senior thesis.**
