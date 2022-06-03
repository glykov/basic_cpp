#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

// 1. Выделить в памяти динамический одномерный массив типа int. Размер массива запросить у пользователя.
// Инициализировать его числами степенями двойки: 1, 2, 4, 8, 16, 32, 64, 128 ... Вывести массив на экран.
void fill_array(int *arr, size_t size);
void print_array(const int *arr, const size_t size);
void task_01();

// 2. Динамически выделить матрицу 4х4 типа int. Инициализировать ее псевдослучайными числами через функцию rand.
// Вывести на экран. Разбейте вашу программу на функции которые вызываются из main
void fill_matrix(int **mtx, const size_t rows, const size_t cols);
void print_matrix(const int **mtx, const size_t rows, const size_t cols);
void task_02();

// 3. Написать программу, которая создаст два текстовых файла (*.txt), примерно по 50-100 символов в каждом
// (особого значения не имеет с каким именно содержимым). Имена файлов запросить у польлзователя.
bool create_file(const std::string file_name, std::string text);
void task_03();

// 4. Написать функцию, «склеивающую» эти файлы в третий текстовый файл (имя файлов спросить у пользователя).
bool merge_files(std::string first_filename, std::string second_filename, std::string result_filename);
void task_04();

// 5. Написать программу, которая проверяет присутствует ли указанное пользователем при запуске программы
// слово в указанном пользователем файле (для простоты работаем только с латиницей).
// Используем функцию find которая есть в строках std::string.
bool check_word(std::string filename, std::string word);
void task_05(const char *filename, const char* word);

int main(int argc, char **argv)
{
	//task_01();

	//task_02();

	// task_03();

	// task_04();

	if (argc < 3) {
		std::cout << "Proper usage is program_name file_to_check word_to_check\n";
	}
	else {
		task_05(argv[1], argv[2]);
	}
	return 0;
}

// 1. Выделить в памяти динамический одномерный массив типа int. Размер массива запросить у пользователя.
// Инициализировать его числами степенями двойки: 1, 2, 4, 8, 16, 32, 64, 128 ... Вывести массив на экран.
void fill_array(int* arr, size_t size)
{
	int value = 1;
	for (size_t i = 0; i < size; ++i) {
		arr[i] = value;
		value *= 2;
	}
}

void print_array(const int* arr, const size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		std::cout << arr[i] << " ";
	}
}

void task_01()
{
	// get the size of array from user
	size_t size;
	std::cout << "Enter the size of array: ";
	std::cin >> size;
	
	// create array
	int* powers_of_two = new (std::nothrow) int[size];
	if (powers_of_two == nullptr) {
		std::cout << "Cannot allocate memory for array!\n";
		return;
	}

	// filling array with values
	fill_array(powers_of_two, size);

	// printing the array
	std::cout << "Here is your array:\n";
	print_array(powers_of_two, size);
	std::cout << std::endl;

	// removing array from memory
	delete[] powers_of_two; 
}

// 2. Динамически выделить матрицу 4х4 типа int. Инициализировать ее псевдослучайными числами через функцию rand.
// Вывести на экран. Разбейте вашу программу на функции которые вызываются из main
void fill_matrix(int **mtx, const size_t rows, const size_t cols)
{
	srand(time(0));
	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < cols; ++col) {
			mtx[row][col] = 1 + rand() % 10;
		}
	}
}

void print_matrix(const int **mtx, const size_t rows, const size_t cols)
{
	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < cols; ++col) {
			std::cout << std::setw(4) << mtx[row][col];
		}
		std::cout << std::endl;
	}
}

void task_02()
{
	// these could be just variables, not constants :)
	const size_t rows {4};
	const size_t cols {4};

	// creating dynamic two-dimentional array
	int **matrix = new(std::nothrow) int*[rows];
	if (matrix == nullptr) {
		std::cout << "Cannot allocate memory for matrix.\n";
		return;
	}
	for (size_t row = 0; row < rows; ++row) {
		matrix[row] = new(std::nothrow) int[cols];
		if (matrix[row] == nullptr) {
			std::cout << "Cannot allocate memory for matrix row " << row << "!\n";
			return;
		}
	}

	// filling the matrix with values
	fill_matrix(matrix, rows, cols);

	// printing the matrix
	std::cout << "Here is your matrix:\n";
	print_matrix((const int**)matrix, rows, cols);
	std::cout << std::endl;

	// removing matrix from memory
	for (size_t row = 0; row < rows; ++row) {
		delete[] matrix[row];
	}
	delete[] matrix;
}

