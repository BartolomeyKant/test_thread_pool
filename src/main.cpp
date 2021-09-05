#include <iostream>

#include <string>
#include <regex>
#include <list>
#include <thread_pool/thread_pool.hh>

#include "src/simple_delims.hh"

using namespace std;

thread_pool::ThreadPool pool;

list<thread_pool::ActionResult> tasks;

void restart_pool() {
	pool = thread_pool::ThreadPool(pool.threads_num() + 1);
	cout << "recreate all threads and add new one" << endl;
	tasks.clear();
}

void do_simple_delims(SimpleDelims::num_t N, string task_name)
{
	auto res = SimpleDelims::gen_simple_delims(N);
	cout << "result for " << task_name << endl;
	cout << "[";
	for (auto n : res.get())
	{
		cout << n << " ";
	}
	cout << "]" << endl;
}

void simple_delims(const string &num, const string &prio)
{
	try
	{
		SimpleDelims::num_t N = stoull(num);
		int P = stol(prio);
		string task_name = "task_" + to_string(tasks.size());
		cout << "start task for find all simple delimiters N:" << N << " P:" << P
			 << " name " << task_name << endl;

		auto res = pool.run_action(&do_simple_delims, N, task_name);
		res.set_priority(P);
		tasks.push_back(res);
	}
	catch (exception &e)
	{
		cerr << "get wrong numbers " << num << " " << prio << " " << e.what() << endl;
	}
}

void run_loop()
{
	bool do_work = true;
	// для парсинга чисел будем использовать регех
	smatch m;
	regex re(R"((\d+) +(\d+))");
	char buf[2048];

	auto usage = []()
	{
		cout << "Try one of the following commands:" << endl
			 << "\t"
			 << "restart - for restart current thread pool and loose all results" << endl
			 << "\t"
			 << "exit - wait for complete all tasks, print result and exit" << endl
			 << "\t"
			 << "<N> <P> - two numbers - start task for calculate all simple delimiters for N with priority P" << endl
			 << "\t" << "help - for print this message" << endl;
	};
	// читаем пользовательский ввод и разбираем, что он от нас хочет
	while (do_work)
	{
		// печатем промпт
		cout << "> ";
		cin.getline(buf, sizeof(buf));
		string command = buf;

		if (!cin.good()) {
			do_work = false;
		}

		if (command == "exit") {
			do_work = false;
		}
		else if (command == "restart") {
			restart_pool();
		}
		else if(regex_match(command, m, re)) {
			simple_delims(m[1], m[2]);
		}
		else if (command == "help")
		{
			usage();
		}
		else if (command.empty()) {
			continue;
		}
		else {
			cerr << "Get unknown command" << endl;
			usage();
		}
	}

	for(auto& r: tasks) {
		r.wait();
	}
}

int main()
{
	run_loop();

	return 0;
}
