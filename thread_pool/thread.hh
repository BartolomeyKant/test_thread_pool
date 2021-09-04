#ifndef THREAD_HH
#define THREAD_HH

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>

#include "action.hh"

namespace thread_pool
{
	class ThreadPool;
	/**
	 * Класс обертка над стандартным thread
	*/
	class Thread
	{
	public:
		Thread() = default;
		~Thread();

		/**
		 * Запуск непосредственно потока
		*/
		void start();

		/**
		 * Задать новое действие для потока и запустить выполнение
		 * 
		 * @param action действие, которое нужно запустить в этом потоке
		*/
		void run_action(std::unique_ptr<ActionBase> &&action);

		/**
		 * Проверка, если поток занят
		*/
		bool is_busy() const { return _is_busy.load(); }

		/**
		 * Проверка, был ли поток запущен @see Thread::start
		*/
		bool is_started() const { return _is_started; }

	private:
		void loop();

		bool _is_started = false;
		std::atomic_bool _is_busy = false;
		std::atomic_bool _need_work = true;

		std::thread _thr;
		std::mutex _cv_mutex;
		std::condition_variable _cv_action;

		std::unique_ptr<ActionBase> _action;

		// для упрощения доступа к _cb_end_action добавляем friend class 
		friend class ThreadPool;
		// callback для уведомления об окончании выполнения действия
		std::function<void(Thread &)>
			_cb_end_action;
	};

	struct ThreadBusyException : public std::runtime_error
	{
		ThreadBusyException() : std::runtime_error("thread already busy in action") {}
	};
} // namespace thread_pool

#endif /*THREAD_HH*/
