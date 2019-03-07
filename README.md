# BrickBreaker
A classic implementation of a Brick Breaker game.

BRICK BREAKER

This is a classic version of Brick Breaker implemented in OpenGL using C++.

The game starts with some bricks linned up at the top of the screen and with
a ball that is hovering above a platform.The platform will always follow the
mouse pointer. When you left click the ball will fly vertically towards the
bricksand will come back towards the platform when it will smash a brick or
when it will reach the top wall. When the ball comes towards the platform and
the platform does not catch it you will lose a life(you have 3 lives, which
are represented in the bottom left corner of the screen).When you lose all of
the 3 lives you lose the game and it will reset. The objective of the game is
to smash all the bricks, at which point the game will reset.
There also exists some special bricks hidded among the other which will drop
a PowerUp.There are two types of PowerUps:
Shield - It will spawn a fourth wall at the bottom of the screen and it will
speed up the ball and it will last 20 seconds.
Get Big - As the name implies it will make the platform bigger.

In order to play the game you have to open Framework_EGC.sln(which is located
in the Visual Studio folder) and compile the program.The sources are located
in the Source/BrickBreaker folder.
