#pragma once

#include "IQueue.h"
#include <queue>


class Queue : public IQueue {
public:
	void push_back(std::shared_ptr<ICommand> comm) override {
		_q.emplace(comm);
	}
	void pop() override {
		_q.pop();
	}

	std::shared_ptr<ICommand> top() override {
		return _q.front();
	}

	bool empty() override {
		return _q.empty();
	}

private:
	std::queue<std::shared_ptr<ICommand>> _q; //first-in-first-out
};