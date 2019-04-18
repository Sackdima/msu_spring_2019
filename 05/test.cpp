#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

std::condition_variable cond;
std::mutex m;
const int N = 500000;
bool ping_flag = true;

void ping()
{
	for (size_t i = 0; i < N; i++)
	{
		std::unique_lock<std::mutex> lock(m);
		cond.wait(lock, []() { return ping_flag; });
		std::cout << "ping" << '\n';
		ping_flag = !ping_flag;
		cond.notify_one();
	}
}

void pong()
{
	for (size_t i = 0; i < N; i++)
	{
		std::unique_lock<std::mutex> lock(m);
		cond.wait(lock, []() { return !ping_flag; });
		std::cout << "pong" << '\n';
		ping_flag = !ping_flag;
		cond.notify_one();
	}
}

int main()
{
	std::thread ping_t(ping);
	std::thread pong_t(pong);

	ping_t.join();
	pong_t.join();

	return 0;
}
