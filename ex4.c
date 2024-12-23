/******************
Name:
ID:
Assignment:
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cheerleaders human pyramid dimensions, e.g. 5 means 5x5 pyramid
#define CL_DIMENSIONS 5

/*
 Parentheses validation:
 This is not the first input char. Not using a boolean because we need the actual char on first scanned char
 Needed in order to support "3{" style input, when no whitespaces at all after "3"
*/
#define NOT_FIRST_CHAR '\0'

// Assuming max Queens Battle grid dimension is 20
#define MAX_QB_DIM 20

// Queens Battle solution board symbols
#define EMPTY_POS '*'
#define QUEEN_POS 'X'

// Crossword available slot directions
#define HORIZONTAL 'H'
#define VERTICAL 'V'

// Crossword given max sizes
#define MAX_SLOTS 100
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 15
#define MAX_CW_DIM 30

// Crossword constants
#define FIRST_LETTER 'A'
#define LAST_LETTER 'z'
#define NO_LETTER '#'
#define NO_AVAILABLE_WORD (-1)

// Crossword Slot
typedef struct {
    int row;
    int column;
    int length;
    char direction;
    int wordIndex;
} Slot;

// Crossword Word
typedef struct {
    char value[MAX_WORD_LENGTH];
    int used;
} Word;

// Crossword board Position
typedef struct {
    int row;
    int column;
} Position;

// Task Defined Functions

void robotPaths();
void humanPyramid();
void parenthesisValidator();
void queensBattle();
void crosswordGenerator();

// Task Logic

// Robot Paths
int calculatePathsCount(int column, int row);

// Human Pyramid
int isValidPColumn(int column, int row);
float calculateSupportedWeight(float weights[][CL_DIMENSIONS], int row, int column);

// Parenthesis Validator
char getCloserMatch(char opener);
int isOpenParenthesis(char c);
int isCloseParenthesis(char c);
int validateParenthesisBalance(char firstChar, char opener);

// Queens Battle
int isAdjacentToQueen(int queenRows[], int row, int column);
int isRowAvailable(int queenRows[], int row, int column, int index);
int isAvailablePosition(int queenRows[], int row, int column);
int isVisitedRegion(char visitedRegions[], char region, int visitedCount, int index);
int getNextAvailableRow(char regionsBoard[][MAX_QB_DIM], char visitedRegions[], int queenRows[],
    int dimension, int row, int column);
int solveQueenBattles(char regionsBoard[][MAX_QB_DIM], char visitedRegions[], int queenRows[],
    int dimension, int row, int column);

// Crossword Generator
Position getDirectedPosition(int row, int column, char direction, int index);
char getOppositeDirection(char direction);
int isPositionCaught(char position);
int isDirectedCaught(char board[][MAX_CW_DIM], int dimension, Slot slot, char word[], int index);
int isMatchSlot(char board[][MAX_CW_DIM], int dimension, Slot slot, Word word);
int getNextAvailableWordIndex(char board[][MAX_CW_DIM], int dimension, Slot slot, Word words[],
    int wordsCount, int wordIndex);
void insertWordToSlot(char board[][MAX_CW_DIM], Slot slot, Word word, int posIndex);
int hasAdjacentWord(char board[][MAX_CW_DIM], int dimension, int row, int column, char direction);
void removeWordFromBoard(char board[][MAX_CW_DIM], int dimension, Slot slot, int posIndex);
int solveCrossword(char board[][MAX_CW_DIM], int dimension, Slot slotsDetails[], int slotsCount, int slotsIndex,
    Word words[], int wordsCount, int wordIndex);

int main() {
    int task = -1;
    do {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");

        if (scanf("%d", &task)) {
            switch (task) {
                case 6:
                    printf("Goodbye!\n");
                    break;
                case 1:
                    robotPaths();
                    break;
                case 2:
                    humanPyramid();
                    break;
                case 3:
                    parenthesisValidator();
                    break;
                case 4:
                    queensBattle();
                    break;
                case 5:
                    crosswordGenerator();
                    break;
                default:
                    printf("Please choose a task number from the list.\n");
                    break;
            }
        }
        else {
            // Cleans the buffer for invalid input type (e.g. string)
            scanf("%*s");
        }
    } while (task != 6);
}

