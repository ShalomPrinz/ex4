/******************
Name:
ID:
Assignment:
*******************/

#include <stdio.h>
#include <string.h>

// Cheerleaders human pyramid dimensions, e.g. 5 means 5x5 pyramid
#define CL_DIMENSIONS 5

// Task Defined Functions
void task1_robot_paths();
void task2_human_pyramid();
void task3_parenthesis_validator();
void task4_queens_battle();
void task5_crossword_generator();

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
                    task1_robot_paths();
                    break;
                case 2:
                    task2_human_pyramid();
                    break;
                case 3:
                    task3_parenthesis_validator();
                    break;
                case 4:
                    task4_queens_battle();
                    break;
                case 5:
                    task5_crossword_generator();
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
    // Reached invalid column or invalid row values, path is not counted
    if (column < 0 || row < 0) return 0;
    // Reached robot's home, count path
    if (column == 0 && row == 0) return 1;

    // Robot is at the most left column, so he must move down
    if (column == 0) return calculatePathsCount(column, row - 1);
    // Robot is at the most bottom row, so he must move left
    if (row == 0) return calculatePathsCount(column - 1, row);
    // Robot can move both left or down, count each of them as a valid move
    return calculatePathsCount(column, row - 1) + calculatePathsCount(column - 1, row);
}

void task1_robot_paths() {
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
float calculateSupportedWeight(float weights[CL_DIMENSIONS][CL_DIMENSIONS], int row, int column) {
    // Reached invalid column or invalid row values, no cheerleader weight to return
    if (column < 0 || row < 0 || !isValidPColumn(column, row)) return 0;

    // Calculates the supported weight of the left parent of current cheerleader
    float leftParentWeight = calculateSupportedWeight(weights, row - 1, column - 1);
    // Calculates the supported weight of the right parent of current cheerleader
    float rightParentWeight = calculateSupportedWeight(weights, row - 1, column);
    // Calculates current cheerleader supported weight by given formula
    return weights[row][column] + (float) .5 * (leftParentWeight + rightParentWeight);
}

void task2_human_pyramid() {
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

void task3_parenthesis_validator() {
    // Todo
}

void task4_queens_battle() {
    // Todo
}

void task5_crossword_generator() {
    // Todo
}
