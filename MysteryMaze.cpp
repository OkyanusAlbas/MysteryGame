#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <string>

// Global variables for maze dimensions, player, enemy, exit, and timer
int WIDTH, HEIGHT;  // Width and Height of the maze
int playerX = 1, playerY = 1;  // Initial player position (X, Y)
int exitX = 0, exitY = 0;  // Position of the exit
int timerLimit = 60;  // Timer limit for the game (in seconds)
char** maze;  // Maze represented as a 2D array

// Enemy structure to store enemy properties (position and freeze state)
struct Enemy {
    int x, y;  // Enemy position
    bool frozen = false;  // Enemy freeze state, initially false
};

// One enemy object in the game
Enemy enemy;

// Function to shuffle directions randomly (used for maze generation)
void manualShuffle(std::vector<int>& directions) {
    // Shuffle the directions array
    for (size_t i = 0; i < directions.size(); ++i) {
        int j = rand() % directions.size();  // Random index to swap with
        std::swap(directions[i], directions[j]);  // Swap the elements
    }
}

// Maze generation using Depth First Search (DFS)
void generateMaze(int x, int y) {
    int dx[] = {0, 0, -1, 1};  // Movement directions for X (left, right, up, down)
    int dy[] = {-1, 1, 0, 0};  // Movement directions for Y (up, down, left, right)
    
    maze[y][x] = ' ';  // Mark the current position as a path

    // Directions to try for movement (0: Up, 1: Down, 2: Left, 3: Right)
    std::vector<int> directions = {0, 1, 2, 3};
    manualShuffle(directions);  // Shuffle directions for randomness

    // Explore each direction
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;  // New X coordinate after moving two steps
        int ny = y + dy[directions[i]] * 2;  // New Y coordinate after moving two steps

        // Check if the new position is within bounds and a wall
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == '#') {
            maze[ny][nx] = ' ';  // Open the new position
            maze[y + dy[directions[i]]][x + dx[directions[i]]] = ' ';  // Open the adjacent block
            generateMaze(nx, ny);  // Recursively generate the maze from the new position
        }
    }
}

// Initialize maze with walls, paths, puzzles, enemies, and the exit
void initializeMaze() {
    // Initialize the maze with walls
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#';  // Set all cells as walls initially
        }
    }

    generateMaze(1, 1);  // Start maze generation from position (1, 1)

    maze[playerY][playerX] = 'P';  // Place the player ('P')

    // Place the enemy ('E') at a random position
    int ex = rand() % (WIDTH - 2) + 1;  // Random X coordinate for the enemy
    int ey = rand() % (HEIGHT - 2) + 1;  // Random Y coordinate for the enemy
    if (maze[ey][ex] == ' ' && !(ex == playerX && ey == playerY)) {
        enemy.x = ex;  // Set enemy's X position
        enemy.y = ey;  // Set enemy's Y position
        maze[enemy.y][enemy.x] = 'E';  // Place the enemy on the maze
    }

    // Place puzzles ('L') at random positions
    int puzzlesToPlace = 5;  // Number of puzzles to place
    for (int i = 0; i < puzzlesToPlace; ++i) {
        int px, py;
        do {
            px = rand() % (WIDTH - 2) + 1;  // Random X for puzzle
            py = rand() % (HEIGHT - 2) + 1;  // Random Y for puzzle
        } while (maze[py][px] != '#' || (px == playerX && py == playerY));  // Ensure the position is empty
        maze[py][px] = 'L';  // Place puzzle at position
    }

    // Place power-ups ('$') at random positions
    int powerUpsToPlace = 3;  // Number of power-ups to place
    for (int i = 0; i < powerUpsToPlace; ++i) {
        int px, py;
        do {
            px = rand() % (WIDTH - 2) + 1;  // Random X for power-up
            py = rand() % (HEIGHT - 2) + 1;  // Random Y for power-up
        } while (maze[py][px] != '#' || (px == playerX && py == playerY));  // Ensure the position is empty
        maze[py][px] = '$';  // Place power-up at position
    }

    // Place the exit ('X')
    bool exitPlaced = false;  // Flag to check if the exit is placed
    while (!exitPlaced) {
        int xRand = rand() % WIDTH;  // Random X coordinate for exit
        int yRand = rand() % HEIGHT;  // Random Y coordinate for exit
        if (maze[yRand][xRand] == '#' && !(xRand == playerX && yRand == playerY) && !(xRand == enemy.x && yRand == enemy.y)) {
            maze[yRand][xRand] = 'X';  // Place the exit
            exitX = xRand;  // Set exit's X position
            exitY = yRand;  // Set exit's Y position
            exitPlaced = true;

            // Make sure the exit is surrounded by 'L' blocks (puzzles)
            if (exitX > 0 && maze[exitY][exitX - 1] == ' ') maze[exitY][exitX - 1] = 'L';  // Left
            if (exitX < WIDTH - 1 && maze[exitY][exitX + 1] == ' ') maze[exitY][exitX + 1] = 'L';  // Right
            if (exitY > 0 && maze[exitY - 1][exitX] == ' ') maze[exitY - 1][exitX] = 'L';  // Up
            if (exitY < HEIGHT - 1 && maze[exitY + 1][exitX] == ' ') maze[exitY + 1][exitX] = 'L';  // Down
        }
    }
}

// Display maze with the timer
void displayMaze(int timeLeft) {
    #ifdef _WIN32
        system("cls");  // Clear screen for Windows
    #else
        system("clear");  // Clear screen for UNIX/Linux
    #endif

    // Display the time left
    std::cout << "Time left: " << timeLeft << " seconds\n\n";

    // Display the maze
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << maze[i][j];  // Print each cell in the maze
        }
        std::cout << std::endl;
    }
    std::cout << "Press 'E' to quit anytime.\n";  // Option to quit
}

