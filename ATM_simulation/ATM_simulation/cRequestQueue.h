#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

class cRequestQueue {
public:
	static cRequestQueue& getQueue()
	{
		static cRequestQueue singleton;
		return singleton;
	}


	
private:
	
	cRequestQueue() {}                                  // Private constructor
	~cRequestQueue() {}
	cRequestQueue(const cRequestQueue&);                 // Prevent copy-construction
};