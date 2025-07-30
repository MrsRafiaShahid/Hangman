#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cctype>
std::vector<std::string> loadWords(const std::string& filename);
void drawHangman(int triesLeft);
void playGame(const std::string& secretWord, int initialTries);
void displayGameState(int triesLeft, const std::string& guessedWord, const std::string& incorrectGuesses);
char getPlayerGuess(const std::string& allGuessedLetters);
void processPlayerGuess(char playerGuess, const std::string& secretWord, std::string& guessedWord, int& triesLeft, std::string& incorrectGuesses);


// ... main function follows ...
int main() {
    std::cout << "Welcome to the Hangman Game!" << std::endl;

     std::mt19937 engine(static_cast<unsigned int>(std::time(0))); 
    const std::string word_filename = "words.txt";
    std::vector<std::string> wordList = loadWords(word_filename);
    if (wordList.empty()) {
        std::cerr << "No words were loaded. The game cannot start." << std::endl;
        return 1; // Return a non-zero value to indicate an error.
    }
char playAgain; // This variable will store the user's choice ('y' or 'n').

 do {
        std::cout << "\nChoose a Difficulty Level:" << std::endl;
        std::cout << "1. Easy" << std::endl;
        std::cout << "2. Medium" << std::endl;
        std::cout << "3. Hard" << std::endl;
        std::cout << "Enter your choice (1-3): ";
         int difficultyChoice;
        
        std::cin >> difficultyChoice;
         int startingTries;
        std::vector<std::string> filteredWords;
        // Use a switch statement to translate the user's choice into a game setting.
        switch (difficultyChoice) {
            case 1: // If the user entered 1
                startingTries = 8; // Set for Easy mode
                for (const std::string& word : wordList) {
                    // If a word's length is 5 or less, it's an "Easy" word.
                    if (word.length() <= 5) {
                        // Add the matching word to our temporary list.
                        filteredWords.push_back(word);
                    }
                }
                break; // Exit the switch block
            
            case 2: // If the user entered 2
                startingTries = 6; // Set for Medium mode
                 for (const std::string& word : wordList) {
                    // If a word's length is between 6 and 8, it's "Medium".
                    if (word.length() >= 6 && word.length() <= 8) {
                        filteredWords.push_back(word);
                    }
                }
                break; // Exit the switch block
            
            case 3: // If the user entered 3
                startingTries = 4; // Set for Hard mode
                 for (const std::string& word : wordList) {
                    // If a word's length is greater than 8, it's "Hard".
                    if (word.length() > 8) {
                        filteredWords.push_back(word);
                    }
                }
                break; // Exit the switch block
            
            default: // If the user entered anything else
                 std::cout << "Invalid choice. Defaulting to Medium." << std::endl;
                startingTries = 6;
                // --- NEW: FILTERING LOGIC FOR DEFAULT ---
                // If the choice is invalid, we can default to Medium words.
                for (const std::string& word : wordList) {
                    if (word.length() >= 6 && word.length() <= 8) {
                        filteredWords.push_back(word);
                    }
                }
                break; // Exit the switch block
        }
      const std::vector<std::string>& wordsToUse = filteredWords.empty() ? wordList : filteredWords;
        //    Create the distribution for the chosen list's size.
            std::uniform_int_distribution<int> distribution(0, static_cast<int>(wordsToUse.size() - 1));
            // Select the secret word for this round.
        const std::string secretWord = wordsToUse[distribution(engine)];

          playGame(secretWord, startingTries);

        // Asks the user if they want to play again.
        std::cout << "\nPlay again? (y/n): ";

        // Reads the user's answer into the `playAgain` variable.
        std::cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y'); 
    std::cout << "\nThanks for playing! Come back soon!" << std::endl;

  
    return 0;
}


std::vector<std::string> loadWords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file '" << filename << "'. Please make sure it exists." << std::endl;
        return {};
    }

    std::vector<std::string> words;
    std::string currentWord;

    while (std::getline(file, currentWord)) {
            for (char& c : currentWord) {
        c = std::tolower(c);
    }
        words.push_back(currentWord);
    }

    return words;
}
void drawHangman(int triesLeft) {
    switch (triesLeft) {
        case 6:
            std::cout << R"(
   +---+
   |   |
       |
       |
       |
       |
 =========)" << std::endl;
            break;
        case 5:
            std::cout << R"(
   +---+
   |   |
   O   |
       |
       |
       |
 =========)" << std::endl;
            break;
        case 4:
            std::cout << R"(
   +---+
   |   |
   O   |
   |   |
       |
       |
 =========)" << std::endl;
            break;

        case 3:
            std::cout << R"(
   +---+
   |   |
   O   |
  /|   |
       |
       |
 =========)" << std::endl;
            break;
        case 2:
            std::cout << R"(
   +---+
   |   |
   O   |
  /|\  |
       |
       |
 =========)" << std::endl;
            break;
        case 1:
            std::cout << R"(
   +---+
   |   |
   O   |
  /|\  |
  /    |
       |
 =========)" << std::endl;
            break;
        case 0:
            std::cout << R"(
   +---+
   |   |
   O   |
  /|\  |
  / \  |
       |
 =========)" << std::endl;
            break;
        default:
            std::cout << "An error occurred in drawing the hangman." << std::endl;
            break;
    }
}
// Notice how this function now reads like a high-level overview of the game.
// It delegates all the detailed work to its helper functions.
void playGame(const std::string& secretWord, int initialTries){

    std::string guessedWord(secretWord.length(), '_');
     int triesLeft = initialTries; 
    std::string incorrectGuesses;
    std::string allGuessedLetters;

    // 2. The Main Game Loop for this round.
    while (triesLeft > 0 && guessedWord != secretWord) {
        // Delegate the task of displaying the game state.
        displayGameState(triesLeft, guessedWord, incorrectGuesses);
        
        // Delegate the task of getting a valid guess.
        char playerGuess = getPlayerGuess(allGuessedLetters);
        
        // Update our record of all letters guessed.
        allGuessedLetters += playerGuess;

        // Delegate the task of processing the guess and updating the game state.
        processPlayerGuess(playerGuess, secretWord, guessedWord, triesLeft, incorrectGuesses);
    }

    // 3. The "Game Over" phase.
    // We can REUSE our display function to show the final board state! No more duplicate code.
    displayGameState(triesLeft, guessedWord, incorrectGuesses);

    // Announce the final outcome of the round.
    if (guessedWord == secretWord) {
        std::cout << "\nCongratulations! You win!" << std::endl;
    } 
    else {
        std::cout << "\nYou lose! The hangman is complete." << std::endl;
        std::cout << "The word was: " << secretWord << std::endl;
    }
}
void displayGameState(int triesLeft, const std::string& guessedWord, const std::string& incorrectGuesses) {
    // Clear the screen
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // Draw the hangman figure
    drawHangman(triesLeft);
    
    // Display the word puzzle
    std::cout << "\nWord: ";
    for (char c : guessedWord) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
    
    // Display remaining tries and incorrect guesses
    std::cout << "Tries remaining: " << triesLeft << std::endl;
    if (!incorrectGuesses.empty()) {
        std::cout << "Incorrect Guesses: ";
        for (char c : incorrectGuesses) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
}

// This function's only job is to get a valid character input from the player.
// It will loop until a valid, new letter is entered.
char getPlayerGuess(const std::string& allGuessedLetters) {
    char playerGuess;
    while (true) { // Loop indefinitely until a valid input is returned.
        std::cout << "\nGuess a letter: ";
        std::cin >> playerGuess;

        if (!isalpha(playerGuess)) {
            std::cout << "Invalid input. Please enter a single letter." << std::endl;
        } else {
            playerGuess = tolower(playerGuess);
            if (allGuessedLetters.find(playerGuess) != std::string::npos) {
                std::cout << "You have already guessed that letter. Try another." << std::endl;
            } else {
                return playerGuess; // Valid, new letter. Exit the function.
            }
        }
    }
}

// This function's only job is to process the guess and update the state variables.
// Notice it takes several parameters by reference (`&`) so it can modify them.
void processPlayerGuess(char playerGuess, const std::string& secretWord, 
                        std::string& guessedWord, int& triesLeft, std::string& incorrectGuesses) {
    
    bool isGuessCorrect = false;
    for (size_t i = 0; i < secretWord.length(); ++i) {
        if (secretWord[i] == playerGuess) {
            guessedWord[i] = playerGuess;
            isGuessCorrect = true;
        }
    }
    
    if (!isGuessCorrect) {
        triesLeft--;
        incorrectGuesses += playerGuess;
    }
}