/*
 Calculates valid paths for the robot to get home (0, 0), Only left (column - 1) and down (row - 1) allowed
*/
int calculatePathsCount(int column, int row) {
    // Base case: Reached invalid column or invalid row values, path is not counted
    if (column < 0 || row < 0) return 0;
    // Base case: Reached robot's home, count path as a valid path
    if (column == 0 && row == 0) return 1;

    // Robot is at the most left column, so he must move down
    if (column == 0) return calculatePathsCount(column, row - 1);
    // Robot is at the most bottom row, so he must move left
    if (row == 0) return calculatePathsCount(column - 1, row);
    // Robot can move both left or down, count each of them as a valid move
    return calculatePathsCount(column, row - 1) + calculatePathsCount(column - 1, row);
}

void robotPaths() {
    printf("Please enter the coordinates of the robot (column, row):\n");
    int column, row;
    scanf("%d %d", &column, &row);
    int pathsCount = calculatePathsCount(column, row);
    printf("The total number of paths the robot can take to reach home is: %d\n", pathsCount);
}

/*
 Validates given column is valid in the context of Pyramid
*/
int isValidPColumn(int column, int row) {
    return column <= row;
}

/*
 Calculates a given cheerleader at position (row, column) supported weight by the following formula:
    cheerleader weight = her own weight + .5 * (the total weight of the two cheerleaders above her)
*/
float calculateSupportedWeight(float weights[][CL_DIMENSIONS], int row, int column) {
    // Base case: Reached invalid column or invalid row values, no cheerleader weight to return
    if (column < 0 || row < 0 || !isValidPColumn(column, row)) return 0;

    // Calculates the supported weight of the left parent of current cheerleader
    float leftParentWeight = calculateSupportedWeight(weights, row - 1, column - 1);
    // Calculates the supported weight of the right parent of current cheerleader
    float rightParentWeight = calculateSupportedWeight(weights, row - 1, column);
    // Calculates current cheerleader supported weight by given formula
    return (float) .5 * (leftParentWeight + rightParentWeight) + weights[row][column];
}

void humanPyramid() {
    printf("Please enter the weights of the cheerleaders:\n");

    // Scan weights pyramid inputs
    float weights[CL_DIMENSIONS][CL_DIMENSIONS];
    for (int row = 0; row < CL_DIMENSIONS; row++) {
        for (int column = 0; isValidPColumn(column, row); column++) {
            scanf("%f", &weights[row][column]);

            // Validate cheerleader weight is not a negative number
            if (weights[row][column] < 0) {
                printf("Negative weights are not supported.\n");
                return;
            }
        }
    }

    // Calculate and print each cheerleader supported weight
    printf("The total weight on each cheerleader is:\n");
    for (int row = 0; row < CL_DIMENSIONS; row++) {
        for (int column = 0; isValidPColumn(column, row); column++) {
            printf("%.2f ", calculateSupportedWeight(weights, row, column));
        }
        printf("\n");
    }
}

/*
 Returns the closer parenthesis match of given opener parenthesis, or '\0' if it's not a valid opener
*/
char getCloserMatch(char opener) {
    switch (opener) {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        case '<':
            return '>';
        default:
            return '\0';
    }
}

