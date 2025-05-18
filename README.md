# Nine Men’s Morris (Console Version)

A terminal-based version of the classic board game **Nine Men’s Morris (Jeu de Moulin in French)**, developed in C using the Code::Blocks IDE.

---

## Game Overview

**Nine Men’s Morris** is a two-player strategy game that involves forming "mills" (three pieces in a row) to remove the opponent’s pieces. The console version implements:

Modes of the game:
- **Player vs Player** mode
- **Player vs AI** mode with two levels:
  - Easy (random moves)
  - Hard (Alpha-Beta Pruning)
---

## Project Structure

```
Nine Men’s Morris/
├── game_intro.txt              # ASCII art intro displayed at launch
├── JueDeMoulin.cbp             # Code::Blocks project file
├── include/                    # Header files
│   ├── algorithme.h
│   ├── board.h
│   ├── color.h
│   └── interface_menu.h
├── src/                        # C source files
│   ├── algorithme.c
│   ├── board.c
│   ├── color.c
│   ├── Final.c
│   ├── interface_menu.c
│   └── main.c
├── .gitignore                  
└── README.md                   
```

---

## How to Run

### On Windows (via Code::Blocks)
1. Open `JueDeMoulin.cbp` in Code::Blocks.
2. Press **Build & Run** (`F9`) to compile and launch the game.
3. Play directly in the terminal.

### On Linux (coming soon!)

---

## Features

- Text-based interface with ASCII art intro from `game_intro.txt`
- Handling the 3 game phases (placement, movement, flying)
- Clear prompts and color-coded output 
- Structured and modular codebase using header/source separation
- Turn switching and win condition detection
- AI uses Alpha-Beta pruning for "Hard" difficulty for smart moves

---

### Algorithms Used

- **Alpha-Beta Pruning**: Used in the "Hard" difficulty level. This is an optimized version of Minimax that skips unneeded branches in the decision tree, improving performance significantly.

- **Random Strategy**: In "Easy" mode, the computer places pieces randomly on available positions without any strategic evaluation.

---

## License

This project is made for educational purposes. Not licensed for commercial distribution.
