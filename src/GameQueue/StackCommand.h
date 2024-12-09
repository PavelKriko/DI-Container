#pragma once

#include"IGameQueue.h"
#include<stack>
#include<memory>

class StackCommand: public IGameQueue {
public:
	void push_back(std::shared_ptr<ICommand> command) override {
		stackCommands.push(command);
	}
	void pop() override {
		stackCommands.pop();
	}
	std::shared_ptr<ICommand> top() override {
		return stackCommands.top();
	}

	bool canContinue() {
		return !stackCommands.empty();
	}
private:
	std::stack<std::shared_ptr<ICommand>> stackCommands;
};
