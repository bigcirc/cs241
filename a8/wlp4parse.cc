#include <fstream>
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
  string content;
  //vector<string> token;
  list<Tree*> children;

  ~Tree() {
    for(list<Tree*>::iterator it=children.begin(); it != children.end(); it++) {  // delete all subtrees
      delete (*it);
    }
  }
};

void readsyms(set<string> &t, ifstream &in) {
  int n;
  string temp;
  getline(in,temp);
  istringstream iss(temp);
  iss >> n;
  if (iss.fail()) return;
  for(int i = 0; i < n; i++) {
    getline(in,temp);
    t.insert(temp);
  }
}
void readsyms1(vector<string> &t, ifstream &in){
  int n;
  string temp;
  getline(in,temp);
  istringstream iss(temp);
  iss >> n;
  if (iss.fail()) return;
  for(int i = 0; i < n; i++) {
    getline(in,temp);
    t.push_back(temp);
  }
}

void traverse(Tree *t) {
  if(t->rule == "dcls" || t->rule == "statements"){
    if(t->content == ""){
      t->children.push_front(NULL);
    }
  }
  if((t->children).front() != NULL){
    if(t->content != ""){
      cout << t->rule << " " << t->content << endl; // print root
    } else{
      cout << t->rule << endl;
    }
    for(list<Tree*>::iterator it=(t->children).begin(); it != (t->children).end(); it++) {  // print all subtrees
      traverse(*it);
    }
  } else{
    if(t->content != ""){
      cout << t->rule << " " << t->content << endl;
    } else{
      cout << t->rule << endl;
    }
  }
}
/*
void dele(Tree *t) {
  if((t->children).front() != NULL){
    for(list<Tree*>::iterator it=(t->children).begin(); it != (t->children).end(); it++) {  // print all subtrees                  
      dele(*it);
    }
  }
  delete t;
}
*/

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

void lrdo(list<string> &r, ifstream &in) {
  string sta, ter;
  getline(in, sta);
  getline(in, ter);
  string l; //  state1
  string f;
  int ter1 = atoi(ter.c_str());
  getline(in,f);
  for(int i = 0; i < ter1 - 1; i++) {
    getline(in,f);
    r.push_back(f);
  }
}

