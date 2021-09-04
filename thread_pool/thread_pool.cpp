#include "thread_pool.hh"

namespace thread_pool
{
	using namespace std;

	ThreadPool::ThreadPool()
	{
		auto hw_threads = std::thread::hardware_concurrency();
		printf("create %d threads\n", hw_threads);
		// добавить  hw_threads потоков
		for (auto i = 0; i < hw_threads; i++)
		{
			_threads.push_back(make_unique<Thread>());
			// Добавить callback для уведомления об окончании действия
			_threads.back()->_cb_end_action = [this](Thread &thread)
			{
				action_completed(thread);
			};
		}
	}

	void ThreadPool::run_action(const shared_ptr<Action> &action)
	{
		unique_lock<mutex> lk(_pending_mutex);

		// найти свободный поток и запустить его на выполнение
		for (auto &t : _threads)
		{
			if (!t->is_busy())
			{
				if (!t->is_started())
				{
					t->start();
				}
				t->run_action(action);
				// действие запущено, можно выйти
				return;
			}
		}
		// действие не было запущено, откладываем его в очередь
		_pending_actions.push(action);
	}

	void ThreadPool::action_completed(Thread &thread)
	{
		unique_lock<mutex> lk(_pending_mutex);
		// поток сообщил, что закончил выполнение действия, найдем ему другое
		if (!_pending_actions.empty())
		{
			// !!! так как этот callback вызывается из потока thread
			// следует учитывать, что run_action только добавляет
			// action в thread
			// непосредственный запуск, произойдет после завершения callback'а
			thread.run_action(move(_pending_actions.front()));
			// удаляем переданный элемент
			_pending_actions.pop();
		}
	}

} // namespace thread_pool
