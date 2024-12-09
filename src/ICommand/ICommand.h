#pragma once

#include<exception>

class ICommand {
public:
	virtual void execute() = 0;
	virtual ~ICommand() = default;
};


class CommandException:public std::exception {
public:
	CommandException(const std::string& str) :msg(str) {};

private:
	std::string msg;
};