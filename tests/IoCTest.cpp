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
	};																							//�������� ����� ������ (c��������)
	IoContainer.addStrategy("Queue", stackBuild);												//�������� ���������
	auto stackCommands = IoContainer.resolve<std::shared_ptr<StackCommand>>("Queue");			//�������� �� IoC stack
	
	std::function<std::any(const argumentList&)> canContinue = [&stackCommands](const argumentList&)
	{
		return stackCommands->canContinue();
	};																						   //������� �������� ����������� ���������� ��������� �������(���� ������ �� ����)
	IoContainer.addStrategy("canContinue", canContinue);									   //���������� �����������, ������� ���������� ����� �� ���������� ���������� ������ � �������
	HardReset hardReset(IoContainer);
	IoContainer.addStrategy("HardReset",hardReset);										   //�������� ����������� HardReset
	
	for(int i = 0; i < 5 ; ++i)
	{
		auto com  = std::shared_ptr<ICommand>(new EmptyCommand);
		stackCommands->push_back(com);
	}																						   //���������� 5 ������ ������ � ����
	
	auto hardres = IoContainer.resolve<std::shared_ptr<HardResetCommand>>("HardReset");		   //������� HardReset
	stackCommands->push_back(hardres);														   //���������� HardReset ������� � ����

	for (int i = 0; i < 5; ++i)
	{
		auto com = std::shared_ptr<ICommand>(new EmptyCommand);
		stackCommands->push_back(com);
	}																							//���������� 5 ������ ������ � ����
	int countOfCommands = 0;
	// � stack {empty, empty, empty, empty, empty, hardReset, empty, empty, empty, empty, empty} - 11 ������, ������ ���������� ������ 6 (������� hardReset)
	while (IoContainer.resolve<bool>("canContinue"))
	{
		stackCommands->top()->execute();
		stackCommands->pop();
		++countOfCommands;
	}
	EXPECT_EQ(6, countOfCommands);     
}