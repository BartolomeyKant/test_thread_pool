#include "src/simple_delims.hh"

#include <stdio.h>
#include <math.h>

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
	// перебираем все числа меньше чем N
	for (num_t i = 2; i <= N; i++)
	{
		// если N делится без остатка на i и i простое число, добавим его в список
		if (N % i == 0)
		{
			printf("delim %lu\n", i);
			if (is_prime(i))
			{
				_list.push_back(i);
			}
		}
	}
}

bool SimpleDelims::is_prime(num_t n)
{
	if (n == 1 || n == 0)
	{
		return false;
	}
	// проверяем известный список
	for (num_t i : _primes)
	{
		if (n == i)
		{
			return true;
		}
	}

	// проверяем только числа меньше квадратного корня
	num_t s = sqrt(n);
	// проверяем по нормальному
	// корень числа - вероятный делитель, поэтому начинаем проверку с него
	for (num_t i = s; i >= 2; i--)
	{
		// n не должно ни на что делится кроме себя
		if (n % i == 0)
		{
			return false;
		}
	}

	save_prime(n);
	return true;
}

mutex SimpleDelims::_primes_mutex = {};
SimpleDelims::num_list_t SimpleDelims::_primes = {};

void SimpleDelims::save_prime(num_t n) {
	unique_lock<mutex> lk(_primes_mutex);
	_primes.push_back(n);
}