int main(){
  int indic = 0;
  int indic1 = 0;
  int indic2 = 0;
  list<string> r; 
  //vector<string> print;
  ifstream infile;
  stack<Tree*> tree;
  stack<int> state;
  infile.open ("wlp4.lr1");
  while(!infile.eof()) // To get you all the lines.                                             
    {
      readsyms(terms, infile); // read terminals                                                 
      readsyms(nonterms, infile); // read nonterminals                                           
      getline(infile,start); // read start symbol                                                
      readsyms1(prods, infile); // read production rules                                         
      lrdo(r, infile); // read reverse rightmost derivation                                      
    }
  infile.close();  

  Tree *t = new Tree();
  t->rule = "BOF";
  t->content = "BOF";
  t->children.push_front(NULL);

  int counter = 0;
  string var, type;
  int num;
  string input;
  state.push(0);
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
  }//RECORD ALL REDUCE STATE
  tree.push(t);
  for(list<string>::iterator itt1 = r.begin(); itt1 != r.end(); itt1++){
    istringstream isss1(*itt1);
    int num11;
    string tok11;
    isss1 >> num11 >> tok11;
    if(num11 == 0 && tok11 == "BOF"){
      isss1 >> tok11 >> num11;
      state.push(num11);
      break;
    }
  }//check transition from (0 BOF) push to stack 
  while(cin >> type >> var){
    //    cout << type << " " << var << endl;
    indic = 0;
    int dic = 0;
    num = state.top();
    int n2;
    int n1, num1, numm;
    string x1, xx;
    n2 = num;
    string tok1, tok2;
    while( reduce.count(num) != 0 ){
      dic = 0;
      //cout << "jinlema" << num << type << endl;
      for(list<string>::iterator itt = r.begin(); itt != r.end(); itt++){
	istringstream isss(*itt);
	isss >> num1 >> tok1 >> tok2;
	if(num1 == num && tok1 == type && tok2 == "reduce"){
	  dic = 1;
	  isss >>  numm;
	  //   cout << "numm " << numm << endl;
	  break;
	}
      }
      if( dic == 1){
	stringstream l;
	l.str(prods[numm]);
	Tree *m = new Tree();
	string ab, ab1;
	l >> ab;
	m->rule = ab;
	//cout << ab << " ab" << endl;
	m->content = "";
	//	m->children.push_front(NULL);
	int c = 0;
	while(l >> ab1){
	  c++;
	  m->content += ab1 + " ";
	}
	if(c != 0){
	  m->content = m->content.substr(0,m->content.size() - 1);
	  //cout << "content  " << m->content << endl;
	  for(int i = 0; i < c; i++){
	    //cout << tree.top()->rule << state.top()<< "  needto pop" << endl;
	    m->children.push_front(tree.top());
	    state.pop();
	    tree.pop();
	  }
	}
	tree.push(m);
	for(list<string>::iterator it = r.begin(); it != r.end(); it++){
	  istringstream iss(*it);
	    iss >> n2 >> x1;
	    if((tree.top())->rule  == x1 && state.top() == n2){
	      iss >> xx >> n2;
	      //  cout << "n2 " << n2 << endl;
	      state.push(n2);
	      num = state.top();
	      break;
	    }
	}
      }
      if(dic == 0){
	break;
      }
    }
    Tree *n = new Tree();
    n->rule = type;
    n->content = var;
    n->children.push_front(NULL);
    tree.push(n);
    counter++;                                                             
    int n3;                                                                
    string y;                                                              
    for(list<string>::iterator it1 = r.begin(); it1 != r.end(); it1++){   
      istringstream iss1(*it1);                                           
      iss1 >> n3 >> y;                                                    
      if(n3 == state.top() && y == type){                                    
	indic = 1;                                                         
	iss1 >> y >> n3;                                                  
	state.push(n3);                                                   
	//cout << "go to state " << n3 << endl;
	break;                                                             
      }
    }
    if(indic == 0){
      cerr << "ERROR at " << counter << endl;
      indic1 = 1;
      indic2 = 1;
      break;
    }
  }
  //cout << "indic2 " << indic2 << endl;
  if(indic2 == 0){
    int indic3 = 0;
    type = "EOF";
    var = "EOF";
    indic1 = 0;
    int dic = 0;
    num = state.top();
    int n2;
    int n1, num1, numm;
    string x1, xx;
    n2 = num;
    string tok1, tok2;
    while( reduce.count(num) != 0 ){
      dic = 0;
      for(list<string>::iterator itt = r.begin(); itt != r.end(); itt++){
	istringstream isss(*itt);
	isss >> num1 >> tok1 >> tok2;
	if(num1 == num && tok1 == type && tok2 == "reduce"){
	  dic = 1;
	  isss >>  numm;
	  break;
	}
      }
      if( dic == 1){
	stringstream l;
	l.str(prods[numm]);
	Tree *m = new Tree();
	string ab, ab1;
	l >> ab;
	m->rule = ab;
	m->content = "";
	int c = 0;
	while(l >> ab1){
	  c++;
	  m->content +=ab1 + " ";
	  //cout << m->content << endl;
	}
	if(c != 0){
	  m->content = m->content.substr(0,m->content.size() - 1);
	  for(int i = 0; i < c; i++){
	    m->children.push_front(tree.top());
	    //cout << "need t po" << state.top() << tree.top()->rule << endl;
	    state.pop();
	    tree.pop();
	  }
	}
	//cout << state.top() << tree.top()->rule <<endl;
	tree.push(m);
	//cout<<tree.top()->rule  << endl;
	for(list<string>::iterator it = r.begin(); it != r.end(); it++){
	  istringstream iss(*it);
	  iss >> n2 >> x1;
	  if((tree.top())->rule  == x1 && state.top() == n2){
	    iss >> xx >> n2;
	    state.push(n2);
	    //  cout << n2 << endl;
	    num = state.top();
	    break;
	  }
	}
      }
      else if(dic == 0) break;
    }
    Tree *n = new Tree();
    n->rule = type;
    n->content = var;
    n->children.push_front(NULL);
    tree.push(n);
    counter++;
    int n3;
    string y;
    //cout << state.top() << type << endl;
    for(list<string>::iterator it1 = r.begin(); it1 != r.end(); it1++){
      istringstream iss1(*it1);
      iss1 >> n3 >> y;
      if(n3 == state.top() && y == type){
	indic1 = 1;
	iss1 >> y >> n3;
	state.push(n3);
	break;
      }
    }
    if(indic1 == 0){
      cerr << "ERROR at " << counter << endl;
      indic1 = 1;
    }
  }
  istringstream l("start BOF procedures EOF");
  Tree *m = new Tree();
  string ab, ab1;
  l >> ab;
  m->rule = ab;
  m->content = "";
  int c = 0;
  while(l >> ab1){
    c++;
    m->content +=ab1 + " ";
    //cout << m->content << endl;
  }
  if(c != 0){
    m->content = m->content.substr(0,m->content.size() - 1);
    for(int i = 0; i < c; i++){
      m->children.push_front(tree.top());
      //cout << "need t po" << state.top() << tree.top()->rule << endl;
      state.pop();
      tree.pop();
    }
  }
  //cout << state.top() << tree.top()->rule <<endl;
  tree.push(m);


  if(indic == 1){
    //    cout << tree.size() << " size" << endl;
    traverse(tree.top());
  }
  delete tree.top();
  
}
