#include <iostream>
#include <cstdint>

// 2. Создать перечисление (enum) с возможными вариантами символов для игры в крестики-нолики.
// если создавать перечисление для вариантов символов
enum Symbols {zero, cross};
// если создавать перечисление для состояний клетки (объялено как enum class во избежание конфликта имен)
enum class State {free_cell, zero, cross};

// 3. Создать массив, способный содержать значения такого перечисления и инициализировать его.
const int FIELD_SIZE = 3;
State game_field[FIELD_SIZE][FIELD_SIZE] = {State::free_cell};

// 4. * Создать структуру (struct) данных «Поле для игры в крестики-нолики» и снабдить его всеми необходимыми свойствами (подумайте что может понадобиться).
// 
struct GameField
{
	// матрица состояний игрового поля
	State game_cells[FIELD_SIZE][FIELD_SIZE];
	// game mode игра игрок-игрок (true) или игрок-компьютер (false)
	bool is_pvp;
	// игра работает/на паузе
	bool is_running;
	// true - ход игрока, flase - ход компьютера
	// или true - ход первого игрока, false - ход второго
	bool human_turn;
	// состояние игры - продолжить/определить победителя
	bool has_winner;
	// заполнители для игрового поля
	const char* symbols[3] = {"[ ]", " O ", " X "};
	// координаты хода
	struct Cell { int x; int y; };
	// имена игроков - второй игрок по умолчанию будет инициализироваться именем Computer
	char first_player_name[30];
	char second_player_name[30];
	// кто чем играет symbols[State::zero] или symbols[Sate::cross]
	State fpm;		// first_player_marker -> symbols[fpm]
	State spm;		// second_player_marker -> symbols[spm]
};

// 5. ** Создать структуру (struct MyVariant) объединяющую: union MyData (int, float, char) и 
// 3-и битовых поля (флага) указывающими какого типа значение в данный момент содержится в объединении (isInt, isFloat, isChar). 
// Продемонстрировать пример использования в коде этой структуры.
struct MyVariant
{
	union
	{
		int i;
		float f;
		char c;
	} MyData;
	struct 
	{
		unsigned char is_int: 1;
		unsigned char is_float: 1;
		unsigned char is_char: 1; 
	};
};

int main()
{
	// 1. Создать и инициализировать переменные пройденных типов данных
	// integer types
	char character = 'A';
	uint8_t std_character = character;
	char str[] { "This is a string" };		// массив символов -- строка в стиле С
	short short_int = 250;
	int16_t std_short = short_int;
	int integer = 2'000'000'000;
	int32_t std_integer = integer;
	unsigned unsigned_int = 4'000'000'000U;
	uint32_t sdt_uint = unsigned_int;
	long lng = 10'000'000'000;				// в моей системе (Linux/GCC 9) long == long long, поэтому переполнения не боюсь :)
	int64_t std_long = lng * 10;
	// floating-point types
	float f = 3.14159f;
	double d = -1.60217e-19;				// экспоненциальная запись вещественного литерала
	// bolean type
	bool boolean = true;

	// 3. смотрим что в массиве после инициализации
	// просто было интересно сработает на двумерном массиве или нет
	// хотя, двумерный массив он для нас, для компа он все равно одномерный :)
	for (int i = 0; i < FIELD_SIZE; ++i) {
		for (int j = 0; j < FIELD_SIZE; ++j) {
			if (game_field[i][j] == State::free_cell) {
				std::cout << "[ ]";
			}
			else if (game_field[i][j] == State::zero) {
				std::cout << " O ";
			}
			else {
				std::cout << " X ";
			}
		}
		std::cout << std::endl;
	}
	/* Инициализация прошла успешно:
	[] [] []
	[] [] []
	[] [] []
	*/

	// 5** Продемонстрировать пример использования в коде этой структуры.
	// лучше было бы создать суеттеры (set_int(), set_float(), set_char())
	// которые бы устанавливали как само згачение, так и флаг
	// void MyVariant::set_int(int x) { this.MyData.i = x; this.i = 1; }
	// мне кажется как-то так, но это не точно :)
	MyVariant mv[3]; 
	mv[0].MyData.i = 10;
	mv[0].is_int = 1;
	mv[1].MyData.f = 2.71828;
	mv[1].is_float = 1;
	mv[2].MyData.c = 'Z';
	mv[2].is_char = 1;

	return 0;
}