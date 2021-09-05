#include "thread_pool.hh"

namespace thread_pool
{
	using namespace std;

	ThreadPool::ThreadPool(uint num)
	{
		if (num == 0)
		{
			num = std::thread::hardware_concurrency();
		}
		// добавить  hw_threads потоков
		for (auto i = 0; i < num; i++)
		{
			_threads.push_back(make_unique<Thread>());
			// Добавить callback для уведомления об окончании действия
			_threads.back()->_cb_end_action = [this](Thread &thread)
			{
				action_completed(thread);
			};
		}
	}
	ThreadPool::ThreadPool(ThreadPool &&other)
		: _threads(move(other._threads)),
		  _pending_actions(move(other._pending_actions))
	{
	}

	ThreadPool& ThreadPool::operator=(ThreadPool&& other)
	{
		if (this != &other) {
			_threads = move(other._threads);
			_pending_actions = move(other._pending_actions);
			_pending_mutex.unlock();
		}
		return *this;
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
		add_to_pending(action);
	}

	void ThreadPool::add_to_pending(const std::shared_ptr<Action> &action)
	{
		_pending_actions.push_back(action);
	}

	std::shared_ptr<Action> ThreadPool::get_action()
	{
		// ищем действие с максимальным приоритетом
		pair<int, decltype(_pending_actions.begin())> max_prio;
		for (auto it = _pending_actions.begin(); it != _pending_actions.end(); it++)
		{
			if ((*it)->priority() > max_prio.first)
			{
				max_prio.first = (*it)->priority();
				max_prio.second = it;
			}
		}
		auto a = *max_prio.second;
		_pending_actions.erase(max_prio.second);
		return a;
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
			thread.run_action(get_action());
		}
	}

} // namespace thread_pool
