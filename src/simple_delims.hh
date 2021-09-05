#ifndef  SIMPLE_DELIMS_HH
#define SIMPLE_DELIMS_HH

#include <inttypes.h>
#include <vector>

// Класс хранит список всех простых делителей числа N
class SimpleDelims {
public:
	using num_t = uint64_t;
	using num_list_t = std::vector<num_t>;

	/**
	 * Функция для генерации списка простых делителей числа N
	*/
	static SimpleDelims gen_simple_delims(num_t N);

private:
	// Инициализация числом N
	SimpleDelims(num_t N);
public:
	// конструктор копирования
	SimpleDelims(const SimpleDelims& other);
	// конструктор перемещения
	SimpleDelims(SimpleDelims &&other);

	const num_list_t &get() const { return _list; }

private:
	void init(num_t N);
	// проверяет правильность числа
	bool is_prime(num_t n);

	num_list_t _list;
};

#endif /*SIMPLE_DELIMS_HH*/
