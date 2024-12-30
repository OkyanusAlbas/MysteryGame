#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>  // For std::swap (used to shuffle directions)
#include <chrono>     // For timing functions (used for countdown timer)
#include <string>

// Global variables for the maze dimensions and player position
int WIDTH, HEIGHT;
int playerX = 1, playerY = 1;  // Initial player position (1,1)
int exitX = 0, exitY = 0;  // Exit position (initialized to 0,0)
int timerLimit = 60;  // Timer limit for the game (60 seconds by default)
char** maze;  // Maze array (dynamic, as it changes based on size)

// Define an Enemy structure for easy manipulation
struct Enemy {
    int x, y;
    bool frozen = false;  // Freeze state for the enemy
};

Enemy enemy;  // One enemy in the game

// Function to manually shuffle directions (used to randomize maze paths)
void manualShuffle(std::vector<int>& directions) {
    for (size_t i = 0; i < directions.size(); ++i) {
        int j = rand() % directions.size();  // Generate a random index
        std::swap(directions[i], directions[j]);  // Swap elements to shuffle
    }
}

/***********************************************************************
 * Function to generate the maze with paths, walls, and exits.
 * This function uses a depth-first search (DFS) approach to carve out the maze.
 * It starts from the point (x, y) and creates paths by marking cells as open (' ').
 * The maze is created with 2-block wide corridors, and walls are represented by '#'.
 ***********************************************************************/
void generateMaze(int x, int y) {
    // Directions (up, down, left, right)
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    // Mark the current cell as a path
    maze[y][x] = ' ';

    // Create a vector of directions and shuffle them
    std::vector<int> directions = { 0, 1, 2, 3 };
    manualShuffle(directions);  // Call the manual shuffle function

    // Loop through each direction to carve paths recursively
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;  // Calculate the new x-coordinate
        int ny = y + dy[directions[i]] * 2;  // Calculate the new y-coordinate

        // Make sure the new cell is within bounds and is still a wall
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == '#') {
            maze[ny][nx] = ' ';  // Make a path
            maze[y + dy[directions[i]]][x + dx[directions[i]]] = ' ';  // Create a connection

            generateMaze(nx, ny);  // Recurse to the next cell to generate the maze
        }
    }
}

/***********************************************************************
 * Function to initialize the maze with walls, paths, enemies, puzzles,
 * and power-ups.
 * The maze is generated, and additional elements such as power-ups, puzzles, 
 * and the exit are placed randomly within the maze.
 ***********************************************************************/
void initializeMaze() {
    // Fill the maze with walls ('#')
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#';  // Place walls at all locations initially
        }
    }

    // Create the maze paths starting from (1, 1)
    generateMaze(1, 1);  // Start maze generation from (1, 1)

    // Place the player ('P') at the starting position
    maze[playerY][playerX] = 'P';  // Player's initial position is marked as 'P'

    // Place one enemy ('E') at a random location, avoiding player and exit
    int ex = rand() % (WIDTH - 2) + 1;  // Random x-coordinate for enemy
    int ey = rand() % (HEIGHT - 2) + 1;  // Random y-coordinate for enemy
    if (maze[ey][ex] == ' ' && !(ex == playerX && ey == playerY)) {  // Check if the position is valid
        enemy.x = ex;  // Set enemy's x-coordinate
        enemy.y = ey;  // Set enemy's y-coordinate
        maze[enemy.y][enemy.x] = 'E';  // Place the enemy in the maze
    }

    // Place puzzles ('L') at random locations on walls (#)
    int puzzlesToPlace = 5;  // Number of puzzles to place
    for (int i = 0; i < puzzlesToPlace; ++i) {
        int px, py;
        do {
            px = rand() % (WIDTH - 2) + 1;  // Random x-coordinate for puzzle
            py = rand() % (HEIGHT - 2) + 1;  // Random y-coordinate for puzzle
        } while (maze[py][px] != '#' || (px == playerX && py == playerY));  // Retry if position is invalid
        maze[py][px] = 'L';  // Place a puzzle
    }

    // Place power-ups ('$') at random locations on walls (#)
    int powerUpsToPlace = 3;  // Number of power-ups to place
    for (int i = 0; i < powerUpsToPlace; ++i) {
        int px, py;
        do {
            px = rand() % (WIDTH - 2) + 1;  // Random x-coordinate for power-up
            py = rand() % (HEIGHT - 2) + 1;  // Random y-coordinate for power-up
        } while (maze[py][px] != '#' || (px == playerX && py == playerY));  // Retry if position is invalid
        maze[py][px] = '$';  // Place a power-up
    }

    // Randomly place the exit ('X') on the walls
    int exitPlaced = false;
    while (!exitPlaced) {
        int xRand = rand() % WIDTH;  // Random x-coordinate for exit
        int yRand = rand() % HEIGHT;  // Random y-coordinate for exit

        // Exit must be on a wall and not on the player or enemy
        if (maze[yRand][xRand] == '#' && !(xRand == playerX && yRand == playerY) && !(xRand == enemy.x && yRand == enemy.y)) {
            maze[yRand][xRand] = 'X';  // Place the exit
            exitX = xRand;  // Set exit's x-coordinate
            exitY = yRand;  // Set exit's y-coordinate
            exitPlaced = true;  // Exit placement is successful

            // Surround the exit with puzzles 'L'
            // Ensure the cells adjacent to the exit are 'L' (puzzles)
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    // Avoid placing 'L' directly on the exit (X) itself
                    int nx = exitX + dx;  // Calculate adjacent x-coordinate
                    int ny = exitY + dy;  // Calculate adjacent y-coordinate

                    // Only place puzzle blocks if it's within bounds and is a wall
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && maze[ny][nx] == '#' && !(nx == exitX && ny == exitY)) {
                        maze[ny][nx] = 'L';  // Place a puzzle
                    }
                }
            }
        }
    }
}

