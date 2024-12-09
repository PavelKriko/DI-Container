#pragma once

#include "IoC/IStrategy.h"
#include"CDT/MoveAdapter/IMovable.h"
#include"ICommand/ICommand.h"

#include<list>
#include<string>
#include <any>

class Logger {
public:
	Logger() {};
	void writeMsg(const std::string& msg) {
		logs.push_back(msg);
	}

	std::list<std::string> getLogs(){
		return logs;
	}

private:
	std::list<std::string> logs;
};

class ExceptionHandlerMove : public IStrategy {
public:
	std::any executeStrategy(const argumentList& args) override {
		auto com = std::any_cast<std::shared_ptr<ICommand>>(args.at("command"));
		auto log = std::any_cast<std::shared_ptr<Logger>>(args.at("log"));
		try {
			com->execute();
			com->execute();
		}
		catch (...) {
			log->writeMsg("can't move");
		}
		return true;
	}
};