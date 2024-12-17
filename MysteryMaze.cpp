#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>  // For std::swap
#include <chrono>     // For chrono functions
#include <string>

// Global variables for the maze dimensions and player position
const int WIDTH = 20;
const int HEIGHT = 10;
int playerX = 1, playerY = 1;  // Initial player position
int exitX = WIDTH - 2, exitY = HEIGHT - 2;  // Exit position
int timerLimit = 60;  // Timer limit for the game
char maze[HEIGHT][WIDTH];  // Maze array

// Define an Enemy structure for easy manipulation
struct Enemy {
    int x, y;
};

Enemy enemy;  // One enemy in the game

// Function to manually shuffle directions
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

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == '#') {
            maze[ny][nx] = ' ';  // Make a path
            maze[y + dy[directions[i]]][x + dx[directions[i]]] = ' ';  // Create a connection

            generateMaze(nx, ny);  // Recurse to the next cell
        }
    }
}

// Function to initialize the maze with walls and enemies
void initializeMaze() {
    // Fill the maze with walls ('#')
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#';
        }
    }

    // Create the maze paths starting from (1, 1)
    generateMaze(1, 1);

    // Place the player ('P')
    maze[playerY][playerX] = 'P';

    // Place one enemy ('E') at random locations, avoiding player and exit
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
        maze[py][px] = 'L';
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
        }
    }
}

// Function to display the maze
void displayMaze() {
    #ifdef _WIN32
        system("cls"); // Use "cls" for Windows
    #else
        system("clear"); // Use "clear" for Unix-based systems
    #endif

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Function to check and move the player
void movePlayer(char direction) {
    int newX = playerX, newY = playerY;

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

        // Move the player
        maze[playerY][playerX] = ' ';
        playerX = newX;
        playerY = newY;
        maze[playerY][playerX] = 'P';  // Place player at new position
    }
}

// Function to move the enemy towards the player
void moveEnemy() {
    int dx = 0, dy = 0;

    if (enemy.x < playerX) dx = 1;
    else if (enemy.x > playerX) dx = -1;

    if (enemy.y < playerY) dy = 1;
    else if (enemy.y > playerY) dy = -1;

    // Check if the enemy can move in the selected direction (no walls)
    if (maze[enemy.y + dy][enemy.x + dx] != '#' && maze[enemy.y + dy][enemy.x + dx] != 'E') {
        maze[enemy.y][enemy.x] = ' ';  // Clear the old enemy position
        enemy.x += dx;
        enemy.y += dy;
        maze[enemy.y][enemy.x] = 'E';  // Place enemy in new position

        // Check if the enemy collides with the player
        if (enemy.x == playerX && enemy.y == playerY) {
            std::cout << "You were caught by the enemy! You lost!\n";
            exit(0);  // End the game
        }
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

        switch (choice) {
            case 1: { // Start Game
                // Adjust difficulty (if needed)
                if (difficulty == 2) {
                    timerLimit = 45; // Less time on harder levels
                } else if (difficulty == 3) {
                    timerLimit = 30; // Even less time
                }

                // Initialize maze with walls, paths, enemies, and puzzles
                initializeMaze();

                // Game loop with timer
                auto start = std::chrono::steady_clock::now();
                bool escaped = false;

                while (true) {
                    // Timer check
                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

                    if (elapsed > timerLimit) {
                        std::cout << "Time's up! You failed to escape the maze.\n";
                        break;
                    }

                    // Display the maze with player
                    displayMaze();

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
                        break;
                    }
                }

                if (!escaped) {
                    std::cout << "Game Over!\n";
                }

                break;
            }

            case 2: // Instructions
                std::cout << "Use WASD (lowercase or uppercase) to move. Avoid enemies and solve puzzles!\n";
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
                return 0;

            default:
                std::cout << "Invalid option.\n";
                break;
        }
    }
}
