Introduction
------------
This project is a fork of my old project 'Speed Dreams' which was itself at its creation a fork of the 'TORCS' project.

No longer really in agreement with the direction taken by the new development team, I became more of a hindrance to this team due to my own vision of the project. So I decided to let this project live its life and founded a new project that corresponded more to my expectations.

During the fifteen years that I managed the 'Speed Dreams' project I realized that many members joined the development team 
to be able to integrate what interested them and then left the project. This didn't really bother me as long as it served my own vision of the project, but today that is no longer the case.

Goals
-----
I'm almost starting from a blank page, not really since the whole base is there. 

But from the start the player will not be able to have access to all the resources, these will be unlocked as the player unlocks certain situations. As a result, I have modifications to make throughout the racesituation section, the cars, tracks and raceman interfaces. 

I am also thinking of completely redesigning the first page of the menu to have direct access to all the shopping categories directly on this page. 

I am also thinking of migrating all the 3D models in bin format, this will save considerable loading time, but I have not yet decided on which format this migration will be carried out.

The main idea of the game is to be as close as possible to reality in racing (I'm not talking about the simulation part yet). 

So all the championships will therefore be over 355 km (calculation based on Monza for 62 laps) for the formula category. 

Subsequently, there will be endurance championships (24H type) in real time. Of course, no player to my knowledge could last 24 hours continuously, but the idea is to do this type of championship with several people, like real-life teams. This will be explained during the implementation of this championship. But for that I have to find the solution for night lighting, which at the moment is not the case.

It appeared with the development of Speed Dreams and the development of the new graphics engine based on openscenegraph, that we were losing multitexturing on the implementation of new functions. Indeed, OSG is a very good library but remains basic development in simple texture. 

In addition, OSG is no longer developed, just maintained (for the moment), which implies that in a few years we would be in the same problem as with PLIB which is no longer supported at all. My basic idea is to develop a graphics engine based on PLIB, which was a very good opengl 1.2 library (but extended for OpenGL2, OpenGL3 and Vulkan)

Changes since last release
--------------------------
See CHANGES.txt


Things that are still to be done, ideas to be ...
-------------------------------------------------
See TODO.txt


Installation instructions
-------------------------
See INSTALL.txt


Manual
------
MANUAL.txt for English version
MANUAL-FR.txt for French version

Testing
-------
If you find problems which should be already fixed or new ones 
please report them to the cars-sports-racing-users mailing list or ticket


Getting Help
------------
During the game press F1
(do it as well during the ride to learn about the options).

For more in depth information visit https://sourceforge.net/projects/cars-sports-racing/,
you'll hopefully find there some information (documentation, HOWTO, FAQ ... work-in-progress).

If you are stuck please report problems through cars-sports-racing-users mailing list
or our Tracker (visit https://sourceforge.net/p/cars-sports-racing/tickets/ and login to SF.net for that).


Car Setups
----------
You can have you own version on the car setups for the human driver
- Linux   : ~/.config/cars-sports-racing/drivers/human/...
- Windows : <My documents>\cars-sports-racing.settings\human\...
depending on the car used. You can change them with a text editor.
