#include <iostream>
#include <string>
#include <fstream>
#include "mylib.h"

// 2. Описать макрокоманду (через директиву define), проверяющую, входит ли переданное ей число
// (введенное с клавиатуры) в диапазон от нуля (включительно) до переданного ей второго аргумента (исключительно)
// и возвращает true или false, вывести на экран «true» или «false». 
#define CHECK_RANGE(x, y) ((x) >= 0 && (x) < y ? true : false)

// 3. Задайте массив типа int. Пусть его размер задается через директиву препроцессора #define.
// Инициализируйте его через ввод с клавиатуры. Напишите для него свою функцию сортировки (например Пузырьком).
// Реализуйте перестановку элементов как макрокоманду SwapINT(a, b). Вызывайте ее из цикла сортировки.
#define ARR_SIZE 10
#define SWAP(x, y) {int t = (x);\
 (x) = (y);\
 (y) = t;}
// решил попрактиковаться с простыми методами сортировки :)
void bubble_sort(int arr[], const size_t size, bool ascending = true);
void selection_sort(int arr[], const size_t size, bool ascending = true);
void insertion_sort(int arr[], const size_t size, bool ascending = true);
// вспомогательные функции
void print_array(const int arr[], const size_t size);
void init_array(int arr[], const size_t size);

// 4. Объявить структуру Сотрудник с различными полями.
// Сделайте для нее побайтовое выравнивание с помощью директивы pragma pack.
// Выделите динамически переменную этого типа. Инициализируйте ее. Выведите ее на экран и ее размер с помощью sizeof.
// Сохраните эту структуру в текстовый файл.
#pragma pack(push, 1)
struct Employee
{
	std::string first_name;
	std::string last_name;
	int id;
	double salary;
};
#pragma pack(pop)

void writeEmployee(const Employee *e, std::string filename);
void readEmployee(Employee *e, std::string filename); 


int main()
{
	// 1, 5. Создайте проект из 2х cpp файлов и заголовочного (main.cpp, mylib.cpp, mylib.h)
	// во втором модуле mylib объявить 3 функции: для инициализации массива (типа float),
	// печати его на экран и подсчета количества отрицательных и положительных элементов.
	// Вызывайте эти 3-и функции из main для работы с массивом.
	const size_t SIZE = 10;
	float arr[SIZE];
	mylib::init_array(arr, SIZE);
	mylib::print_array(arr, SIZE);
	std::cout << "\n";
	int pos, neg;
	mylib::count_pos_neg(arr, SIZE, pos, neg);
	std::cout << "Number of positive elements in array is " << pos << "\n";
	std::cout << "Number of negative elements in array is " << neg << "\n";

	// 2. Используем макрос -- работает :)
	std::cout.setf(std::ios_base::boolalpha);
	std::cout << CHECK_RANGE(5, 10) << std::endl;			// true 
	std::cout << CHECK_RANGE(2 + 3, 10) << std::endl;		// true
	std::cout << CHECK_RANGE(10, 10) << std::endl;			// false

	// 3. тестируем все три алгоритма
	int test_array[ARR_SIZE]; //= {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	init_array(test_array, ARR_SIZE);
	print_array(test_array, ARR_SIZE);
	std::cout << std::endl;
	
	bubble_sort(test_array, ARR_SIZE);
	print_array(test_array, ARR_SIZE);
	bubble_sort(test_array, ARR_SIZE, false);
	print_array(test_array, ARR_SIZE);
	std::cout << std::endl;
	
	selection_sort(test_array, ARR_SIZE);
	print_array(test_array, ARR_SIZE);
	selection_sort(test_array, ARR_SIZE, false);
	print_array(test_array, ARR_SIZE);
	std::cout << std::endl;
	
	insertion_sort(test_array, ARR_SIZE);
	print_array(test_array, ARR_SIZE);
	insertion_sort(test_array, ARR_SIZE, false);
	print_array(test_array, ARR_SIZE);
	std::cout << std::endl;

	// 4. Для упрощения отладки решил заполнить структуру без пользовательского ввода
	Employee  *emp1 = new Employee;
	emp1->first_name = "Lewis";
	emp1->last_name = "Carroll";
	emp1->id = 27011832;
	emp1->salary = 1116.00;

	writeEmployee(emp1, "employee.csv");

	Employee *emp2 = new Employee;

	readEmployee(emp2, "employee.csv");

	std::cout << "Employee name is " << emp2->first_name << " ";
	std::cout << emp2->last_name << ".\n";
	std::cout << "Employee ID is " << emp2->id << "\n";
	std::cout << "Employee salary is $" << emp2->salary << "\n";
	std::cout << "The size of Employee is " << sizeof (*emp2) << ".\n";

	return 0;
}

