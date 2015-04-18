# What is Bidding System Socket Programming Demo?

Bidding System Socket Programming Demo is a console bidding system developed to demonstrate the fundamentals of socket programming. It is a network program implemented with C using a client/server model. There are two versions for the client program. One is written for Linux systems; the other is for Windows. The server program must be run on a Linux system.

The Linux client/server program was written using the BSD Socket API. The Windows client program uses the Winsock API.

# Program Walkthrough

The server program can be started by typing the following command in a Linux console:

```C
Server
```

The client program can be started by typing the following command in a Linux or Windows console:

```C
client (IP_address|host_name) port_num command{parameter}
```

command{parameter} can be any of the following
* post itemName minBid
* list
* bid itemId biddingPrice

The response will:
* either return a nonzero itemID for success or a zero for failure(e.g. duplicate itemName)
* return a list of itemId itemName minBid maxBid
* either return the biddingPrice (means maxBid becomes biddingPrice) or a zero for losing the bid (i.e. biddingPrice <= maxBid)

# References

* [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
* [Microsoft's DevGuide for Winsock](https://msdn.microsoft.com/en-us/library/windows/desktop/ms740632%28v=vs.85%29.aspx)

# Credits

This software was developed by Hinsen Chan at Santa Clara University in Fall 2013.
