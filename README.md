# Maze Escape Game

## Game Overview
Welcome to the **Maze Escape Game**! In this game, you must navigate through a maze filled with walls, puzzles, power-ups, and an enemy trying to catch you. Your goal is to escape the maze before time runs out, all while avoiding the enemy and solving puzzles along the way.

---

## How to Play

1. **Starting the Game:**
   - Upon starting the game, you'll be presented with a menu to select your action:
     - **1. Start Game**: Begins the game where you can choose a difficulty level and start navigating the maze.
     - **2. Instructions**: Displays a list of instructions and how to play the game.
     - **3. Settings**: Change the difficulty level of the game.
     - **4. Quit**: Exit the game.

2. **Navigating the Maze:**
   - Use the **WASD** keys to move your character through the maze:
     - `W` = Move Up
     - `A` = Move Left
     - `S` = Move Down
     - `D` = Move Right
   - Avoid moving into walls (`#`) and the enemy (`E`).
   - You can press **E** to quit the game at any time.

3. **Puzzles:**
   - Some areas in the maze are blocked by puzzle blocks (`L`).
   - When you step on one of these blocks, you’ll be asked an arithmetic question (e.g., "What is 23 + 42?").
   - If you answer correctly, the block will be removed, allowing you to pass. If you answer incorrectly, you cannot proceed and must find another path.

4. **Power-ups:**
   - Power-ups (`$`) are scattered throughout the maze. When you collect a power-up, the enemy will be frozen for one move, giving you a chance to escape or avoid it.
   - The enemy remains frozen for just one turn after collecting a power-up, so plan your moves accordingly.

5. **Goal of the Game:**
   - The goal is to reach the exit (`X`) before time runs out.
   - If the enemy catches you, you lose the game. If you run out of time, you also lose.
   - If you successfully reach the exit, you win the game!

6. **Time Limit:**
   - The game is time-bound. You have 60 seconds to escape the maze before time runs out and you lose the game.
   
---

## Maze Generation

The maze is generated using a **Depth-First Search (DFS)** algorithm. The steps of the maze generation are as follows:

1. **Walls and Paths:**
   - The maze starts as a grid filled entirely with walls (`#`).
   - The DFS algorithm is then used to carve out paths (` `) in a random, non-repetitive manner.

2. **Randomized Directions:**
   - The maze generation algorithm shuffles the possible movement directions (up, down, left, right) for each recursion step to ensure a randomized maze layout.
   
3. **Exit Placement:**
   - The exit (`X`) is placed at a random valid location within the maze, ensuring it is not near the player or the enemy.
   - Puzzle blocks (`L`) surround the exit to prevent the player from accessing it too easily.

4. **Enemy and Power-up Placement:**
   - An enemy (`E`) is placed at a random position in the maze, avoiding the player’s starting position.
   - Power-ups (`$`) are also placed randomly, and collecting them freezes the enemy for one turn.

---

## Additional Features

1. **Difficulty Levels:**
   - The game has three difficulty levels that affect the maze size:
     - **Easy (1)**: 20x10 maze
     - **Normal (2)**: 40x20 maze (default)
     - **Hard (3)**: 60x30 maze
   - You can select the difficulty level from the settings menu before starting the game.

2. **Dynamic Maze Generation:**
   - Each time you start a new game, the maze will be randomly generated, providing a unique experience each time.

3. **Timer:**
   - You have a set amount of time (60 seconds) to escape the maze before time runs out and you lose the game.
   
4. **Enemy AI:**
   - The enemy moves towards your position in the maze, trying to catch you.
   - If the enemy touches you, you lose the game.
   - The enemy can be frozen temporarily by collecting power-ups.

---

## Game Controls

- **WASD Keys**: Move the player up, down, left, or right.
- **E**: Quit the game at any time.
- **1**: Start the game.
- **2**: View instructions.
- **3**: Change the difficulty.
- **4**: Quit the game.

---

## Tips and Strategies

1. **Avoid the Enemy:**
   - Keep an eye on the enemy’s movements and plan your moves carefully. Use the power-ups to freeze the enemy and gain a strategic advantage.

2. **Solve Puzzles to Progress:**
   - Don’t skip the puzzles! Correctly solving them will allow you to pass through blocked paths and get closer to the exit.

3. **Manage Time Wisely:**
   - The clock is ticking! Make sure to move quickly but cautiously. The timer is your enemy, too!

4. **Use Power-ups:**
   - Grab power-ups to freeze the enemy for one move, buying yourself some extra time to escape.

---

## FAQ

- **Q: How do I change the difficulty?**
  - You can change the difficulty level from the settings menu before starting the game. The difficulty affects the size of the maze.

- **Q: Can I pause the game?**
  - Currently, the game does not support pausing. If you want to stop playing, you can quit by pressing **E**.

- **Q: What happens if the enemy catches me?**
  - If the enemy touches your character, you lose the game, and the maze will reset.

- **Q: What if I don’t solve the puzzle?**
  - If you answer the puzzle incorrectly, you cannot pass that puzzle block (`L`), and you’ll need to find another route.

---

## Rules of Engagement

- **External Resources Used:**
  - The **Depth-First Search (DFS) algorithm** for maze generation was adapted from the following resources:
    - [DFS Maze Generation - GeeksforGeeks](https://www.geeksforgeeks.org/recursive-depth-first-search/) (GeeksforGeeks)
    - [DFS Algorithm for Maze Generation](https://www.geeksforgeeks.org/recursive-depth-first-search/) (GeeksforGeeks)

  - For the **WASD movement controls** and other game mechanics, the game logic and structure were inspired by the following C++ tutorials:
    - [Learn C++ - Cplusplus.com](https://www.cplusplus.com/doc/tutorial/) (cplusplus.com)
    - [C++ Game Programming - LearnCpp](https://www.learncpp.com/cpp-tutorial/game-programming/) (LearnCpp.com)

  - **Puzzle mechanics** (such as the arithmetic questions and blocked paths) were based on general game design principles:
    - [Game Programming Patterns](http://gameprogrammingpatterns.com/)

- **Fair Use:**
  - All external resources used for educational purposes in the game (such as algorithms or techniques) are cited in this section. Where appropriate, any related code snippets, algorithms, or concepts have been modified and adapted to meet the specific needs of this game.

---

## Enjoy the Game!

Good luck escaping the maze, and may your puzzle-solving skills help you survive the chase!
