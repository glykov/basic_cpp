#include <iostream>
#include <cstdlib>
#include <ctime>

enum chip: char 
{
	cross = 'X',
	nought = 'O',
	empty = '_'
};

void init_field(char **field, size_t rows, size_t cols);

void print_field(char **filed, size_t rows, size_t cols);

bool is_valid_cell(int row, int col, size_t rows, size_t cols);

bool is_empty_cell(char** field, int row, int col);

void human_turn(char **field, size_t rows, size_t cols);

void ai_turn(char **filed, size_t rows, size_t cols);

bool is_human_winning(char **filed, size_t rows, size_t cols);

bool try_to_win(char **filed, size_t rows, size_t cols);

bool is_field_full(char **field, size_t rows, size_t cols);

bool check_for_winner(char **field, size_t rows, size_t cols, char ch, size_t win_cond);

bool check_horizontal(char **field, size_t rows, size_t cols, char ch, size_t win_cond);

bool check_vertical(char **field, size_t rows, size_t cols, char ch, size_t win_cond);

bool check_diagonal(char **field, size_t rows, size_t cols, char ch, size_t win_cond);

void play_one_round();

int main()
{
	while (true) {
		play_one_round();
		std::cout << "Do you want to play again (Y/N)? ";
		char choice;
		std::cin >> choice;
		if (toupper(choice) == 'N') {
			break;
		}
	}
	return 0;
}

void init_field(char **field, size_t rows, size_t cols)
{
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			field[row][col] = empty;
		}
	}
}

void print_field(char **field, size_t rows, size_t cols)
{
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			std::cout << "+---";
		}
		std::cout << "+" << std::endl << "|";
		for (size_t col = 0; col < cols; col++) {
			std::cout << " " << field[row][col] << " |";
		}
		std::cout << std::endl;
	}
	for (size_t col = 0; col < cols; col++) {
			std::cout << "+---";
		}
		std::cout << "+" << std::endl; 
}

bool is_valid_cell(int row, int col, size_t rows, size_t cols)
{
	return (row >= 0 && row <= rows && col >= 0 && col <= cols);
}

bool is_empty_cell(char** field, int row, int col)
{
	return field[row][col] == empty;
}

void human_turn(char **field, size_t rows, size_t cols)
{
	int row, col;
	
	do {
		std::cout << "Enter the row form 1 to " << rows << ": ";
		std::cin >> row;
		row--;
		std::cout << "Enter the column form 1 to " << cols << ": ";
		std::cin >> col;
		col--;
	} while (!is_valid_cell(row, col, rows, cols) && !is_empty_cell(field, row, col));
	field[row][col] = cross;
}

void ai_turn(char **field, size_t rows, size_t cols)
{
	if (is_human_winning(field, rows, cols)) {		// попытаться заблокировать игрока
		;
	}
	else if (try_to_win(field, rows, cols)) {		// попытаться победить
		;
	}
	else {
		srand(time(nullptr));						// если ни то, ни другое не получилось, попытать удачу
		size_t row, col;
		do {
			row = rand() % rows;
			col = rand() & cols;
		} while (!is_empty_cell(field, row, col));
		field[row][col] = nought;
	}
}

bool is_human_winning(char **field, size_t rows, size_t cols)
{
	for (size_t row = 0; row < rows - 1; row++) {
		for (size_t col = 0; col < cols - 1; col++) {
			// проверить горизонталь и попытаться заблокировать игрока
			if (field[row][col] == cross && field[row][col + 1] == cross) {
				if (is_valid_cell(row, col + 2, rows, cols) && is_empty_cell(field, row, col + 2)) {
					field[row][col + 2] == nought;
					return true;
				}
			}
			// проверить вертикаль и попытаться заблокировать игрока
			if (field[row][col] == cross && field[row + 1][col] == cross) {
				if (is_valid_cell(row + 2, col, rows, cols) && is_empty_cell(field, row + 2, col)) {
					field[row + 2][col] == nought;
					return true;
				}
			}
			// проверить главную диагональ и попытаться заблокировать игрока
			if (field[row][col] == cross && field[row + 1][col + 1] == cross) {
				if (is_valid_cell(row + 2, col + 2, rows, cols) && is_empty_cell(field, row + 2, col + 2)) {
					field[row][col + 2] == nought;
					return true;
				}
			}
			// проверить побочную диагональ и попытаться заблокировать игрока
			if (field[rows - 1 - row][col] == cross && field[rows - 2 - row][col + 1] == cross) {
				if (is_valid_cell(rows - 3 - row, col + 2, rows, cols) && is_empty_cell(field, rows - 3 - row, col + 2)) {
					field[row][col + 2] == nought;
					return true;
				}
			}
		}
	}
	return false;
}

