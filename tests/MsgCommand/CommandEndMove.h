#pragma once

#include <vector>
#include "ICommand/ICommand.h"
#include "Movable.h"
#include <algorithm>
#include <memory>
#include "vectors/vectorOps.h"

template<typename T>
class CommandEndMove: public ICommand
{
private:
	std::shared_ptr<Movable<T>> _moveAdapter;

public:
	CommandEndMove() = delete;
	CommandEndMove(std::shared_ptr<Movable<T>> obj): _moveAdapter(obj){}

	void execute()
	{
		_moveAdapter->setVelocity(std::vector<T>{0,0});
	}
};