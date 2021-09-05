#ifndef THREAD_POOL_HH
#define THREAD_POOL_HH

#include <list>
#include <mutex>
#include <type_traits>

#include "action.hh"
#include "thread.hh"

namespace thread_pool
{
	class ThreadPool
	{
	public:
		ThreadPool(uint num = 0);
		ThreadPool(const ThreadPool &other) = delete;
		ThreadPool(ThreadPool &&other);
		~ThreadPool();

		ThreadPool &operator=(ThreadPool &&other);

		/**
		 * Запустить действие в свободном потоке.
		 * Действие запускается сразу же если есть свободный поток,
		 * иначе откладывается в очередь
		 *
		 * @param func - указатель на функцию член класса C
		 * @param obj - объект класса C
		 * @param args - список аргументов
		 *
		 * @note аргументы копируются по значению, объект передается и сохраняется по ссылке
		 * следить за временем жизни объекта obj ответственность пользователя.
		 * Если нужно передать аргументы по ссылке смотрите перегрузку
		 * @see run_action<Callable,Args...> и используйте
		 * лямбда функции
		*/
		template <class R, class C, class... Args>
		ActionResultImpl<R>
		run_action(R (C::*func)(Args... arg), C &obj, Args... args, int priority)
		{
			auto a = std::bind(func, obj, std::forward<Args>(args)...);
			auto ptr = std::make_shared<ActionImpl<R>>(a);
			auto a_res = ActionResultImpl<R>(ptr);
			run_action(std::dynamic_pointer_cast<Action>(ptr), priority);
			return a_res;
		}

		/**
		 * Запустить действие в свободном потоке.
		 * Действие запускается сразу же если есть свободный поток,
		 * иначе откладывается в очередь
		 *
		 * @param func - указатель на обычную функцию
		 * @param args - список аргументов
		 *
		 * @note аргументы копируются по значению.
		 * Если нужно передать аргументы по ссылке смотрите перегрузку
		 * @see run_action<Callable,Args...> и используйте
		 * лямбда функции
		*/
		template <class R, class... Args>
		ActionResultImpl<R>
		run_action(R (*func)(Args... arg), Args... args)
		{
			auto a = std::bind(func, std::forward<Args>(args)...);
			auto ptr = std::make_shared<ActionImpl<R>>(a);
			auto a_res = ActionResultImpl<R>(ptr);
			run_action(std::dynamic_pointer_cast<Action>(ptr));
			return a_res;
		}

		/**
		 * Запустить действие в свободном потоке.
		 * Действие запускается сразу же если есть свободный поток,
		 * иначе откладывается в очередь
		 *
		 * @param func - указатель на обычную функцию
		 * @param args - список аргументов
		 *
		 * @note аргументы копируются по значению.
		 * используйте лямбда функции
		 * [&val](int new_val){val = new_val;}
		*/
		template <class R, class Callable, class... Args>
		typename std::enable_if<std::is_invocable_r<R, Callable, Args...>::value, ActionResultImpl<R>>::type
		run_action(Callable func, Args &&...args)
		{
			auto a = std::bind(func, std::forward<Args>(args)...);
			auto ptr = std::make_shared<ActionImpl<R>>(a);
			auto a_res = ActionResultImpl<R>(ptr);
			run_action(std::dynamic_pointer_cast<Action>(ptr));
			return a_res;
		}

		/**
		 * Получить количество потоков
		*/
		uint threads_num() const { return _threads.size(); }

	private:
		// функция для запуска нового действия
		void run_action(const std::shared_ptr<Action> &action);

		// добавление действия в список ожидания
		void add_to_pending(const std::shared_ptr<Action> &action);

		// callback, который вызывает поток thread при завершении действия
		void action_completed(Thread &thread);
		std::shared_ptr<Action> get_action();

		std::list<std::unique_ptr<Thread>> _threads;
		std::list<std::shared_ptr<Action>> _pending_actions;

		std::mutex _pending_mutex;
	};
} // namespace thread_pool

#endif /*THREAD_POOL_HH*/
