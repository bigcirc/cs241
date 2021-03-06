#include <map>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
map<string, vector<string> > proc;

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
  tmp->rule = line;                                                                            
  istringstream iss(line);
  iss >> tok;
  if(tok == ""){
    idc = 1;
    return;
  }
  if(tok == "BOF" || tok == "BECOMES" || tok == "COMMA" || tok == "ELSE" ||
     tok == "EOF" || tok == "EQ" || tok == "GE"  || tok == "ID" || tok == "IF" ||
     tok == "GT" ||tok == "INT" || tok == "LBRACE" || tok == "LE" || tok == "LPAREN" || tok == "WAIN" ||                              
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

void find2(Tree *tmp, map<string, string> &sym, map<string, vector<string> > &proc){
  if(tmp->children[0] == NULL){
    istringstream ss(tmp->rule);
    string tok;
    ss >> tok;
    if(tok == "ID"){
      ss >> tok;
      if(proc.count(tok) > 0 ){
	cerr << "ERROR duplicate define procedure" << endl;
	return;
      } else{
	proc.insert(pair<string, vector<string> >(tok, vector<string>()));
      }
    }
  }	
}

void find1(Tree *tmp, map<string, string> &sym, map<string, vector<string> > &proc, string t){
  string t0, t1;
  if(tmp->children[0] != NULL){
    if(tmp->rule != "dcl type ID" && tmp->rule != "paramlist dcl" && tmp->rule != "paramlist dcl COMMA paramlist"){
      for(int i = 0; i < tmp->children.size(); i++){
	istringstream ss(tmp->children[i]->rule);
	ss >> t0;
	if( t0 == "INT"){
	  if( (i + 1) < tmp->children.size()){
	    istringstream ss1(tmp->children[i + 1]->rule);
	    ss1 >> t1;
	    if( t1 == "ID" ){
	      if( (i + 2) < tmp->children.size()){
		istringstream ss2(tmp->children[i + 2]->rule);
		ss2 >> t1;
		if( t1 == "LPAREN"){
		  find2(tmp->children[i + 1],sym, proc);
		}
	      }
	      ss1 >> t0;
	      t = t0;
	    }
	  }
	}
	find1(tmp->children[i], sym, proc, t);
      }
    } else if(tmp->rule == "dcl type ID"){
      string type, type1;
      istringstream ss1(tmp->children[0]->rule); 
      ss1 >> type >> type1; 
      if(ss1 >> type1){
        type = "int*";
      } else{
        type = "int";
      }
      istringstream ss2(tmp->children[1]->rule);
      string name;
      ss2 >> name >> name;
      if(sym.count(name) > 0 ){
	map<string, string>::iterator it = sym.find(name);
      } else{
        sym.insert(pair<string, string>(name, type));
      }
    } else if(tmp->rule == "paramlist dcl" || tmp->rule == "paramlist dcl COMMA paramlist"){
      for(int i = 0; i < tmp->children.size(); i++){
	if(tmp->children[i]->rule == "dcl type ID"){
	  string type, type1;
	  istringstream ss1(tmp->children[i]->children[0]->rule);
	  ss1 >> type >> type1;
	  if(ss1 >> type1){
	    type = "int*";
	  } else{
	    type = "int";
	  }
	  istringstream ss2(tmp->children[i]->children[1]->rule);
	  string name;
	  ss2 >> name >> name;
	  if(sym.count(name) > 0 ){
	    map<string, string>::iterator it = sym.find(name);
	  } else{
	    sym.insert(pair<string, string>(name, type));
	  }
	  proc.find(t)->second.push_back(type);
	}else{
	  find1(tmp->children[i], sym, proc, t);
	}
      }
    }
  } else if(tmp->children[0] == NULL){
    if(tmp->rule == "params"){
      cerr << t << " " << "" << endl;
      cerr << endl;
    }
    return;
  }
}

void find4(Tree * tmp, map<string, string> &sym_tab, map<string, string> &sym1, map<string, vector<string> > &proc){
  if(tmp->rule == "dcl type ID"){
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
      return;
    } else{
      sym_tab.insert(pair<string, string>(name, type));
    } 
    sym_tab.find("wain")->second += type + " ";
  }
}
void find(Tree * tmp, map<string, string> &sym_tab, map<string, string> &sym1, map<string, vector<string> > &proc){
  if(tmp->children[0] != NULL){
    if(tmp->rule != "dcl type ID" && tmp->rule != "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE" ){
      for(int i = 0; i < tmp->children.size(); i++){
	if(tmp->children[i]->rule == "dcl type ID"){
	  if( i + 1 < tmp->children.size()){
	    if(tmp->children[i + 1]->rule == "COMMA ,"){
	      if( i+ 2 < tmp->children.size()){
		if(tmp->children[i + 2]->rule == "dcl type ID"){
		  find4(tmp->children[i], sym_tab, sym1, proc);
		  find4(tmp->children[i + 2], sym_tab, sym1, proc);
		  i += 3;
		}
	      }
	    }
	  }
	}
	find(tmp->children[i], sym_tab, sym1, proc);
      }
    } else if(tmp->rule == "dcl type ID"){
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
	sym_tab.insert(pair<string, string>(name, type));
      }
    } else if("procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
      find1(tmp, sym1, proc, "");
      if(!proc.empty()){
	cerr << proc.begin()->first << " ";
	for(vector<string>::iterator it = proc.begin()->second.begin(); it != proc.begin()->second.end(); it++){
	  cerr << (*it) << " ";
	}
	cerr << endl;
	cerr << endl;
	proc.erase(proc.begin());
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
	sym_tab.insert(pair<string, string>(tok, ""));
      }
    }	
  }
  return;
}

void print(Tree * tmp, map<string, string> &sym_tab, map<string, string> &sym1, int &indic){
  if(tmp->children[0] != NULL){
    if(tmp->rule != "factor ID" && tmp->rule != "factor NUM"){
      for(int i = 0; i < tmp->children.size(); i++){
	print(tmp->children[i], sym_tab, sym1, indic);
      }
    } else if(tmp->rule == "factor ID"){
      string a;
      istringstream ss(tmp->children[0]->rule);
      ss >> a >> a;
      if(sym_tab.count(a) == 0 ){
	if(sym1.count(a) == 0){
	  cerr << "ERROR never define some variables" << endl;
	  indic = 1;
	  return;
	}
      } 
    } else if(tmp->rule == "factor NUM"){
      return;
    }
  }
  //if(tmp->children[0] == NULL){
  // cout << tmp->rule << endl;
  // }
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
    } else if(tmp->rule == "ID LAPREN arglist RPAREN"){
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
  map<string, string> sym_tab;
  map<string, string> sym1;
  int idc = 0;
  Tree *tmp = new Tree();
  traverse(tmp, idc);
  //if(idc == 1){
   // cerr << "Input is ERROR" << endl;
   // return 1;
  //}
  int indic = 0;
  try{
    check(tmp, sym1, sym_tab, proc);
  } catch(string err){
    cerr << err << endl;
  }
  //  find(tmp, sym_tab, sym1, proc);
  //print(tmp, sym_tab, sym1, indic);
  //  if(indic == 0){
    
  // cerr << sym_tab.find("wain")->first << " " << sym_tab.find("wain")->second << endl;
    //map<string, string>::iterator it = sym_tab.find("wain");
    //sym_tab.erase(it);
    //for(map<string, string>::iterator it = sym_tab.begin(); it != sym_tab.end(); it++){
    //cerr << it->first << " " << it->second << endl;  
    // }
    // }
}
    
