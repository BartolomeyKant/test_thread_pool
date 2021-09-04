#ifndef THREAD_POOL_HH
#define THREAD_POOL_HH

#include <list>
#include <queue>
#include <mutex>

#include "action.hh"
#include "thread.hh"

namespace thread_pool
{
	class ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool() = default;

		/**
		 * Запустить действие в свободном потоке.
		 * Действие запускается сразу же если есть свободный поток,
		 * иначе откладывается в очередь
		 * 
		 * @param func - функция член класса C
		 * @param obj - объект класса C
		 * @param args - список аргументов
		*/
		template <class Callable, class... Args>
		void run_action(Callable func, Args... args)
		{
			run_action(std::unique_ptr<ActionBase>(make_action(func, args...)));
		}

	private:
		// функция для запуска нового действия
		void run_action(std::unique_ptr<ActionBase> &&action);

		// callback, который вызывает поток thread при завершении действия
		void action_completed(Thread &thread);

		std::list<std::unique_ptr<Thread>> _threads;
		std::queue<std::unique_ptr<ActionBase>> _pending_actions;

		std::mutex _pending_mutex;
	};
} // namespace thread_pool

#endif /*THREAD_POOL_HH*/
