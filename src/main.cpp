#include <iostream>

#include <thread>
#include <functional>

#include <thread_pool/thread_pool.hh>

using namespace std;


void do_sleep(int time, float d) {
	cout << "start action d value is " << d <<endl;
	for (auto i = 0; i < time; i++)
	{
		cout << "sleep for 1 sec d: " << d << endl << flush;
		this_thread::sleep_for(chrono::seconds(1));
	}
}

struct A {
	void foo(string s) {
		cout << "call to foo " << s << endl;
	}
};

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	thread_pool::ThreadPool _pool;

	auto fs = [](int i, float a)
	{ do_sleep(i, a); };

	function<void (int, float)> f = &do_sleep;

	A a;

	_pool.run_action([](int i, float f)
					 { cout << "run lambda " << i << " " << 4.3 << endl; },
					 5, 4.3f);
	_pool.run_action(&do_sleep, 8, 4.12f);
	_pool.run_action(&A::foo, a, string("asdasd"));

	this_thread::sleep_for(chrono::seconds(15));

	return 0;
}
