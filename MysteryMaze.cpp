#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm> // For std::swap

// Define constants for the maze dimensions
const int WIDTH = 64;
const int HEIGHT = 64;

// 2D array representing the maze
char maze[HEIGHT][WIDTH];

// Player's position
int playerX = 1, playerY = 1; 

// Enemy structure
struct Enemy {
    int x, y;
    void moveRandomly() {
        int move = rand() % 4;
        if (move == 0 && maze[y-1][x] != '#') y--; // Move up
        else if (move == 1 && maze[y+1][x] != '#') y++; // Move down
        else if (move == 2 && maze[y][x-1] != '#') x--; // Move left
        else if (move == 3 && maze[y][x+1] != '#') x++; // Move right
    }
};

// Inventory structure
struct Item {
    std::string name;
    int quantity;
};

// Store items collected by the player
std::vector<Item> inventory;

// Directions for recursive backtracking (up, down, left, right)
int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

// Function to initialize the maze with walls
void initializeMaze() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = '#'; // Fill the maze with walls initially
        }
    }
}

// Function to display the maze
void displayMaze() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

// Function to check if a cell is within the maze bounds
bool inBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

// Manual shuffling for directions in the maze generation
void carveMaze(int x, int y) {
    maze[y][x] = ' '; // Mark the current cell as a path

    // Randomize the directions manually using simple swaps
    std::vector<int> dirs = {0, 1, 2, 3};  // Directions: 0 = Up, 1 = Down, 2 = Left, 3 = Right

    // Manually shuffle directions using a simple swapping technique
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;  // Random index between 0 and 3
        std::swap(dirs[i], dirs[j]);  // Swap elements i and j in the directions array
    }

    // Try all directions in the randomized order
    for (int i = 0; i < 4; ++i) {
        int dir = dirs[i];
        int newX = x + directions[dir][0];
        int newY = y + directions[dir][1];

        if (inBounds(newX, newY) && maze[newY][newX] == '#') {
            // Carve a path to the neighboring cell
            maze[newY][newX] = ' ';
            // Carve the wall between the current cell and the new cell
            maze[y + directions[dir][1] / 2][x + directions[dir][0] / 2] = ' ';
            // Recursively carve the maze from the new cell
            carveMaze(newX, newY);
        }
    }
}

// Function to add an item to the inventory
void addItem(const std::string& itemName, int quantity) {
    bool found = false;
    for (auto& item : inventory) {
        if (item.name == itemName) {
            item.quantity += quantity;
            found = true;
            break;
        }
    }
    if (!found) {
        inventory.push_back({itemName, quantity});
    }
}

// Function to display the inventory
void displayInventory() {
    std::cout << "Inventory:\n";
    for (const auto& item : inventory) {
        std::cout << item.name << " x" << item.quantity << "\n";
    }
}

// Function to ask the player a puzzle question
void puzzle() {
    int answer;
    std::cout << "Solve this puzzle to proceed: 5 + 3 = ";
    std::cin >> answer;
    if (answer == 8) {
        std::cout << "Correct! You may proceed.\n";
    } else {
        std::cout << "Incorrect, try again.\n";
    }
}

// Function to show the main menu
void showMenu() {
    std::cout << "1. Start Game\n";
    std::cout << "2. Instructions\n";
    std::cout << "3. Quit\n";
}

// Main game function
int main() {
    srand(time(0)); // Seed the random number generator

    // Display menu and get user choice
    int choice;
    showMenu();
    std::cin >> choice;

    if (choice == 1) {
        // Start the game
        initializeMaze();
        carveMaze(1, 1); // Start carving from position (1, 1)

        // Add a resource (example: Key)
        addItem("Key", 1);
        displayInventory();

        // Create an enemy
        Enemy enemy = {5, 5}; // Initial position of enemy
        maze[enemy.y][enemy.x] = 'E'; // Represent enemy with 'E'

        // Game loop
        while (true) {
            // Clear the old positions (before updating the maze)
            maze[enemy.y][enemy.x] = ' '; // Remove the enemy from the previous position
            maze[playerY][playerX] = ' '; // Remove the player from the previous position

            // Move the enemy randomly
            enemy.moveRandomly();

            // Place the player and enemy in the maze again
            maze[enemy.y][enemy.x] = 'E'; // Update enemy position
            maze[playerY][playerX] = 'P'; // Update player position

            // Display the maze with player and enemy
            displayMaze();

            // Ask player to move
            char direction;
            std::cout << "Move (WASD): ";
            std::cin >> direction;

            // Move player based on direction input
            if (direction == 'W' && maze[playerY - 1][playerX] != '#') playerY--;
            else if (direction == 'S' && maze[playerY + 1][playerX] != '#') playerY++;
            else if (direction == 'A' && maze[playerY][playerX - 1] != '#') playerX--;
            else if (direction == 'D' && maze[playerY][playerX + 1] != '#') playerX++;

            // Check for interaction (e.g., puzzle)
            if (playerX == 2 && playerY == 2) {
                std::cout << "You've reached a puzzle area!\n";
                puzzle();
            }

            // Display inventory after player move
            displayInventory();

            // Update the maze again to ensure the player and enemy are in the correct spots
            maze[enemy.y][enemy.x] = 'E'; // Update enemy position again (for the next loop)
            maze[playerY][playerX] = 'P'; // Update player position again (for the next loop)
        }

    } else if (choice == 2) {
        // Display instructions
        std::cout << "Use WASD to move. Avoid enemies and solve puzzles!\n";
    } else if (choice == 3) {
        // Quit the game
        std::cout << "Exiting the game...\n";
    }

    return 0;
}