// Move the player in the maze based on input (WASD)
void movePlayer(char direction) {
    int newX = playerX, newY = playerY;

    if (direction == 'w') newY--;  // Move up
    else if (direction == 's') newY++;  // Move down
    else if (direction == 'a') newX--;  // Move left
    else if (direction == 'd') newX++;  // Move right

    // Check if the new position is a valid path (not a wall or enemy)
    if (maze[newY][newX] != '#' && maze[newY][newX] != 'E') {
        if (maze[newY][newX] == 'L') {  // If the position is a puzzle
            int num1 = rand() % 100 + 1;  // Generate first random number
            int num2 = rand() % 100 + 1;  // Generate second random number
            int correctAnswer = num1 + num2;  // Calculate the correct answer

            std::cout << "Puzzle: What is " << num1 << " + " << num2 << "? ";
            int answer;
            std::cin >> answer;

            if (answer == correctAnswer) {  // If the answer is correct
                std::cout << "Correct! You may pass.\n";
                maze[newY][newX] = ' ';  // Open the path
            } else {  // If the answer is incorrect
                std::cout << "Incorrect! You cannot pass through.\n";
                return;
            }
        }

        if (maze[newY][newX] == '$') {  // If the position is a power-up
            std::cout << "You collected a power-up! The enemy is frozen for one move.\n";
            enemy.frozen = true;  // Freeze the enemy for one move
            maze[newY][newX] = ' ';  // Remove the power-up from the maze
        }

        maze[playerY][playerX] = ' ';  // Clear the previous player position
        playerX = newX;  // Update player's X position
        playerY = newY;  // Update player's Y position
        maze[playerY][playerX] = 'P';  // Place the player at the new position
    }
}

// Move the enemy towards the player (if not frozen)
void moveEnemy() {
    if (enemy.frozen) {
        std::cout << "The enemy is frozen for one move!\n";
        enemy.frozen = false;  // Unfreeze the enemy
        return;
    }

    int dx = 0, dy = 0;  // Change in X and Y for enemy movement

    // Determine direction of movement based on player's position
    if (enemy.x < playerX) dx = 1;
    else if (enemy.x > playerX) dx = -1;

    if (enemy.y < playerY) dy = 1;
    else if (enemy.y > playerY) dy = -1;

    // Move the enemy horizontally if possible
    if (dx != 0) {
        if (maze[enemy.y][enemy.x + dx] != '#' && maze[enemy.y][enemy.x + dx] != 'E') {
            maze[enemy.y][enemy.x] = ' ';  // Clear previous enemy position
            enemy.x += dx;  // Move the enemy horizontally
            maze[enemy.y][enemy.x] = 'E';  // Place the enemy at the new position
        }
    }

    // Move the enemy vertically if possible
    if (dy != 0) {
        if (maze[enemy.y + dy][enemy.x] != '#' && maze[enemy.y + dy][enemy.x] != 'E') {
            maze[enemy.y][enemy.x] = ' ';  // Clear previous enemy position
            enemy.y += dy;  // Move the enemy vertically
            maze[enemy.y][enemy.x] = 'E';  // Place the enemy at the new position
        }
    }
}

// Main game loop
int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed random number generator

    int difficulty = 2;  // Default difficulty level (Normal)

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
                if (difficulty == 1) { WIDTH = 20; HEIGHT = 10; }
                else if (difficulty == 2) { WIDTH = 40; HEIGHT = 20; }
                else { WIDTH = 60; HEIGHT = 30; }

                // Dynamically allocate maze memory
                maze = new char*[HEIGHT];
                for (int i = 0; i < HEIGHT; ++i) {
                    maze[i] = new char[WIDTH];
                }

                initializeMaze();  // Initialize maze with walls, paths, etc.

                auto start = std::chrono::high_resolution_clock::now();  // Start timer
                int timeLeft = timerLimit;  // Set the initial time left

                while (timeLeft > 0) {
                    auto now = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<int> elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
                    timeLeft = timerLimit - elapsed.count();  // Update remaining time

                    displayMaze(timeLeft);  // Display the current maze with time left

                    char move;
                    std::cout << "Use WASD to move: ";
                    std::cin >> move;

                    if (move == 'e' || move == 'E') {
                        std::cout << "You chose to quit the game.\n";
                        break;  // Exit the game loop
                    }

                    movePlayer(move);  // Move player based on input
                    moveEnemy();  // Move the enemy towards the player

                    // Check if player reached the exit
                    if (playerX == exitX && playerY == exitY) {
                        std::cout << "Congratulations! You escaped the maze!\n";
                        break;
                    }

                    // Check if time has run out
                    if (timeLeft <= 0) {
                        std::cout << "Time's up! You lost.\n";
                        break;
                    }

                    // Check if the enemy caught the player
                    if (playerX == enemy.x && playerY == enemy.y) {
                        std::cout << "The enemy caught you! You lost.\n";
                        break;
                    }
                }

                // Free the maze memory after the game ends
                for (int i = 0; i < HEIGHT; ++i) {
                    delete[] maze[i];
                }
                delete[] maze;

                break;
            }

            case 2:  // Instructions
                std::cout << "Instructions: Navigate the maze using WASD. Solve puzzles to pass certain blocks.\n";
                break;

            case 3:  // Settings
                std::cout << "Select difficulty (1 = Easy, 2 = Normal, 3 = Hard): ";
                std::cin >> difficulty;  // Select difficulty level
                break;

            case 4:  // Quit
                std::cout << "Thank you for playing! Goodbye.\n";
                return 0;
        }
    }

    return 0;
}
