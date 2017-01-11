#include "tree.h"
#include <iostream>
#include <sstream>
#include <cctype>


using namespace std;





Tree::Tree()  {}

Tree::~Tree() {
	for (vector<Tree*>::iterator it = children.begin(); it != children.end(); it++) {
		delete (*it);
	}
}


extern map<string, pair<vector<string>, map<string, treeNode> > > symTable;
map<string, treeNode> currTable;
string currProcedure;
vector<string> currSig;
bool noteProcedure = false;
bool noteSig = false;
int offset = 0;
int numOfWhile = 0;
int numOfIf = 0;
int labelCounter = 0;

bool Tree::buildTree() {
	getline(cin, rule);
	stringstream ss(rule);
	string nextWord, LHS;
	ss >> LHS;
	tokens.push_back(LHS); //push back LHS of rule
	while (ss >> nextWord) {
		tokens.push_back(nextWord);

		if (LHS == "procedures") {
			noteProcedure = true;
			noteSig = true;
		}
		
		if (noteSig and nextWord == "RPAREN") {
			if (tokens[0] == "procedure") {
				(*children[3]).extractParamList(currSig);
				
			}
			else if (tokens[0] == "main") {
				string type1 = (*(*children[3]).children[0]).tokens.size() == 2 ? "int" : "int*";
				string type2 = (*(*children[5]).children[0]).tokens.size() == 2 ? "int" : "int*";
				
				if (type2 == "int*") {
					cerr << "ERROR: second argument in main needs to be of type int" << endl;
					return false;
				}
				
				currSig.push_back(type1);
				currSig.push_back(type2);
			}
			
			noteSig = false;
		}

		if (not isupper(LHS[0])) {
			Tree* child = new Tree;
			bool result = child->buildTree();
			if (not result) {
				return false;
			}
			children.push_back(child);
		}
	}
	
	
	
	
	if (rule == "procedures procedure procedures") {
		(*children[1]).code(); //always generate code for main first, since mips executes top down
		(*children[0]).code();
	}
	else if (rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		string id = (*children[1]).tokens[1];

		map<string, pair<vector<string>, map<string, treeNode> > >::iterator result = symTable.find(id);
		if (result != symTable.end()) {
			cerr << "ERROR: procedure wain has already beed defined" << endl;
			return false;
		}
		
		if ((*children[11]).getType() != "int") {
			cerr << "ERROR: procedure wain not returning int" << endl;
			return false;
		}
		
		//add to symTable
		vector<string> paramList;
		string type1 = (*(*children[3]).children[0]).tokens.size() == 2 ? "int" : "int*";
		string type2 = (*(*children[5]).children[0]).tokens.size() == 2 ? "int" : "int*";
		paramList.push_back(type1);
		paramList.push_back(type2);
		
		
		
		
		
		cout << ";;------------->one part of prologue<-------------" << endl;
		cout << ".import print" << endl;
		cout << "lis $4" << endl << ".word 4" << endl;
		cout << "lis $11" << endl << ".word 1" << endl;
		cout << "sub $29, $30, $4" << endl;
		cout << ";;------------------>end of part<-------------------" << endl;
		
		push(1); //push a
		push(2); //push b
		
		//another part of prologue
		cout << ";;------------->another part of prologue<-------------" << endl;
		if (type1 == "int") {
			cout << "add $2, $0, $0" << endl;
		}
		cout << ".import init" << endl;
		cout << ".import new" << endl;
		cout << ".import delete" << endl;
		
		cout << ";;---call init" << endl;
		push(31);
		cout << "lis $5" << endl;
		cout << ".word init" << endl;
		cout << "jalr $5" << endl;
		pop(31);
		
		
		cout << ";;------------------>START OF CODE BODY<-------------------" << endl;
		
                //(*children[3]).code();
                //(*children[5]).code();
                (*children[8]).code();
                (*children[9]).code();
                cout << ";;---------->Return" << endl;
                (*children[11]).code();
                cout << "add$30, $29, $4" << endl;
                cout << "jr $31" << endl;
		
		
		pair<vector<string>, map<string, treeNode> > p = make_pair(paramList, currTable);
                map<string, treeNode> newTable;
                currTable = newTable; //refreash currTable
                vector<string> newSig;
		currSig = newSig; //refreash currSig
                symTable[id] = p;
                offset = 0; //reset offset
	}
	else if (rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		string id = (*children[1]).tokens[1];
  
  		map<string, pair<vector<string>, map<string, treeNode> > >::iterator result = symTable.find(id);
		if (result != symTable.end()) {
			cerr << "ERROR: procedure " << id << " has already beed defined" << endl;
			return false;
		}
		
		if ((*children[9]).getType() != "int") {
			cerr << "ERROR: procedure " << id << " not returning int" << endl;
			return false;
		}
		
		//add to symTable
		vector<string> paramList;
		(*children[3]).extractParamList(paramList);
		
		pair<vector<string>, map<string,treeNode> > p = make_pair(paramList, currTable);
		map<string, treeNode> newTable;
		currTable = newTable; //refreash currTable
		vector<string> newSig;
		currSig = newSig; //refreash currSig
		symTable[id] = p;
		offset = 0; //reset offset
	}
	else if (noteProcedure and (rule.substr(0, 3) == "ID " or rule.substr(0, 4) == "WAIN")) {
		stringstream ss(rule);
		ss >> currProcedure;
		ss >> currProcedure;
		noteProcedure = false;
	}
	else if (rule == "dcl type ID") {
		string id = (*children[1]).tokens[1];
		string type = (*children[0]).tokens.size() == 2 ? "int" : "int*" ;

		map<string, treeNode>::iterator result = currTable.find(id);
		if (result != currTable.end()) {
			cerr << "ERROR: variable " << id << " has already been defined" << endl;
			return false;
		}
		//add to table
		treeNode n;
		n.type = type;
		n.offset = offset;
		currTable[id] = n;
		offset -= 4;
	}
	else if (rule == "factor ID") {
		string id = (*children[0]).tokens[1];

		map<string, treeNode>::iterator result = currTable.find(id);
		if (result == currTable.end()) {
		        cerr << "1ERROR: variable " << id << " has not been defined" << endl;
			return false;
		}
	}
	else if (rule == "factor ID LPAREN arglist RPAREN") {
		string id = (*children[0]).tokens[1];

		map<string, pair<vector<string>, map<string, treeNode> > >::iterator result = symTable.find(id);
		if (result == symTable.end() and id != currProcedure) {
			cerr << "ERROR: procedure " << id << " has not been definded" << endl;
			return false;
		}

		map<string, treeNode>::iterator anotherResult = currTable.find(id);
		if (anotherResult != currTable.end()) {
			cerr << "ERROR: id " << id << " has already been used as a variable name" << endl;
			return false;
		}
	}
	else if (rule == "factor ID LPAREN RPAREN") {
                string id = (*children[0]).tokens[1];
                
		map<string, pair<vector<string>, map<string, treeNode> > >::iterator result = symTable.find(id);
	        if (result == symTable.end() and id != currProcedure) {
	 		cerr << "ERROR: procedure " << id << " has not been definded" << endl;
		        return false;
	        }

		map<string, treeNode>::iterator anotherResult = currTable.find(id);
                if (anotherResult != currTable.end()) {
                        cerr << "ERROR: id " << id << " has already been used as a variable name" << endl;
                        return false;
                }
	}
	else if (rule == "expr term") {
		string type = children[0]->getType();
		if (type == "") {
			cerr << "ERROR: not well typed in procedure " << currProcedure << endl;
			return false;
		}
	}
	else if (rule == "expr expr PLUS term") {
		string type1 = children[0]->getType();
		string type2 = children[2]->getType();
		if (type1 == type2 and type1 == "int*") {
			cerr << "ERROR: not well typed in procedure " << currProcedure << endl;
			return false;
		}
	}
	else if (rule == "expr expr MINUS term") {
		string type1 = children[0]->getType();
		string type2 = children[2]->getType();
		
		if (type1 == "int" and type2 == "int*") {
			cerr << "ERROR: not well typed in procedure " << currProcedure << endl;
			return false;
		}
	}
	else if (rule == "statement lvalue BECOMES expr SEMI") {
		string type1 = children[0]->getType();
		string type2 = children[2]->getType();
				
		if (not ((*children[0]).isID() and type1 == type2)) {
			cerr << "ERROR: invalid assignment in procedure " << currProcedure << endl;
			return false;
		}
	}
	else if (rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
		string type = (*children[2]).getType();
		if (type != "int") {
			cerr << "ERROR: attemp to print non-int type" << endl;
			return false;
		}
	}
	else if (rule == "statement DELETE LBRACK RBRACK expr SEMI") {
		string type = (*children[3]).getType();
		if (type != "int*") {
			cerr << "ERROR: attemp to delete non-pointer type" << endl;
			return false;
		}
	}
	else if (rule == "test expr EQ expr" or 
		 rule == "test expr NE expr" or
		 rule == "test expr LT expr" or
		 rule == "test expr LE expr" or
		 rule == "test expr GE expr" or
		 rule == "test expr GT expr") {
		
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		
		if (type1 != type2 or type1 == "" or type2 == "") {
			cerr << "ERROR: test not correct in procedure " << currProcedure << endl;
			return false;
		}
	}
	else if (rule == "dcls dcls dcl BECOMES NUM SEMI") {
		string type = (*children[1]).getType();
		
		if (type == "int*") {
			cerr << "ERROR: trying to assign num to variable of type int* in procedure " << currProcedure << endl; 
			return false;
		}
		
	}
	else if (rule == "dcls dcls dcl BECOMES NULL SEMI") {
		string type = (*children[1]).getType();
	
		if (type == "int") {
			cerr << "ERROR: trying to assign NULL to variable of type int in procedure " << currProcedure << endl; 
			return false;
		}
	}
	

	return true;
}


