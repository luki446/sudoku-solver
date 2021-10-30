#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#define bool int
#define true 1
#define false 0
#define calc_index(x, y) (x * 9 + y)

const static int groups_indexes[9][9] = {
        { calc_index(0, 0), calc_index(0, 1), calc_index(0, 2), calc_index(1, 0), calc_index(1, 1), calc_index(1, 2), calc_index(2, 0), calc_index(2, 1), calc_index(2, 2) },
        { calc_index(0, 3), calc_index(0, 4), calc_index(0, 5), calc_index(1, 3), calc_index(1, 4), calc_index(1, 5), calc_index(2, 3), calc_index(2, 4), calc_index(2, 5) },
        { calc_index(0, 6), calc_index(0, 7), calc_index(0, 8), calc_index(1, 6), calc_index(1, 7), calc_index(1, 8), calc_index(2, 6), calc_index(2, 7), calc_index(2, 8) },
        { calc_index(3, 0), calc_index(3, 1), calc_index(3, 2), calc_index(4, 0), calc_index(4, 1), calc_index(4, 2), calc_index(5, 0), calc_index(5, 1), calc_index(5, 2) },
        { calc_index(3, 3), calc_index(3, 4), calc_index(3, 5), calc_index(4, 3), calc_index(4, 4), calc_index(4, 5), calc_index(5, 3), calc_index(5, 4), calc_index(5, 5) },
        { calc_index(3, 6), calc_index(3, 7), calc_index(3, 8), calc_index(4, 6), calc_index(4, 7), calc_index(4, 8), calc_index(5, 6), calc_index(5, 7), calc_index(5, 8) },
        { calc_index(6, 0), calc_index(6, 1), calc_index(6, 2), calc_index(7, 0), calc_index(7, 1), calc_index(7, 2), calc_index(8, 0), calc_index(8, 1), calc_index(8, 2) },
        { calc_index(6, 3), calc_index(6, 4), calc_index(6, 5), calc_index(7, 3), calc_index(7, 4), calc_index(7, 5), calc_index(8, 3), calc_index(8, 4), calc_index(8, 5) },
        { calc_index(6, 6), calc_index(6, 7), calc_index(6, 8), calc_index(7, 6), calc_index(7, 7), calc_index(7, 8), calc_index(8, 6), calc_index(8, 7), calc_index(8, 8) },
}; 

struct Board{
	char board[81];
};

struct Board* read_board_from_file(struct FILE* file_pointer) {
	struct Board* result = malloc(sizeof(struct Board));
	char processed_character;
	int index = 0;

	if(!result)
		return NULL;

	while((processed_character = fgetc(file_pointer)) != EOF) {
		if(processed_character == '\r' || processed_character == '\n') {
			continue;
		}
		else if(isdigit(processed_character)) {
			result->board[index++] = processed_character - '0';	
		}
		else if(processed_character == '.') {
			result->board[index++] = 0;
		} else {
			fprintf(stderr, "Invalid character in input file 0x%X\n", processed_character);
			return NULL;
		}
	}
	assert(index == 81);

		
	return result;
}

void print_board(struct Board* brd) {
	int x, y;
	char char_to_print;
	for(x = 0; x < 9 ; ++x) {
		if(x == 3 || x == 6) {
			for(y = 0 ; y < 11 ; ++y)
				putchar('-');
			putchar('\n');
		}

		for(y = 0; y < 9; ++y) {
			char_to_print = brd->board[x * 9 + y];
			if(char_to_print)
				putchar(char_to_print + '0');
			else
				putchar('.');
			if(y == 2 || y == 5) 
				putchar('|');
			/* putchar(char_to_print = brd->board[x * 9 + y] ? char_to_print : '.'); */	
		}
		putchar('\n');
	}
}

bool is_board_valid(struct Board* board) {
	int x, y, field_value, processed_array[10];
	
	/* rows */
	for(x = 0; x < 9 ; ++x) {
		memset(processed_array, 0, sizeof(processed_array));
		for(y = 0; y < 9; ++y ) {
			field_value = board->board[x * 9 + y];
			if(field_value) {
				if(processed_array[field_value]) {
					// fprintf(stderr, "Row %d is not valid\n", x);
					return false;
				}
				else
					processed_array[field_value]++;
			}
		}
	}

	/* columns */
	for(y = 0; y < 9 ; ++y) {
		memset(processed_array, 0, sizeof(processed_array));
		for(x = 0; x < 9; ++x ) {
			field_value = board->board[x * 9 + y];
			if(field_value) {
				if(processed_array[field_value]) {
					// fprintf(stderr, "Column %d is not valid\n", y);
					return false;
				}
				else
					processed_array[field_value]++;
			}
		}
	}

	/* groups */
	for(x = 0; x < 9 ; ++x) {
		memset(processed_array, 0, sizeof(processed_array));
		for(y = 0; y < 9; ++y ) {
			field_value = board->board[groups_indexes[x][y]];
			if(field_value) {
				if(processed_array[field_value]) {
					// fprintf(stderr, "Group %d is not valid\n", x);
					return false;
				}
				else
					processed_array[field_value]++;
			}
		}
	}

	return true;
}

bool solve(struct Board* board, int index) {
	int i;
	
	while(board->board[index] && index < 81) {
		index++;
	}

	if(index > 80) {
		return true;
	}

	for(i = 1 ; i <= 9; ++i) {
		board->board[index] = i;
		if(is_board_valid(board) && solve(board, index))
			return true;
	}

	board->board[index] = 0;
	return false;
}

int main(int argc, char** argv) {
	struct Board* board;
	struct FILE* fp;
	clock_t start, end;
	double solving_time;

	if(argc < 2) {
		fprintf(stderr, "Usage: %s <sudoku_board_file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	fp = fopen(argv[1], "rt");
	
	if(!fp) {
		fprintf(stderr, "Cannot open %s file\n", argv[1]);
		return EXIT_FAILURE;
	}

	board = read_board_from_file(fp);

	print_board(board);

	puts("==================================");
	
	printf("Is board valid: %d\n", is_board_valid(board));

	puts("==================================");

	start = clock();
	solve(board, 0);
	end = clock();

	solving_time = ((double) end - start) / CLOCKS_PER_SEC;

	print_board(board);
	
	puts("==================================");

	printf("Solving time: %lf s\n", solving_time);
	return EXIT_SUCCESS;
}
