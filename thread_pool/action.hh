#ifndef ACTION_HH
#define ACTION_HH

#include <functional>
#include <type_traits>

namespace thread_pool
{
	/**
	 * Базовый класс для описания действия
	*/
	class ActionBase
	{
	public:
		virtual void run() = 0;
	};

	class Action : public ActionBase
	{
	public:
		Action(std::function<void()> func) : _action(func) {}
		void run() override
		{
			_action();
		}

	private:
		std::function<void()> _action;
	};

	template <class Callable, class... Args>
	Action *make_action(Callable func, Args... args)
	{
		auto action = [args..., func]() -> void
		{
			func(args...);
		};
		return new Action(action);
	}
} // namespace thread_pool
#endif /*ACTION_HH*/
