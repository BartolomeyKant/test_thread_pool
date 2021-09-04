#include "action.hh"

namespace thread_pool
{
	using namespace std;

	void Action::wait()
	{
		unique_lock<mutex> lock(_wait_mutex);
		if (!_is_complete)
		{
			lock.unlock();
			mutex mm;
			unique_lock<mutex> lk(mm);
			// ожидание пока нам не дадут сигнал об окончании и _is_complete не станет true
			_wait_cv.wait(lk, [&]()
						  { return this->_is_complete; });
		}
	}

	void Action::run()
	{
		// запускаем функцию
		do_action();

		// даём сигнал завершения выполнения
		unique_lock<mutex> lock(_wait_mutex);
		_is_complete = true;
		_wait_cv.notify_all();
	}

	void ActionResult::wait()
	{
		return _action->wait();
	}

} // namespace thread_pool
