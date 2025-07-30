# 🎮 Hangman Console Game in C++

This is a classic **Hangman** word guessing game built in C++ for the terminal.  
The player attempts to guess a hidden word one letter at a time before running out of tries.

## 🚀 Features
- Loads words dynamically from a file (`words.txt`)
- Clean and simple terminal interface
- Hint functionality (reveals the first letter with a penalty)
- Validates input and avoids repeated guesses
- Tracks and displays remaining attempts

## 🕹️ How to Play
1. The word is hidden as underscores.
2. Guess one letter per turn.
3. Each incorrect guess reduces your available tries.
4. Use `hint` once per game to reveal the first letter (costs one try).
5. Win by guessing all letters correctly before running out of attempts!

## 🧠 Educational Concepts
This project demonstrates:
- File I/O in C++
- Loops and conditionals
- String manipulation
- Functions and input validation
- Error handling

## 📂 Setup
Make sure you have:
- A C++ compiler (e.g., `g++`)
- A `words.txt` file in the same folder, with one word per line

### Compile and Run
```bash
g++ main.cpp -o main
./main
