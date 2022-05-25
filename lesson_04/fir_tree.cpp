// fir_tree.cpp -- Елочка
#include <iostream>

int main()
{
	std::cout << "Enter a fir-tree size: ";
	int fir_tree_size{ 0 };
	std::cin >> fir_tree_size;
	for (int j = 1; j <= fir_tree_size; ++j) {
		for (int k = 0; k < fir_tree_size - j; ++k) {
			std::cout << " ";
		}
		for (int f = 0; f < j * 2 - 1; f++) {
			std::cout << "*";
		}
		std::cout << std::endl;
	}
}