#include <map>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
map<string, map<string, int> > off;//offset
string offset;
int counter = 0;
int indic = 0;
map<string, vector<string> > proc;
map<string, string> sym_tab;

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
     tok == "GT" || tok == "INT" || tok == "LBRACE" || tok == "LE" || tok == "LPAREN" || tok == "WAIN" ||                              
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

string key(Tree *tmp){
  string a;
  istringstream ss(tmp->rule);
  ss >> a;
  return a;
}

string name(Tree * tmp){
  string name, b;
  istringstream ss(tmp->rule);
  ss >> b;
  while(ss >> b){
    name += b;
  }
  return name;
}

string type(Tree *tmp, map<string, string> &sym_tab){
  string first = key(tmp);
  string second = name(tmp);
  if(first == "expr"){
    if(tmp->children.size() > 1){
      string expr1 = type(tmp->children[0], sym_tab);
      string expr2 = type(tmp->children[2], sym_tab);
      if(tmp->children[1]->rule == "expr expr PLUS term" || tmp->children[1]->rule == "expr expr MINUS term"){
        if((expr1 == "int" && expr2 == "int*")
           || (expr1 == "int*" && expr2 == "int")){
          return "int*";
        } else if(expr1 == "int" && expr2 == "int"){
          return "int";
        } else if(expr1 == "int*" && expr2 == "int*"){
          string err = "ERROR not valid for plus or minus";
          throw err;
        }
      }
    } else{
      string expr1 = type(tmp->children[0], sym_tab);
      return expr1;
    }
  } else if(first == "factor"){
    if(tmp->rule == "factor ID" || tmp->rule == "factor NUM" || tmp->rule == "factor NULL"){
      istringstream ss(tmp->children[0]->rule);
      string name;
      ss >> name >> name;
      if(second == "ID"){
        return sym_tab[name];
      } else if(second == "NULL"){
        return "int*";
      } else if(second == "NUM"){
        return "int";
      }
    } if(tmp->rule == "factor LPAREN expr RPAREN"){
      string e = type(tmp->children[1], sym_tab);
      return e;
    } else if(tmp->rule == "factor AMP lvalue"){
      string e = type(tmp->children[1], sym_tab);
      if(e == "int"){
        return "int*";
      } else{
        string err = "ERROR no int** this type";
        throw err;
      }
    } else if(tmp->rule == "factor STAR factor"){
      string e = type(tmp->children[1], sym_tab);
      if(e == "int*"){
	return "int";
      } else{
	string err = "ERROR no int&& this type";
	throw err;
      }
    } else if(tmp->rule == "factor ID LAPREN RPAREN"){
      istringstream ss(tmp->children[0]->rule);
      string name;
      ss >> name >> name;
      if(proc[name].size() == 0){
	return "int";
      }
    } else if(tmp->rule == "factor ID LAPREN arglist RPAREN"){
      istringstream ss(tmp->children[0]->rule);
      string name;
      ss >> name >> name;
      Tree * temp = tmp->children[2];
      vector<string> para;
      while(temp->children.size() > 1){
	string e = type(temp->children[0], sym_tab);
	para.push_back(e);
	temp = temp->children[2];
      }
      temp = temp->children[0];
      string e = type(temp->children[0], sym_tab);
      para.push_back(e);
      if(para.size() != proc[name].size()){
	string err = "ERROR func paramaters not equal";
	throw err;
      }
      for(int i = 0; i < proc[name].size(); i++){
	string front = para[i];
	string back = proc[name][i];
	if(front != back){
	  string err = "ERROR not match argument";
	  throw err;
	}
      }
      return "int";
    } else if(tmp->rule == "factor NEW INT LBRACK exor RBRACK"){
      string e = type(tmp->children[3], sym_tab);
      if(e == "int"){
	return "int*";
      } else{
	string err = "ERROR wrong with malloc";
	throw err;
      }
    }
  } else if(first == "term"){
    if(tmp->children.size() == 1){
      string e = type(tmp->children[0], sym_tab);
      return e;
    } else{
      if(tmp->rule == "term term STAR factor"){
	string expr1 = type(tmp->children[0], sym_tab);
	string expr2 = type(tmp->children[2], sym_tab);
	if(expr1 == "int" && expr2 == "int"){
	  return "int";
	} else{
	  string err = "ERROR int** invalid";
	  throw err;
	}
      } else if(tmp->rule == "term term SLASH factor"){
	string expr1 = type(tmp->children[0], sym_tab);
	string expr2 = type(tmp->children[2], sym_tab);
	if(expr1 == "int" && expr2 == "int"){
	  return "int";
	} else{
	  string err = "ERROR int** invalid";
	  throw err;
	}
      } else if(tmp->rule == "term PCT factor"){
	string expr1 = type(tmp->children[0], sym_tab);
	string expr2 = type(tmp->children[2], sym_tab);
	if(expr1 == "int" && expr2 == "int"){
	  return "int";
	} else{
	  string err = "ERROR int** invalid";
	  throw err;
	}
      }
    }
  } else if(first == "lvalue"){
    if(tmp->children.size() == 1){
      istringstream ss(tmp->children[0]->rule);
      string name;
      ss >> name >> name;
      return sym_tab[name];
    } else if(tmp->rule == "lvalue STAR factor"){
      string expr1 = type(tmp->children[1], sym_tab);
      if(expr1 == "int"){
	string err = "ERROR ";
	throw err;
      }
      return "int";
    } else if(tmp->rule == "lvalue LPAREN lvalue RPAREN"){
      string e =  type(tmp->children[1], sym_tab);
      return e;
    }
  } else if(first == "main"){
    istringstream ss(tmp->children[5]->children[0]->rule);
    string name;
    ss >> name >> name;
    if(ss >> name){
      return "int*";
    } else{
      return "int";
    }
  } else if(first == "dcls"){
    if(tmp->rule == "dcls dcls dcl BECOMES NUM SEMI"){
      istringstream ss(tmp->children[1]->children[0]->rule);
      string name;
      ss >> name >> name;
      if(ss >> name){
	name = "int*";
      } else{
	name = "int";
      }
      return name;
    } else if(tmp->rule == "dcls dcl BECOMES NULL SEMI"){
      istringstream ss(tmp->children[1]->children[0]->rule);
      string name;
      ss >> name >> name;
      if(ss >> name){
	name = "int*";
      } else{
	name = "int";
      }
      return name;
    }
  }
}