// 3. Написать программу, которая создаст два текстовых файла (*.txt), примерно по 50-100 символов в каждом
// (особого значения не имеет с каким именно содержимым). Имена файлов запросить у польлзователя.
// тут будем отркывать файл с использованием конструктора
bool create_file(const std::string file_name, const std::string text)
{
	std::ofstream fout(file_name + ".txt", std::ios_base::out);
	if (!fout.is_open()) {
		return false;
	}
	fout << text;
	fout.close();
	return true;

}
void task_03()
{
	// text to write into files
	std::string text_01 = "Alice was beginning to get very tired of sitting by her sister on the bank"
							", and of having nothing to do: ";
	std::string text_02 = "once or twice she had peeped into the book her sister was reading,";

	// getting file names from user
	std::string filename_01, filename_02;
	std::cout << "Enter the name of the first file: ";
	std::getline(std::cin, filename_01);
	std::cout << "Enter the name of the second file: ";
	std::getline(std::cin, filename_02);

	if (!create_file(filename_01, text_01)) {
		std::cout << "Cannot create file \"" << filename_01 << ".txt\" to write in!\n";
	}

	if (!create_file(filename_02, text_02)) {
		std::cout << "Cannot open file \"" << filename_02 << ".txt\" to write in!\n";
	}
}

// 4. Написать функцию, «склеивающую» эти файлы в третий текстовый файл (имя файлов спросить у пользователя).
// тут будем открывать файлы с использованием присваивания
bool merge_files(const std::string first_filename, const std::string second_filename, const std::string result_filename)
{
	std::ifstream fin;
	fin.open(first_filename.c_str(), std::ios_base::in);
	if (!fin.is_open()) {
		std::cout << "Cannot open file \"" << first_filename << "\" for reading.\n";
		return false;
	}

	std::ofstream fout;
	fout.open(result_filename.c_str(), std::ios_base::out);
	if (!fout.is_open()) {
		std::cout << "Cannot open file \"" << result_filename << "\" for writing.\n";
		fin.close();
		return false;
	}

	char ch;
	while ((ch = fin.get()) != EOF) {
		fout << ch;
	}

	// closing both files just to try ios_base::app :)
	fin.close();
	fout.close();

	fin.open(second_filename.c_str(), std::ios_base::in);
	if (!fin.is_open()) {
		std::cout << "Cannot open file \"" << second_filename << "\" for reading.\n";
		return false;
	}

	fout.open(result_filename.c_str(), std::ios_base::app);
	if (!fout.is_open()) {
		std::cout << "Cannot open file \"" << result_filename << "\" for writing.\n";
		fin.close();
		return false;
	}

	while ((ch = fin.get()) != EOF) {
		fout << ch;
	}

	fin.close();
	fout.close();

	return true;
}
void task_04()
{
	// getting file names from user
	std::string first_filename, second_filename, result_filename;
	std::cout << "Enter the first file name: ";
	std::getline(std::cin, first_filename);
	std::cout << "Enter the second file name: ";
	std::getline(std::cin, second_filename);
	std::cout << "Enter the resulting file name: ";
	std::getline(std::cin, result_filename);

	// merging files
	if (merge_files(first_filename, second_filename, result_filename)) {
		std::cout << "Job is done! Enjoy your new file!\n";
	}
	else {
		std::cout << "Something went wrong. Take a rest and try again later.\n";
	}
}

// 5. Написать программу, которая проверяет присутствует ли указанное пользователем при запуске программы
// слово в указанном пользователем файле (для простоты работаем только с латиницей).
// Используем функцию find которая есть в строках std::string.
bool check_word(std::string filename, std::string word)
{
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		std::cout << "Cannot open file \"" << filename << "\" to read\n";
		return false;
	}

	bool found = false;
	std::string s;
	// varian 1. using std::cin
	while (std::cin >> s && !found) {
		if (s == word) {
			found = true;
		}
	}
	
	// variant 2. using std::getline and std::string.find()
	fin.clear();
	fin.seekg(0, std::ios_base::beg);
	found = false;
	while (std::getline(fin, s) && !found) {
		found = (s.find(word) != std::string::npos);
	}

	fin.close();
	return found;
}
void task_05(const char *filename, const char* word)
{
	if (check_word(filename, word)) {
		std::cout << "Word " << word << " is found in file " << filename << ".\n";
	}
	else {
		std::cout << "Word " << word << " is not found in file " << filename << ".\n";
	}
}