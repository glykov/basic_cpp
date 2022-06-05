
// #pragma once
// мы легких путей не ищем
#ifndef MYLIB_H
#define MYLIB_H

namespace mylib
{
	void init_array(float arr[], const size_t size);
	void print_array(float arr[], const size_t size);
	void count_pos_neg(float arr[], const size_t size, int &pos, int &neg);
};

#endif