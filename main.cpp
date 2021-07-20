/********************************
 * Author: Braeden Kilburn
 * Date Created: 
 * Description: Play hangman from
 * a file with many different words
 * for the user to guess!
 * 
 * Status: Complete
 *******************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <limits>

using namespace std;

const int MAX_TRIES = 6;

void welcomePlayer();
void printGuessedCharacter(char, string, vector<char> &, bool);
void startNewGame(vector<char> &, int &, string &, int &, string &, vector<char> &);

int main()
{
    // Print welcome screen
    welcomePlayer();

    // Open and read the words from the file to a vector
    ifstream fin;
    fin.open("wordBank.txt");

    if (!fin.is_open())
    {
        cout << "There was an error opening your word bank!" << endl;
        return -1;
    }

    // Input each word into a vector called wordBank and keep track of how many words have been input
    unsigned int numberOfWords{0};
    vector<string> wordBank;
    string newWord;

    while (fin >> newWord)
    {
        wordBank.push_back(newWord);
        numberOfWords++;
    }

    // Keep track if player won
    bool playerWon = false;
    bool playAgain = true;

    // Keeps track of player's guesses so they don't guess the same character twice
    vector<char> historyOfGuesses;

    do
    {
        // Start new game
        // While loop controlled by a counter of incorrect guesses which cannot exceed the allotted incorrect guesses
        int incorrectGuesses{0};
        string playerGuess;

        // Randomly select a word using random number index
        srand(time(NULL));
        int randomWordIndex = rand() % numberOfWords;

        string answer = wordBank.at(randomWordIndex);

        // Set up the user to play
        cout << "The answer is " << answer.length() << " characters long." << endl;

        // Declare an array that acts as the hidden answer
        vector<char> answerLetters;

        // Initialize all characters to be "hidden" as underscores
        for (unsigned int i = 0; i < answer.length(); i++)
        {
            answerLetters.push_back('_');
            cout << answerLetters[i];
        }

        cout << endl;

        while (incorrectGuesses != MAX_TRIES)
        {
            // Check that the user hasn't guessed all the characters correctly already
            // Checks each letter in the hidden answer array to see if there's still underscores left to solve
            // If it checks all the characters and finds no more underscores, the word is solved and the game is over
            for (unsigned int i = 0; i < answer.length(); i++)
            {
                if (answerLetters[i] == '_')
                    break;
                else if (i == answer.length() - 1 && answerLetters[i] != '_')
                {
                    playerWon = true;
                    break;
                }
            }

            // Exit the while loop if the player has won already
            // Changed to true if they guessed all the letters or solved the word
            if (playerWon)
                break;

            // Prompt users how many guesses they have left
            cout << "\nYou have " << MAX_TRIES - incorrectGuesses << " guesses remaining!" << endl;

            // Prompt the user to guess a letter
            cout << "\nEnter a single character (or guess the word): ";
            cin >> playerGuess;

            // For a single letter guess, we'll take the first letter of the string
            char character = playerGuess[0];

            // Check the guess is in the alphabet
            while (!((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z')))
            {
                cout << "\nInvalid character! Try again.\n"
                     << endl;
                cout << "Enter a single character (or guess the word): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> playerGuess;
                character = playerGuess[0];
            }

            // Change the player's guess to lowercase
            for (unsigned int i = 0; i < playerGuess.length(); i++)
            {
                playerGuess[i] = tolower(playerGuess[i]);
            }

            character = playerGuess[0];

            // If the player guessed a single character, we will check each letter in the string
            // to see if the letter exists in the string
            if (playerGuess.length() == 1)
            {
                // Check if the user already guessed this character
                bool repeatedGuess = false;
                historyOfGuesses.push_back(character);

                // For loop checks every character in the vector except for the last character
                // (the last character being the most recent guess, the one we want to check if
                // its already been guessed)
                for (unsigned int i = 0; i < historyOfGuesses.size() - 1; i++)
                {
                    if (character == historyOfGuesses[i])
                    {
                        repeatedGuess = true;
                        break;
                    }
                }

                // If they already guessed that character, prompt them to guess a new character and restart the while loop
                if (repeatedGuess)
                {
                    cout << "\nLooks like you already guessed " << character << ".\nTry a new character!" << endl;
                    printGuessedCharacter(character, answer, answerLetters, false);
                    continue;
                }

                // If it exists, replace the blank with the letter
                for (unsigned int i = 0; i < answer.length(); i++)
                {
                    if (character == answer[i])
                    {
                        // If the character's a vowel, we need to change our grammar accordingly
                        if (character == 'a' || character == 'e' || character == 'i' || character == 'o' || character == 'u')

                            cout << "\nYes, there's an " << character << " in the word!" << endl;
                        else
                            cout << "\nYes, there's a " << character << " in the word!" << endl;
                        printGuessedCharacter(character, answer, answerLetters, true);
                        break;
                    }
                    else if (i == answer.length() - 1 && character != answer[i])
                    {
                        cout << "\nThere's no " << playerGuess << " in the word!" << endl;
                        printGuessedCharacter(character, answer, answerLetters, false);
                        incorrectGuesses++;
                    }
                }
            }
            else
            {
                if (playerGuess == answer)
                {
                    cout << "\nCorrect! You win!" << endl;
                    playerWon = true;
                    break;
                }
                else
                {
                    cout << "\nIncorrect Guess, try again!" << endl;
                    // incorrectGuesses++; SHOULD WE PENALIZE FOR GUESSING THE WRONG WORD?
                }
            }
        }

        if (playerWon)
        {
            cout << "\nCongratulations! You guessed the word correctly!" << endl;
            cout << "The word was: " << answer << endl
                 << endl;
        }
        else
        {
            cout << "\nThat's unfortunate, looks like you lost!" << endl;
            cout << "The word was: " << answer << endl
                 << endl;
        }

        char playAgainChar;
        cout << "Would you like to play again? ('Y' or 'N'): ";
        cin >> playAgainChar;
        playAgainChar = tolower(playAgainChar);

        if (playAgainChar == 'n')
            playAgain = false;
        else
        {
            playerWon = false;
            playAgain = true;
            randomWordIndex = rand() % numberOfWords;
            answer = wordBank.at(randomWordIndex);
            historyOfGuesses.clear();
            answerLetters.clear();
        }

    } while (playAgain);

    // Close the word bank
    fin.close();

    return 0;
}

void welcomePlayer()
{
    cout << "Hangman" << endl;

    cout << "\nA word will be randomly selected and you must guess the letters that make up that word." << endl;
    cout << "You have six (6) attempts to guess the correct letters or solve the puzzle.\n"
         << endl;

    cout << "You will be penalized for guessing the same letter twice. Guessing the same letter twice" << endl;
    cout << "still counts as a new guess, so be careful!" << endl;
    cout << "Good luck, have fun!\n"
         << endl;
}

void printGuessedCharacter(char playerGuess, string answer, vector<char> &answerLetters, bool answeredCorrect)
{
    // If they answered correctly, then we will search the answer for that character and it's index to replace the underscore
    if (answeredCorrect)
    {
        // Get index for the character
        for (unsigned int index = 0; index < answer.length(); index++)
        {
            if (playerGuess == answer[index])
            {
                // Change the underscore to the character at that position
                answerLetters[index] = playerGuess;
            }
        }
    }

    // Print the array with the changes, if applicable
    for (unsigned int i = 0; i < answer.length(); i++)
    {
        cout << answerLetters[i];
    }
    cout << endl;
}