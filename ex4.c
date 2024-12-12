/******************
Name:
ID:
Assignment:
*******************/

#include <stdio.h>
#include <string.h>

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

void task2_human_pyramid() {
    // Todo
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