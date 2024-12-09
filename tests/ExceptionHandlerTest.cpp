#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include"ExceptionHandler/ExceptionHandler.h"
#include"ExceptionHandler/ErrorMoveCommand.h"
#include "IoC/IoC.h"

#include<stack>
#include<string>

TEST(MoveCom, Exception) {

	auto log = std::make_shared<Logger>();
	
	ExceptionHandlerMove handler; //add handler for move com to ioc
	IoContainer.addStrategy("moveCommand exception", handler);

	auto errorCom = std::shared_ptr<ICommand>(new ErrorMoveCommand);

	std::stack<std::shared_ptr<ICommand>> queue;

	queue.emplace(errorCom); //add com which will throw ex

	bool result = false;

	try {
		queue.top()->execute();
	}
	catch (std::exception& ex) {
		auto msg = ex.what();
		result =  IoContainer.resolve<bool>(msg, { {"command",errorCom},{"log",log}});
	}

	EXPECT_TRUE(result);
	auto logs = log->getLogs();

	std::string msg = "can't move";
	EXPECT_EQ(logs.back(),msg);
}