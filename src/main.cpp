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

void do_work(int &res, int) {
	res = 10;
}

struct A {
	void foo(string& s) {
		s = "new value";
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
	string ss = "asdasd";
	auto res_wait1 = _pool.run_action(&A::foo, a, ss);

	auto res_wait2 = _pool.run_action(&do_sleep, 3, 4.12f);
	auto res_wait3 = _pool.run_action<void>([](int i, float f)
					 { cout << "run lambda " << i << " " << 4.3 << endl; },
					 5, 4.3f);

	auto res_wait4 = _pool.run_action<int>([](int a)->int {return a+1;}, 3);

	int change_value =  0;
	auto res_wait5 = _pool.run_action<void>([&change_value](){change_value++;});

	res_wait4.wait();
	cout << "wait4 res: " << res_wait4.res() << endl;

	res_wait5.wait();
	cout << "wait5 res: " << change_value << endl;

	res_wait1.wait();
	cout << "1 completed" << endl;
	res_wait2.wait();
	cout << "2 completed" << endl;
	res_wait3.wait();
	cout << "3 completed" << endl;

	return 0;
}
