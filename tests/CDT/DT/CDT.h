#pragma once

#include "IDT.h"

#include <vector>
using DataObject = std::vector<double>;

using X = std::vector<DataObject>;
using y = std::vector<bool>;



class CDT : public IDT <std::vector<double>,bool> {
public:
	CDT(int max_tree_depth, int min_node_rec = 1) : max_tree_depth(max_tree_depth), min_node_rec(min_node_rec) {}

	void set_tree(const Node* root) override {
		root = root;
	}

	void fit(const X& x_train, const y& y_train) override;

	bool predict(const std::vector<double>& object) override;

	std::vector<bool> predict(const X& objects) override;

private: 
	Node* root{nullptr};
	int max_tree_depth;
	int min_node_rec; 
};