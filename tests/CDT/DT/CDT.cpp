#include "CDT.h"
#include <cassert>
#include <algorithm>
#include <limits>

struct SubDataSet {
	X x_data;
	y y_data;
};

struct subTree {
	Node* node{ nullptr };
	SubDataSet leftTree;
	SubDataSet rightTree;
};

double gini_index(const size_t split_ind, const double splitValue, const SubDataSet&subDataSet) {
	assert(subDataSet.x_data.size() == subDataSet.y_data.size());
	assert(!subDataSet.x_data.empty());
	assert(split_ind <= subDataSet.x_data[0].size());

	size_t countOfTrueLeft = 0;
	size_t countOfTrueRight = 0;

	size_t LeftDataSetLen = 0;
	size_t RightDataSetLen = 0;


	for (size_t i= 0; i < subDataSet.x_data.size(); i++) {
		if (subDataSet.x_data[i][split_ind] > splitValue) {
			RightDataSetLen++;
			if (subDataSet.y_data[i]) {
				countOfTrueRight++;
			}
		}
		else{
			LeftDataSetLen++;
			if (subDataSet.y_data[i])
			{
				countOfTrueLeft++;
			}
		}


	}
	const double sb_left_true_p =
		static_cast<double>(countOfTrueLeft) / static_cast<double>(LeftDataSetLen);
	const double sb_right_true_p =
		static_cast<double>(countOfTrueRight) / static_cast<double>(RightDataSetLen);

	return (sb_left_true_p - sb_left_true_p * sb_left_true_p) +
		(sb_right_true_p - sb_right_true_p * sb_right_true_p);
}

subTree split_node(const size_t ind_split, const double value, const SubDataSet& subDataSet) {
	assert(subDataSet.x_data.size() == subDataSet.y_data.size());
	assert(!subDataSet.x_data.empty());
	assert(ind_split <= subDataSet.x_data[0].size());

	SubDataSet left_part;
	SubDataSet right_part;

	const size_t dataset_len = subDataSet.x_data.size();

	for (size_t i = 0; i < dataset_len; i++) {
		if (subDataSet.x_data[i][ind_split] > value) {
			right_part.x_data.push_back(subDataSet.x_data[i]);
			right_part.y_data.push_back(subDataSet.y_data[i]);
		}
		else {
			left_part.x_data.push_back(subDataSet.x_data[i]);
			left_part.y_data.push_back(subDataSet.y_data[i]);
		}
	}

	return{ nullptr,left_part,right_part };
}


subTree get_split(const SubDataSet& subDataSet) {
	assert(subDataSet.x_data.size() == subDataSet.y_data.size());
	assert(!subDataSet.x_data.empty());

	const size_t features_len = subDataSet.x_data[0].size();
	const size_t data_len = subDataSet.x_data.size();

	auto* node = new Node;

	subTree nodeTree;
	nodeTree.node = node;

	double min_gini = std::numeric_limits<double>::infinity();

	for (size_t i= 0; i < features_len; i++) {
		for (size_t j = 0; j < data_len; j++) {
			const double gini = gini_index(i, subDataSet.x_data[j][i], subDataSet);
			if (gini < min_gini) {
				node->index = i;
				node->value = subDataSet.x_data[j][i];
				min_gini = gini;
			}

		}
	}

	auto try_split = split_node(node->index, node->value, subDataSet);
	nodeTree.leftTree = std::move(try_split.leftTree);
	nodeTree.rightTree = std::move(try_split.rightTree);

	return nodeTree;
}

void make_leaf(Node* node, const SubDataSet& subDataSetLeft, const SubDataSet& subDataSetRight) {
	const size_t countOfTrueLeft = std::count(subDataSetLeft.y_data.begin(), subDataSetLeft.y_data.end(), true);
	const size_t countOfFalseLeft = subDataSetLeft.y_data.size() - countOfTrueLeft;

	const size_t countOfTrueRight = std::count(subDataSetRight.y_data.begin(), subDataSetRight.y_data.end(), true);
	const size_t countOfFalseRight = subDataSetRight.y_data.size() - countOfTrueRight;

	if (countOfTrueRight + countOfTrueLeft > countOfFalseRight + countOfFalseLeft)
	{
		node->value = 1;
	}
	else
	{
		node->value = 0;
	}
}


void split_recursive(const subTree & subTree,const int maxDepth, const size_t min_node_examples_count,
	const int current_depth) {
	
	if (current_depth > maxDepth) {
		make_leaf(subTree.node, subTree.leftTree, subTree.rightTree);
		return;
	}

	if (subTree.leftTree.y_data.size() < min_node_examples_count) {
		subTree.node->left = new Node;
		make_leaf(subTree.node->left, subTree.leftTree, subTree.rightTree);
	}
	else {
		const auto child_node = get_split(subTree.leftTree);
		subTree.node->left = child_node.node;

		split_recursive(child_node, maxDepth, min_node_examples_count, current_depth+1);
	}

	if (subTree.rightTree.y_data.size() < min_node_examples_count) {
		subTree.node->right = new Node;
		make_leaf(subTree.node->right, subTree.leftTree, subTree.rightTree);
	}
	else {
		const auto child_node = get_split(subTree.rightTree);
		subTree.node->right = child_node.node;

		split_recursive(child_node, maxDepth, min_node_examples_count, current_depth+1);
	}
}

void free_node(Node* node) {
	if (node != nullptr) {
		free_node(node->left);
		free_node(node->right);
		delete node;
	}
}

void CDT::fit(const X& x_train, const y& y_train) {
	if (root != nullptr) {
		free_node(root);
		root = nullptr;
	}

	const subTree root_split = get_split({ x_train,y_train });

	root = root_split.node;
	split_recursive(root_split, max_tree_depth, min_node_rec,1);
}


bool CDT::predict(const std::vector<double>& object){
	const Node* node = root;
	while (node->left != nullptr && node->right != nullptr) {
		if (object[node->index] > node->value) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}
	return static_cast<bool>(node->value);
}


std::vector<bool> CDT::predict(const X& objects) {
	
	std::vector<bool> pred;
	for (const auto& object : objects) {
		pred.push_back(this->predict(object));
	}
	return pred;
}



