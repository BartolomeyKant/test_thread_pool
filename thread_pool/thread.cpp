#include "thread.hh"

namespace thread_pool
{
	using namespace std;

	Thread::~Thread()
	{
		_need_work.store(false);
		_cv_action.notify_one();
		if (!_is_busy.load() && _thr.joinable())
		{
			// дожидаемся безопасного завершения потока
			_thr.join();
		}
		else
		{
			// иначе даже не пытаемся дождаться, вызываем terminate
			_thr.~thread();
		}
	}

	void Thread::start()
	{
		_thr = thread(&Thread::loop, this);
		_is_started = true;
	}

	void Thread::run_action(const shared_ptr<Action> &action)
	{
		unique_lock<mutex> lk(_action_mutex);
		_is_busy.store(true);
		_action = action;
		_cv_action.notify_one();
	}

	void Thread::loop()
	{
		unique_lock<mutex> lk(_cv_mutex);

		// лочит мутекс и возвращает true, так что это можно использовать в условии while
		auto lock_if = [&mx = this->_action_mutex](bool val)
		{
			mx.lock();
			if (!val) {
				mx.unlock();
			}
			return val;
		};
		do
		{
			while (lock_if(static_cast<bool>(_action)))
			{
				_action->run();
				_action.reset();
				_action_mutex.unlock();
				if (_cb_end_action)
				{
					// при вызове _cb_end_action может обновится _action
					_cb_end_action(*this);
				}
			}
			_is_busy.store(false);
			_cv_action.wait(lk);
		} while (_need_work.load());
	}

} // namespace thread_pool
