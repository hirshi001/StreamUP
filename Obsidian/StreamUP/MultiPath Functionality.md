Connections can have multiple paths between the endpoints to send data. We would like to use this fact.

For instance, a phone can connect to a server using both wifi and cellular data. Since they will take different paths, we can increase the bandwidth

Proposal: Create a Path class for each path open to a connection. This requires the user which handles sending and receiving packets to also know which interface they came from, and provide a mapping from the user defined interface to a format compatible with StreamUP

```C++
class Path {
	Interface interface;
	RTTEstimator estimator;
	CongestionControl congestionControl;
}
```

Each path will have its own outgoing interface, an RTTEstimator, and CongestionControl algorithm.

The RTTEstimator and CongestionControl algorithm will provide estimates of rtt, packet loss rate, and bandwidth. We can use this fact to determine which streams to put in which Path based on weighted priority and reliability.

```C++
class Connection {
	std::vector<std::unique_ptr<Path>> paths;
}
```