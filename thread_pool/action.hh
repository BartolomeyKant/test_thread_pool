#ifndef ACTION_HH
#define ACTION_HH

#include <mutex>
#include <condition_variable>
#include <functional>

namespace thread_pool
{
	class Thread;
	/**
	 * Класс для сохранения созданного действия
	*/
	class Action
	{
	public:
		virtual ~Action() = default;
		/**
		 * Блокирует текущий поток до завершения работы действия.
		 * Если действие уже завершено, то возвращает управление немедленно.
		*/
		void wait();

	protected:
		virtual void do_action() = 0;

	private:
		// добавляем дружеский класс для удобства доступа к run
		friend class Thread;
		void run();

		bool _is_complete = false;
		std::mutex _wait_mutex;
		std::condition_variable _wait_cv;
	};

	/**
	 * Шаблонная реализация действия
	 * R - тип результата выполнения действия
	*/
	template <class R>
	class ActionImpl : public Action
	{
	public:
		ActionImpl(std::function<R()> f) : _func(f) {}

		/**
		 * Получить результат выполнения
		*/
		const R &res() const { return *_res; }
		/**
		 * Получить результат выполнения
		*/
		R &res() { return *_res; }

	protected:
		void do_action() { _res = std::make_unique<R>(_func()); }

	private:
		std::unique_ptr<R> _res;
		std::function<R()> _func;
	};

	template <>
	class ActionImpl<void> : public Action
	{
	public:
		ActionImpl(std::function<void()> f) : _func(f) {}

	protected:
		void do_action() { _func(); }

	private:
		std::function<void()> _func;
	};

	class ActionResult
	{
	public:
		ActionResult(const std::shared_ptr<Action> &action) : _action(action) {}
		ActionResult(const ActionResult &other) : _action(other._action){};
		ActionResult(ActionResult &&other)
		{
			std::swap(_action, other._action);
		}
		virtual ~ActionResult() = default;

		/**
			 * Блокирует текущий поток до завершения работы действия.
			 * Если действие уже завершено, то возвращает управление немедленно.
			*/
		void wait();

	protected:
		std::shared_ptr<Action> _action;
	};

	template <class R>
	class ActionResultImpl : public ActionResult
	{
	public:
		ActionResultImpl(const std::shared_ptr<Action> &action) : ActionResult(action) {}
		ActionResultImpl(const ActionResultImpl &other) : ActionResult(other._action){};
		ActionResultImpl(ActionResultImpl &&other) : ActionResult(std::forward<ActionResultImpl>(other._action))
		{
		}

		/**
		 * Получить результат выполнения
		*/
		const R &res() const { return std::dynamic_pointer_cast<ActionImpl<R>>(_action)->res(); }
		/**
		 * Получить результат выполнения
		*/
		R &res() { return std::dynamic_pointer_cast<ActionImpl<R>>(_action)->res(); }
	};

	template <>
	class ActionResultImpl<void> : public ActionResult
	{
	public:
		ActionResultImpl(const std::shared_ptr<Action> &action) : ActionResult(action) {}
		ActionResultImpl(const ActionResultImpl &other) : ActionResult(other._action){};
		ActionResultImpl(ActionResultImpl &&other) : ActionResult(std::forward<ActionResultImpl>(other._action))
		{
		}
	};
} // namespace thread_pool
#endif /*ACTION_HH*/
