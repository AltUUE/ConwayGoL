# Game of Life

This is a c++ implementation of Conway's Game of Life with SDL2 ui.

Algorithm designed and implemented by Altay Kılıç.  
Ui elements and functionality designed and implemented by Umut Utku Erşahince.

Usage of this software is also discussed in the in-game help (press h in game).

## GENERAL INFORMATION

- You can use "w", "a", "s", "d" to move around the map.
- You can use the mouse wheel to zoom-in (scroll up) and zoom-out (scroll down).
- Press "x" to delete all living squares.
- Pressing "space" at any time will append the current state to saves.txt in vanilla format. For more information about vanilla format, see "Default Input Specifications".
- This implementation comes with 3 modes:
	- In the default mode, input mode, you can add or delete squares (also called living squares) with left click.
	No new generation will be created. You can access this mode with pressing "i".
	- In the automatic mode, new generations are created periodically. You can manipulate the speed with "j" (speed up) or "k" (slow down).
	You can access this mode with pressing "enter". Note that if you press "enter" with 0 living squares. The game will load the default input.
	More information about default input can be found in the corresponding section.
	- In the stepwise mode, new generations are created at will by clicking "t". You can access this mode  with pressing "t" while another mode is active.
	- Note that if you press "t" with 0 living squares, default input will launch.
- Multilife (abbr. multi) is an original implementation of GoL in which squares have colors (Think of it as races.) where the color of the newly birthed square is calculated using anaverage value function in the RGB scale. You can access Multilife in-game with pressing 1-6 (selects colors). You can exit with 0.
- For further information, see in-game help or the corresponding section.
- Pressing "r" will switch to Multilife and assign random colors to every living square.
- To exit the game, simply use the UI.

## DEFAULT INPUT SPECIFICATIONS

To have a default input you need to supply input.txt with a valid vanilla format (described below) starting position.  
Here is the syntax description of input.txt (Which is the in-game default input) and saves.txt:

> rule: vanilla  
(x1) (y1) (x2) (y2) ...
