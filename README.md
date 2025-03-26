Byte-Bandits_Tetris
IT206TetrisGame Project-2

Implementation Details:
1. Block Representation:
   - The game uses 4x4 grids to represent blocks, and each block type is a shape made of 1s and 0s.
   - The `block_list` stores the different shapes of the blocks (7 types in total).

2. Game Loop:
   - The `gameLoop()` function continuously runs the game until `gameover` is set to `true`.
   - It checks for user inputs (arrow keys and spacebar) and spawns new blocks after a fixed interval.

3. Collision Detection:
   - Blocks can be moved or rotated unless they collide with other blocks or the boundary.
   - The function `isCollide(x, y)` checks if a block can be placed at a given position.

4. Score Calculation:
   - Score increases based on the number of cleared lines (every time a row is full, it’s cleared).
   - The `updateScore()` function adds points for each cleared row.

5. Display:
   - The game display is refreshed using the `display()` function, where the score and highscore are shown along with the current game field.

6. Game Over:
   - The game ends when a new block collides with the top of the field (i.e., when it can't be placed).
   - After game over, the player can choose to restart or view the high score.

7. High Score:
   - The high score is saved to and loaded from a file (`highscore.txt`) so it persists between game sessions.
  




Game Controls:
1. Arrow Keys:
   - Left Arrow (←): Move the block left.
   - Right Arrow (→): Move the block right.
   - Up Arrow (↑): Rotate the block clockwise.
   - Down Arrow (↓): Move the block down (soft drop).

2. Spacebar: Drop the block quickly (hard drop) to the bottom.

3. Escape (ESC): Quit or pause the game.
