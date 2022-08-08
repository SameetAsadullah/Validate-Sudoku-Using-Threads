#include<iostream>
#include<semaphore.h>
#include<pthread.h>
using namespace std;

int grid[9][9] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

int col = 0, row = 0, subGridRow = 0, subGridCol = 0;
sem_t semRow, semCol, semSubGrid;
bool valid = true;

void* checkRow(void* arg) {
    sem_wait(&semRow);

    if (valid == true) {
        for (int i = 1; i <= 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (grid[row][j] == i) {
                    break;
                }

                else if (j == 8) {
                    valid = false;
                    sem_post(&semRow);
                    return NULL;
                }
            }
        }
        row++;
    }
    sem_post(&semRow);
    pthread_exit(NULL);
}

void* checkCol(void* arg) {
    sem_wait(&semCol);

    if (valid == true) {
        for (int i = 1; i <= 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (grid[j][col] == i) {
                    break;
                }

                else if (j == 8) {
                    valid = false;
                    sem_post(&semCol);
                    return NULL;
                }
            }
        }
        col++;
    }
    sem_post(&semCol);
    pthread_exit(NULL);
}

void* checkSubGrid(void* arg) {
    sem_wait(&semSubGrid);

    if (valid == true) {
        for (int i = 1; i <= 9; ++i) {
            bool found = false;
            for (int j = subGridRow; j < (subGridRow + 3); ++j) {
                for (int k = subGridCol; k < (subGridCol + 3); ++k) {
                    if (grid[j][k] == i) {
                        found = true;
                        break;
                    }

                    else if (j == subGridRow + 2 && k == subGridCol + 2) {
                        valid = false;
                        sem_post(&semSubGrid);
                        return NULL;
                    }
                }

                if (found == true) {
                    break;
                }
            }
        }

        if (subGridCol + 3 > 8) {
            subGridRow += 3;
            subGridCol = 0;
        }

        else {
            subGridCol += 3;
        }
    }
    sem_post(&semSubGrid);
    pthread_exit(NULL);
}

int main() {    
    pthread_t tid[27];
    int count = 0;

    sem_init(&semRow, 0, 1);
    sem_init(&semCol, 0, 1);
    sem_init(&semSubGrid, 0, 1);
    
    for (int i = 0 ; i < 9 ; ++i) {
        pthread_create((tid + count), NULL, checkRow, NULL);
        count++;
    }

    for (int i = 0 ; i < 9 ; ++i) {
        pthread_create((tid + count), NULL, checkCol, NULL);
        count++;
    }

    for (int i = 0 ; i < 9 ; ++i) {
        pthread_create((tid + count), NULL, checkSubGrid, NULL);   
        count++;
    }

    for (int i = 0; i < 27; ++i) {
        pthread_join(tid[i], NULL);
    }

    if (valid == true) {
        cout << "=> The given Sudoku puzzle is valid" << endl;
    }

    else {
        cout << "=> The given Sudoku puzzle is invalid" << endl;
    }

    pthread_exit(NULL);
}