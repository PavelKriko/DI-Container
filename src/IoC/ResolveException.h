#pragma once

#include<exception>
#include<stdexcept>

class ResolveDependencyException : public std::runtime_error
{
public:
    ResolveDependencyException(const char* msg) : std::runtime_error(msg){}
};

