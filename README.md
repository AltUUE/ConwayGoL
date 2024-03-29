# Game of Life

This is a C++ implementation of Conway's Game of Life with SDL2 UI.

Algorithm designed and implemented by Altay Kılıç.  
Ui elements and functionality designed and implemented by Umut Utku Erşahince.

Usage of this software is also discussed in the in-game help (press h in game).

## GENERAL INFORMATION

- To launch this program, `cd into the main directory` and execute `bin/main`. If your working directory is not the main directory, default input and saves will fail to load resulting most likely in a crash if the user wants to use these features. If you want to use these features without launching using the said method, you need to recompile this codebase after you change `std::ifstream myFile("input.txt");` from GoL.cpp `readFromFile` function and `std::fstream appFile("saves.txt", std::fstream::app);` from GoL.cpp `saveGameState` function to include the absolute path to `input.txt` and `saves.txt`.
- You can use "w", "a", "s", "d" to move around the map.
- You can use the mouse wheel to zoom-in (scroll up) and zoom-out (scroll down).
- Press "x" to delete all living squares.
- Pressing "space" at any time will append the current state to saves.txt in vanilla format. For more information about vanilla format, see "Default Input Specifications".
	- A name for the save will be required for the game to continue.
	- The user may enter a string to the command prompt window to name the save.
	- Pressing "enter" without providing a string will leave the save unnamed.
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

To have a default input you need to supply `input.txt` with a valid vanilla format (described below) starting position.  
Here is the syntax description of `input.txt` (Which is the in-game default input) and `saves.txt`:

    rule: vanilla  
    (x1) (y1) (x2) (y2) ...

First line describes the rule. Vanilla is a standart implementation of Game of Life.  
The second line contains all `(x, y)` pairs. No line breaks are present. No particular order is necessary.

    rule: multi  
    (x1) (y1) (red1) (green1) (blue1) ...

In the second line a cell is described by 5 parameters. 2 for position and 3 for color.  
Second line syntax is very similar to vanilla syntax. Instead of `(x, y)` pairs there are `(x, y, red, green, blue)` quintuplets.

Putting '#' at the beginning of a line will make a comment line.

There are converters in the converter folder with which you can convert popular input formats to usable vanilla format.  
Vanilla is the format described above with which you can supply the `input.txt` file.  
Note that this implementation of GoL will not work with other formats as inputs such as `rle` or plaintext (abbr. ptxt).  
A converter CLI is in place to easily convert formats. Please see "Convertercli Usage".

## CONVERTERCLI USAGE

The general usage of convertercli is as follows:
- Supply converter/in.txt with input.
- Run this command: `convertercli.exe <input_format> <output_format>`
	- input_format is the format of converter/input.txt. Valid options: `van` (vanilla), `ptxt` (plaintext), `rle`, `mul` (multi).
	- output_format is the format of converter/output.txt. Valid options vary based on input_format:
		- input_format = `van` => valid output_format options: `ptxt`, `rle`.
		- input_format = `rle` => valid output_format options: `van` (only option in 2.3.2), `ptxt` (since 2.3.3).
		- input_format = `ptxt` => valid output_format options: `van` (only option in 2.3.2), `rle` (since 2.3.3).
		- input_format = `mul` => valid output_format options: `van` (No other converters will be added).
- You can find the converted output in converter/out.txt.