/***********************************************************************
 * Function to display the current maze and the remaining time.
 * The maze is printed line-by-line, showing the player position, enemies,
 * and other elements such as puzzles, power-ups, and the exit.
 * The remaining time is displayed at the top of the maze.
 ***********************************************************************/
void displayMaze(int timeLeft) {
    #ifdef _WIN32
        system("cls"); // Clear screen for Windows
    #else
        system("clear"); // Clear screen for Unix-based systems
    #endif

    // Display the time left on the screen
    std::cout << "Time left: " << timeLeft << " seconds\n\n";

    // Print the entire maze row by row
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << maze[i][j];  // Display each character in the maze
        }
        std::cout << std::endl;  // Newline after each row
    }
}

/***********************************************************************
 * Function to move the player based on user input (WASD).
 * The player moves in the corresponding direction unless blocked by walls.
 * Puzzles ('L') require the player to solve them before moving past them.
 * Power-ups ('$', which freeze the enemy) can be collected during movement.
 ***********************************************************************/
void movePlayer(char direction) {
    int newX = playerX, newY = playerY;

    // Update player position based on direction
    if (direction == 'w' || direction == 'W') {
        newY--;  // Move up
    } else if (direction == 's' || direction == 'S') {
        newY++;  // Move down
    } else if (direction == 'a' || direction == 'A') {
        newX--;  // Move left
    } else if (direction == 'd' || direction == 'D') {
        newX++;  // Move right
    }

    // Check if the new position is valid (not a wall or out of bounds)
    if (maze[newY][newX] != '#' && maze[newY][newX] != 'E') {
        if (maze[newY][newX] == 'L') {
            // The player is trying to move to a puzzle block
            int num1 = rand() % 100 + 1;
            int num2 = rand() % 100 + 1;
            int correctAnswer = num1 + num2;

            std::cout << "Puzzle: What is " << num1 << " + " << num2 << "? ";
            int answer;
            std::cin >> answer;

            if (answer == correctAnswer) {
                std::cout << "Correct! You may pass.\n";
                maze[newY][newX] = ' ';  // Remove the puzzle block
            } else {
                std::cout << "Incorrect! You cannot pass through.\n";
                return;  // Do not allow the player to move
            }
        }

        if (maze[newY][newX] == '$') {
            // The player collects a power-up
            std::cout << "You collected a power-up! The enemy is frozen for one move.\n";
            enemy.frozen = true;  // Freeze the enemy
            maze[newY][newX] = ' ';  // Remove the power-up
        }

        // Move the player to the new position
        maze[playerY][playerX] = ' ';  // Clear the old player position
        playerX = newX;  // Set the player's new x-coordinate
        playerY = newY;  // Set the player's new y-coordinate
        maze[playerY][playerX] = 'P';  // Place player at new position
    }
}

/***********************************************************************
 * Function to move the enemy towards the player.
 * The enemy moves one step at a time, either horizontally or vertically.
 * If the enemy collides with the player, the game ends.
 * If the enemy is frozen (due to power-up), it skips its move.
 ***********************************************************************/
void moveEnemy() {
    if (enemy.frozen) {
        std::cout << "The enemy is frozen for one move!\n";
        enemy.frozen = false;  // Unfreeze the enemy after one move
        return;
    }

    int dx = 0, dy = 0;

    // Move horizontally towards the player
    if (enemy.x < playerX) {
        dx = 1;  // Move right
    } else if (enemy.x > playerX) {
        dx = -1; // Move left
    }

    // Move vertically towards the player
    if (enemy.y < playerY) {
        dy = 1;  // Move down
    } else if (enemy.y > playerY) {
        dy = -1; // Move up
    }

    // Prioritize horizontal movement if both horizontal and vertical movement is possible
    if (dx != 0) {
        // Check if the enemy can move horizontally (no walls)
        if (maze[enemy.y][enemy.x + dx] != '#' && maze[enemy.y][enemy.x + dx] != 'E') {
            maze[enemy.y][enemy.x] = ' ';  // Clear the old enemy position
            enemy.x += dx;  // Move the enemy horizontally
            maze[enemy.y][enemy.x] = 'E';  // Place enemy in new position
        }
    }
    
    if (dy != 0) {
        // Check if the enemy can move vertically (no walls)
        if (maze[enemy.y + dy][enemy.x] != '#' && maze[enemy.y + dy][enemy.x] != 'E') {
            maze[enemy.y][enemy.x] = ' ';  // Clear the old enemy position
            enemy.y += dy;  // Move the enemy vertically
            maze[enemy.y][enemy.x] = 'E';  // Place enemy in new position
        }
    }
}

/***********************************************************************
 * Main game loop.
 * The user can choose between starting a new game, reading instructions,
 * changing settings, or quitting. In the game mode, the player uses WASD to
 * move, the timer counts down, and the game ends if the player reaches the exit 
 * or if time runs out.
 ***********************************************************************/
int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

    bool inGame = false;
    int difficulty = 2;  // Default difficulty (medium)

    // Main menu loop
    while (true) {
        std::cout << "1. Start Game\n";
        std::cout << "2. Instructions\n";
        std::cout << "3. Settings\n";
        std::cout << "4. Quit\n";
        std::cout << "Select an option (1-4): ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {  // Start Game
                // Set maze dimensions based on difficulty
                if (difficulty == 1) {  // Easy
                    WIDTH = 20; HEIGHT = 10;
                } else if (difficulty == 2) {  // Medium
                    WIDTH = 40; HEIGHT = 20;
                } else {  // Hard
                    WIDTH = 60; HEIGHT = 30;
                }

                // Allocate memory for the maze
                maze = new char*[HEIGHT];
                for (int i = 0; i < HEIGHT; ++i) {
                    maze[i] = new char[WIDTH];
                }

                // Initialize the maze
                initializeMaze();

                // Timer setup
                auto start = std::chrono::high_resolution_clock::now();
                int timeLeft = timerLimit;

                // Game loop
                while (timeLeft > 0) {
                    // Calculate the time left by comparing to the start time
                    auto now = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<int> elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
                    timeLeft = timerLimit - elapsed.count();

                    // Display the maze with the remaining time
                    displayMaze(timeLeft);

                    // Handle player input for movement
                    char move;
                    std::cout << "Use WASD to move: ";
                    std::cin >> move;

                    movePlayer(move);  // Move the player based on input
                    moveEnemy();  // Move the enemy towards the player

                    // Check if player reached the exit
                    if (playerX == exitX && playerY == exitY) {
                        std::cout << "Congratulations! You escaped the maze!\n";
                        break;  // Player has escaped successfully
                    }
                }

                // Clean up the dynamically allocated memory for the maze
                for (int i = 0; i < HEIGHT; ++i) {
                    delete[] maze[i];
                }
                delete[] maze;

                // Reset the game state
                inGame = false;
                break;
            }
            case 2: {  // Instructions
                std::cout << "\nInstructions:\n";
                std::cout << "1. Use WASD keys to navigate through the maze.\n";
                std::cout << "2. Collect power-ups ('$') to freeze the enemy for one move.\n";
                std::cout << "3. Solve puzzles (L) to unlock paths.\n";
                std::cout << "4. Reach the exit (X) before time runs out to win.\n";
                std::cout << "5. Avoid the enemy (E) that tries to catch you.\n\n";
                break;
            }
            case 3: {  // Settings
                // Allow player to choose difficulty
                std::cout << "\nSettings:\n";
                std::cout << "1. Easy\n";
                std::cout << "2. Medium\n";
                std::cout << "3. Hard\n";
                std::cout << "Select difficulty (1-3): ";
                std::cin >> difficulty;
                break;
            }
            case 4: {  // Quit
                std::cout << "Thanks for playing! Goodbye.\n";
                return 0;  // Exit the program
            }
        }
    }
}