string Tree::getType() {
	if (rule == "expr term" or rule == "term factor") {
		return (*children[0]).getType();
	}
	else if (rule == "expr expr PLUS term") {
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		if (type1 == type2 and type1 == "int") {
			return "int";
		}
		else if ((type1 == "int" and type2 == "int*") or (type1 == "int*" and type2 == "int")) {
			return "int*";
		}
	}
	else if (rule == "expr expr MINUS term") {
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		if (type1 == type2 and (type1 == "int" or type1 == "int*")) {
			return "int";
		}
		else if (type1 == "int*" and type2 == "int") {
			return "int*";
		}
	}
	else if (rule == "term term STAR factor" or rule == "term term SLASH factor" or rule == "term term PCT factor") {
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		if (type1 == type2 and type1 == "int") {
			return "int";
		}
	}
	else if (rule == "factor ID" or rule == "lvalue ID") {
		map<string, treeNode>::iterator result = currTable.find((*children[0]).tokens[1]);
		if (result == currTable.end()) {
			cerr << "ERROR: variable " << (*children[0]).tokens[1] << " not defined" << endl;
			return "";
		}
		return result->second.type;
	}
	else if (rule == "factor NUM") {
		return "int";
	}
	else if (rule == "factor NULL") {
		return "int*";
	}
	else if (rule == "factor LPAREN expr RPAREN") {
		return (*children[1]).getType();
	}
	else if (rule == "factor NEW INT LBRACK expr RBRACK") {
		string result = (*children[3]).getType();
		if (result == "int") {
			return "int*";
		}
	}
	else if (rule == "factor ID LPAREN RPAREN") {
		string id = (*children[0]).tokens[1];
		vector<string> &params = symTable.find(id)->second.first;
		if (params.size() == 0) {
			return "int";
		}
	}
	else if (rule == "factor ID LPAREN arglist RPAREN") {
		string id = (*children[0]).tokens[1];
		
		vector<string> &params = id == currProcedure ? currSig : symTable.find(id)->second.first;
		vector<string> args;
		(*children[2]).extractArgList(args);
			
		
		if (params.size() == args.size()) {
			for (int i = 0; i < params.size(); i++) {
				if (params.at(i) != args.at(i)) {
					return "";
				}
			}
			return "int";
		}
	}
	else if (rule == "factor STAR factor") {
		string result = (*children[1]).getType();
		if (result == "int*") {
			return "int";
		}
	}
	else if (rule == "factor AMP lvalue") {
		if (/*(*children[1]).isID() and*/ (*children[1]).getType() == "int") {
			return "int*";
		}
	}
	else if (rule == "lvalue STAR factor") {
		if (/*(*children[1]).isID() and*/ (*children[1]).getType() == "int*") {
			return "int";
		}
	}
	else if (rule == "lvalue LPAREN lvalue RPAREN") {
		return (*children[1]).getType();
	}
	else if (rule == "dcl type ID") {
		return (*children[0]).tokens.size() == 2 ? "int" : "int*";
	}
	
	cerr << "DEBUG: no type caused by: " << rule << endl;
	return "";	
}

