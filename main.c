#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdbool.h>

///validString takes in a 2D array of words, an integer input representing the length of words, the number of words, and two character arrays for the starting and ending words.
///It will ask the user to enter two words and checks if the words first are of the same length as the integer passed in, and second if they are valid dictionary words.
///If the words are valid, the function will set the startFound and endFound flags to true and return.
///If the words are not valid, it will ask the user to enter new words until valid words are found.
void validString(char ***words, int *input, int *num_words, char startWord[20], char endWord[20]) {
    bool startFound = false;
    bool endFound = false;
    bool wordsFound = false;
    while (wordsFound == false) {
        printf("Enter starting and ending words, or 'r' for either for a random word: ");
        scanf("%s %s", startWord, endWord);
        if (strcmp(startWord, "r") == 0) {
            strcpy(startWord, (*words)[rand() % (*num_words)]);
        }
        if (strcmp(endWord, "r") == 0) {
            strcpy(endWord, (*words)[rand() % (*num_words)]);
        }
        if (strlen(startWord) != *input) { //Check to see if input is correct length
            printf("Your word, '%s', is not a %d-letter word. Try again.\n", startWord, *input);
            continue;
        } else if (strlen(endWord) != *input) { //Check to see if input is correct length
            printf("Your word, '%s', is not a %d-letter word. Try again.\n", endWord, *input);
            continue;
        }
        for (int i = 0; i < *num_words; i++) {
            if (strcmp(startWord, (*words)[i]) == 0) {
                startFound = true;
            }
        }
        if (startFound == false) {
            printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startWord);
            startFound = false;
            endFound = false;
            continue;
        }
        for (int i = 0; i < *num_words; i++) {
            if (strcmp(endWord, (*words)[i]) == 0) {
                endFound = true; //Checks if end word is valid
            }
        }
        if (endFound == false) {
            printf("Your word, '%s', is not a valid dictionary word. Try again.\n", endWord);
            startFound = false;
            endFound = false;
            continue;
        }
        if (startFound == true && endFound == true) {
            wordsFound = true;
            return;
        }
    }
}

///playGame plays the game where the user is asked to enter a word of a given length that is one letter different from the previous word.
///It takes in as parameters a 3D array of words, a pointer to the number of words in the array,
///the starting word, the ending word, a pointer to the length of the words, and a boolean pointer to indicate whether the game has been quit.
///It first checks if the user entered "q" to quit, and if so, sets the quitGame boolean to true and breaks the loop and ends the game.
///If not, it checks if the input word is the correct length, then checks if it is found in the array of words.
///If it is not found, it prints an error and continues the loop.
///If it is found, it checks if the two words are only one character different, and if not, prints an error and continues the loop.
///If the two words are one character different, it tallies up the number of tries and increments the counter.
///If the input word is the same as the endWord, the user has won the game and the loop is broken.
///If not, the current word is copied to the previousWord variable and the loop continues.
void playGame(char ***words, int *num_words, char startWord[20], char endWord[20], int *input, bool *quitGame) {
    bool endGame = false;
    bool inputWordFound = false;
    char inputWord[20];
    char previousWord[20];
    int counter = 1;
    int numberTries = 0;
    strcpy(previousWord, startWord);
    while (endGame == false) {
        printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", counter, previousWord, endWord);
        scanf("%s", inputWord);
        if (strcmp(inputWord, "q") == 0) {
            *quitGame = true;
            break;
        }
        if (strlen(inputWord) != *input) { //Check to see if input is correct length
            printf("Your word, '%s', is not a %d-letter word. Try again.\n", inputWord, *input);
            continue;
        }
        for (int i = 0; i < *num_words; i++) {
            if (strcmp((*words)[i], inputWord) == 0) {
                inputWordFound = true; //Checks if input word is found
            }
        }
        if (inputWordFound == false) {
            printf("Your word, '%s', is not a valid dictionary word. Try again.\n", inputWord);
            continue;
        } else {
            inputWordFound = false;
        }
        int letterCount = 0;
        for (int i = 0; i < strlen(inputWord); i++) {
            if (inputWord[i] != previousWord[i])
                letterCount++;
        }
        if (letterCount != 1) {
            letterCount = 0;
            printf("Your word, '%s', is not exactly 1 character different. Try again.\n", inputWord);
            continue;
        }
        numberTries++;
        counter++;
        if (strcmp(inputWord, endWord) == 0) {
            printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", startWord, endWord, numberTries);
            break;
        }
        strcpy(previousWord, inputWord);
    }
}


