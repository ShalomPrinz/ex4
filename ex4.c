/******************
Name:
ID:
Assignment:
*******************/

#include <stdio.h>
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

#define HORIZONTAL 'H'
#define VERTICAL 'V'
#define MAX_SLOTS 100
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 15
#define MAX_CW_DIM 30

#define FIRST_ASCII_LETTER 'A'
#define LAST_ASCII_LETTER 'z'
#define NO_LETTER '#'
#define NO_AVAILABLE_WORD (-1)

typedef struct {
    int row;
    int column;
    int length;
    char direction;
    int wordIndex;
} Slot;

typedef struct {
    char value[MAX_WORD_LENGTH];
    int used;
} Word;

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
// TODO

// Crossword Generator
// TODO

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
    // Base case #1: Reached invalid column or invalid row values, path is not counted
    if (column < 0 || row < 0) return 0;
    // Base case #2: Reached robot's home, count path as a valid path
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
    return prevQueenRow == row - 1 || prevQueenRow == row || prevQueenRow == row + 1;
}

/*
 Recursion until a queen is found in row or until no queen is found up to column
*/
int isRowAvailable(int queenRows[], int row, int column, int index) {
    // Base case #1: reached current column => no other queen in row
    if (index == column) return 1;
    // Base case #2: found a queen in given row
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
 Return whether given region is already visited
*/
int isVisitedRegion(char visitedRegions[], char region, int visitedCount, int index) {
    // Base case #1: region doesn't match any of visitedRegions, add it to visitedRegions
    if (index == visitedCount) {
        visitedRegions[index] = region;
        return 0;
    }

    // Base case #2: region is found in visitedRegions
    if (visitedRegions[index] == region) return 1;
    return isVisitedRegion(visitedRegions, region, visitedCount, index + 1);
}

/*
 Return a given column queen's row
*/
int getQueenRow(int queenRows[], int column) {
    return queenRows[column];
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

    // Determines whether (row, column) is a valid queen position
    if (isAvailablePosition(queenRows, row, column) &&
        !isVisitedRegion(visitedRegions, regionsBoard[row][column], column, 0))
        return row;

    return getNextAvailableRow(regionsBoard, visitedRegions, queenRows, dimension, row + 1, column);
}

/*
 Solve queens battle. Start at 0, 0 and move according to the rules:
 - if (row, column) is a valid queen row, put a queen there and move on to next column to find a valid row
 - if no row is valid in column, backtrack to previous column until a valid position is found
*/
int solveQueenBattles(char regionsBoard[][MAX_QB_DIM], char visitedRegions[], int queenRows[],
    int dimension, int row, int column) {

    // Base case #1: successfully placed all queens by the rules in each column
    if (column == dimension) return 1;

    // Get available row in given column, with row as initial row to check
    int availableRow = getNextAvailableRow(regionsBoard, visitedRegions, queenRows, dimension, row, column);
    /*
     Get available row in next column, with 0 as initial row to check.
     Optimization in order to reduce recursion depth.
     if no available row in next column, behave like availableRow is actually not available
    */
    int nextAvailableRow = getNextAvailableRow(regionsBoard, visitedRegions, queenRows, dimension, 0, column + 1);

    /*
     Validate queen position (availableRow, column):
        - availableRow must be a valid row, and:
        - if it's the first column, optimization is irrelevant. put a queen there
        - otherwise next column's available row must be a valid row
    */
    if (availableRow != dimension && (column == 0 || nextAvailableRow != dimension)) {
        queenRows[column] = availableRow;
        return solveQueenBattles(regionsBoard, visitedRegions, queenRows, dimension, 0, column + 1);
    }

    /*
      No valid position in current column, backtrack to previous column.
      Base case #2: checked all rows in column 0 and no possible solution found.
    */
    if (column == 0) return 0;

    // Backtrack: try again with previous column, pick another row bigger than previous row picked
    int prevColumn = column - 1;
    int prevQueenRow = getQueenRow(queenRows, prevColumn);
    queenRows[prevColumn] = -1; // Remove previous queen location

    // Recursion call: try to solve with different queen row in previous column
    return solveQueenBattles(regionsBoard, visitedRegions, queenRows, dimension, prevQueenRow + 1, prevColumn);
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

Position getDirectedPosition(int row, int column, char direction, int index) {
    return direction == HORIZONTAL
        ? (Position) { row, column + index }
        : (Position) { row + index, column };
}

char getOppositeDirection(char direction) {
    return direction == HORIZONTAL ? VERTICAL : HORIZONTAL;
}

int isPositionCaught(char position) {
    return position <= LAST_ASCII_LETTER && position >= FIRST_ASCII_LETTER;
}

int isDirectedCaught(char board[][MAX_CW_DIM], int dimension, Slot slot, char word[], int index) {
    Position pos = getDirectedPosition(slot.row, slot.column, slot.direction, index);
    if (index == slot.length || pos.row >= dimension || pos.column >= dimension) return 0;

    char posValue = board[pos.row][pos.column];
    if (isPositionCaught(posValue) && word[index] != posValue) return 1;

    return isDirectedCaught(board, dimension, slot, word, index + 1);
}

int isMatchSlot(char board[][MAX_CW_DIM], int dimension, Slot slot, Word word) {
    if ((int) strlen(word.value) != slot.length) return 0;
    return !isDirectedCaught(board, dimension, slot, word.value, 0);
}

int getNextAvailableWordIndex(char board[][MAX_CW_DIM], int dimension, Slot slot, Word words[], int wordsCount, int wordIndex) {
    if (wordIndex == wordsCount) return NO_AVAILABLE_WORD;
    if (words[wordIndex].used != 1 && isMatchSlot(board, dimension, slot, words[wordIndex])) return wordIndex;
    return getNextAvailableWordIndex(board, dimension, slot, words, wordsCount, wordIndex + 1);
}

void insertWordToSlot(char board[][MAX_CW_DIM], Slot slot, Word word, int posIndex) {
    if (posIndex == slot.length) return;

    Position pos = getDirectedPosition(slot.row, slot.column, slot.direction, posIndex);
    board[pos.row][pos.column] = word.value[posIndex];
    insertWordToSlot(board, slot, word, posIndex + 1);
}

int hasAdjacentWord(char board[][MAX_CW_DIM], int dimension, int row, int column, char direction) {
    char adjacentsDirection = getOppositeDirection(direction);
    Position next = getDirectedPosition(row, column, adjacentsDirection, 1);
    Position previous = getDirectedPosition(row, column, direction, -1);
    return
        (next.row < dimension && isPositionCaught(board[next.row][next.column])) ||
        (previous.row > 0 && isPositionCaught(board[previous.row][previous.column]));
}

void removeWordFromBoard(char board[][MAX_CW_DIM], int dimension, Slot slot, int posIndex) {
    if (posIndex == slot.length) return;

    Position pos = getDirectedPosition(slot.row, slot.column, slot.direction, posIndex);
    if (!hasAdjacentWord(board, dimension, pos.row, pos.column, slot.direction))
        board[pos.row][pos.column] = NO_LETTER;
    removeWordFromBoard(board, dimension, slot, posIndex + 1);
}

int solveCrossword(char board[][MAX_CW_DIM], int dimension, Slot slotsDetails[], int slotsCount, int slotsIndex,
    Word words[], int wordsCount, int wordIndex, int depth) {

    if (slotsIndex == slotsCount) return 1;
    if (slotsIndex == 0 && wordIndex == wordsCount) return 0;
    int availableWordIndex = getNextAvailableWordIndex(board, dimension, slotsDetails[slotsIndex], words, wordsCount, wordIndex);

    int prevSlotsIndex = slotsIndex - 1;
    int prevWordIndex = slotsDetails[prevSlotsIndex].wordIndex;
    if (availableWordIndex == NO_AVAILABLE_WORD) {
        words[prevWordIndex].used = 0;
        removeWordFromBoard(board, dimension, slotsDetails[prevSlotsIndex], 0);
        return solveCrossword(board, dimension, slotsDetails, slotsCount, prevSlotsIndex, words, wordsCount, prevWordIndex + 1, depth + 1);
    }

    insertWordToSlot(board, slotsDetails[slotsIndex], words[availableWordIndex], 0);
    words[availableWordIndex].used = 1;
    slotsDetails[slotsIndex].wordIndex = availableWordIndex;
    return solveCrossword(board, dimension, slotsDetails, slotsCount, slotsIndex + 1, words, wordsCount, 0, depth + 1);
}

void crosswordGenerator() {
    printf("Please enter the dimensions of the crossword grid:\n");
    int dimension;
    scanf("%d", &dimension);

    printf("Please enter the number of slots in the crossword:\n");
    int slotsCount;
    scanf("%d", &slotsCount);

    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
    Slot slotsDetails[MAX_SLOTS];
    for (int slot = 0; slot < slotsCount; slot++) {
        scanf("%d", &slotsDetails[slot].row);
        scanf("%d", &slotsDetails[slot].column);
        scanf("%d", &slotsDetails[slot].length);
        scanf(" %c", &slotsDetails[slot].direction);
    }

    printf("Please enter the number of words in the dictionary:\n");
    int wordsCount;
    scanf("%d", &wordsCount);
    while (wordsCount < slotsCount) {
        printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", slotsCount);
        scanf("%d", &wordsCount);
    }

    printf("Please enter the words for the dictionary:\n");
    Word words[MAX_WORDS];
    for (int word = 0; word < wordsCount; word++) {
        scanf(" %s", words[word].value);
        words[word].used = 0; // TODO check if allowed in loops
    }

    char board[MAX_CW_DIM][MAX_CW_DIM] = {0};
    if (!solveCrossword(board, dimension, slotsDetails, slotsCount, 0, words, wordsCount, 0, 0)) {
        printf("This crossword cannot be solved.\n");
        return;
    }

    for (int row = 0; row < dimension; row++) {
        for (int column = 0; column < dimension; column++) {
            char value = isPositionCaught(board[row][column]) ? board[row][column] : NO_LETTER;
            printf("| %c ", value);
        }
        printf("|\n");
    }
}
