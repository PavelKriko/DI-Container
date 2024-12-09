#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include"IoC/IoC.h"
#include"IoC/Strategies/BuildTank.h"
#include"UObject/UObject.h"
#include"vectors/vectorOps.h"
#include"GameQueue/StackCommand.h"
#include"ICommand/EmptyCommand.h"
#include"IoC/Strategies/HardReset.h"
#include"ExceptionHandler/ExceptionHandler.h"



TEST(IoCTest_1,AddIStrategyAndResolve) {
	BuildTank tanlss;
	IoContainer.addStrategy("Tank", tanlss);
	auto tank = IoContainer.resolve<std::shared_ptr<UObject>>("Tank");
	tank->setPropertyValue("position", std::vector<int>{5, 5});
	std::vector<int> correctPos{ 5,5 };
	EXPECT_EQ(std::any_cast<std::vector<int>>(tank->getProperty("position")), correctPos);
}

TEST(IoCTest_2, AddLambdaAndResolve) {
	IoContainer.addStrategy("BuildTank", [](const argumentList&)->std::any {
		std::shared_ptr<UObject> tank = std::make_shared<UObject>();
		tank->addProperty("position", std::vector<int>{0, 0});
		tank->addProperty("velocity", int(0));
		tank->addProperty("moveDirection", std::vector<int>{0, 0});
		return tank;
	});
	auto tank = IoContainer.resolve<std::shared_ptr<UObject>>("BuildTank");
	tank->setPropertyValue("position", std::vector<int>{5, 5});
	std::vector<int> correctPos{ 5,5 };
	EXPECT_EQ(std::any_cast<std::vector<int>>(tank->getProperty("position")), correctPos);
}

TEST(QUEUE, HARDRESET) {
	auto stackBuild = [](const argumentList&)->std::any
	{
		std::shared_ptr<StackCommand> stackcom = std::make_shared<StackCommand>();
		return stackcom;
	};																							//Создание стека команд (cтратегия)
	IoContainer.addStrategy("Queue", stackBuild);												//Добавили стратегию
	auto stackCommands = IoContainer.resolve<std::shared_ptr<StackCommand>>("Queue");			//Получили из IoC stack
	
	std::function<std::any(const argumentList&)> canContinue = [&stackCommands](const argumentList&)
	{
		return stackCommands->canContinue();
	};																						   //Функция проверки возможности продолжить выполнять команды(стек команд не пуст)
	IoContainer.addStrategy("canContinue", canContinue);									   //Добавление зависимости, которая показывает можно ли продолжить выполнения команд в очереди
	HardReset hardReset(IoContainer);
	IoContainer.addStrategy("HardReset",hardReset);										   //Добавили зависимость HardReset
	
	for(int i = 0; i < 5 ; ++i)
	{
		auto com  = std::shared_ptr<ICommand>(new EmptyCommand);
		stackCommands->push_back(com);
	}																						   //Добавление 5 пустых команд в стек
	
	auto hardres = IoContainer.resolve<std::shared_ptr<HardResetCommand>>("HardReset");		   //Команда HardReset
	stackCommands->push_back(hardres);														   //Добавление HardReset команды в стек

	for (int i = 0; i < 5; ++i)
	{
		auto com = std::shared_ptr<ICommand>(new EmptyCommand);
		stackCommands->push_back(com);
	}																							//Добавление 5 пустых команд в стек
	int countOfCommands = 0;
	// в stack {empty, empty, empty, empty, empty, hardReset, empty, empty, empty, empty, empty} - 11 команд, должно выполнится только 6 (включая hardReset)
	while (IoContainer.resolve<bool>("canContinue"))
	{
		stackCommands->top()->execute();
		stackCommands->pop();
		++countOfCommands;
	}
	EXPECT_EQ(6, countOfCommands);     
}