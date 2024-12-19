
Quick path to paste For Developer ONLY

cd users/albao/documents/dev/MysteryMaze
g++ -std=c++11 -o MysteryMaze MysteryMaze.cpp
.\MysteryMaze.exe

# Maze Escape Game

Welcome to the **Maze Escape Game**, a maze puzzle game where you need to navigate through a maze to reach the exit while avoiding enemies, solving puzzles, and collecting power-ups. 

## Table of Contents

1. [How to Play](#how-to-play)
2. [Algorithm Overview](#algorithm-overview)
3. [Code Explanation](#code-explanation)
4. [Game Features](#game-features)
5. [Credits](#credits)

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

## Credits

This game was created by [Okyanus Albas]. Special thanks to all the open-source libraries and resources that helped in the creation of this game.

---

## How to Run the Game

1. Clone this repository to your local machine.
2. Open the terminal or command prompt.
3. Navigate to the folder containing the project.
4. Compile the game using a C++ compiler (e.g., `g++ -std=c++11 -o MysteryMaze MysteryMaze.cpp`).
5. Run the compiled program (`./MysteryMaze.exe` on Unix or `.\MysteryMaze.exe` on Windows).
6. Enjoy playing!

---

## License
