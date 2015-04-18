# What is Bidding System Socket Programming Demo?

Bidding System Socket Programming Demo is a console bidding system developed to demonstrate the fundamentals of socket programming. It is a network program implemented with C using a client/server model. There are two versions for the client program. One is written for Linux systems; the other is for Windows. The server program must be run on a Linux system.

The Linux client/server program was written using the BSD Socket API. The Windows client program uses the Winsock API.

# Features Description

The server program can be started by typing the following command in a Linux console:

```C
server
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
* post
  * will either return a message indicating a successful or unsuccessful posting
* list
  * will return a list with itemId itemName minBid maxBid
* bid
  * either return the biddingPrice (means new maxBid equals biddingPrice) or a message that indicates bid was too low

# Program Demo

## Server: Start the program

![img1_server_start](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img1_server_start.png)

## Client: Query server for the item list

![img2_client_empty_list](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img2_client_empty_list.png)

## Server: Respond with empty item list

![img3_server_empty_list_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img3_server_empty_list_resp.png)

## Client: Post a new item for bid

![img4_client_post](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img4_client_post.png)

## Server: Accept new item and update list

![img5_server_post_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img5_server_post_resp.png)

## Client: Post another item for bid

![img6_client_post](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img6_client_post.png)

## Server: Accept new item and update list

![img7_server_post_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img7_server_post_resp.png)

## Client: Query server for the item list

![img8_client_list](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img8_client_list.png)

## Server: Respond with new item list

![img9_server_list_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img9_server_list_resp.png)

## Client: Bid on an item with a new high bid

![img10_client_bid](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img10_client_bid.png)

## Server: Accept valid bid

![img11_server_bid_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img11_server_bid_resp.png)

## Client: Bid on an item without a new high bid

![img12_client_failbid](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img12_client_failbid.png)

## Server: Reject invalid bid

![img13_server_failbid_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img13_server_failbid_resp.png)

## Client: Query server for updated list with new bid amount

![img14_client_list](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img14_client_list.png)

## Server: Respond with updated list with new bid amount

![img15_server_list_resp](https://github.com/hinsenchan/bidsys_socket_prog_demo/blob/master/readme/img15_server_list_resp.png)

# References

* [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
* [Microsoft's DevGuide for Winsock](https://msdn.microsoft.com/en-us/library/windows/desktop/ms740632%28v=vs.85%29.aspx)

# Credits

This software was developed by Hinsen Chan at Santa Clara University in Fall 2013.
