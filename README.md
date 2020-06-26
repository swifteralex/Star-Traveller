# Star-Traveller

Star Traveller is a Windows desktop application that has the player travel around in an infinite sea of stars.

![Star Traveller Demo](demo/demo.gif)

## About

Star Traveller's 3D graphics engine is built from scratch. Every frame, the program calculates where each star will be on the screen and draws them
in order of their distance to the player for a depth buffer. Moving the mouse around changes the player's direction, which is done by applying a rotation
matrix to the player's axis. 

## Controls

To player Star Traveller, download and run the .exe within this repository. Left click accelerates the player forward and right click accelerates the
player backwards. Pressing SHIFT increases the amount of acceleration and pressing CTRL decreases the amount of acceleration.
