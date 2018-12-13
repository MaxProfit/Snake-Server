### Completed for week 1
##### November 9

    - [x] Get a server to communicate with a process on another port

    This week was really about making sure I could get a simple server working
    on a different port with BOOST ASIO. 

### Completed for week 2
##### November 16

    - [x] Get two computers to communicate together
    - [x] Parse JSON and output the key:value pair to ensure that it works    
    - [x] Write a data format for world data, storing snakes and food locations
    - [x] Write a data format for configuration, one for client, one for server
    - [x] Host the server code on an AWS instance
    
    This week started off debugging a ton of stuff with the AWS server, making
    sure I had the security group setup such that I could get the port recieving
    inbound connections to the server on port 49145. The biggest issue with the
    server and the client mismatch and developing on two entirely different
    systems was really getting the same BOOST library setup on both systems.
    Using brew gave an outdated version, but using apt-get on Ubuntu gave a
    different outdated version. I ended up just installing BOOST and compiling
    it myself. The next challenge I faced was really about serializing the JSON
    and then passing that through network buffers. This didn't work out the way
    I was hoping it too and passing a vector into a buffer didn't seem to pass
    through to the callback function. Speaking of callbacks, I gained much
    insight this week into how threading, asyncronous tasks and lambda functions
    work in c++ and what their actual purpose comes out to be.

### Completed for week 3
##### November 23

Canceled the amazon instance for now to start developing locally because it's
easier for about every mean. Debugged the network io_context such that it is now
able to do polling which allows the server to not only deal with sending and 
receiving of the JSON but also the the server logic at the same time which is
actually a huge issue for this problem. The real time aspect of this project is 
the hardest. Because there really is no waiting to make sure the socket is closed
or that we don't have garbage data because at that point it is already too late.

### Completed for week 4
##### November 30

Took week off to study calc

### Completed for week 5
##### December 7

Got the networking working perfectly. Able to now encapsulate within the game 
loop on client side and a timer on the server side, without the connection 
closing and threads working such that we can send messages and recieve them at 
the same time while dealing with the server loop. 

### Completed for week 6
##### December 14

Got the game actually created with everything generated and the JSON being able 
to be passed through the network to actually service each individual client.
Created many optimizations but didn't have time to look into the syncronization 
of network sockets which I didn't think was going to be this big of a problem in
the final solution. 
