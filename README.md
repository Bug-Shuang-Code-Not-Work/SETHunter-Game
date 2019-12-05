# SETHunter-Game
This is a racing game built with C++ and DirectX 

Control: Up Arrow to accelerate, Down Arrow to decelerate. Left and Right Arrow to control direction.


Gameplay Rules:
The game mechanism in the game can be broken into several stages:

The first stage is the initial stage. All objects are initialized and only player car is rendered inside the screen and all other objects other than the randomly generated plants are not visible in the screen. At this stage, the player car is staying at the initial position with speed of 0, waiting for player keyboard input. The background music is playing in loop.

The second stage is the start of game, it requires the player to press the up key to accelerate the car. As soon as the car is started, the scores starts to increase as long as the car stays on the road.

If it goes off the road, the scores will stop and back on road after 3 seconds punishment before resume on increasing.
Once the player car is started, the game timer will render the enemy car from the bottom of the screen to appear in the screen. At the same time, the background music will be switched to enemy music. If the user is on the road, enemy car will launch missile to attempt to destroy the player. If the player is off the road, then the enemy will stick to the road but will resume to attack user once the scores start to increase after the user come back on road. The user is given 3 life in the game and user has the choice to press space to set a land mine on the road to destroyer enemy car. During the combat on the road between the player and the enemy, the enemy will keep following the position of the player on the road and user has the choice to accelerate the car to increase the distance to the enemy and the acceleration sound effect will be triggered. Of course, the player car can crash into the plants.

The next stage is that if the enemy car is destroyed, it will disappear from the screen before it shows up again in 5 seconds to keep bothering the player.

If the user got hit by the missile, the life will be deducted, and all objects are reset to the initial stage waiting for user to start the car to initiate a second round of game.


Initial Start of the Game
![Inital Screen]
(https://github.com/Bug-Shuang-Code-Not-Work/SETHunter-Game/blob/master/screenshots/initial.png)

Enemy Pursuing player car and attack 
![Enemy Pursuit]
(https://github.com/Bug-Shuang-Code-Not-Work/SETHunter-Game/blob/master/screenshots/enemypurse.png)

Ongoing Battle between user and enemy
![Ongoling Battle]
(https://github.com/Bug-Shuang-Code-Not-Work/SETHunter-Game/blob/master/screenshots/ongolingcombat.png)

The Score is reset if user is dead
![Score Reset]
(https://github.com/Bug-Shuang-Code-Not-Work/SETHunter-Game/blob/master/screenshots/scorereset.png)


