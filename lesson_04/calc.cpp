#include <iostream>
#include <cstdlib>
#include <cmath>

int main(int argc, char *argv[])
{
	if (argc != 4) {
		std::cout << "Proper usage is: calc number1 number2 operation_sign.\n";
		std::cout << "possible operations are: + - * / % ^\n";
		return 1;
	}

	double x, y, result;
	char op;
	bool is_ok = true;
	x = atof(argv[1]);
	y = atof(argv[2]);
	op = argv[3][0];

	switch(op) {
		case '+':
			result = x + y;
			break;
		case '-':
			result = x - y;
			break;
		case '*':
			result = x * y;
			break;
		case '/':
			result = x / y;
			break;
		case '%':
			result = fmod(x, y);
			break;
		case '^':
			result = pow(x, y);
			break;
		default:
			is_ok = false;
	}

	if (is_ok) {
		std::cout << x << " " << op << " " << y << " = " << result << '\n';
	}
	else {
		std::cout << "you entered invalid operation sign.\n";
	}

	return 0;
}