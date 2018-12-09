# FinalProject
Final Project

# Final Project: Snake.io
## :snake: Slither.io + Snake :snake:

Snake.io is a simple C++ multiplayer game that brings sliter.io to the desktop!
Created out of the need for learning about networking code and building a fast
running and reliable game engine, snake.io is made simple to setup, using common
libraries. The game is a proof of concept for simple TCP game server that uses
JSON to communicate between the server and the client.

## Getting Started

These instructions will get you a copy of the project up and running on your
local machine for development and testing purposes. See deployment for notes on
how to deploy the project on a live system.

### Prerequisites

1. Get [boost](https://www.boost.org/users/download/) 1.68.0 or later installed
on your system. (Boost 1.68.0 is the only version currently tested on all systems.)

2. Download and install OpenFrameworks from [OpenFrameworks](openframeworks.cc).
Openframeworks contains the tools and APIs needed for the code to function.

### Installing

1. Download the zip from the top of this repository. Then run the OpenFrameworks
[Project Generator](https://openframeworks.cc/learning/01_basics/create_a_new_project/)
to get all of the libraries and APIs setup that we will need for the code to run
on your native system. Once the project generator is finished running, it will
show you that you can open the project locally in your IDE of choice, and from
there you're able to run the client version of the code.

2. Click on run and the code will use the server that I am running on AWS as the
host for the game, eliminating the need for you to use your own server or host
one locally, and using LAN.

## Running the tests

To test the client, run the tests subfolder, which will run the unit tests with
respect to the client logic. This logic includes ensuring the game client is
able to connect to the server and is able to properly represent the gameboard
and make alterations to the snakes.

## Deployment

If you for some reason want to make your own server for this game, the server
files reside in the server folder. All you have to do is go into the server
folder and run make and it will create the server executable. This executable
will run on port 49145, which is configurable in the header files for both the
server and the client. Note this will only work if connecting outside the
network if the router is port forwarded on that port, allowing the outside
connections to come through.

## Built With

* [Boost](https://www.boost.org/) - The library that allows for networking
* [OpenFrameworks](https://openframeworks.cc/) - Graphics and GUI API

## Authors

* **Matthew Williams** - *Initial work* - [MaxProfit](https://github.com/MaxProfit)

## Acknowledgments

* Thanks to Boost/ASIO for a great starter examples that really helped
* CS126 at UIUC, with Joan my code review moderator, and professor Evans
* My fellow code review victims :joy:: Ethan, Amanda, Eric, and Kadri