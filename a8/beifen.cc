#include <map>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;


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

void find2(Tree *tmp, map<string, string> &sym, map<string, pair<vector<string>,map<string, string> > > &proc){
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
	proc.insert(pair<string, pair<vector<string>,map<string, string> > >(tok, pair<vector<string>,map<string, string> >()));
      }
    }
  }	
}

void find1(Tree *tmp, map<string, string> &sym, map<string, pair<vector<string>,map<string, string> > > &proc, string t, map<string, string> &symm, int &j){
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
	find1(tmp->children[i], sym, proc, t, symm, j);
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
      
      pair<vector<string>, map<string, string> >  inner = proc.begin()->second;
      //vector<pair<vector<string>, map<string, string> > > inner1;
      //for(int i = 0; i < inner.size(); i++){
      //	map<string, pair<vector<string>, map<string, string> > >::iterator it = inner[i];
      //	pair<vector<string>, map<string, string> > in = (*it).second;
      //	inner1.push_back(in);
      // }
      vector<string> in = inner.first;
      if(sym.count(name) > 0){
	cerr << name << "1ERROR happens" << endl;
	j = 0;
	return;
	map<string, string>::iterator it = sym.find(name);
      } else if(sym.count(name) == 0){
	sym.insert(pair<string, string>(name, type));
      }
      if(symm.count(name) > 0){
        cerr << name << "2ERROR happens" << endl;
        map<string, string>::iterator it = symm.find(name);
      } else if(symm.count(name) == 0){
        symm.insert(pair<string, string>(name, type));
      }
	
      if(proc[t].second.count(name) > 0 ){
	//map<string, string>::iterator it = sym.find(name);
	cerr << "duplicate procedure element ERROR" << endl;
	return;
      } else{
	proc[t] = make_pair(in, symm);
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


	  if(sym.count(name) > 0){
	    cerr << name << "3ERROR happens" << endl;
            j = 0;
            map<string, string>::iterator it = sym.find(name);
          } else{
            sym.insert(pair<string, string>(name, type));
          }
	  proc.find(t)->second.first.push_back(type);   
	  if(symm.count(name) > 0){
	    cerr << name << "4ERROR happens" << endl;
	    j = 0;
	    map<string, string>::iterator it = symm.find(name);
	  } else if(symm.count(name) == 0){
	    symm.insert(pair<string, string>(name, type));
	  }
	  pair<vector<string>, map<string, string> >  in = proc.begin()->second;
	  vector<string> inn = in.first;
	  if(proc[t].second.count(name) > 0 ){
	    cerr <<  "ERROR duplicate var in procedure's dcl" << endl;
	  } else{
	    proc[t] = make_pair(inn,symm);
	  }
	  //proc.find(t)->second.first.push_back(type);




	}else{
	  find1(tmp->children[i], sym, proc, t, symm, j);
	}
      }
    }
  } else if(tmp->children[0] == NULL){
    if(tmp->rule == "params"){
      proc.find(t)->second.first.push_back("");
    }
    return;
  }
}