string funname;

void push(string reg){
  cout << "sw " << reg << ", -4($30)" << endl;
  cout << "sub $30, $30, $4" << endl;
}

void pop(string reg){
  cout << "add $30, $30, $4" << endl;
  cout << "lw " << reg << ", -4($30)" << endl;
}

int q = 0;
void test(Tree * tmp){
  if(tmp->rule == "start BOF procedures EOF"){
    test(tmp->children[1]);
  } else if(tmp->rule == "procedures main"){
    test(tmp->children[0]);
  } else if(tmp->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
    //string funname;
    istringstream ss(tmp->rule);
    ss >> funname >> funname >> funname;
    cout << "lis $4" << endl;
    cout << ".word 4" << endl;
    cout << "sub $29, $30, $4" << endl;
    string type;
    istringstream ss1(tmp->children[3]->children[1]->rule);
    ss1 >> type >> type;
    off[funname][type] = 0;
    counter++;
    cout << "sw $1, 0($29)" << endl;
    string type1;
    istringstream ss2(tmp->children[5]->children[1]->rule);
    ss2 >> type1 >> type1;
    off[funname][type1] = -4;
    counter++;
    cout << "sw $2, -4($29)" << endl;
    cout << "sub $30, $30, $4" << endl;
    cout << "sub $30, $30, $4" << endl;
    test(tmp->children[8]);
    test(tmp->children[9]);
    test(tmp->children[11]);
    cout << "jr $31" << endl;
  } else if(tmp->rule == "expr term"){
    test(tmp->children[0]);
  } else if(tmp->rule == "term factor"){
    test(tmp->children[0]);
  } else if(tmp->rule == "factor ID"){
    string name;
    istringstream ss(tmp->children[0]->rule);
    ss >> name >> name;
    cout << "lw $3, " << off[funname][name] << "($29)" << endl;
  } else if(tmp->rule == "factor LPAREN expr RPAREN"){
    test(tmp->children[1]);
  } else if(tmp->rule == "expr expr PLUS term"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "add $3, $8, $3" << endl;
  } else if(tmp->rule == "expr expr MINUS term"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "sub $3, $8, $3" << endl;
  } else if(tmp->rule == "term term STAR factor"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "mult $3, $8" << endl;
    cout << "mflo $3" << endl;
  } else if(tmp->rule == "term term SLASH factor"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "div $8, $3" << endl;
    cout << "mflo $3" << endl;
  } else if(tmp->rule == "term term PCT factor"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "div $8, $3" << endl;
    cout << "mfhi $3" << endl;
  } else if(tmp->rule == "factor NUM"){
    string num;
    istringstream ss(tmp->children[0]->rule);
    ss >> num >> num;
    cout << "lis $3" << endl;
    cout << ".word " << num << endl;
  } else if(tmp->rule == "statements statements statement"){
    test(tmp->children[0]);
    test(tmp->children[1]);
  } else if(tmp->rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
    test(tmp->children[2]);
    push("$1");
    cout << "add $1, $3, $0" << endl;
    push("$31");
    cout << "lis $9" << endl;
    cout << ".word print" << endl;
    cout << "jalr $9" << endl;
    pop("$31");
    pop("$1");
  } else if(tmp->rule == "dcls dcls dcl BECOMES NUM SEMI"){
    test(tmp->children[0]);
    string type1;
    istringstream ss2(tmp->children[1]->children[1]->rule);
    ss2 >> type1 >> type1;
    off[funname][type1] = -4 * (counter);
    counter++;
    cout << "lis $7" << endl;
    string num;
    istringstream ss(tmp->children[3]->rule);
    ss >> num >> num;
    cout << ".word " << num << endl;
    cout << "sw $7, " << off[funname][type1] << "($29)" << endl;
    cout << "sub $30, $30, $4" << endl;
  } else if(tmp->rule == "statement lvalue BECOMES expr SEMI"){
    test(tmp->children[0]);
    test(tmp->children[2]);
    cout << "sw $3, " << off[funname][offset] << "($29)" << endl;
  } else if(tmp->rule == "lvalue ID"){
    istringstream ss1(tmp->children[0]->rule);
    ss1 >> offset >> offset;
  } else if(tmp->rule == "lvalue LPAREN lvalue RPAREN"){
    test(tmp->children[1]);
  } else if(tmp->rule == "test expr LT expr"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$5");
    cout << "slt $3, $5, $3" << endl;
  } else if(tmp->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
    q++;
    int tempq = q;
    cout << "loop" << tempq << ":" << endl;
    test(tmp->children[2]);
    cout << "beq $3, $0, endloop" << tempq << endl;
    test(tmp->children[5]);
    cout << "beq $0, $0, loop" << tempq << endl;
    cout << "endloop" << tempq << ":" << endl;
  } else if(tmp->rule == "test expr EQ expr"){
    cout << "lis $11" << endl;
    cout << ".word 1" << endl;
    test(tmp->children[0]);
    cout << "add $5, $3, $0" <<endl;
    test(tmp->children[2]);
    cout << "slt $6, $3, $5" <<endl;
    cout << "slt $13, $5, $3" << endl;
    cout << "add $3, $13, $6" << endl;
    cout << "sub $3, $11, $3" << endl;
  } else if(tmp->rule == "test expr NE expr"){
    test(tmp->children[0]);
    cout << "add $5, $3, $0" << endl;
    test(tmp->children[2]);
    cout << "slt $6, $3, $5" << endl;
    cout << "slt $13, $5, $3" << endl;
    cout << "add $3, $13, $6" << endl;
  } else if(tmp->rule == "test expr LE expr"){
    cout << "lis $11" << endl;
    cout << ".word 1" << endl;
    test(tmp->children[0]);
    cout << "add $5, $3, $0" <<endl;
    test(tmp->children[2]);
    cout << "slt $6, $3, $5" <<endl;
    cout << "slt $13, $5, $3" << endl;
    cout << "add $3, $13, $6" << endl;
    cout << "sub $12, $11, $3" << endl;
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$5");
    cout << "slt $3, $5, $3" << endl;
    cout << "add $3, $3, $12" << endl;
  } else if(tmp->rule == "test expr GE expr"){
    cout << "lis $11" << endl;
    cout << ".word 1" << endl;
    test(tmp->children[0]);
    cout << "add $5, $3, $0" <<endl;
    test(tmp->children[2]);
    cout << "slt $6, $3, $5" <<endl;
    cout << "slt $13, $5, $3" << endl;
    cout << "add $3, $13, $6" << endl;
    cout << "sub $12, $11, $3" << endl;
    test(tmp->children[2]);
    push("$3");
    test(tmp->children[0]);
    pop("$5");
    cout << "slt $3, $5, $3" << endl;
    cout << "add $3, $3, $12" << endl;
  } else if(tmp->rule == "test expr GT expr"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$5");
    cout << "slt $3, $3, $5" << endl;
  } else if(tmp->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
    q++;
    int tempq = q;
    test(tmp->children[2]);
    cout << "beq $3, $0, else" << tempq << endl;
    test(tmp->children[5]);
    cout << "beq $0, $0, endif" << tempq << endl;
    cout << "else" << tempq <<  ":" << endl;
    test(tmp->children[9]);
    cout << "endif" << tempq << ":" << endl;
  } else if(tmp->rule == "INT STAR"){

  } else if(tmp->rule == "dcls dcls dcl BECOMES NULL SEMI"){

  } else if(tmp->rule == "factor NULL"){
    string e = type(tmp, sym_tab);
    cout << e << endl;
    cout << "add $3, $0, $0" << endl;
  } else if(tmp->rule == "factor AMP lvalue"){
    
  } else if(tmp->rule == "factor STAR factor"){
    test(tmp->children[1]);
    cout << "lw $3, 0($3)" << endl;
  } else if(tmp->rule == "lvalue STAR factor"){
  }
}