// 3. Задайте массив типа int. Пусть его размер задается через директиву препроцессора #define.
// Инициализируйте его через ввод с клавиатуры. Напишите для него свою функцию сортировки (например Пузырьком).
// Реализуйте перестановку элементов как макрокоманду SwapINT(a, b). Вызывайте ее из цикла сортировки.
void print_array(const int arr[], const size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

void init_array(int arr[], const size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		std::cout << "Enter integer value #" << (i + 1) << ": ";
		std::cin >> arr[i];
	}
}

void bubble_sort(int arr[], const size_t size, bool ascending)
{
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = i; j < size; ++j) {
			if (arr[i] > arr[j] && ascending) {
				SWAP(arr[i], arr[j]);
			}
			else if (arr[i] < arr[j] && !ascending) {
				SWAP(arr[i], arr[j]);
			}
		}
	}
} 

void selection_sort(int arr[], const size_t size, bool ascending)
{
	size_t i, j, min;
	for (i = 0; i < size - 1; ++i) {
		min = i;
		for (j = i + 1; j < size; ++j) {
			if (arr[j] < arr[min] && ascending) {
				SWAP(arr[j], arr[min]);
			}
			else if (arr[j] > arr[min] && !ascending) {
				SWAP(arr[j], arr[min]);
			}
		}
	}
}

void insertion_sort(int arr[], const size_t size, bool ascending)
{
	size_t in, out;
	for (out = 1; out < size; ++out) {
		int temp = arr[out];
		in = out;
		while (in > 0) {
			if (ascending && temp < arr[in - 1]) {
				arr[in] = arr[in - 1];
			}
			else if (!ascending && temp > arr[in - 1]) {
				arr[in] = arr[in - 1];
			}
			--in;
		}
		arr[in] = temp;
	}
}

// 4. Функции для сохранения в файл и чтения из файла структуры
void writeEmployee(const Employee *e, std::string filename)
{
	std::ofstream fout;
	fout.open(filename, std::ios::out | std::ios::app);
	
	if (fout.is_open()) {
		std::string s;
		s += e->first_name + ",";
		s += e->last_name + ",";
		s += std::to_string(e->id) + ",";
		s += std::to_string(e->salary) + "\n";
		fout << s;
		fout.close();
	}
	else {
		std::cout << "Cannot open file \"" << filename << "\" to write.\n";
	}
}

// потратил на отладку этой функции полдня :)
// жаль, что в С++ нет удобной функции split как в Java или Python :)
// зато разобрался с тем, что substr вторым аргументом принимает не позицию
// окончания чтрения (как в Java или Python), а количество символов, которые нужно прочесть
void readEmployee(Employee *e, std::string filename)
{
	std::ifstream fin(filename, std::ios::in);

	if (fin) {
		int start{0}, stop{0};
		std::string t;
		std::getline(fin, t);
		fin.close();
		stop = t.find(',');
		e->first_name = t.substr(start, stop);
		start = stop + 1;
		stop  = t.find(',', start);
		e->last_name = t.substr(start, stop - start);
		start = stop + 1;
		stop = t.find(',', start);
		e->id = std::stoi(t.substr(start, stop - start));
		start = stop + 1;
		e->salary = std::stod(t.substr(start));
	}
	else {
		std::cout << "Cannot open file \"" << filename << "\" to read.\n";
	}
}