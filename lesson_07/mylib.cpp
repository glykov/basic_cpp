#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mylib.h"

namespace mylib
{
	void init_array(float arr[], const size_t size)
	{
		srand(static_cast<unsigned> (time(0)));
		for (size_t i = 0; i < size; ++i) {
			arr[i] = 0.5 - static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
		}

	}

	void print_array(float arr[], const size_t size)
	{
		std::cout.setf(std::ios_base::fixed);
		std::cout.precision(2);
		for (size_t i = 0; i < size; ++i) {
			std::cout << arr[i] << " ";
		}
	}

	// тут, на мой взгляд, возможно несколько решений:
	// 1. функция может просто печатать количество положительных и отрицательных элементов
	// 2. функция может возвращать количество положительных и отрицательных элементов для дальнейшей обработки
	// 2.1. можно создать структуру типа Pair {int pos; int neg} и возвращать ее
	// 2.2. можно возвращать через аргументы переданные по значению - я выбрал этот вариант
	// 2.3. можно создать двухэлементный динамический массив и возвращать указатель на него 
	// (главное не забыть его удалить в вызывающей функции)
	void count_pos_neg(float arr[], const size_t size, int &pos, int &neg)
	{
		pos = neg = 0;
		for (size_t i = 0; i < size; ++i) {
			if (arr[i] < 0) {
				++neg;
			}
			else {
				++pos;
			}
		}
	}
};