bool try_to_win(char **field, size_t rows, size_t cols)
{
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			if (field[row][col] == nought) {
				// попытаться продолжить по горизонтали
				if (is_valid_cell(row, col + 1, rows, cols) && is_empty_cell(field, row, col + 1)) {
					field[row][col + 1] = nought;
					return true;
				}
				// попытаться продолжить по вертикали
				if (is_valid_cell(row + 1, col, rows, cols) && is_empty_cell(field, row + 1, col)) {
					field[row][col + 1] = nought;
					return true;
				}
				// попытаться продолжить по диагонали
				if (is_valid_cell(row + 1, col + 1, rows, cols) && is_empty_cell(field, row + 1, col + 1)) {
					field[row + 1][col + 1] = nought;
					return true;
				}
			}
		}
	}
	return false; 
}

bool is_field_full(char **field, size_t rows, size_t cols)
{
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < cols; col++) {
			if (field[row][col] == empty) {
				return false;
			}
		}
	}
	return true;
}

bool check_for_winner(char **field, size_t rows, size_t cols, char ch, size_t win_cond)
{
	return check_horizontal(field, rows, cols, ch, win_cond) || 
			check_vertical(field, rows, cols, ch, win_cond) || 
			check_diagonal(field, rows, cols, ch, win_cond);
}

bool check_horizontal(char **field, size_t rows, size_t cols, char ch, size_t win_cond)
{
	for (size_t row = 0; row < rows; row++) {
		int counter = 0;
		for (int col = 0; col < cols; col++) {
			if (field[row][col] == ch) counter++;
		}
		if (counter == win_cond) {
			return true;
		}
	}
	return false;
}

bool check_vertical(char **field, size_t rows, size_t cols, char ch, size_t win_cond)
{
	for (size_t col = 0; col < cols; col++) {
		int counter = 0;
		for (int row = 0; row < rows; row++) {
			if (field[row][col] == ch) counter++;
		}
		if (counter == win_cond) {
			return true;
		}
	}
	return false;
}

bool check_diagonal(char **field, size_t rows, size_t cols, char ch, size_t win_cond)
{
	int main_diag = 0;
    int side_diag = 0;
    for (size_t row = 0; row < rows; row++) {
        if (field[row][row] == ch)
            main_diag++;
        if (field[row][cols - 1 - row] == ch) {
            side_diag++;
        }
    }
    return (main_diag == win_cond) || (side_diag == win_cond);
}

void play_one_round() {
    // получить размеры поля и количество заполненных клеток, необходимое для победы
	size_t rows, cols, win_condition;
	std::cout << "Enter the size of game field: ";
	std::cin >> rows >> cols;
	std::cout << "Enter the win condition: ";
	std::cin >> win_condition;
	// создать игровое поле
	char **field = new char*[rows];
	for (size_t row = 0; row < rows; row++ ) {
		field[row] = new char[cols];
	}
	init_field(field, rows, cols); 
	print_field(field, rows, cols);
        
	while (true) {
		std::cout << "---------- PLAYER TURN -------------\n";
        human_turn(field, rows, cols);
        print_field(field, rows, cols);
        if (check_for_winner(field, rows, cols, cross, win_condition)) {
            std::cout << "Player won!\n";
            break;
        }
        if (is_field_full(field, rows, cols)) {
            std::cout << "Draw!\n";
            break;
        }
		std::cout << "------------ AI TURN ---------------\n";
        ai_turn(field, rows, cols);
        print_field(field, rows, cols);
        if (check_for_winner(field, rows, cols, nought, win_condition)) {
            std::cout << "Computer won!\n";
            break;
        }
        if (is_field_full(field, rows, cols)) {
            std::cout << "Draw!\n";
            break;
        }
    }
	
	for (size_t row = 0; row < rows; row++) {
		delete[] field[row];
	}
	delete[] field;
}