#pragma once

#include <vector>


struct Node {
	size_t index = 0;
	double value = 0;
	Node* left{ nullptr };
	Node* right{ nullptr };
};


template<typename ObjectType,typename Mark>
class IDT {
public:
	virtual void set_tree(const Node * root) = 0;
	
	virtual void fit(const std::vector<ObjectType>& x_train, const std::vector <Mark>& y_train) = 0;

	virtual Mark predict(const ObjectType & object) = 0;

	virtual std::vector<Mark> predict(const std::vector<ObjectType>& objects) = 0;
};