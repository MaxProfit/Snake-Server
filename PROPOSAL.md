# Project Proposal
## :snake: Slither.io + Snake :snake:

### Proposal

My proposal is to create a multiplayer version of the game Snake that would
behave much like the online game [_sliter.io_](http://slither.io/). I would create a game such that it
would involve a client server relationship and would require optimizing the
game loop and creating additional rules and graphics for the game to make it
more user friendly and more like a production ready version of the game. This
might also require using native GUI libraries to get the best looks for each
operating system, making executables seperate for each system that have a nice
GUI for playing either multiplayer or single player, changing the skin of the
snake, etc. The server will also have a configurable portion consisting of a
json file for simple configuration.

### Objectives

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

### Libraries

* [EA STL](github.com/electronicarts/EASTL)
* [Boost](www.boost.org/)
* [ASIO](think-async.com/)
* [OpenFrameworks](openframeworks.cc)

### Resources

* I will use whatever game design + networking things I can find on the internet
* Youtube channel that I have been studying C++ off of called [TheChernoProject](https://www.youtube.com/user/TheChernoProject)

### Things I Hope to Learn

* Network programming
* Speed & Optimization
* How to use native GUIs
* How to make a shippable game

### Deadlines (self imposed)

1. November 9th
    - [x] Get a server on a port then connect with another process
    - [ ] Get two computers to communicate together
    - [ ] Parse JSON and output the key:value pair to ensure that it works
    - [ ] Write unit tests testing the network, most likely testing localhost
    - [ ] Write a data format for world data, storing snakes and food locations
    - [ ] Write a data format for configuration, one for client, one for server
    - [ ] Learn about syncronous vs asyncronous servers and their benefits
    - [ ] Host the server code on an AWS instance
2. November 16th
    - [ ] Get two snakes in the same game together
    - [ ] Make the snakes conform to the new rules of slitherio
3. November 23rd
    - [ ] Make the game go beyond the boundaries of the window
    - [ ] Make a minimap for the game
4. November 30th
    - [ ] Clean up the graphics and make some new snake skins
    - [ ] Have the leaderboard be able to store names associated with score
    - [ ] Figure out how to unit test something as complex as this
5. December 7th

6. December 14th