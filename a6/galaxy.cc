#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
using namespace::std;

int exprsi(string &str);

int term(string &str){
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos) return 1;

  size_t end = str.find_last_not_of(" \t\n");
  string expr = str.substr(begin + 5, end);
  if(expr == "( expr )"){
    string line;
    getline(cin, line);
    int id = exprsi(line);
    return id;
  }
  int id = 42;
  //cout << id << "id here" << endl;
  return id;
}

int exprsi(string &str){
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos) return 1;
  
  size_t end = str.find_last_not_of(" \t\n");
  
  string exprs, lines;
  exprs = str.substr(begin + 4, 5);
  
  
  if( exprs == " expr" || exprs == " expr -"|| exprs == " expr "){
    getline(cin, lines);
    //    getline(cin, lines);
    int id = exprsi(lines);
    getline(cin, lines);
    int id1 = term(lines);
    int id2 = id - id1;
    return id2;
  }
  if( exprs == " term"){
    getline(cin, lines);
    return term(lines);
  }
}


int main(){
  int id;
  string expr;
  string els;
  cin >> id;
  for(int i = 0; i < id; i++){
    cin >> expr;
  }
  cin >> id;
  for(int i = 0; i < id; i++){
    cin >> expr;
  }
  cin >> expr >> id;
  for(int i = 0; i < id; i++){
    getline(cin, expr);
  }
  getline(cin, expr);
  cin >> expr >> els; //S, BOF
  cin >> expr >> els;//expr, EOF
  if( expr == "expr"){
    string line;
    getline(cin, line);
    getline(cin, line);
    int id1 = exprsi(line);
    getline(cin, line);
    int id2 = term(line);
    cout << id1 << endl;
  }
  else if( expr == "term"){
    string line;
    getline(cin, line);
    getline(cin, line);
    int id1 = term(line);
    cout << id1 << endl;
  }
  return 0;
}





//overload input operator                                                               
//read arbitrary length of strings into string s                                        
//std::istream &operator>>(std::istream &,utf8::string &);

//overload output operator                                                              
//output string s                                                                       
//std::ostream &operator<<(std::ostream &,const utf8::string &);


