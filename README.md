# Maze Escape Game

Welcome to the **Maze Escape Game**, a maze puzzle game where you need to navigate through a maze to reach the exit while avoiding enemies, solving puzzles, and collecting power-ups.

## Table of Contents

1. [How to Play](#how-to-play)
2. [Algorithm Overview](#algorithm-overview)
3. [Code Explanation](#code-explanation)
4. [Game Features](#game-features)
5. [Bugs and Issues](#bugs-and-issues)
6. [Credits](#credits)
7. [How to Compile and Run the Game](#how-to-compile-and-run-the-game)

---

## How to Play

### Game Setup

Upon starting the game, you will be prompted to select a difficulty level:
- **Easy**: A 10x10 maze with 30 seconds.
- **Medium**: A 20x20 maze with 45 seconds.
- **Hard**: A 30x30 maze with 60 seconds.

You will start at the top-left corner of the maze, and your goal is to reach the exit ('X') before time runs out.

### Controls
- Use **W**, **A**, **S**, **D** to move the player up, left, down, or right respectively.
- The player can interact with **puzzle blocks ('L')**, which require you to solve a simple math problem to proceed.
- Collect **power-ups ('\$')** to freeze enemies temporarily.
- The enemy ('E') moves towards you each turn. Avoid the enemy or they will catch you and the game will end.

### Objective
- Navigate through the maze and reach the exit ('X') before time runs out.
- Avoid the enemy and solve puzzles to proceed through blocked paths.
- Collect power-ups to freeze the enemy for a limited time.

---

## Algorithm Overview

### Maze Generation
The maze is created using a **recursive backtracking algorithm**. Starting from an initial point in the maze (1, 1), the algorithm carves out a path by randomly choosing directions (up, down, left, right), marking valid paths as open. The maze is designed to have walls with a minimum width of two blocks, ensuring that paths are wide enough for movement.

### Enemy Movement
The enemy follows the **Manhattan distance** algorithm, moving one step towards the player in both the horizontal and vertical direction. If the player is located to the right, the enemy moves right; if the player is below, the enemy moves down, and so on. The enemy can be temporarily frozen by collecting power-ups.

### Puzzles
Puzzles are randomly placed in the maze. They are simple math questions (e.g., "What is 5 + 8?"). To pass through a puzzle block ('L'), the player must correctly answer the question. If the player answers incorrectly, they cannot move through that block.

### Power-ups
Power-ups are scattered throughout the maze. Collecting a power-up freezes the enemy for one move, allowing the player to move freely without being pursued by the enemy.

### Timer
The game has a countdown timer that is based on the selected difficulty:
- Easy: 30 seconds
- Medium: 45 seconds
- Hard: 60 seconds
If the player does not reach the exit before time runs out, the game ends.

---

## Code Explanation

### Key Sections of the Code

#### 1. **Global Variables**
- `WIDTH` and `HEIGHT`: These variables store the dimensions of the maze.
- `playerX` and `playerY`: Store the player's current position.
- `enemy`: A structure containing the enemy's position and state (frozen or not).
- `maze`: A dynamically allocated 2D array that represents the maze.

#### 2. **Maze Generation (Backtracking Algorithm)**
- The maze is generated using a **recursive function** `generateMaze(x, y)` which randomly selects directions and carves paths by marking walls as open spaces (' ').
- The algorithm starts at (1,1) and recursively carves out paths by checking if the chosen direction leads to an unvisited position.

#### 3. **Movement**
- The player can move using **WASD** keys. The function `movePlayer(char direction)` updates the player's position based on input.
- The enemy moves towards the player in the function `moveEnemy()`. The enemy's movement is restricted by walls and only moves towards the player if there is no obstacle in the way.
- The enemy's movement is frozen when the player collects a power-up.

#### 4. **Puzzle and Power-up Interactions**
- Puzzles are placed randomly in the maze. When the player attempts to move into a puzzle block, they are prompted with a math question.
- If the player answers correctly, they can move through; otherwise, they must try another direction.
- Power-ups are placed randomly, and collecting one freezes the enemy for one move.

#### 5. **Timer and End Conditions**
- A timer is started when the game begins and counts down each second.
- The game ends if the timer runs out or if the player reaches the exit. If the enemy catches the player, the game also ends.

---

## Game Features

- **Difficulty Levels**: Choose from Easy, Medium, or Hard.
- **Dynamic Maze Generation**: Each playthrough generates a unique maze using a random backtracking algorithm.
- **Enemy AI**: The enemy moves towards the player and can be frozen temporarily using power-ups.
- **Puzzles**: Solve math-based puzzles to clear blocked paths.
- **Power-ups**: Collect power-ups to temporarily freeze the enemy.
- **Timer**: Complete the maze before the time runs out.

---

## Bugs and Issues

While the game is fully playable, there are a few bugs and issues that may affect gameplay. These are actively being worked on for future improvements.

### 1. **Enemy AI May Get Stuck in Walls**
   - **Issue**: Occasionally, the enemy AI may get stuck or move in a way that causes it to collide with walls, making it unable to move toward the player.
   - **Status**: This issue is sporadic and happens under specific conditions where the maze layout limits the enemy’s movement choices.
   - **Workaround**: Currently, no direct workaround is available other than restarting the game. I am working on improving the pathfinding algorithm to prevent this from happening.
   - **Fix**: I am refining the enemy AI to handle movement more effectively and avoid getting stuck in walls.

### 2. **Puzzle Block ('L') Not Clearing After Passing**
   - **Issue**: Sometimes after the player solves a puzzle, the puzzle block ('L') doesn’t disappear, making it impossible to pass through again.
   - **Status**: This happens intermittently when the puzzle question is answered correctly, but the block remains in place.
   - **Workaround**: If this issue occurs, the player should attempt to move in a different direction or restart the game.
   - **Fix**: I am working on to ensure puzzle blocks are correctly cleared after a correct answer.

### 3. **Power-Ups Not Freezing Enemy Correctly**
   - **Issue**: In some instances, collecting a power-up doesn't properly freeze the enemy for the expected number of turns.
   - **Status**: This bug may arise due to timing issues or conflicts with other game mechanics such as player movement or enemy behavior updates.
   - **Workaround**: The game will still function, but the freeze might last for an unexpected number of turns. The enemy might also unfreeze prematurely.
   - **Fix**: I am investigating the issue and planning a fix to ensure power-ups behave as expected.

### 4. **Maze Boundaries May Cause Out-of-Bounds Errors**
   - **Issue**: Occasionally, the player or enemy might attempt to move outside the boundaries of the maze, leading to an out-of-bounds error or undefined behavior.
   - **Status**: This happens when either the player or the enemy gets too close to the edge of the maze, where checks for boundary conditions may not be handled properly.
   - **Workaround**: Avoid moving too close to the edges of the maze. Future updates will improve boundary handling to prevent this error.
   - **Fix**: The game’s boundary checks are being adjusted to prevent out-of-bounds behavior.

### 5. **Game Timer Display May Flicker**
   - **Issue**: On some systems, the timer display may flicker or update inconsistently, making it difficult to track the remaining time.
   - **Status**: This is an issue with how the game’s display is cleared and updated in real-time, particularly on certain platforms.
   - **Workaround**: No immediate workaround for the timer flicker, but the game still works as expected despite the visual glitch.
   - **Fix**: I am working on improving the display update logic to make the timer display more consistent.

### 6. **Enemy Sometimes Doesn't Spawn**
   - **Issue**: Occasionally, the enemy fails to spawn in the maze during initialization, leaving the player without an opponent.
   - **Status**: This issue happens when the random placement logic for the enemy doesn’t find a valid position, possibly due to maze constraints or random placement conflicts.
   - **Workaround**: Restarting the game may resolve the issue. I am working on improving the enemy spawn logic to ensure it always occurs properly.
   - **Fix**: Updates are being made to ensure that the enemy always spawns in a valid location.

### 7. **Player Can Spawn Surrounded by Walls**
   - **Issue**: In some cases, the player might spawn in a position surrounded by walls, making it impossible to move.
   - **Status**: This happens due to a flaw in the maze generation algorithm, where paths are not always fully guaranteed to surround the player’s spawn point.
   - **Workaround**: Restarting the game may help, as the player may spawn in a more favorable location.
   - **Fix**: The maze generation algorithm is being improved to ensure that the player never spawns surrounded by walls.

### 8. **Game Enters an Infinite Loop When Movement Key is Pressed Instead of Answering Puzzle**
   - **Issue**: If the player is prompted to solve a puzzle and accidentally presses a movement key (WASD) or any other key instead of the correct answer, the game enters an infinite loop, failing to continue properly.
   - **Status**: This is caused by improper input handling, where the game logic doesn’t account for non-answer key presses during the puzzle phase, causing an unexpected loop.
   - **Workaround**: The player should ensure to input the correct answer to avoid this. Future updates will add input validation to prevent this issue from interrupting the game flow.
   - **Fix**: A fix is in progress to handle invalid inputs more effectively and ensure smooth gameplay.

### 9. **No Escape Path Due to Enemy Spawn Location**
   - **Issue**: Sometimes, the maze is generated in a way that the player cannot escape from the enemy due to the enemy’s spawn location, effectively trapping the player in the maze.
   - **Status**: This happens when the maze and enemy spawn points are not fully independent, leading to rare cases where the enemy is placed too close to the player.
   - **Workaround**: Restart the game or adjust movement to try and escape.
   - **Fix**: Future updates will ensure that the enemy and player do not start too close to each other.

---

## Credits

The **Maze Escape Game** wouldn't have been possible without the contributions and resources of the following:

- **Game Design**: The core gameplay and mechanics were conceptualized and designed by Okyanus Albas.
- **Maze Generation Algorithm**: Special thanks to [https://stackoverflow.com/questions/42811988/recursive-backtracking-in-c] for the recursive backtracking maze generation algorithm that ensures each maze is unique and challenging.
- **Enemy AI**: The enemy behavior was inspired by classic pursuit algorithms, specifically the Manhattan distance algorithm.
- **Puzzle Design**: The simple math puzzles were created to offer a mix of challenge and fun, with inspiration from Specification Guide on puzzles from our coursework.
- **Testing and Feedback**: Special thanks to [Okyanus Albas and Nills] for their invaluable feedback and bug reports that helped improve the game.

We also want to acknowledge the use of the following open-source libraries/tools:

#include <iostream>   // Provides input and output functionality, such as printing to the console and reading input from the user.

#include <cstdlib>    // Includes functions for general utilities, such as random number generation (e.g., rand() for randomness).

#include <ctime>      // Used for handling date and time, such as seeding random number generation with the current time.

#include <vector>     // The std::vector container is used for dynamic arrays to store and manipulate data like the maze elements.

#include <algorithm>  // Contains algorithms like std::swap, which is used for shuffling directions during maze generation.

#include <chrono>     // Provides functionality for working with time, including the countdown timer and time-based operations.

#include <string>     // Provides support for manipulating strings, which is useful for input handling, such as solving puzzles.

---

## How to Compile and Run the Game

### Quick Path for Developers ONLY

1. Open a terminal or command prompt.

2. Navigate to the project directory:
    OpenDeveleoper Command Prompt for VS 2022
    cd into where you downlaoded the game.
    cd users/albao/documents/dev/MysteryMaze

3. How to Compile
    g++ -std=c++11 -o MysteryMaze MysteryMaze.cpp

    g++ -o MysteryMaze.exe MysteryMaze.cpp -static-libgcc -static-libstdc++

4. Run the game using:
    Windows Systems
        .\MysteryMaze.exe
    UNIX Systems
        ./MysteryMaze
