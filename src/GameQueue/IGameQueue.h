#pragma once
#include"ICommand/ICommand.h"
#include<any>
#include<memory>

class IGameQueue {
public:
	virtual void push_back(std::shared_ptr<ICommand>) = 0;
	virtual void pop() = 0;
	virtual std::shared_ptr<ICommand> top() = 0;
	virtual ~IGameQueue() = default;
};