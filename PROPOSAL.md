# Project Proposal
### Slither.io for of-snake

I have thought about this for a weekend and I think that turning the snake game into a multiplayer
game that's like slitherio would be interesting. It would involve using the TCP networking and
creating a server that would spawn the food and keep track of the players and it would also
consist of player entities that would connect to the server on the specific port and play the
game. This project would also entail using XML to configure the server specification and file path
also for the configuration of the server. I am also interested in learning how to do threading so 
maybe I could offload the work of the server onto another thread and choose to boot it from the
game's main menu. Another library that I would have to use is the application core and gui.
Maybe if i get all of that completed I could figure out how to run this controller off of arduino.

:snake: Sliter.io just got some competition: snake.io. This game is designed to mimic the
functionality of slither.io but in the form of the of-snake assignment that was just recently given
to us. I plan to expand this concept greatly

1. Create a server and be able to play through the network through TCP.
2. Create a map and expand the game beyond just the tiles we can see
3. Optimize the event loop and maybe add threading to support the server.
4. Create an XML file that can edit the configuration of the server
- things such as: Food spawn rate, amount of snake increase per food, etc.
- this data would then have to be sent in the initial packet back to the clients.
5. I would also have to use the application core of OF for creating the application itself
6. Along with the Gui for spawning up a server, playing single player, or joining a multiplayer game

:vulcan_sulute:
