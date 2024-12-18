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
        int j = rand() % directions.size();
        std::swap(directions[i], directions[j]);  // Swap elements to shuffle
    }
}

// Function to generate the maze with 2-block wide corridors
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
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        // Make sure the new cell is within bounds and is still a wall
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == '#') {
            maze[ny][nx] = ' ';  // Make a path
            maze[y + dy[directions[i]]][x + dx[directions[i]]] = ' ';  // Create a connection

            generateMaze(nx, ny);  // Recurse to the next cell to generate the maze
        }
    }
}

// Function to initialize the maze with walls, paths, enemies, puzzles, and power-ups
void initializeMaze() {
    // Fill the maze with walls ('#')
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#';
        }
    }

    // Create the maze paths starting from (1, 1)
    generateMaze(1, 1);

    // Place the player ('P') at the starting position
    maze[playerY][playerX] = 'P';

    // Place one enemy ('E') at a random location, avoiding player and exit
    int ex = rand() % (WIDTH - 2) + 1;
    int ey = rand() % (HEIGHT - 2) + 1;
    if (maze[ey][ex] == ' ' && !(ex == playerX && ey == playerY)) {
        enemy.x = ex;
        enemy.y = ey;
        maze[enemy.y][enemy.x] = 'E';  // Place the enemy in the maze
    }

    // Place puzzles ('L') at random locations on walls (#)
    int puzzlesToPlace = 5;  // Number of puzzles to place
    for (int i = 0; i < puzzlesToPlace; ++i) {
        int px, py;
        do {
            px = rand() % (WIDTH - 2) + 1;
            py = rand() % (HEIGHT - 2) + 1;
        } while (maze[py][px] != '#' || (px == playerX && py == playerY));  // Retry if position is invalid
        maze[py][px] = 'L';  // Place a puzzle
    }

    // Place power-ups ('$') at random locations on walls (#)
    int powerUpsToPlace = 3;  // Number of power-ups to place
    for (int i = 0; i < powerUpsToPlace; ++i) {
        int px, py;
        do {
            px = rand() % (WIDTH - 2) + 1;
            py = rand() % (HEIGHT - 2) + 1;
        } while (maze[py][px] != '#' || (px == playerX && py == playerY));  // Retry if position is invalid
        maze[py][px] = '$';  // Place a power-up
    }

    // Randomly place the exit ('X') on the walls
    int exitPlaced = false;
    while (!exitPlaced) {
        int xRand = rand() % WIDTH;
        int yRand = rand() % HEIGHT;

        // Exit must be on a wall and not on the player or enemy
        if (maze[yRand][xRand] == '#' && !(xRand == playerX && yRand == playerY) && !(xRand == enemy.x && yRand == enemy.y)) {
            maze[yRand][xRand] = 'X';  // Place the exit
            exitX = xRand;
            exitY = yRand;
            exitPlaced = true;

            // Surround the exit with puzzles 'L'
            // Ensure the cells adjacent to the exit are 'L' (puzzles)
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    // Avoid placing 'L' directly on the exit (X) itself
                    int nx = exitX + dx;
                    int ny = exitY + dy;

                    // Only place puzzle blocks if it's within bounds and is a wall
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && maze[ny][nx] == '#' && !(nx == exitX && ny == exitY)) {
                        maze[ny][nx] = 'L';  // Place a puzzle
                    }
                }
            }
        }
    }
}

// Function to display the maze and remaining time
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
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Function to move the player based on input (WASD)
void movePlayer(char direction) {
    int newX = playerX, newY = playerY;

    // Update player position based on direction
    if (direction == 'w' || direction == 'W') {
        newY--;
    } else if (direction == 's' || direction == 'S') {
        newY++;
    } else if (direction == 'a' || direction == 'A') {
        newX--;
    } else if (direction == 'd' || direction == 'D') {
        newX++;
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
        maze[playerY][playerX] = ' ';
        playerX = newX;
        playerY = newY;
        maze[playerY][playerX] = 'P';  // Place player at new position
    }
}

