#pragma once

#include <vector>

template <typename T>
class Movable
{
public:
	virtual std::vector<T> getPosition() = 0;
	virtual void setPosition(const std::vector<T> &value) = 0;
	virtual std::vector<T> getVelocity() = 0;
	virtual void setVelocity(const std::vector<T> &value) = 0;
	virtual ~Movable() = default;
};