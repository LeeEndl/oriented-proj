/*

 No copyright claims if use.
 credits are required if distributed -- not required but respectfully try to.

 !! this project is for fun and has no serious future plans. !!

  | writter: LeeEndl
  | comments: LeeEndl
  | coding style: LeeEndl & https://en.cppreference.com/w/cpp & std toolchain
 
*/

#include <unordered_map> // obviously std::unordered_map<>{...}
#include <future> // std::async

#include <iostream> // std::cout
#include <fstream> // std::(i)fstream & std::(o)fstream

std::mutex timed{};
template<typename Re> std::unordered_map<long long, std::future<Re>> channel{};

template<typename stream, typename Tr, typename... Tw>
static Tr file(const char* path, std::format_string<Tw...> format, Tw&&... peram) {
	if (std::string{ typeid(stream).name() }.find("ofstream") not_eq std::string::npos) {
		std::ofstream o{ path };
		// todo: implement buffers for massive information
		std::print(o, format, std::forward<Tw>(peram)...);
	}
	else if (std::string{ typeid(stream).name() }.find("ifstream") not_eq std::string::npos) {
		std::ifstream i{ path };
		Tr cache{};
		for (std::string line; getline(i, line); ) // credits: https://en.cppreference.com/w/cpp/string/basic_string/getline
			cache += std::move(line);
		return std::move(cache);
	}
	return std::move(Tr{});
}

static void route(const long long& pos)
{
	/* take turns for high trafficing, 'push' back if already locked. */
push: if (timed.try_lock())
{
	std::cout << file<std::ifstream, std::string>(std::format("{0}.txt", pos).c_str(), "{0}", "this is a test") << std::endl;
	timed.unlock();
	channel<void>.erase(pos);
}
else goto push;
}

int main()
{
	long long tofrom{};
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
		channel<void>.emplace(++tofrom, std::async(std::launch::async, route, tofrom));
	}
	return 0;
}