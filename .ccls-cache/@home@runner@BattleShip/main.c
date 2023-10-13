/*BattleShip Game
  Created By: Derian Comas*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 10

typedef enum {CRUISER = 2, SUBMARINE = 3, DESTROYER = 3, BATTLESHIP = 4, AIRCRAFT_CARRIER = 5} ShipType;

typedef struct {
    ShipType type;
    int size;
    char symbol;
    int hits;
} Ship;

int num_missiles;
int num_ships_sunk;
Ship ships[] = {{CRUISER, CRUISER, 'C', 0},
                {SUBMARINE, SUBMARINE, 'S', 0},
                {DESTROYER, DESTROYER, 'D', 0},
                {BATTLESHIP, BATTLESHIP, 'B', 0},
                {AIRCRAFT_CARRIER, AIRCRAFT_CARRIER, 'A', 0}};

void print_board(char board[][BOARD_SIZE], int show_ships) {
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'H' || board[i][j] == 'M') {
                printf("%c ", board[i][j]);
            } else if (show_ships && board[i][j] != ' ') {
                printf("%c ", board[i][j]);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

void initialize_board(char board[][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

int place_ship(char board[][BOARD_SIZE], ShipType type, int row, int col, int direction) {
    int size = type;
    if (direction == 0) {  // horizontal
        if (col + size > BOARD_SIZE) {
            return 0;
        }
        for (int i = col; i < col + size; i++) {
            if (board[row][i] != ' ') {
                return 0;
            }
        }
        for (int i = col; i < col + size; i++) {
            board[row][i] = ships[type].symbol;
        }
    } 
    else {  // vertical
        if (row + size > BOARD_SIZE) {
            return 0;
        }
        for (int i = row; i < row + size; i++) {
            if (board[i][col] != ' ') {
                return 0;
            }
        }
        for (int i = row; i < row + size; i++) {
            board[i][col] = ships[type].symbol;
        }
    }
    return 1;
}

void place_ships_randomly(char board[][BOARD_SIZE]) {
    srand(time(NULL));
    for (int i = 0; i < sizeof(ships) / sizeof(Ship); i++) {
        int row, col, direction;
        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
            direction = rand() % 2;  // 0 for horizontal, 1 for vertical
        } while (!place_ship(board, ships[i].type, row, col, direction));
    }
}

int check_shot(char board[][BOARD_SIZE], int row, int col) {
  if (board[row][col] == ' ') {
    board[row][col] = 'M';
    return 0;
  } 
  else if (board[row][col] == 'M' || board[row][col] == 'H') {
    return -1;
  } 
  else {
    board[row][col] = 'H';
    for (int i = 0; i < sizeof(ships) / sizeof(Ship); i++) {
      if (board[row][col] == ships[i].symbol) {
        ships[i].hits++;
        if (ships[i].hits == ships[i].size) {
          printf("You sunk the %c!\n", ships[i].symbol);
          num_ships_sunk++;
        }
        break;
      }
    }
    return 1;
  }
}

void play_game() {
  char player_board[BOARD_SIZE][BOARD_SIZE];
  char computer_board[BOARD_SIZE][BOARD_SIZE];
    initialize_board(player_board);
    initialize_board(computer_board);
    place_ships_randomly(player_board);
    place_ships_randomly(computer_board);
    num_missiles = 0;
    num_ships_sunk = 0;
  while (num_ships_sunk < sizeof(ships) / sizeof(Ship)) {
    printf("Your board:\n");
    print_board(player_board, 1);
    printf("Computer's board:\n");
    print_board(computer_board, 0);
    printf("Enter row and column to fire missile (e.g. 1A): ");
    char input[3];
    scanf("%s", input);
    int row = input[0] - '1';
    int col = input[1] - 'A';
    int result = check_shot(computer_board, row, col);
    if (result == 1) {
      printf("Hit!\n");
    } 
    else if (result == 0) {
      printf("Miss!\n");
      printf("Computer is firing missile...\n");
      int comp_row, comp_col;
      do {
        comp_row = rand() % BOARD_SIZE;
        comp_col = rand() % BOARD_SIZE;
      } while (check_shot(player_board, comp_row, comp_col) == -1);
      printf("Computer fires at %c%d ", 'A' + comp_col, comp_row + 1);
      if (check_shot(player_board, comp_row, comp_col) == 1) {
        printf("and hits!\n");
      } 
      else{
        printf("and misses!\n");
      }
    } 
    else{
      printf("You already fired at that location!\n");
    }
    num_missiles++;
  }
  printf("You win! It took you %d missiles.\n", num_missiles);
}

int main() {
  printf("Welcome to Battleship!\n");
  printf("You have five ships to sink: a Cruiser (2 spaces), a Submarine (3 spaces), a Destroyer (3 spaces), a Battleship (4 spaces), and an Aircraft Carrier (5 spaces).\n");
  printf("To fire a missile, enter the row (1-10) and column (A-J) of the target (e.g. 1A).\n");
  printf("Good luck!\n");
  play_game();
return 0;}