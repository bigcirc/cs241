#include "tree.h"

#include <iostream>
#include <map>
#include <vector>
#include <utility>

using namespace std;

map<string, pair<vector<string>, map<string, treeNode> > > symTable;


void printTable();
void prologue();


int main()  {

	
	Tree t;

	if (t.buildTree()) {
		//printTable();
		return 0;
	}
	return 1;
}







void printTable() {
	for (map<string, pair<vector<string>, map<string, treeNode> > >::iterator it = symTable.begin(); it != symTable.end(); it++) {
		cerr << it->first;
		//params
		for (vector<string>::iterator itt = (it->second).first.begin(); itt != (it->second).first.end(); itt++) {
			cerr << " " << *itt;
		}
		cerr << endl;
		
		//IDs
		for (map<string, treeNode>::iterator ittt = (it->second).second.begin(); ittt != (it->second).second.end(); ittt++) {
			cerr << ittt->first << " " << ittt->second.type  << " " << ittt->second.offset << endl;
		}

		cerr << endl; 
	}
}