void find4(Tree * tmp, map<string, string> &sym_tab, map<string, string> &sym1, map<char, map<string, pair<vector<string>, map<string, string> > > > &order, int &idc, char &counter){
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
      cerr << "ERROR duplicate variables" << endl;
      idc = 1;
      return;
    } else{
      sym_tab.insert(pair<string, string>(name, type));
    } 
    sym_tab.find("wain")->second += type + " ";
  }
}
void find(Tree * tmp, map<string, string> &sym_tab, map<string, string> &sym1, map<string, pair<vector<string>,map<string, string> > > &proc, map<char, map<string, pair<vector<string>,map<string, string> > > > &order, char &counter, int &idc, map<string, string> &symm, int &j){

  if(tmp->children[0] != NULL){
    if(tmp->rule != "dcl type ID" && tmp->rule != "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE" ){
      for(int i = 0; i < tmp->children.size(); i++){
	if(tmp->children[i]->rule == "dcl type ID"){
	  if( i + 1 < tmp->children.size()){
	    if(tmp->children[i + 1]->rule == "COMMA ,"){
	      if( i+ 2 < tmp->children.size()){
		if(tmp->children[i + 2]->rule == "dcl type ID"){
		  find4(tmp->children[i], sym_tab, sym1, order, idc, counter);
		  find4(tmp->children[i + 2], sym_tab, sym1, order, idc, counter);
		  i += 3;
		}
	      }
	    }
	  }
	}
	find(tmp->children[i], sym_tab, sym1, proc, order, counter, idc, symm, j);
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
    } else if(tmp->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
      find1(tmp, sym1, proc, "", symm, j);
      if(j == 0) return;
      if(!proc.empty()){
	order.insert(pair<char, map<string, pair<vector<string>,map<string, string> > > >(counter, map<string, pair<vector<string>,map<string, string> > >()));
	order[counter].insert(make_pair(proc.begin()->first, proc.begin()->second));
	counter++;
	proc.erase(proc.begin());
	symm.clear();
	//cerr << proc.begin()->first << " ";
	// for(vector<string>::iterator it = proc.begin()->second.begin(); it != proc.begin()->second.end(); it++){
	//  cerr << (*it) << " ";
	// }
	// cerr << endl;
	// cerr << endl;
	// proc.erase(proc.begin());
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

void print(Tree * tmp, map<string, string> &sym_tab, map<string, string> &sym1, int &indic, map<char, map<string, pair<vector<string>, map<string, string> > > > &order, int &indic1, char &counter, int &k){
  if(indic1 == 1) return;
  if(tmp->children[0] != NULL){
    if(tmp->rule != "procedure INT WAIN LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE" &&
       tmp->rule != "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
      for(int i = 0; i < tmp->children.size(); i++){
	print(tmp->children[i],sym_tab, sym1, indic, order, indic1, counter, k);
      }
    } else if(tmp->rule == "procedure INT WAIN LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
      if(tmp->rule !="factor ID LPAREN arglist RPAREN" &&tmp->rule != "factor ID" && tmp->rule != "factor NUM"){
	for(int i = 0; i < tmp->children.size(); i++){
	  k = 1;
	  print(tmp->children[i], sym_tab, sym1, indic, order, indic1, counter, k);
	}
      } 
    } else if(tmp->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
      if(tmp->rule !="factor ID LPAREN arglist RPAREN" &&tmp->rule != "factor ID" && tmp->rule != "factor NUM"){
        for(int i = 0; i < tmp->children.size(); i++){
	  k = 0;
          print(tmp->children[i], sym_tab, sym1, indic, order, indic1, counter, k);
        }
      }
    }
    if(tmp->rule == "factor ID"){
      if(k == 1){
	string a;
	istringstream ss(tmp->children[0]->rule);
	ss >> a >> a;
	if(sym_tab.count(a) == 0 ){
	  char t = 'a';
	  vector<map<string, pair<vector<string>,map<string, string> > > > inner;
	  for( map<char, map<string, pair<vector<string>,map<string, string> > > >::iterator it = order.begin(); it != order.end(); it++){
	    map<string, pair<vector<string>,map<string, string> > > in =  (*it).second;
	    inner.push_back(in);
	    t++;
	  }
	  //if(sym1.count(a) == 0){
	  //cerr << "ERROR never define some variables11" << endl;
	  //indic = 1;
	  //return;
	  //}
	  for(int i = 0; i < inner.size(); i++){
	    map<string, pair<vector<string>,map<string, string> > >::iterator it = inner[i].begin();
	    if( (*it).first.find(a)  > 0){
	      indic = 1;
	      indic1 = 1;
	    }
	  }
	  if(indic1 == 1){
	    cerr << "ERROR never define some variables2" << endl;
	    return;
	  }
	}
      } else if(k == 0){
	char t = 'a';
	string a;
	istringstream ss(tmp->children[0]->rule);
	ss >> a >> a;
	vector<map<string, pair<vector<string>,map<string, string> > > > inner;
	for( map<char, map<string, pair<vector<string>,map<string, string> > > >::iterator it = order.begin(); it != order.end(); it++){
	  map<string, pair<vector<string>,map<string, string> > > in =  (*it).second;
	  inner.push_back(in);
	  t++;
	}
	for(int i = 0; i < inner.size(); i++){
	  map<string, pair<vector<string>,map<string, string> > > ii = inner[i];
	  map<string, pair<vector<string>,map<string, string> > >::iterator it = ii.begin();
	  pair<vector<string>,map<string, string> > ii1 = it->second;
	  
	  map<string, string> ii2 = ii1.second;
	  if(ii2.empty()){
	    cerr << "ERROR duplicate procedure return1" << endl;
	    indic1 = 1;
	    return;
	  }
	  if((ii2).count(a) > 0){
	    cerr << "ERROR duplicate procedure return2" << endl;
	    indic1 = 1;
	    return;
	  }
	}
      }
    } else if(tmp->rule == "factor NUM"){
      return;
    } else if(tmp->rule =="factor ID LPAREN arglist RPAREN"){
      string type, type1;
      istringstream ss1(tmp->children[0]->rule);
      ss1 >> type >> type;
      if(order.empty()){ 
	cerr << "ERROR no procedure" << endl;
	indic1 = 1;
	return;
      }
      for(char a = 'a'; a < counter - 1; a++){
	if(order[a].count(type) == 0){
	  cerr << "ERROR no procedure" << endl;
	  indic1 = 1; 
	  return;
	}
      }
    }
  }
    //if(tmp->children[0] == NULL){
  // cout << tmp->rule << endl;
  // }
  return;
}



int main(){
  int indic1 = 0;
  int j = 1;
  map<string, string> sym_tab;
  map<string, string> sym1;
  map<string, pair<vector<string>,map<string, string> > > proc;
  map<char, map<string, pair<vector<string>, map<string, string> > > > order;
  map<string, string> symm;
  char counter = 'a';
  int idc = 0;
  Tree *tmp = new Tree();
  traverse(tmp, idc);
  if(idc == 1){
    cerr << "Input is ERROR" << endl;
    return 1;
  }
  int indic = 0;
  int k = 0;
  find(tmp, sym_tab, sym1, proc, order, counter, idc, symm, j);
  if(j == 0) return 1;
  if(idc == 0){
    print(tmp, sym_tab, sym1, indic, order, indic1, counter, k);
    if(indic1 == 1) return 1;
    if(indic == 0){
      char t = 'a';
      vector<map<string, pair<vector<string>,map<string, string> > > > inner;
      for( map<char, map<string, pair<vector<string>,map<string, string> > > >::iterator it = order.begin(); it != order.end(); it++){
	map<string, pair<vector<string>,map<string, string> > > in =  (*it).second;
	inner.push_back(in);
	t++;
      }
      for(int j = 0; j < inner.size(); j++){
	map<string, pair<vector<string>,map<string, string> > > ii = inner[j];
	map<string, pair<vector<string>,map<string, string> > >::iterator it = ii.begin();
	for(int i = 0; i < inner.size(); i ++){
	  map<string, pair<vector<string>,map<string, string> > > ii1 = inner[i];
	  map<string, pair<vector<string>,map<string, string> > >::iterator it1 = ii1.begin();
	  if( i != j){
	    if((*it).first == (*it1).first){
	      cerr << "ERROR duplicate define of procedure" << endl;
	      return 1;
	    }
	  }
	}
      }
      /*      for(int i = 0; i < inner.size(); i++){
	map<string, pair<vector<string>,map<string, string> > > ii = inner[i];
	map<string, pair<vector<string>,map<string, string> > >::iterator it = ii.begin();
	vector<string>::iterator s = it->second.first.begin();
	if((*s) ==  ""){
	  cerr << "ERROR empty" << endl;
	  return 1;
	}
	}*/
      for(int i = 0; i < inner.size(); i++){
	map<string, pair<vector<string>,map<string, string> > > ii = inner[i];
	map<string, pair<vector<string>,map<string, string> > >::iterator it = ii.begin();
	cerr << (*it).first << " ";
	for(vector<string>::iterator itt = it->second.first.begin(); itt != it->second.first.end(); itt++){    
	  cerr << (*itt) << " ";                                                                                      
	}                                                                                                           
	cerr << endl;                                                                                               
	pair<vector<string>,map<string, string> > ii1 = it->second;
	map<string, string> ii2 = ii1.second;
	for(map<string, string>::iterator it1 = ii2.begin(); it1 != ii2.end(); it1++){
	  cerr << it1->first << " " << it1->second << endl;
	}
	
	cerr << endl; 
      }
      cerr << sym_tab.find("wain")->first << " " << sym_tab.find("wain")->second << endl;
      map<string, string>::iterator it = sym_tab.find("wain");
      sym_tab.erase(it);
      for(map<string, string>::iterator it = sym_tab.begin(); it != sym_tab.end(); it++){
	cerr << it->first << " " << it->second << endl;  
      }
    }
  }
}
    
