#include <iostream>

int main()
{
	/*
	* 1. Написать программу, проверяющую что сумма двух (введенных с клавиатуры) чисел лежит в пределах от 10 до 20 (включительно), 
	* если да – вывести строку "true", в противном случае – "false";
	*/
	int a,b, sum;
	std::cout << "Enter two numbers: ";
	std::cin >> a >> b;
	sum = a + b;
	if (sum >= 10 && sum <= 20) {
		std::cout << "true\n";
	}
	else {
		std::cout << "false\n";
	}
	// 2-й вариант решения
	std::cout << std::boolalpha << (sum >= 10 && sum <= 20) << std::endl;
	// 3-й вариант решения
	bool in_range = (sum >= 10 && sum <= 20);
	std::cout << std::boolalpha << in_range << std::endl;

	/*
	* 2. Написать программу, выводящую на экран строку “true”, если две целочисленные константы, 
	* объявленные в её начале либо обе равны десяти сами по себе, либо их сумма равна десяти. Иначе "false".
	*/
	const int x = 5;
	const int y = 5;
	bool is_ten = (x == 10 && y == 10 || x + y == 10);
	std::cout << std::boolalpha << is_ten << std::endl;

	/*
	* 3. Написать программу которая выводит на экран список всех нечетных чисел он 1 до 50.
	* Например: "Your numbers: 1 3 5 7 9 11 13 ...". Для решения используйте любой С++ цикл.
	*/
	std::cout << "Your numbers: ";
	for (int i = 1; i <= 50; ++i) {
		if (i % 2 == 1) {				// можно и так if (i % 2 != 0)
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
	// 2-й вариант решения
	std::cout << "Your numbers again: ";
	int i = 1;
	while (i <= 50) {
		std::cout << i << " ";
		i += 2;
	}
	std::cout << std::endl;

	/*
	* 4. Написать программу, проверяющую, является ли некоторое число - простым. 
	* Простое число — это целое положительное число, которое делится без остатка только на единицу и себя само.
	*/
	std::cout << "Enter a number to check if it is prime: ";
	int n;
	bool is_prime = true;
	std::cin >> n;
	if (n == 1) {
		std::cout << n << " is neither prime nor composite number.\n";
	}
	else {
		for (int j = 2; j <= n / 2; ++j) {		// вообще проверку нужно проводить до j <= sqrt(n), решил обойтись без cmath
			if (n % j == 0) {
				is_prime = false;
				break;
			}
		}
		std::cout << (is_prime ? "it is prime number" : "it is composite number") << std::endl;
	}

	/*
	* 5. Пользователь вводит с клавиатуры число (год): от 1 до 3000.
	* Написать программу, которая определяет является ли этот год високосным.
	* Каждый 4-й год является високосным, кроме каждого 100-го, при этом каждый 400-й – високосный. Вывести результаты работы программы в консоль.
	*/
	int year;
	std::cout << "Enter a year: ";
	std::cin >> year;
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		std::cout << "this is leap year.\n";
	}
	else {
		std::cout << "this is not leap year.\n";
	}

	return 0;
}