///availableWords reads from "words.txt" and stores words of a certain inputted length into a char** words array.
///It takes in an int* for the number of letters to use and a char*** for the array of words.
///It then scans the file for words that match the length specified by the user.
///It will then dynamically allocate memory for these words and store them in the char***.
///Finally, it will store the number of words in the int* num_words and close the file.
void availableWords(int *input, int *num_words,char ***words) {
    scanf("%d", &(*input)); //Scans input number of letters to use
    *words = (char **) malloc(sizeof(char *));
    FILE *file = fopen("words.txt", "r");
    if (file == NULL) {
        printf("Error!");
        exit(1);
    }
    int i = 0;
    //read until end of file
    char inputString[81];
    while (fscanf(file, "%s", inputString) != EOF) {
        //if length of word matches length from user input
        if (strlen(inputString) == *input) {
            (*words)[i] = (char *) malloc(sizeof(char) * (strlen(inputString) + 1));
            strcpy((*words)[i], inputString);
            i++;
            //reallocate memory for words
            *words = (char **) realloc(*words, (i + 1) * sizeof(char *));
        }
    }
    *num_words = i;
    fclose(file); //close words.txt
}

///Main function for the game Weaver.
///It sets up and manages the game by prompting the user to enter the number of letters in the words,
///finding and validating the starting and ending words,
///playing the game, and allowing the user to choose between playing again,
///changing the number of letters in the words, or exiting the program.
///At the end of the main it will free memory of the words array or if option 2 is selected it will free the memory and then restart the loop
int main() {
    srand(1);
    int gameEnd = 0;
    bool option1 = false;
    bool option2 = false;
    int input;
    int *ptrToInput = &input;
    int num_words;
    int *num_words_ptr = &num_words;
    char **words;
    bool quitGame = false;
    bool *quitGamePtr = &quitGame;
    while (gameEnd == 0) { //Plays game until the user inputs end the game
        if (option1 == false) {//If menu options 1 or 2 are not called
            printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
            printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
            printf("Enjoy!\n\n");
            printf("How many letters do you want to have in the words? ");
            availableWords(ptrToInput, num_words_ptr, &words); //Calls the available words function to create the dynamic array
            printf("Number of %d-letter words found: %d.\n\n", input, num_words);
        }
        if (option2 == true) {//If menu option 2 is called
            printf("How many letters do you want to have in the words? ");
            availableWords(ptrToInput, num_words_ptr, &words); //Calls the available words function to create the dynamic array
            printf("Number of %d-letter words found: %d.\n\n", input, num_words);
            option2 = false;
        }
        char startWord[20];
        char endWord[20];
        validString(&words, ptrToInput, num_words_ptr, startWord, endWord); //Calls validString to see whether or not start or end words are valid words to use
        printf("Your starting word is: %s.\n", startWord);
        printf("Your ending word is: %s.\n", endWord);
        printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
        printf("You may also type in 'q' to quit guessing.\n");
        playGame(&words, num_words_ptr, startWord, endWord, ptrToInput, quitGamePtr);//Calls playGame and plays the weaver game
        int menuNumber;
        printf("\nEnter: \t1 to play again,\n");
        printf("\t2 to change the number of letters in the words and then play again, or\n");
        printf("\t3 to exit the program.\n");
        printf("Your choice --> ");
        scanf("%d", &menuNumber); //Takes in menu number input
        if (menuNumber == 1) {//Executes menu option 1
            option1 = true;
            continue;
        }
        if (menuNumber == 2) {//Executes menu option 2
            for (int j = 0; j < num_words; j++) {
                free(words[j]);
            }
            free(words);
            option2 = true;
            option1 = true;
            continue;
        }
        if (menuNumber == 3) {//Executes menu option 3
            printf("\nThanks for playing!\n" "Exiting...");
            gameEnd = 1;
            break;
        }
    }
    for (int i = 0; i < num_words; i++){
        free(words[i]);
    }
    free(words);
    return 0;
}
