# Project Proposal
### :snake: Slither.io for of-snake :snake:


I wish to create a multiplayer version of the game snake that would behave similarly
to the game slitherio. I would essentially create the game such that it would use
a server and then each individual client would connect. It might involve threads
for the use of creating a server thread for the others to connect to, but it may
just be easier for port forwarding purposes just to offload that to somewhere
where I can port forward anyway.

The libraries I plan to use are the libraries BOOST, ASIO, EASTL (for speed), 
maybe different graphical engines that I find I like more that work better for
native platforms better than I like how openframeworks works 

Here are my objectives:

1. Add multiplayer with TCP connections
    * Possibly using JSON to pass data between client and server
    * JSON to conigure things on the server on startup
        * Food Spawn rate
        * Length increase for each food
        * Size of map
        * speed of snakes?
    * Create a stand alone server to host the game on something like AWS
2. Adjust the game mechanics for more of a sliterio feel
3. Make the game go beyond the boundaries of the window
4. Make a minimap that displays locations of the players
5. Be able to name your snake and have that name be associated with scoreboard
6. Add a nicer GUI with options for adding name, starting server
7. Make the food line up with the boundaries (personal OCD)
8. Make the snakes show orientation or eyes or something to distinguish them
9. Create a seperate server executable that is configurable that can be run

[Boost!](https://www.boost.org/)
[Boost ASIO](https://think-async.com/)
[EA STL (for speed!)](https://github.com/electronicarts/EASTL)
