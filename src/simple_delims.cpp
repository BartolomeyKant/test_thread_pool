#include "src/simple_delims.hh"

#include <stdio.h>

using namespace std;

SimpleDelims SimpleDelims::gen_simple_delims(num_t N)
{
	return SimpleDelims(N);
}

SimpleDelims::SimpleDelims(num_t N)
{
	init(N);
}

SimpleDelims::SimpleDelims(const SimpleDelims &other) : _list(other._list)
{
}

SimpleDelims::SimpleDelims(SimpleDelims &&other) : _list(move(other._list))
{
}

void SimpleDelims::init(num_t N)
{
	// перебираем все числа меньше чем sqrt(N)
	// при переполнении sqrt будет меньше i
	for (num_t i = 2, sqrt = i * i; !(sqrt > N || sqrt < i); i++, sqrt = i * i)
	{
		// если N делится без остатка на i и i простое число, добавим его в список
		if (N % i == 0 && is_prime(i))
		{
			_list.push_back(i);
		}
	}
}

bool SimpleDelims::is_prime(num_t n)
{
	if (n == 1 || n == 0)
	{
		return false;
	}
	// список известных простых чисел
	// остальное вычислим
	constexpr num_t simples[] = {2, 3, 5, 7, 11};

	// проверяем известный список
	for (num_t i : simples)
	{
		if (n == i)
		{
			return true;
		}
	}

	// проверяем по нормальному
	for (num_t i = simples[4] + 1, sqrt = i * i; !(sqrt > n || sqrt < i); i++, sqrt = i * i)
	{
		// n не должно ни на что делится кроме себя
		if (n % i == 0)
		{
			return false;
		}
	}

	return true;
}
