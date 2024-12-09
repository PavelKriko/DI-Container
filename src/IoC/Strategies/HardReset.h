#pragma once

#include"IoC/IStrategy.h"
#include"ICommand/ICommand.h"
#include<any>

class HardResetCommand : public ICommand {
public: 
	HardResetCommand(IoC& ref) :_ref(ref) {}
	void execute() override 
	{
		_ref.addStrategy("canContinue", [](const argumentList&)->std::any {return false; });
	}

private:
	IoC& _ref;
};


class HardReset : public IStrategy {
 public: 
	 HardReset() = delete;
	 HardReset(IoC& ref) :_ref(ref) {}
	 std::any executeStrategy(const argumentList& args) override 
	 {	 
		 return std::make_shared<HardResetCommand>(_ref);		//В случае появления hardReset меняем стратегию canContinue 
	 }
	 IoC& _ref;
};

class HardResetCommand_2 : public ICommand {
public:
	HardResetCommand_2(std::function<bool()>& func) : _func(func) {}
	void execute() override
	{
		_func = []()->bool {return false; };
	}

private:
	std::function<bool()>& _func;
};