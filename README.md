![Dice Invaders](/Content/Images/DICE_Invaders.png?raw=true "Dice Invaders")

# DiceInvaders
A sample Space Invaders game for DICE studios in L.A.

This is a small code exercise part of the interview process at DICE L.A.

This game is written in C++ and uses the provided library for inputs and sprites visualisation.

Repository
----------------

https://github.com/guillaumep-nvizzio/DiceInvaders.git


Build Procedures
----------------

1) Clone the repository from github
2) Open the solution file DiceInvaders.sln
3) Compile in Debug or Release
4) Output file in \Binaries\Win32 folder

Development Notes
-----------------

I have setup the base project in visual studio by creating an empty solution and starting with the provided sample code. The game builds on the x86 platform in Debug and Release. The post-build steps are copying 
the data assets to the output directory.

1) I started by creating a 'Game' class. That manages the initialization/deinitialization of the provided library, handles inputs, manages game objects and other global functionalities. 
2) I then created a 'GameObject' class that will encapsulate all the game objects functionalities. It will be the parent class of classes like 'Player', 'Aliens', 'Bombs' and 'Rockets'. 
3) Sketched the Player and Alien classes. Create the player on screen when the game starts and make sure the inputs are processed properly.
4) Created the AlienFormation class that will represent a group of aliens on screen. This class will manage most of the game logic.
5) Created the Bombs and PlayerRockets game objects: these are similar and pretty simple since they only manage the objects movement.
6) Implemented most of the game logix based on the requirements of the assignement: player health, player death, alien death.
7) Did collision detection with a few functions in the GameObject class: Contains(), OnScreen(), Intersect() and make sure all the objects are deleted properly when no longer needed.
8) Implemented a basic User Interface with player score, health and ammunitions. When the game is over, the UI listens to Inputs to restart the game in space is pressed.

Coding Standards
-----------------
In terms of code standards, I mainly use the Google style guides (C++, JavaScript, Objective-C, and Python) for my personal projets so this is the coding style I adopted for this project.
Consistency is most important to strive for, the header of the provided library is formated differently from the rest of the projet, but that's about it.

https://github.com/google/styleguide