bool Tree::isID() {
	if (rule == "lvalue ID" or rule == "factor ID") {
		return true;
	}
	else if (rule == "lvalue STAR factor" or rule == "lvalue LPAREN lvalue RPAREN" or rule == "factor AMP lvalue" or rule == "factor LPAREN expr RPAREN") {
		return (*children[1]).isID();
	}
	else if (rule == "expr expr PLUS term") {
		return (*children[0]).isID() or (*children[2]).isID();
	}
	else if (rule == "expr term" or rule == "term factor") {
		return (*children[0]).isID();
	}
	else {
		return false;
	}
}


void Tree::extractArgList(vector<string> &argList) {
	if (children.size() == 3) {
		argList.push_back((*children[0]).getType());
		(*children[2]).extractArgList(argList);
	}
	else {
		argList.push_back((*children[0]).getType());
	}
}

void Tree::extractParamList(vector<string> &paramList) {
	if (children.size() == 0) {
		return;
	}
	else {
		for (int tokIndex = 1; tokIndex < tokens.size() ; tokIndex++) {
			int childIndex = tokIndex - 1;
			if (tokens[tokIndex] == "dcl") {
				Tree* type = (*children[childIndex]).children[0];

				if ((type->tokens).size() == 2) { //type INT
					paramList.push_back("int");
				}
				else if ((type->tokens).size() == 3) { //type INT STAR
					paramList.push_back("int*");
				}
			}
			else if (tokens[tokIndex] == "paramlist") {
				children[childIndex]->extractParamList(paramList);
			}
			else if (tokens[tokIndex] == "COMMA") {
				continue;
			}
		}
	}
}



