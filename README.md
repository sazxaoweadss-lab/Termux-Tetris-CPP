# Termux-Tetris-Portable

A lightweight and high-performance Tetris engine written in C++, specifically optimized for the **Termux** environment on Android. This project is designed for mobile users, featuring responsive controls and a unique hidden cheat system.

### 🚀 Key Features
* **7-Bag Randomization System**: Implements advanced spawning logic to ensure a fair distribution of shapes and prevent the same block from appearing too many times in a row.
* **Non-Blocking Input**: Uses low-level terminal I/O (`termios` and `fcntl`) to handle real-time key presses without freezing the game loop.
* **Native Termux Controls**: Fully compatible with the virtual arrow keys found in the Termux interface.
* **Combo-Based Cheat System**: Includes a hidden "Combo" mode activated by the `C` key, allowing players to trigger special abilities using arrow-key sequences.
* **Flicker-Free Rendering**: Utilizes ANSI escape sequences for smooth screen updates and vibrant terminal graphics.

### 🛠 How to Use
1. **Compile the code**:
   `g++ tetris.cpp -o tetris -lpthread`
2. **Run the game**:
   `./tetris`

### 🎮 Controls
* **Up (↑)**: Rotate piece
* **Left / Right (← / →)**: Move piece
* **Down (↓)**: Speed up fall
* **Q**: Quit game

### ⚡ Cheat Codes (Combo System)
Press `C` to enter combo mode, then input:
1. **NUKER (Down, Down, Down, Up)**: Clears the bottom 5 rows of the grid.
2. **SLOW_MO (Left, Left, Right, Right)**: Reduces game speed significantly.
3. **BACK_TO_REALITY (Up, Right, Down, Left)**: Resets speed back to normal.

---
**Technical Note**: Developed as a portable solution for mobile developers and students who want to enjoy classic gaming inside a terminal emulator.
