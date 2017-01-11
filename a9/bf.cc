#include <map>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
int off;//offset

struct Tree {
  string rule;
  vector <Tree*> children;
  vector <string> tokens;
  ~Tree() {
    for(vector<Tree*>::iterator it = children.begin(); it != children.end(); it++) {  // delete all subtrees      
      delete (*it);
    }
  }
};


void traverse(Tree * tmp, int &idc){
  string line, tok;
  int counter = 0;
  getline(cin, line);
  if(line == "INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
    
  }
  tmp->rule = line;                                                                            
  istringstream iss(line);
  iss >> tok;
  if(tok == ""){
    idc = 1;
    return;
  }
  if(tok == "BOF" || tok == "BECOMES" || tok == "COMMA" || tok == "ELSE" ||
     tok == "EOF" || tok == "EQ" || tok == "GE"  || tok == "ID" || tok == "IF" ||
     tok == "INT" || tok == "LBRACE" || tok == "LE" || tok == "LPAREN" || tok == "WAIN" ||                              
     tok == "LT" || tok == "MINUS" || tok == "NE" || tok == "PCT" || tok == "NUM" ||                      
     tok == "PCT" || tok == "PLUS" || tok == "PRINTLN" || tok == "RBRACE" || tok == "RETURN" ||          
     tok == "RPAREN" || tok == "SEMI" || tok == "SLASH" || tok == "STAR" || tok == "WHILE" ||            
     tok == "AMP" || tok == "LBRACK" || tok == "RBRACK" || tok == "NEW" || tok == "DELETE" ||             
     tok == "NULL"){
    tmp->tokens.push_back(tok); 
    iss >> tok;
    tmp->tokens.push_back(tok); 
  } else{
    while(iss >> tok){                                                                               
      counter++;                                                                                      
      tmp->tokens.push_back(tok);                                                                   
    } 
  }
  if(counter != 0){
    for(int i = 0; i < counter; i++){                                                                   
      Tree *n = new Tree();                                                             
      tmp->children.push_back(n);                                                                       
    } 
    for(vector<Tree*>::iterator it = tmp->children.begin(); it != tmp->children.end(); it++){
      traverse(*it, idc);
    }
  } else{
    tmp->children.push_back(NULL);
  }
  
}

void find(Tree * tmp, map<string, string> &sym_tab, vector<map<string, string> > &vecofst){
  if(tmp->children[0] != NULL){
    if(tmp->rule != "dcl type ID"){
      for(int i = 0; i < tmp->children.size(); i++){
	find(tmp->children[i], sym_tab, vecofst);
      }
    } else{
      string type, type1;
      istringstream ss1(tmp->children[0]->rule);
      ss1 >> type >> type;
      if(ss1 >> type1){
	type = "int*";
      } else{
	type = "int";
      }
      istringstream ss2(tmp->children[1]->rule);
      string name;
      ss2 >> name >> name;
      if(sym_tab.count(name) > 0){
	cerr << "ERROR duplicate define variables" << endl;
        return;
      } else{
	map<string, string> temp;
	temp[name] = type;
	sym_tab.insert(pair<string, string>(name, type));
	vecofst.push_back(temp);
      }
    }
  }
  if(tmp->children[0] == NULL){
    istringstream ss(tmp->rule);
    string tok;
    ss >> tok;
    if(tok == "WAIN"){
      ss >> tok;
      if(sym_tab.count(tok) > 0){                                                                                               
	cerr << "ERROR duplicate define Wain" << endl;                                                                                                      
	return;                                                                                                                       
      } else{
	map<string, string> temp;
	temp["wain"] = "";
	sym_tab.insert(pair<string, string>(tok, ""));
	vecofst.push_back(temp);
      }
    }	
  }
  return;
}

void print(Tree * tmp, map<string, string> &sym_tab, int &indic){
  if(tmp->children[0] != NULL){
    if(tmp->rule != "factor ID"){
      for(int i = 0; i < tmp->children.size(); i++){
	print(tmp->children[i], sym_tab, indic);
      }
    } else{
      string a;
      istringstream ss(tmp->children[0]->rule);
      ss >> a >> a;
      if(sym_tab.count(a) == 0){
	cerr << "ERROR never define some variables" << endl;
	indic = 1;
	return;
      } 
    }
  }
  return;
}

void test(Tree * tmp, map<string, string> &sym_tab, vector<string> &code, vector<map<string, string> > &vecofst){
  code.push_back("lis $4");
  code.push_back(".word 4");
  code.push_back("sub $29,$30,$4");
  code.push_back("sub $30,$30,$4");
  if(tmp->children[0] != NULL){
    if(tmp->rule != "expr term"){
      for(int i = 0; i < tmp->children.size(); i++){
        test(tmp->children[i], sym_tab, code, vecofst);
      }
    } else if(tmp->rule == "expr term"){
      if(tmp->children[0]->children[0]->rule == "factor ID"){
	Tree * now;
	now = tmp->children[0]->children[0];
	istringstream ss(now->children[0]->rule);
	string name;
	ss >> name >> name;
	if(sym_tab.count(name) > 0){
	  int counter = 0;
	  for(vector<map<string, string> >::iterator it = vecofst.begin(); it != vecofst.end(); it++){
	    map<string, string> temp = *it;
	    map<string, string>::iterator i = temp.begin();
	    if(i->first == name){
	      break;
	    }
	    counter++;
	  }
	  if(counter == 1){
	    code.push_back("add $3, $1, $0");
	  } else{
	    code.push_back("add $3, $2, $0");
	  }
	  code.push_back("jr $31");
	  return;
	} else{
	  return;
	}
      } else if(tmp->children[0]->children[0]->rule == "factor LPAREN expr RPAREN"){
	Tree *now;
	now = tmp->children[0]->children[0]->children[1];
	for(int i = 0; i < tmp->children.size(); i++){
	  test(tmp->children[i], sym_tab, code, vecofst);
	}
	return;
      }
    }
  }
  return;
}


int main(){
  map<string, string> sym_tab;
  vector<map<string, string> > vecofst;
  vector<string> code;
  int idc = 0;
  Tree *tmp = new Tree();
  traverse(tmp, idc);
  int indic = 0;
  find(tmp, sym_tab, vecofst);
  print(tmp, sym_tab, indic);
  if(indic == 0){
    cerr << sym_tab.find("wain")->first << endl;
    map<string, string>::iterator it = sym_tab.find("wain");
    sym_tab.erase(it);
    for(map<string, string>::iterator it = sym_tab.begin(); it != sym_tab.end(); it++){
      cerr << it->first << " " << it->second << endl;  
    }
  }
  test(tmp, sym_tab, code, vecofst);
  for(vector<string>::iterator it = code.begin(); it != code.end(); it++){
    cout << (*it) << endl;
  }
}
    
