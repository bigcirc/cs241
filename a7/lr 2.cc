#include <map>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <cstdio>
#include <cstdlib>
using namespace std;

set<string> terms;
set<string> nonterms;
vector<string> prods;
string start;

struct Tree {
  string rule;
  list<Tree*> children;

  ~Tree() {
    for(list<Tree*>::iterator it=children.begin(); it != children.end(); it++) {  // delete all subtrees
      delete (*it);
    }
  }
};

void readsyms(set<string> &t) {
  int n;
  string temp;
  getline(cin,temp);
  istringstream iss(temp);
  iss >> n;
  if (iss.fail()) return;
  for(int i = 0; i < n; i++) {
    getline(cin,temp);
    t.insert(temp);
  }
}
void readsyms1(vector<string> &t){
  int n;
  string temp;
  getline(cin,temp);
  istringstream iss(temp);
  iss >> n;
  if (iss.fail()) return;
  for(int i = 0; i < n; i++) {
    getline(cin,temp);
    t.push_back(temp);
  }
}

void traverse(Tree *t, int d) {
  for(int i = 0; i < d; i++) cout << " ";
  cout << t->rule << endl; // print root
  for(list<Tree*>::iterator it=(t->children).begin(); it != (t->children).end(); it++) {  // print all subtrees
    traverse(*it, d+1);
  }
}

void dump(set<string> &h) {
  cout << h.size() << endl;
  for(set<string>::iterator it=h.begin(); it != h.end(); it++) {
    cout << *it << endl;
  }
}

void popper(stack<Tree *> &myStack, list<string> &rhs, string rule) {
  Tree *n = new Tree();
  n->rule = rule;
  for(list<string>::iterator it=rhs.begin(); it != rhs.end(); it++){
    Tree *tmp = myStack.top();
    n->children.push_front(tmp);
    myStack.pop();
  }
  myStack.push(n);
}

void lrdo(list<string> &r) {
  int sta, ter;
  cin >> sta >> ter;
  string l; //  state1
  string f;
  getline(cin,f);
  for(int i = 0; i < ter; i++) {
    getline(cin,f);
    r.push_back(f);
  }
}

int main(){
  int indic = 0;
  int indic1 = 0;
  list<string> r; 
  
  readsyms(terms); // read terminals
  readsyms(nonterms); // read nonterminals
  getline(cin,start); // read start symbol
  readsyms1(prods); // read production rules
  
  vector<string> print;
  stack<string> sym;
  stack<int> state;
  lrdo(r); // read reverse rightmost derivation
  int counter = 1;
  string x;
  int num;
  string input;
  counter = 1;
  state.push(0);
  cin >> x;
  multimap <int,string> reduce;
  for(list<string>::iterator it3 = r.begin(); it3 != r.end(); it3++){
    istringstream iss(*it3);
    int numb;
    string tok;
    iss >> numb >> tok;
    iss >> tok;
    if(tok == "reduce"){
      reduce.insert( pair<int,string>( numb, tok) );
    }
  }
  if(x == "BOF"){
    sym.push(x);
    for(list<string>::iterator itt1 = r.begin(); itt1 != r.end(); itt1++){
      istringstream isss1(*itt1);
      int num11;
      string tok11;
      isss1 >> num11 >> tok11;
      if(num11 == 0 && tok11 == x){
	isss1 >> tok11 >> num11;
	state.push(num11);
	break;
      }
    }
    while(cin >> x){
      indic = 0;
      int dic = 0;
      num = state.top();
      int n2;
      int n1, num1, numm;
      string x1, xx;
      n2 = num;
      string tok1, tok2;
      while( reduce.count(num) != 0 ){
	for(list<string>::iterator itt = r.begin(); itt != r.end(); itt++){
	  istringstream isss(*itt);
	  isss >> num1 >> tok1 >> tok2;
	  if(num1 == num && tok1 == x && tok2 == "reduce"){
	    dic = 1;
	    isss >>  numm;
	    break;
	  }
	}
	if( dic == 1){
	  stringstream l;
	  l.str(prods[numm]);
	  cout << prods[numm] << endl;
	  string ab, ab1;
	  l >> ab;
	  int c = 0;
	  while(l >> ab1){
	    c++;
	  }
	  for(int i = 0; i < c; i++){
	    state.pop();
	    sym.pop();
	  }
	  sym.push(ab);
	  for(list<string>::iterator it = r.begin(); it != r.end(); it++){
	    istringstream iss(*it);
	    iss >> n2 >> x1;
	    if(sym.top() == x1 && state.top() == n2){
	      iss >> xx >> n2;
	      state.push(n2);
	      num = state.top();
	      break;
	    }
	  }
	} else if(dic == 0){
	  break;
	}
      }
      sym.push(x);
      counter++;                                                             
      int n3;                                                                
      string y;                                                              
      for(list<string>::iterator it1 = r.begin(); it1 != r.end(); it1++){   
	istringstream iss1(*it1);                                           
	iss1 >> n3 >> y;                                                    
	if(n3 == state.top() && y == x){                                    
	  indic = 1;                                                         
	  iss1 >> y >> n3;                                                  
	  state.push(n3);                                                   
	  break;                                                             
	}
      }
      if(indic == 0){
	cerr << "ERROR at " << counter << endl;
	indic1 = 1;
	break;
      }
    }
    if(indic == 1){
      cout << prods[0] << endl;
    }
  }
  
  else{
    cout << "ERROR at " << "1" << endl;
  }


  
  
  
  //dump(terms); // write terminals in .cfg format
  //dump(nonterms); // write nonterminals
  //cout << start << endl; // write start symbol
  //dump(prods); // write production rules
  //traverse(parsetree, 0); // write forward leftmost derivation
  //delete parsetree;
}