/*
 Returns whether given char is a valid opener parenthesis
*/
int isOpenParenthesis(char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

/*
 Returns whether given char is a valid closer parenthesis
*/
int isCloseParenthesis(char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
}

/*
 Gets:
    - firstChar: scanned before calling recursion. The recursion always calls itself with firstChar = NO_FIRST_CHAR
    - opener: current opener parenthesis to validate a match closer
 Reading unlimited chars from stdin until user enters '\n' char.
 Validates given chars compose a balanced parentheses string.
 Returns 1 for a balanced string, and 0 otherwise.
*/
int validateParenthesisBalance(char firstChar, char opener) {
    char currentChar = firstChar;
    if (firstChar == NOT_FIRST_CHAR) {
        scanf("%c", &currentChar);
    }

    // Base case: reached '\n' - no more chars to read
    if (currentChar == '\n') {
        // Validates no more openers to close
        return opener == '\0';
    }

    /*
     Reached an opener parenthesis. Validates in that order:
        - currentChar (= new opener) has a closer match
        - "opener" parameter has a closer match
    */
    if (isOpenParenthesis(currentChar))
        return validateParenthesisBalance(NOT_FIRST_CHAR, currentChar) &&
            validateParenthesisBalance(NOT_FIRST_CHAR, opener);

    // Reached a closer parenthesis. Validates "opener" parameter matches current closer
    if (isCloseParenthesis(currentChar)) {
        if (currentChar == getCloserMatch(opener)) return 1;
        /*
         Reaching a wrong closer means no balance, so we might return 0 before input string is fully read.
         We might have extra chars in input before '\n', so we need to clean the buffer.
         This is a scan technique to read all chars until '\n' and then read it too.
        */
        scanf("%*[^\n] ");
        return 0;
    }

    // Reached a not parenthesis char, moves on to the next char
    return validateParenthesisBalance(NOT_FIRST_CHAR, opener);
}

void parenthesisValidator() {
    printf("Please enter a term for validation:\n");
    // Scans firstChar outside of recursion in order to ignore spaces and new lines
    char firstChar;
    scanf(" %c", &firstChar);

    // Prints according to task logic
    if (validateParenthesisBalance(firstChar, '\0'))
        printf("The parentheses are balanced correctly.\n");
    else {
        printf("The parentheses are not balanced correctly.\n");
    }
}

/*
 Return true if there is a queen in the previous column adjacent rows and false otherwise
*/
int isAdjacentToQueen(int queenRows[], int row, int column) {
    if (column == 0) return 0;
    int prevQueenRow = queenRows[column - 1];
    // Validate all adjacent queen rules
    return prevQueenRow == row - 1 || prevQueenRow == row || prevQueenRow == row + 1;
}

/*
 Recursion until a queen is found in row or until no queen is found up to column
*/
int isRowAvailable(int queenRows[], int row, int column, int index) {
    // Base case: reached current column => no other queen in row
    if (index == column) return 1;
    // Base case: found a queen in given row
    if (queenRows[index] == row) return 0;
    return isRowAvailable(queenRows, row, column, index + 1);
}

/*
 Return whether position is available based on queens positions in board
*/
int isAvailablePosition(int queenRows[], int row, int column) {
    return isRowAvailable(queenRows, row, column, 0) && !isAdjacentToQueen(queenRows, row, column);
}

/*
 Return whether given region is already visited.
*/
int isVisitedRegion(char visitedRegions[], char region, int visitedCount, int index) {
    // Base case: region doesn't match any of visitedRegions, add it to visitedRegions
    if (index == visitedCount) {
        visitedRegions[index] = region;
        return 0;
    }

    // Base case: region is found in visitedRegions
    if (visitedRegions[index] == region) return 1;
    // Check if next visited region is the given region
    return isVisitedRegion(visitedRegions, region, visitedCount, index + 1);
}

/*
 Determines next available row in column.
 if there is such a row, return it. otherwise, return dimension as row value (= no available row)
*/
int getNextAvailableRow(char regionsBoard[][MAX_QB_DIM], char visitedRegions[], int queenRows[],
    int dimension, int row, int column) {

    // Base case: no available row in column
    if (row == dimension)
        return dimension;

    /*
     Determines whether (row, column) is a valid queen position.
     Regions are checked until current column, any visited region after previous column is irrelevant
    */
    if (isAvailablePosition(queenRows, row, column) &&
        !isVisitedRegion(visitedRegions, regionsBoard[row][column], column, 0))
        return row;

    // Check if next row is available in current column
    return getNextAvailableRow(regionsBoard, visitedRegions, queenRows, dimension, row + 1, column);
}

/*
 Solve queens battle. Start at 0, 0 and move according to the rules:
 - if (row, column) is a valid queen row, put a queen there and move on to next column to find a valid row
 - if no row is valid in column, backtrack to previous column until a valid position is found
*/
int solveQueenBattles(char regionsBoard[][MAX_QB_DIM], char visitedRegions[], int queenRows[],
    int dimension, int row, int column) {

    // Base case: already put queen at last column, no more columns need a queen -> found a solution
    if (column == dimension) return 1;

    int availableRow = getNextAvailableRow(regionsBoard, visitedRegions, queenRows, dimension, row, column);
    // Base case: no available row left in current column -> no solution with current queens
    if (availableRow == dimension) return 0;

    // Put a queen in (availableRow, column)
    queenRows[column] = availableRow;
    // Solve next column with current queens state. if a solution found, return 1
    if (solveQueenBattles(regionsBoard, visitedRegions, queenRows, dimension, 0, column + 1)) {
        return 1;
    }

    // No solution found with current queens state. try next available row in current column
    return solveQueenBattles(regionsBoard, visitedRegions, queenRows, dimension, availableRow + 1, column);
}

void queensBattle() {
    printf("Please enter the board dimensions:\n");
    int dimension;
    scanf("%d", &dimension);
    printf("Please enter a %d*%d puzzle board:\n", dimension, dimension);

    char regionsBoard[MAX_QB_DIM][MAX_QB_DIM];
    for (int row = 0; row < dimension; row++) {
        for (int column = 0; column < dimension; column++) {
            scanf(" %c", &regionsBoard[row][column]);
        }
    }

    // Solution queens rows, indexed by column. e.g. queens[1] == 2: queen at column 1, row 2
    int queenRows[MAX_QB_DIM];
    // Keeps visited regions during recursion to prevent two queens on the same region
    char visitedRegions[MAX_QB_DIM];
    // Try to find a solution for given regionsBoard and given dimension
    if (dimension != 0 && !solveQueenBattles(regionsBoard, visitedRegions, queenRows, dimension, 0, 0)) {
        printf("This puzzle cannot be solved.\n");
        return;
    }

    // Found a valid solution, print according to task format
    printf("Solution:\n");
    for (int row = 0; row < dimension; row++) {
        for (int column = 0; column < dimension; column++) {
            char value = queenRows[column] == row ? QUEEN_POS : EMPTY_POS;
            printf("%c ", value);
        }
        printf("\n");
    }
}

/*
 Returns calculated Position based on given row and column: moves index times in given direction
*/
Position getDirectedPosition(int row, int column, char direction, int index) {
    Position pos;
    pos.row = direction == HORIZONTAL ? row : row + index;
    pos.column = direction == HORIZONTAL ? column + index : column;
    return pos;
}

/*
 Returns V if given direction is H, and returns H otherwise
*/
char getOppositeDirection(char direction) {
    return direction == HORIZONTAL ? VERTICAL : HORIZONTAL;
}

/*
 Returns whether given position value is a crossword letter, which means its caught in board
*/
int isPositionCaught(char value) {
    return value >= FIRST_LETTER && value <= LAST_LETTER;
}

/*
 Returns whether given slot is caught with a char which doesn't match word's chars
*/
int isDirectedCaught(char board[][MAX_CW_DIM], int dimension, Slot slot, char word[], int index) {
    // Calculates current position value in board
    Position pos = getDirectedPosition(slot.row, slot.column, slot.direction, index);
    // Base case: Validated last slot position => Word matches Slot
    if (index == slot.length) return 0;

    char posValue = board[pos.row][pos.column];
    // Base case: Found a char that doesn't match word's current char
    if (isPositionCaught(posValue) && word[index] != posValue) return 1;

    // Validates match between next char in slot and in word
    return isDirectedCaught(board, dimension, slot, word, index + 1);
}

/*
 Returns whether given Word can be placed in given Slot
*/
int isMatchSlot(char board[][MAX_CW_DIM], int dimension, Slot slot, Word word) {
    // If word length isn't equal slot length => word isn't a match for slot
    if ((int) strlen(word.value) != slot.length) return 0;
    // Validates match with current board state
    return !isDirectedCaught(board, dimension, slot, word.value, 0);
}

/*
 Returns next available word index from given words array, or NO_AVAILABLE_WORD if no word is a match to slot
*/
int getNextAvailableWordIndex(char board[][MAX_CW_DIM], int dimension, Slot slot, Word words[],
    int wordsCount, int wordIndex) {
    // Base case: no word match found to given slot
    if (wordIndex == wordsCount) return NO_AVAILABLE_WORD;
    // Validates word is not used and word matches slot
    if (words[wordIndex].used != 1 && isMatchSlot(board, dimension, slot, words[wordIndex])) return wordIndex;
    // Recursion call to validate next word index in words array
    return getNextAvailableWordIndex(board, dimension, slot, words, wordsCount, wordIndex + 1);
}

/*
 Inserts given word to given slot in board
*/
void insertWordToSlot(char board[][MAX_CW_DIM], Slot slot, Word word, int posIndex) {
    // Base case: filled all slot cells with word chars
    if (posIndex == slot.length) return;

    // Fills next slot position with word's char at posIndex
    Position pos = getDirectedPosition(slot.row, slot.column, slot.direction, posIndex);
    board[pos.row][pos.column] = word.value[posIndex];

    // Inserts next word's char to board
    insertWordToSlot(board, slot, word, posIndex + 1);
}

/*
 Returns whether given (row, column) at board has an adjacent word in the opposite direction of given direction
*/
int hasAdjacentWord(char board[][MAX_CW_DIM], int dimension, int row, int column, char direction) {
    // Gets direction to validate
    char adjacentsDirection = getOppositeDirection(direction);
    // Next position: Move (row, column) 1 step forward
    Position next = getDirectedPosition(row, column, adjacentsDirection, 1);
    // Previous position: Move (row, column) 1 step backward
    Position previous = getDirectedPosition(row, column, adjacentsDirection, -1);

    // Returns whether (next position is valid and caught) or (previous position is valid and caught)
    return
        (next.row < dimension && next.column < dimension && isPositionCaught(board[next.row][next.column])) ||
        (previous.row > 0 && previous.column > 0 && isPositionCaught(board[previous.row][previous.column]));
}

/*
 Removes all chars that aren't a part of other words from given slot in board
*/
void removeWordFromBoard(char board[][MAX_CW_DIM], int dimension, Slot slot, int posIndex) {
    // Base case: traversed all chars in slot
    if (posIndex == slot.length) return;

    // Get next position in slot
    Position pos = getDirectedPosition(slot.row, slot.column, slot.direction, posIndex);
    // If board at pos is not a part of adjacent word, remove it
    if (!hasAdjacentWord(board, dimension, pos.row, pos.column, slot.direction))
        board[pos.row][pos.column] = NO_LETTER;
    // Remove next char from slot
    removeWordFromBoard(board, dimension, slot, posIndex + 1);
}

/*
 Solves crossword puzzle and generates the solved crossword based on given input.
 If crossword is solvable, board will be filled according to input and 1 ("True") is returned.
 If crossword is not solvable, returns 0 ("False").
*/
int solveCrossword(char board[][MAX_CW_DIM], int dimension, Slot slotsDetails[], int slotsCount, int slotsIndex,
    Word words[], int wordsCount, int wordIndex) {

    // Base case: placed words at all slots correctly
    if (slotsIndex == slotsCount) return 1;

    // Gets next available word index that matches current slot
    int availableWordIndex = getNextAvailableWordIndex(board, dimension, slotsDetails[slotsIndex],
        words, wordsCount, wordIndex);

    // Base case: no match found for slot based, backtracks to previous slot and tries another word
    if (availableWordIndex == NO_AVAILABLE_WORD) return 0;

    // Inserts word to slot
    insertWordToSlot(board, slotsDetails[slotsIndex], words[availableWordIndex], 0);
    words[availableWordIndex].used = 1; // Mark word as "used": don't use it again in another slot

    // Tries to solve next slot based on current word-slot match
    if (solveCrossword(board, dimension, slotsDetails, slotsCount, slotsIndex + 1,
        words, wordsCount, 0)) {
        return 1;
    }

    // Match leads to NO_AVAILABLE_WORD at a later slot, remove it and try another word for current slot
    removeWordFromBoard(board, dimension, slotsDetails[slotsIndex], 0);
    words[availableWordIndex].used = 0; // Remove "used" mark: you can use it in any other slot

    // Solve with other available word that matches current slot
    return solveCrossword(board, dimension, slotsDetails, slotsCount, slotsIndex,
        words, wordsCount, availableWordIndex + 1);
}

void crosswordGenerator() {
    // Crossword dimensions (squared)
    printf("Please enter the dimensions of the crossword grid:\n");
    int dimension;
    scanf("%d", &dimension);

    // Number of slots in crossword
    printf("Please enter the number of slots in the crossword:\n");
    int slotsCount;
    scanf("%d", &slotsCount);

    // Slots details input
    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
    Slot slotsDetails[MAX_SLOTS];
    for (int slot = 0; slot < slotsCount; slot++) {
        scanf("%d", &slotsDetails[slot].row);
        scanf("%d", &slotsDetails[slot].column);
        scanf("%d", &slotsDetails[slot].length);
        scanf(" %c", &slotsDetails[slot].direction);
    }

    // Input word count and validate there are at least (slotsCount) number of words
    printf("Please enter the number of words in the dictionary:\n");
    int wordsCount;
    scanf("%d", &wordsCount);
    // Validate wordCount is at least slotsCount
    while (wordsCount < slotsCount) {
        printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", slotsCount);
        scanf("%d", &wordsCount);
    }

    // Input words values
    printf("Please enter the words for the dictionary:\n");
    Word words[MAX_WORDS];
    for (int word = 0; word < wordsCount; word++) {
        scanf(" %s", words[word].value);
        // Mark each word initial "used" value as 0 ("False")
        words[word].used = 0;
    }

    // Declaration of solution crossword board
    char board[MAX_CW_DIM][MAX_CW_DIM] = {0};

    // Solve crossword with given input. start at slot #0 and word #0
    if (!solveCrossword(board, dimension, slotsDetails, slotsCount, 0, words, wordsCount, 0)) {
        // No solution found
        printf("This crossword cannot be solved.\n");
        return;
    }

    // Found a solution, print it in given format
    for (int row = 0; row < dimension; row++) {
        for (int column = 0; column < dimension; column++) {
            char value = isPositionCaught(board[row][column]) ? board[row][column] : NO_LETTER;
            printf("| %c ", value);
        }
        printf("|\n");
    }
}
