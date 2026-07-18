#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* --- Global Configurations and State --- */
// Board array mapping slots 1-9 to their character marks ('1'-'9', 'X', or 'O').
// Index 0 is padded with 'o' to maintain an intuitive 1-based indexing structure.
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};
int choice, player;

/* --- Function Prototypes --- */
int checkForWin();
void displayBoard();
void markBoard(char mark);

/**
 * Main application execution loop.
 * Orchestrates the game phases: rendering, processing turns, updating board state, and detecting win conditions.
 */
int main()
{
    int game_status;
    char mark;
    player = 1;
    
    do {
        displayBoard();
        
        // Alternate turns between Player 1 and Player 2
        player = (player % 2) ? 1 : 2;
        
        printf("Player %d, Enter a number: ", player);
        scanf("%d", &choice);
        
        // Assign token markers based on active player identifier
        mark = (player == 1) ? 'X' : 'O';
        
        markBoard(mark);
        game_status = checkForWin();
        
        player++; // Advance turn sequence
    } while(game_status == -1); // Continue executing while the game is in progress
    
    // Terminal rendering state updates post-game loop termination
    displayBoard();
    
    // Announce match outcome results
    if (game_status == 1) 
    {
        // Revert the final loop increment to identify the true winner
        printf("==>\a Player %d wins!\n", --player);
    } 
    else 
    {
        printf("==>\a Game Draw!\n");
    }
    
    getch();
    return 0;
}

/**
 * Evaluates the board matrices to determine match states.
 * @return 1 if a player wins, 0 on a draw state, or -1 if the match remains active.
 */
int checkForWin()
{
    // Column Win Configurations
    if (square[1] == square[4] && square[4] == square[7]) return 1;
    else if (square[2] == square[5] && square[5] == square[8]) return 1;
    else if (square[3] == square[6] && square[6] == square[9]) return 1; 
    
    // Row Win Configurations
    else if (square[1] == square[2] && square[2] == square[3]) return 1;
    else if (square[4] == square[5] && square[5] == square[6]) return 1;
    else if (square[7] == square[8] && square[8] == square[9]) return 1;
    
    // Diagonal Win Configurations
    else if (square[1] == square[5] && square[5] == square[9]) return 1;
    else if (square[3] == square[5] && square[5] == square[7]) return 1;
    
    // Draw Condition Verification: Evaluates if all elements have shifted away from original numeric values
    else if (square[1] != '1' && square[2] != '2' && square[3] != '3' && 
             square[4] != '4' && square[5] != '5' && square[6] != '6' && 
             square[7] != '7' && square[8] != '8' && square[9] != '9')
        return 0;
        
    // Base Case: No definitive result, match runtime continues
    else return -1;
}

/**
 * Clears terminal buffers and prints the visual Tic-Tac-Toe structural interface.
 */
void displayBoard()
{
    system("cls"); // Flush console output history
    
    printf("\n\nTic Tac Toe\n\n");
    printf("Player 1 (X) - Player 2 (O)\n\n\n");
    
    printf("    |   |   \n");
    printf("  %c | %c | %c  \n", square[1], square[2], square[3]);
    printf("----|---|----   \n");
    printf("  %c | %c | %c  \n", square[4], square[5], square[6]);
    printf("----|---|----   \n");
    printf("  %c | %c | %c  \n", square[7], square[8], square[9]);
    printf("    |   |   \n\n");
}

/**
 * Validates selected input target coordinates and updates token state values.
 * @param mark The symbolic token representation ('X' or 'O') to allocate.
 */
void markBoard(char mark)
{
    if (choice == 1 && square[1] == '1')
        square[1] = mark;
    else if (choice == 2 && square[2] == '2')
        square[2] = mark;
    else if (choice == 3 && square[3] == '3')
        square[3] = mark;
    else if (choice == 4 && square[4] == '4')
        square[4] = mark;
    else if (choice == 5 && square[5] == '5')
        square[5] = mark;
    else if (choice == 6 && square[6] == '6')
        square[6] = mark;
    else if (choice == 7 && square[7] == '7')
        square[7] = mark;
    else if (choice == 8 && square[8] == '8')
        square[8] = mark;
    else if (choice == 9 && square[9] == '9')
        square[9] = mark;
    else
    {
        // Exception handler path for collisions or out-of-bounds selections
        printf("Invalid move... Press Enter to try again.");
        player--; // Decrement execution counter sequence to preserve the active player's turn window
        getch();
    }
}
