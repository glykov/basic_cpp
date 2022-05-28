#include <iostream>

// 1. Написать функцию которая выводит массив double чисел на экран. Параметры функции это сам массив и его размер. Вызвать эту функцию из main.
void print_array(const double *darray, const int size);
// 2. Написать функцию, заменяющую в принятом массиве 0 на 1, 1 на 0. 
// Заодно, еще и перегрузка функции получилась
void print_array(const int *iarray, const int size);
void change_array(int *iarray, const int size);
// 3. Задать пустой целочисленный массив размером 8. Написать функцию, которая с помощью цикла заполнит его значениями 1 4 7 10 13 16 19 22
// решил использовать аргументы по умолчанию
void fill_array(int *iarray, const int size, int step = 1, int start = 1);
// 4. Написать функцию, которой на вход подаётся одномерный массив и число n (может быть положительным, или отрицательным), 
// при этом метод должен циклически сместить все элементы массива на n позиций.
void cyclic_shift(int *array, const int size, int shift);
// 5. Написать функцию, которой передается не пустой одномерный целочисленный массив, она должна вернуть истину если в массиве есть место, 
// в котором сумма левой и правой части массива равны. 
bool check_balance(const int array[], const size_t size);

int main()
{
	// 1. Написать функцию которая выводит массив double чисел на экран. Параметры функции это сам массив и его размер. Вызвать эту функцию из main.
	const int ARR_SIZE = 10;
	double my_darray[ARR_SIZE] = {1.9, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1, 10.0};
	print_array(my_darray, ARR_SIZE);
	std::cout << "\n";

	// 2. Написать функцию, заменяющую в принятом массиве 0 на 1, 1 на 0. 
	int my_iarray[ARR_SIZE] {1, 1, 0, 0, 1, 0, 1, 1, 0, 0};
	print_array(my_iarray, ARR_SIZE);
	std::cout << "\n";
	change_array(my_iarray, ARR_SIZE);
	print_array(my_iarray, ARR_SIZE);
	std::cout << "\n";

	// 3. Задать пустой целочисленный массив размером 8. Написать функцию, которая с помощью цикла заполнит его значениями 1 4 7 10 13 16 19 22
	const int SIZE = 8;
	int my_iarray_2[SIZE] {0};
	fill_array(my_iarray_2, SIZE);			// заполняем по умолчанию
	print_array(my_iarray_2, SIZE);			// выводим массив, заполненный по умолчанию
	std::cout << std::endl;
	fill_array(my_iarray_2, SIZE, 3);		// звполняем как указано в задании
	print_array(my_iarray_2, SIZE);			// выводи массив, заполненный в соответсвии с заданием
	std::cout << std::endl;
	fill_array(my_iarray_2, SIZE, 5, 10);	// звполняем с использованием всех параметров
	print_array(my_iarray_2, SIZE);			// выводи массив, заполненный в соответсвии с заданием
	std::cout << std::endl;
	
	std::cout << std::endl;

	// 4. Написать функцию, которой на вход подаётся одномерный массив и число n (может быть положительным, или отрицательным), 
	// при этом метод должен циклически сместить все элементы массива на n позиций.
	fill_array(my_iarray_2, SIZE);
	print_array(my_iarray_2, SIZE);			// выводи массив, заполненный в соответсвии с заданием
	std::cout << std::endl;
	cyclic_shift(my_iarray_2, SIZE, -2);
	print_array(my_iarray_2, SIZE);			// выводи массив, заполненный в соответсвии с заданием
	std::cout << std::endl;
	cyclic_shift(my_iarray_2, SIZE, 3);
	print_array(my_iarray_2, SIZE);			// выводи массив, заполненный в соответсвии с заданием
	std::cout << std::endl;

	// 5. Написать функцию, которой передается не пустой одномерный целочисленный массив, она должна вернуть истину если в массиве есть место, 
	// в котором сумма левой и правой части массива равны. 
	std::cout << std::endl;
	const int CHK_ARR_SIZE = 5;
	int array_01[CHK_ARR_SIZE] = {1, 1, 1, 2, 1};
	int array_02[CHK_ARR_SIZE] = {2, 1, 1, 2, 1};
	int array_03[CHK_ARR_SIZE] = {10, 1, 2, 3, 4};
	std::cout << std::boolalpha << check_balance(array_01, CHK_ARR_SIZE) << std::endl;	// true
	std::cout << std::boolalpha << check_balance(array_02, CHK_ARR_SIZE) << std::endl;	// false
	std::cout << std::boolalpha << check_balance(array_03, CHK_ARR_SIZE) << std::endl;	// true
	return 0;
}

// 1. Написать функцию которая выводит массив double чисел на экран. Параметры функции это сам массив и его размер. Вызвать эту функцию из main.
void print_array(const double *darray, const int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << darray[i] << " ";
	}
}

// 2. Написать функцию, заменяющую в принятом массиве 0 на 1, 1 на 0. 
// Заодно, еще и перегрузка функции получилась
void print_array(const int *iarray, const int size)
{
	for (int i = 0; i < size; ++i) {
		std::cout << iarray[i] << " ";
	}
}
// варианты без XOR включают проверку значения с заменой --> if (a[i] == 0) {a[i] = 1;} else {a[i] = 0;}
// или с помощью тернарного оператора a[i] = (a[i] == 0 ? 1 : 0);
void change_array(int *iarray, const int size)
{
	for (int i = 0; i < size; ++i) {
		iarray[i] ^= 1;
	}
}

// 3. Задать пустой целочисленный массив размером 8. Написать функцию, которая с помощью цикла заполнит его значениями 1 4 7 10 13 16 19 22
void fill_array(int *iarray, const int size, int step, int start)
{
	iarray[0] = start;
	for (int i = 1; i < size; ++i) {
		iarray[i] = iarray[i - 1] + step;
	}
}

// 4. Написать функцию, которой на вход подаётся одномерный массив и число n (может быть положительным, или отрицательным), 
// при этом метод должен циклически сместить все элементы массива на n позиций.
void cyclic_shift(int *array, const int size, int shift)
{
	if (shift < 0) {
		shift *= -1;
		for (int i = 0; i < shift; ++i) {
			int tmp = array[0];
			for (int j = 0; j < size - 1; ++j) {
				array[j] = array[j + 1]; 
			}
			array[size - 1] = tmp;
		}
	}
	else {
		for (int i = 0; i < shift; ++i) {
			int tmp = array[size - 1];
			for (int j = size - 1; j > 0; --j) {
				array[j] = array[j - 1];
			}
			array[0] = tmp;
		}
	}
}

// 5. Написать функцию, которой передается не пустой одномерный целочисленный массив, она должна вернуть истину если в массиве есть место, 
// в котором сумма левой и правой части массива равны. 
bool check_balance(const int array[], const size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		long long balance = 0;					// чтобы не было переполнения
		for (size_t j = 0; j <= i; ++j) {		// суммируем все элементы до i включительно
			balance += array[j];
		}
		for (size_t k = i + 1; k < size; ++k) {	// вычитаем все элементы после i
			balance -= array[k];
		}
		if (balance == 0) {						// если сумма элементов до i включительно равна сумме элементов после i
			return true;
		}
	}
	return false;
}
