#ifndef TREE_H
#define TREE_H

#include <map>
#include <string>
#include <vector>

void push(int reg);

void pop(int reg);

void pushReg();

void popReg();

std::string genLabelCode();




struct treeNode {
	std::string type;
	int offset;
};




class Tree {
	public:
	std::string rule;
	std::vector<std::string> tokens;
	std::vector<Tree*> children;
	Tree();
	~Tree();
	bool buildTree();
	void extractParamList(std::vector<std::string> &paramList);
	void extractArgList(std::vector<std::string> &argList);
	bool isID();
	std::string getType();
	void code();
};


#endif