void check(Tree *tmp, map<string, string> &sym1, map<string, string> &sym_tab, map<string, vector<string> > &proc){
  string a = key(tmp);
  string second = name(tmp);
  if(a == "procedure"){
    string t;
    istringstream ss1(tmp->children[1]->rule);
    ss1 >> t >> t;
    if(sym1.count(t) != 0){
      string err ="ERROR double defn";
      throw err;
    }
    sym1.insert(pair<string, string>(t, "int"));
    vector<string> list;
    cerr << t;
    Tree* child = tmp->children[3];
    string begin = name(child);
    if(child->children[0]==NULL)cerr << endl;
    if(begin == "paramlist"){
      child = child->children[0];
      while(child->children.size() == 3){
        Tree *child1 = child->children[0];
        string type;
        istringstream ss11(child1->children[0]->rule);
        ss11 >> type >> type;
        if(ss11 >> type){
          type = "int*";
        } else{
          type = "int";
        }
        istringstream ss2(child->children[1]->rule);
        string name;
        ss2 >> name >> name;
        sym_tab[name] = type;
        list.push_back(type);
        cerr << " " << type;
        child = child->children[2];
      }
      Tree *child2 = child->children[0];
      string type2;
      istringstream ss12(child2->children[0]->rule);
      ss12 >> type2 >> type2;
      if(ss12 >> type2){
        type2 = "int*";
      } else{
	type2 = "int";
      }
      cerr << " " << type2 << endl;
    }
    proc[t] = list;
    map<string, string> sym_tabs;
    for(int i = 2; i < tmp->children.size(); i++){
      check(tmp->children[i],sym1, sym_tabs, proc);
    }
    cerr << endl;
    return;
  } else if(a == "main"){
    if(sym1.count("wain") != 0){
      string err = "ERROR wain is declared before";
      throw err;
    }
    sym1["wain"] = "int";
    string name1, name2;
    istringstream n1(tmp->children[3]->children[0]->rule);
    n1 >> name1 >> name1;
    if(n1 >> name1){
      name1 = "int*";
    } else{
      name1 = "int";
    }
    istringstream n2(tmp->children[5]->children[0]->rule);
    n2 >> name2 >> name2;
    if(n2 >> name2){
      name2 = "int*";
    } else{
      name2 = "int";
    }
    cerr << "wain " <<  name1 << " " << name2 << endl;
    map<string, string> sym_tabs;
    sym_tab = sym_tabs;
  } else if(a == "dcl"){
    string t1, t2;
    istringstream iss(tmp->children[0]->rule);
    istringstream iss1(tmp->children[1]->rule);
    iss >> t1 >> t1;
    if(iss >> t1){
      t1 = "int*";
    } else{
      t1 = "int";
    }
    iss1 >> t2 >> t2;
    if(sym_tab.count(t2) != 0){
      string err = "ERROR duplicate defn";
      throw err;
    }
    if(t1 == "int"){
      sym_tab[t2] = t1;
      cerr << t2 << " " << t1 << endl;
    } else{
      sym_tab[t2] = t1;
      cerr << t2 << " " << t1 << endl;
    }
  } else if(a == "ID"){
    if(sym_tab.count(second) == 0){
	string err = "ERROR var never defn";
	throw err;
      }
  } else if(tmp->rule == "factor ID LPAREN RPAREN" || tmp->rule == "factor ID LPAREN arglist RPAREN"){
    string t1;
    istringstream iss(tmp->children[0]->rule);
    iss >> t1 >> t1;
    if(sym_tab.count(t1) != 0){
      string err =  "ERROR var defn before ";
      throw err;
    } else if(sym1.count(t1) == 0){
      string err = "ERROR never defn";
      throw err;
    } else{
      for(int i = 1; i < tmp->children.size(); i++){
        check(tmp->children[i], sym1, sym_tab, proc);
      }
    }
    return;
  }
  if(tmp->children.size() == 0) return;
  if(tmp->children[0] != NULL && tmp->children.size() != 0){
    for(int i = 0; i < tmp->children.size(); i++){
      check(tmp->children[i], sym1, sym_tab, proc);
    }
  }
}


int main(){
  map<string, string> sym1;
  vector<string> code;
  int idc = 0;
  Tree *tmp = new Tree();
  traverse(tmp, idc);
  int indic = 0;
  check(tmp, sym1, sym_tab, proc);
  //  find(tmp, sym_tab, vecofst);
  //print(tmp, sym_tab, indic);
  //if(indic == 0){
  // cerr << sym_tab.find("wain")->first << endl;
  // map<string, string>::iterator it = sym_tab.find("wain");
  // sym_tab.erase(it);
  // for(map<string, string>::iterator it = sym_tab.begin(); it != sym_tab.end(); it++){
  //   cerr << it->first << " " << it->second << endl;  
  // }
  // }
  cout << ".import print" << endl;
  test(tmp);
  for(vector<string>::iterator it = code.begin(); it != code.end(); it++){
    cout << (*it) << endl;
  }
}
    
