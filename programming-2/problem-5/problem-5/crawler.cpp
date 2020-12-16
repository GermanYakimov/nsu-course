#include <thread>
#include <mutex>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "crawler.h"

using namespace std;

string path_to_test_data;


set<string> _discover_all_links(istream& in)
{
	string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

	set<string> discovered_links;
	size_t first = content.find("<a href=\"file://") + strlen("<a href=\"file://");

	while (true)
	{
		size_t second = content.find("\">", first);
		string str = content.substr(first, second - first);
		discovered_links.insert(path_to_test_data + str);

		first = content.find("<a href=\"file://", second);
		if (first == string::npos)
		{
			break;
		}
		first += strlen("<a href=\"file://");
	}

	return discovered_links;
}

void index(set<string>& pages_to_index, set<string>& indexed_pages, atomic_size_t& indicator, 
		   size_t id, mutex& storage_mutex, mutex& indexed_pages_mutex, condition_variable& cond_var)
{
	for (;;)
	{
		unique_lock<mutex> unique_storage_mutex(storage_mutex);
		cond_var.wait(unique_storage_mutex, [pages_to_index]{return !pages_to_index.empty(); });
		indicator++;

		string page_to_index = *pages_to_index.begin();
		pages_to_index.erase(page_to_index);
		unique_storage_mutex.unlock();

		fstream input(page_to_index);
		set<string> discovered_links = _discover_all_links(input);
		input.close();

		if (discovered_links.empty())
		{
			continue;
		}

		unique_lock<mutex> lock_storage(storage_mutex, defer_lock);
		unique_lock<mutex> lock_done_pages(indexed_pages_mutex, defer_lock);

		lock(lock_storage, lock_done_pages);

		for (string page : discovered_links)
		{
			if (indexed_pages.find(page) == indexed_pages.end() 
				&& pages_to_index.find(page) == pages_to_index.end())
			{
				pages_to_index.insert(page);
				cond_var.notify_one();
			}
		}
		indexed_pages.insert(page_to_index);
		cout << indexed_pages.size() << endl;

		lock_storage.unlock();
		lock_done_pages.unlock();
		indicator--;
	}
}

size_t get_pages_to_index_size(set<string>& pages_to_index, mutex& storage_mutex)
{
	lock_guard<mutex> lock_(storage_mutex);
	return pages_to_index.size();
}

void start_indexing(istream& input, ostream& output)
{
	string start_url;
	size_t workers_number;
	vector<thread> pool;

	atomic_size_t indicator = 0;

	set<string> pages_to_index;
	set<string> indexed_pages;

	mutex storage_mutex;
	mutex indexed_pages_mutex;
	condition_variable cond_var;

	input >> path_to_test_data;
	input >> start_url >> workers_number;
	start_url = path_to_test_data + start_url;

	pages_to_index.insert(start_url);
	auto start = chrono::system_clock().now();

	for (size_t i = 0; i < workers_number; i++)
	{
		pool.emplace_back(index, ref(pages_to_index), ref(indexed_pages), ref(indicator), 
			i, ref(storage_mutex), ref(indexed_pages_mutex), ref(cond_var));
		(pool.end() - 1)->detach();
	}

	cond_var.notify_all();
	size_t checking_timeout = 3;

	while (true)
	{
		if (!indicator && get_pages_to_index_size(ref(pages_to_index), ref(storage_mutex)) == 0)
		{
			this_thread::sleep_for(chrono::seconds(checking_timeout));

			if (!indicator && get_pages_to_index_size(ref(pages_to_index), ref(storage_mutex)) == 0)
			{
				break;
			}
		}
		this_thread::sleep_for(chrono::seconds(checking_timeout));
	}

	auto end = chrono::system_clock().now();
	chrono::duration<double> seconds = end - start;

	output << "Indexed pages: " << indexed_pages.size() << endl;
	output << "Time: " << seconds.count() - (double)checking_timeout << endl;
}