void Tree::code() {
	if (rule == "expr term") {
		(*children[0]).code();
	}
	else if (rule == "term factor") {
		(*children[0]).code(); //$3 <- value(factor)
	}
	else if (rule == "term term STAR factor" or rule == "term term SLASH factor" or rule == "term term PCT factor") {
		(*children[0]).code(); //$3 <- value(term)
		push(3); 
		(*children[2]).code(); //$3 <- value(factor)
		pop(5); //$5 <- value(term)
		
		string command = tokens[2];
		string instr,hilo;
		
		if (command == "STAR") {
			instr = "mult";
			hilo = "mflo";
		}
		else if (command == "SLASH") {
			instr = "div";
			hilo = "mflo";
		}
		else if (command == "PCT") {
			instr = "div";
			hilo = "mfhi";
		}
		
		cout << instr <<" $5, $3" << endl; //$3 <- term *,/ factor
		cout << hilo << " $3" << endl; //$3 <- hi/lo
	}
	else if (rule == "factor ID") {
		string id = (*children[0]).tokens[1];
		map<string, treeNode>::iterator result = currTable.find(id);
		
		int offset = result->second.offset;
		cout << "lw $3, " << offset << "($29)" << endl;
	}
	else if (rule == "factor NUM") {
		string num = (*children[0]).tokens[1];
		cout << "lis $3" << endl;
		cout << ".word " << num << endl;
	}
	else if (rule == "factor LPAREN expr RPAREN") {
		(*children[1]).code();
	}
	else if (rule == "statements statements statement") {
		(*children[0]).code();
		(*children[1]).code();
	}
	else if (rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
		cout << ";;---------------->PRINT<----------------" << endl; 
		
		(*children[2]).code(); //$3 <- value(expr)
		cout << "add $1, $3, $0" << endl; //$1 <- value(expr)
		push(31);
		cout << "lis $5" << endl;
		cout << ".word print" << endl;
		cout << "jalr $5" << endl;
		pop(31);
	}
	else if (rule == "dcls dcls dcl BECOMES NUM SEMI") {
		string num = (*children[3]).tokens[1];
		
		string id = (*children[1]).children[1]->tokens[1];
		map<string, treeNode>::iterator result = currTable.find(id);
		
		int offset = result->second.offset;
		
		(*children[0]).code(); //dcls
		
		cout << "lis $3" << endl << ".word " << num << endl; //$3 <- value(NUM)
		cout << "sw $3, " << offset << "($29)" << endl; //value(lvalue) in stack <- value(expr)
		cout << "sub $30, $30, $4" << endl;
	}
	else if (rule == "lvalue LPAREN lvalue RPAREN") {
		(*children[1]).code();
	}
	else if (rule == "factor NULL") {
		cout << "add $3, $0, $11" << endl;
	}
	else if (rule == "factor STAR factor") {
		(*children[1]).code(); // $3 <- value(factor)
		
		cout << ";;----->check for NULL" << endl;
		string label = "NULL" + genLabelCode();
		cout << "beq $3, $11, " << label << endl;
		
		cout << "lw $3, 0($3)" << endl;
		
		cout << label << ":" << endl;
		
	}
	else if (rule == "factor AMP lvalue") {
		(*children[1]).code(); //$3 <- address(lvalue)
	}
	else if (rule == "dcls dcls dcl BECOMES NULL SEMI") {
		(*children[0]).code(); //generate "dcls"'s code
		string id = (*children[1]).children[1]->tokens[1];
		map<string, treeNode>::iterator result = currTable.find(id);
		int offset = result->second.offset;
		
		cout << ";;----------------initialize " << id << " to NULL-------------" << endl;
		
		cout << "sw $11, " << offset << "($29)" << endl;
		cout << "sub $30, $30, $4" << endl;
	}
	else if (rule == "statement lvalue BECOMES expr SEMI") {
		cout << ";;---------------Assignment----------------" << endl;
		
		(*children[0]).code(); // $3 <- value(lvaue), an address
		
		cout << ";;----->check for NULL" << endl;
		string label = "NULL" + genLabelCode();
		cout << "beq $3, $11, " << label << endl;
		
		cout << "add $6, $3, $0" << endl;
		(*children[2]).code();
		cout << "sw $3, 0($6)" << endl;
		
		cout << label << ":" << endl;
		
	}
	else if (rule == "lvalue ID") {
		string id = (*children[0]).tokens[1];
		map<string, treeNode>::iterator result = currTable.find(id);
		int offset = result->second.offset;
		
		cout << "lis $3" << endl;
		cout << ".word " << offset << endl;
		cout << "add $3, $29, $3" << endl;
	}
	else if (rule == "lvalue STAR factor") {
		(*children[1]).code(); // $3 <- address(factor)
	}
	else if (rule == "factor NEW INT LBRACK expr RBRACK") {
		cout << ";;---------->new" << endl;
		
		(*children[3]).code(); // $3 <- value(expr)
		cout << "add $1, $3, $0" << endl; //$1 <- value(expr)
		push(31);
		cout << "lis $5" << endl;
		cout << ".word new" << endl;
		cout << "jalr $5" << endl; // $3 <- address of array
		pop(31);
		
		cout << ";;----->check for NULL" << endl;
		string label = "NULL" + genLabelCode();
		
		cout << "bne $3, $0, " << label << endl;
		cout << "add $3, $11, $0" << endl;
		
		cout << label << ":" << endl;
	}
	else if (rule == "statement DELETE LBRACK RBRACK expr SEMI") {
		cout <<";;--------------->delete" << endl;
		(*children[3]).code(); // $3 <- value(expr), an address
		
		cout << ";;----->check for NULL" << endl;
		string label = "NULL" + genLabelCode();
		cout << "beq $3, $11, " << label << endl;
		
		cout << "add $1, $3, $0" << endl; //$1 <- value(expr)
		push(31);
		cout << "lis $5" << endl;
		cout << ".word delete" << endl;
		cout << "jalr $5" << endl;
		
		
		
		pop(31);
		
		cout << label << ":" << endl;
		
	}
	else if (rule == "expr expr PLUS term") {
		cout << ";;------->PLUS<-------" << endl;
		
		(*children[0]).code(); //$3 <- value(expr)
		push(3); 
		(*children[2]).code(); //$3 <- value(term)
		pop(5); //$5 <- value(expr)
		
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		
		if (type1 == "int" and type2 == "int*") {
			cout << ";;----->check for NULL" << endl;
			string label = "NULL" + genLabelCode();
			cout << "beq $3, $11, " << label << endl;
			
			cout << "mult $5, $4" << endl;
			cout << "mflo $5" << endl;
			
			cout << "add $3, $5, $3" << endl; //$3 <- expr + term
			cout << label << ":" << endl;
		}
		else if (type1 == "int*" and type2 == "int") {
			cout << ";;----->check for NULL" << endl;
			string label = "NULL" + genLabelCode();
			cout << "beq $5, $11, " << label << endl;
			
			cout << "mult $3, $4" << endl;
			cout << "mflo $3" << endl;
			
			cout << "add $3, $5, $3" << endl; //$3 <- expr + term
			cout << label << ":" << endl;
		}
		else if (type1 == type2 and type1 == "int") {
			cout << "add $3, $5, $3" << endl; //$3 <- expr + term
		}
	}
	else if (rule == "expr expr MINUS term") {
		cout << ";;------->MINUS<-------" << endl;
	
		(*children[0]).code(); //$3 <- value(expr)
		push(3); 
		(*children[2]).code(); //$3 <- value(term)
		pop(5); //$5 <- value(expr)
		
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		
		
		if (type1 == "int*" and type2 == "int") {
			cout << ";;----->check for NULL" << endl;
			string label = "NULL" + genLabelCode();
			cout << "beq $5, $11, " << label << endl;
			
			cout << "mult $3, $4" << endl;
			cout << "mflo $3" << endl;
			
			cout << "sub $3, $5, $3" << endl; //$3 <- expr - term
			cout << label << ":" << endl;
		}
		else if (type1 == "int*" and type2 == "int*") {
			cout << ";;----->check for NULL" << endl;
			string label1 = "NULL" + genLabelCode();
			cout << "beq $5, $11, " << label1 << endl;
			string label2 = "NULL" + genLabelCode();
			cout << "beq $3, $11, " << label2 << endl;
			
			cout << "sub $3, $5, $3" << endl; //$3 <- expr - term
			cout << "div $3, $4" << endl;
			cout << "mflo $3" << endl;
			
			cout << label1 << ":" << endl;
			cout << label2 << ":" << endl;
		}
		else if (type1 == "int" and type2 == "int") {
			cout << "sub $3, $5, $3" << endl; //$3 <- expr - term
		}
	}
	else if (rule.substr(0,4) == "test") {
		(*children[0]).code(); // $3 <- value(expr1)
		push(3);
		(*children[2]).code(); // $3 <- value(expr2)
		pop(5);
		
		string type1 = (*children[0]).getType();
		string type2 = (*children[2]).getType();
		
		if (rule == "test expr LT expr") {
			cout << "slt $3, $5, $3" << endl; // $3 = 1 if expr1 > expr2
		}
		else if (rule == "test expr GT expr") {
			cout << "slt $3, $3, $5" << endl; // $3 = 1 if expr1 < expr2
		}
		else if (rule == "test expr EQ expr") {
			cout << "slt $6, $3, $5" << endl;
			cout << "slt $7, $5, $3" << endl;
			cout << "add $3, $6, $7" << endl;
			cout << "sub $3, $11, $3" << endl; // $3 = 1 if expr1 == expr2
		}
		else if (rule == "test expr NE expr") {
			cout << "slt $6, $3, $5" << endl;
			cout << "slt $7, $5, $3" << endl;
			cout << "add $3, $6, $7" << endl; // $3 = 1 if expr1 != expr2
		}
		else if (rule == "test expr LE expr") {
			cout << "slt $3, $3, $5" << endl; // $3 = 1 if expr1 < expr2
			cout << "sub $3, $11, $3" << endl; // $3 = 1 if !(expr1 < expr2) => expr1 >= expr2 
		}
		else if (rule == "test expr GE expr") {
			cout << "slt $3, $5, $3" << endl; // $3 = 1 if expr1 > expr2
			cout << "sub $3, $11, $3" << endl; // $3 = 1 if !(expr1 > expr2) => expr1 <= expr2 
		}
	}
	else if (rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
		stringstream ss;
		ss << numOfWhile;
		
		string numOfWhile_s;
		ss >> numOfWhile_s;	
		
		string label = "while" + numOfWhile_s;
		numOfWhile++;
		
		cout << label << ":" << endl;
		(*children[2]).code(); //$3 <- value(test), 0: false, 1: true
		cout << "beq $3, $0, end" << label << endl;
		(*children[5]).code(); // $3 <- value(statements)
		cout << "beq $0, $0, " << label << endl;
		cout << "end" << label << ":" << endl;
		
		
	}
	else if (rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
		stringstream ss;
		ss << numOfIf;
		
		string numOfIf_s;
		ss >> numOfIf_s;	
		
		string trueLabel = "true" + numOfIf_s;
		string endIfLabel = "endif" + numOfIf_s;
		numOfIf++;
		
		(*children[2]).code(); // $3 <- value(test)
		cout << "bne $3, $0, " << trueLabel << endl;
		(*children[9]).code(); // statements2, execute if test false
		cout << "beq $0, $0, " << endIfLabel << endl;
		cout << trueLabel << ":" << endl;
		(*children[5]).code(); // statemeents1, execute if test true
		cout << endIfLabel << ":" << endl;
	}
	else if (rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		string id = (*children[1]).tokens[1];
		int numOfArgs = symTable.find(id)->second.first.size();
		currTable = symTable.find(id)->second.second;
		
		for (map<string, treeNode>::iterator it = currTable.begin(); it != currTable.end(); it++) {
			it->second.offset += numOfArgs * 4;
		}
		
		
		
		
		cout << ";;-------------procedure " << id << "-------------" << endl;
		cout << ";;-----Prologue of " << id << "-------" << endl;
		cout << "F" << id << ":" << endl;
		cout << "sub $29, $30, $4" << endl;
		(*children[6]).code(); //dcls
		pushReg(); //pushed after dcls so that offset in symtable is not messed up
		(*children[7]).code(); //statements
		cout << ";;----" << id <<" return----" << endl;
		(*children[9]).code(); // $3 <- value(expr)
		popReg();
		cout << "add $30, $29, $4" << endl;
		cout << "jr $31" << endl;
	}
	/*else if (rule == "params paramlist") {
	
	}
	else if (rule == "paramlist dcl") {
	
	}
	else if (rule == "paramlist dcl COMMA paramlist") {
	
	}*/
	else if (rule == "factor ID LPAREN RPAREN") {
		string id = (*children[0]).tokens[1];
		cout << ";;------call procedure " << id << "------" << endl;
		push(29);
		push(31);
		cout << "lis $5" << endl;
		cout << ".word F" << id << endl;
		cout << "jalr $5" << endl;
		pop(31);
		pop(29);
	}
	else if (rule == "factor ID LPAREN arglist RPAREN") {
		string id = (*children[0]).tokens[1];
		int numOfArgs = symTable.find(id)->second.first.size();
		
		cout << ";;------call procedure " << id << "------" << endl;
		push(29);
		push(31);
		(*children[2]).code(); //pushing arguments
		cout << "lis $5" << endl;
		cout << ".word F" << id << endl;
		cout << "jalr $5" << endl;
		
		//pop arguments
		for (int i = 0 ; i < numOfArgs; i++) {
			pop(5);
		}
		
		pop(31);
		pop(29);
	}
	else if (rule == "arglist expr") {
		(*children[0]).code(); //$3 <- value(expr)
		push(3);
	}
	else if (rule == "arglist expr COMMA arglist") {
		(*children[0]).code(); //$3 <- value(expr)
		push(3);
		(*children[2]).code();
	}
	
}



void push(int reg) {
	cout << "sw $" << reg << ", -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
}

void pop(int reg) {
	cout << "lw $" << reg << ", 0($30)" << endl;
	cout << "add $30, $30, $4" << endl;
}

void pushReg() {
	push(6);
}

void popReg() {
	pop(6);
}


string genLabelCode() {
	stringstream ss;
	ss << labelCounter;
	labelCounter++;
	string num;
	ss >> num;
	return num;
}