// Function to move the enemy towards the player (fixed horizontal/vertical movement)
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
            enemy.x += dx;
            maze[enemy.y][enemy.x] = 'E';  // Place enemy in new position
        }
    }
    
    if (dy != 0) {
        // Check if the enemy can move vertically (no walls)
        if (maze[enemy.y + dy][enemy.x] != '#' && maze[enemy.y + dy][enemy.x] != 'E') {
            maze[enemy.y][enemy.x] = ' ';  // Clear the old enemy position
            enemy.y += dy;
            maze[enemy.y][enemy.x] = 'E';  // Place enemy in new position
        }
    }

    // Check if the enemy collides with the player
    if (enemy.x == playerX && enemy.y == playerY) {
        std::cout << "You were caught by the enemy! You lost!\n";
        exit(0);  // End the game
    }
}

// Main game loop
int main() {
    srand(time(0)); // Seed the random number generator
    int choice;
    bool inGame = false;
    int difficulty = 1; // Default difficulty: 1 (Easy)

    // Main menu loop
    while (true) {
        if (!inGame) {
            std::cout << "\nMain Menu:\n";
            std::cout << "1. Start Game\n";
            std::cout << "2. Instructions\n";
            std::cout << "3. Settings\n";
            std::cout << "4. Quit\n";

            // Input validation loop for menu selection
            while (true) {
                std::cout << "Enter your choice (1-4): ";
                std::cin >> choice;

                if (choice >= 1 && choice <= 4) {
                    break; // Valid input, break the loop
                } else {
                    std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
                }
            }
        }

        // Switch case to handle different menu options
        switch (choice) {
            case 1: { // Start Game
                // Set maze size based on difficulty
                if (difficulty == 1) {
                    WIDTH = 10;
                    HEIGHT = 10;
                    timerLimit = 30;
                } else if (difficulty == 2) {
                    WIDTH = 20;
                    HEIGHT = 20;
                    timerLimit = 45;
                } else if (difficulty == 3) {
                    WIDTH = 30;
                    HEIGHT = 30;
                    timerLimit = 60;
                }

                // Allocate memory for the maze dynamically
                maze = new char*[HEIGHT];
                for (int i = 0; i < HEIGHT; ++i) {
                    maze[i] = new char[WIDTH];
                }

                // Initialize maze with walls, paths, enemies, puzzles, and power-ups
                initializeMaze();

                // Game loop with timer
                auto start = std::chrono::steady_clock::now();
                bool escaped = false;

                while (true) {
                    // Timer check
                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
                    int timeLeft = timerLimit - elapsed;

                    if (timeLeft <= 0) {
                        std::cout << "Time's up! You failed to escape the maze.\n";
                        break; // Exit the loop if time is up
                    }

                    // Display the maze with player and timer
                    displayMaze(timeLeft);

                    // Ask player to move or exit
                    char direction;
                    std::cout << "Move (WASD) or press E to Exit: ";
                    std::cin >> direction;

                    // Handle player movement
                    if (direction == 'e' || direction == 'E') {
                        std::cout << "Exiting to Main Menu...\n";
                        break; // Exit the game and return to main menu
                    }

                    // Move the player based on the input (case insensitive)
                    movePlayer(direction);

                    // Move the enemy towards the player
                    moveEnemy();

                    // Check if player reaches the exit
                    if (playerX == exitX && playerY == exitY) {
                        std::cout << "Congratulations! You've escaped the maze!\n";
                        escaped = true;
                        break; // Exit the loop if player escapes
                    }
                }

                // Clean up dynamically allocated maze memory
                for (int i = 0; i < HEIGHT; ++i) {
                    delete[] maze[i];
                }
                delete[] maze;

                if (!escaped) {
                    std::cout << "Game Over!\n";
                }

                break;
            }

            case 2: // Instructions
                std::cout << "Use WASD (lowercase or uppercase) to move. Avoid enemies, solve puzzles, and collect power-ups!\n";
                break;

            case 3: // Settings (difficulty selection)
                std::cout << "Select Difficulty:\n";
                std::cout << "1. Easy\n";
                std::cout << "2. Medium\n";
                std::cout << "3. Hard\n";

                while (true) {
                    std::cin >> difficulty;
                    if (difficulty >= 1 && difficulty <= 3) {
                        break; // Valid input, break the loop
                    } else {
                        std::cout << "Invalid input. Please select a difficulty between 1 and 3.\n";
                    }
                }
                break;

            case 4: // Quit
                std::cout << "Thanks for playing!\n";
                return 0; // Exit the program

            default:
                std::cout << "Invalid choice. Try again.\n";
                break;
        }

        // Reset inGame flag to false for the next loop
        inGame = false;
    }

    return 0;
}
