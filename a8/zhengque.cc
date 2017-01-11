#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
using namespace std;

string state[]={"BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF",
		"INT","LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS",
		"PRINTLN","RBRACE","RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP",
		"LBRACK","RBRACK", "NEW", "DELETE", "NULL"};

vector<string> finalstate;
map<string,string> table;
map<string,vector<string> > funcpa;

void makestate(){
  for(int a=0;a<35;++a){
    finalstate.push_back(state[a]);
  }
}

bool checkstate(string check){
  for(int a=0;a<35;++a){
    if(finalstate[a]==check)return true;
  }
  return false;
}

class Node{
public:
  string key;
  string name;
  int child;
  vector<Node> each;
  Node(string a,string b,int c);

};


Node::Node(string a,string b,int c):key(a),name(b),child(c){}


Node readin(){
  string first,second,temp;
  getline(cin,temp);
  istringstream ss(temp);
  string rest="";
  int counter=0;
  ss >> first;
  if(checkstate(first)==true){
    ss >> second;
  }else{
    while(ss >> second){
      counter++;
      rest=rest+second+" ";
    }
    second=rest.substr(0,rest.length()-1);
  }
  Node newone(first,second,counter);
  //cout << "Key is " << first << endl;
  //cout << "Children is " << second << endl;
  //cout << "Counter is " << counter << endl;
  if(counter!=0){
    for(int a=0;a<counter;++a){
      (newone.each).push_back(readin());
    }
  }
  return newone;
}

void printall(Node tree){
  for(int a=0;a<tree.child;++a){
    printall((tree.each)[a]);

  }
  cout << tree.key;
  cout << " ";
  cout << tree.name << endl;

}


string checktype(Node tree, map<string,string> &variable);


void countarg(Node tree,map<string,string> variable, vector<string> &args){
  string exp1=checktype(tree.each[0],variable);
  args.push_back(exp1);
  if(tree.child > 1){
    countarg(tree.each[2],variable,args);
  }
}


string checktype(Node tree, map<string,string> &variable){
  //cout << "I am in Checktype" << endl;
  string key=tree.key;
  string name=tree.name;
  int child=tree.child;
  //cout << "here is the name " << tree.key << " "<<tree.name  << endl;
  if(key=="expr"){
    //cout << "here is the name " << name <<endl;
    if(child > 1){
      string exp1=checktype(tree.each[0],variable);
      string exp2=checktype(tree.each[2],variable);
      //cout << "here is exp1 " << exp1 << endl;
      //cout << "here is exp2 " << exp2 << endl;
      //cout<<endl;
      if(tree.each[1].key=="PLUS"){
	//cout << "i am fuking your daya " << endl;
	if(exp1=="int"&&exp2=="int")return "int";
	if(exp1=="int*"&&exp2=="int")return "int*";
	if(exp1=="int"&&exp2=="int*")return "int*";
	if(exp1=="int*"&&exp2=="int*"){
	  string err="ERROR: Invalid Operation";
	  throw err;
	}
      }else if(tree.each[1].key=="MINUS"){
	if(exp1=="int"&&exp2=="int")return "int";
	if(exp1=="int*"&&exp2=="int")return "int*";
	if(exp1=="int*"&&exp2=="int*")return "int*";
	if(exp1=="int"&&exp2=="int*"){
	  string err="ERROR: Invalid Operation";
	  throw err;
	}
      }
    }else{
      string exp1=checktype(tree.each[0],variable);
      //cout << tree.each[0].name << " i am before fuking" << endl;;
      //cout << "here is your fuking exp1 " << exp1 << endl;
      return exp1;
    }
  }
  if(key=="factor"){
    if(name=="ID"||name=="NUM"||name=="NULL"){
      //cout << tree.each[0].key << " fuk u 1 " << endl;
      //cout << tree.each[0].name << " fuk u 2" << endl;
      //cout << variable["b"] << endl;
      string temp=tree.each[0].name;
      if(name=="ID")return variable[temp];
      if(name=="NUM")return "int";
      if(name=="NULL")return "int*";
    }else if(name=="LPAREN expr RPAREN"){
      //cout << "I am fuking here " << endl;
      string exp1=checktype(tree.each[1],variable);
      return exp1;
    }else if(name=="AMP lvalue"){
      string exp1=checktype(tree.each[1],variable);
      if(exp1=="int"){
	return "int*";
      }else{
	string err="ERROR: Invalid Process with operation &";
	throw err;
      }
    }else if(name=="STAR factor"){
      //cout << "I am in factor" << endl;
      string exp1=checktype(tree.each[1],variable);
      //cout << "here is exp1 " << exp1 << endl;
      if(exp1=="int*"){
	return "int";
      }else{
	string err="ERROR: Invalid Process with Operation *";
	throw err;
      }
    }else if(name=="ID LPAREN RPAREN"){
      string exp1=tree.each[0].name;
      if(funcpa[exp1].size()!=0){
	string err="The function does not exist";
	throw err;
      }
      return "int";
    }else if(name=="ID LPAREN arglist RPAREN"){
      string func=tree.each[0].name;
      vector <string> arg;
      //cout << "here is the length of arg " << funcpa[func].size() << endl;
      countarg(tree.each[2],variable,arg);
      //cout << "here is the length of artual " << arg.size() << endl;
      if(arg.size()!=funcpa[func].size()){
	string err="ERROR: No match Function";
	throw err;
      }
      for(int a=0;a<funcpa[func].size();++a){
	string first=arg[a];
	string second=(funcpa[func])[a];
	if(first!=second){
	  string err="ERROR: Function Arguments Does not Match";
	  throw err;
	}
      }
      return "int";
    }else if(name=="NEW INT LBRACK expr RBRACK"){
      string exp1=checktype(tree.each[3],variable);
      //cout << "exp1 " << exp1 << endl;
      if(exp1=="int"){
	return "int*";
      }else{
	string err="ERROR: Invalid Process with Operation new";
	throw err;
      }
    }
  }
  if(key=="term"){
    if(child==1){
      string exp1=checktype(tree.each[0],variable);
      return exp1;
    }else{
      if(name=="term STAR factor"){
	cout << "I am in factor" << endl;
	string exp1=checktype(tree.each[0],variable);
	string exp2=checktype(tree.each[2],variable);
	if(exp1=="int"&&exp2=="int"){
	  return "int";
	}else{
	  string err="ERROR: Invalid Process with Operation *";
	  throw err;
	}
      }else if(name=="term SLASH factor"){
	string exp1=checktype(tree.each[0],variable);
	string exp2=checktype(tree.each[2],variable);
	if(exp1=="int"&&exp2=="int"){
	  return "int";
	}else{
	  string err="ERROR: Invalid Process with Operation *";
	  throw err;
	}
      }else if(name=="term PCT factor"){
	string exp1=checktype(tree.each[0],variable);
	string exp2=checktype(tree.each[2],variable);
	if(exp1=="int"&&exp2=="int"){
	  return "int";
	}else{
	  string err="ERROR: Invalid Process with Operation *";
	  throw err;
	}
      }
    }
  }
  if(key=="lvalue"){
    if(child==1){
      string exp1=tree.each[0].name;
      return variable[exp1];
    }else if(name=="STAR factor"){
      string exp1=checktype(tree.each[1],variable);
      if(exp1=="int"){
	string err="ERROR: Invalid Process with Operation *a";
	throw err;
      }
      return "int";
    }else if(name=="LPAREN lvalue RPAREN"){
      string exp1=checktype(tree.each[1],variable);
      return exp1;
    }
  }
  if(key=="test"){
    string exp1=checktype(tree.each[0],variable);
    string exp2=checktype(tree.each[2],variable);
    if(exp1==exp2){
      return "true";
    }else{
      string err="ERROR: Invalid Compare Type";
      throw err;
    }
  }
  if(key=="statements"){
    if(child!=0){
      string exp1=checktype(tree.each[0],variable);
      string exp2=checktype(tree.each[1],variable);
      if(exp1=="true"&&exp2=="true"){
	return "true";
      }else{
	string err="ERROR: Statement is Invalid";
	throw err;
      }
    }
    return "true";
  }
  if(key=="statement"){
    if(name=="lvalue BECOMES expr SEMI"){
      string exp1=checktype(tree.each[0],variable);
      string exp2=checktype(tree.each[2],variable);
      if(exp1==exp2){
	return "true";
      }else{
	string err="ERROR: Invalid Assignment Process";
	throw err;
      }
    }else if(name=="IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
      string exp1=checktype(tree.each[2],variable);
      string exp2=checktype(tree.each[5],variable);
      string exp3=checktype(tree.each[9],variable);
      if(exp1=="true"&&exp2=="true"&&exp3=="true"){
	return "true";
      }else{
	string err="ERROR: Invalid Controlflow Statement";
	throw err;
      }
    }else if(name=="WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
      string exp1=checktype(tree.each[2],variable);
      string exp2=checktype(tree.each[5],variable);
      if(exp1=="true"&&exp2=="true"){
	return "true";
      }else{
	string err="ERROR: While Statement is not Valid";
	throw err;
      }
    }else if(name=="PRINTLN LPAREN expr RPAREN SEMI"){
      string exp1=checktype(tree.each[2],variable);
      if(exp1=="int"){
	return "true";
      }else{
	string err="ERROR: Print Statement is not Valid";
	throw err;
      }
    }else if(name=="DELETE LBRACK RBRACK expr SEMI"){
      string exp1=checktype(tree.each[3],variable);
      if(exp1=="int*"){
	return "true";
      }else{
	string err="ERROR: Invalid Delete Type";
	throw err;
      }
    }
  }
  if(key=="procedure"){
    string exp1=checktype(tree.each[6],variable);
    string exp2=checktype(tree.each[7],variable);
    string exp3=checktype(tree.each[9],variable);
    if(exp1=="true"&&exp2=="true"&&exp3=="int"){
      return "true";
    }else{
      string err="ERROR: Invalid Procedure Step";
      throw err;
    }
  }
  if(key=="main"){
    string exp1=(tree.each[5]).each[0].name;
    if(exp1=="INT")exp1="int";
    if(exp1=="INT STAR")exp1="int*";
    string exp2=checktype(tree.each[8],variable);
    string exp3=checktype(tree.each[9],variable);
    string exp4=checktype(tree.each[11],variable);
    if(exp1=="int"&&exp2=="true"&&exp3=="true"&&exp4=="int"){
      return "true";
    }else{
      string err="ERROR: Main Function Error";
      throw err;
    }
  }
  if(key=="dcls"){
    if(child==0)return"true";
    if(name=="dcls dcl BECOMES NUM SEMI"){
      string exp1=checktype(tree.each[0],variable);
      string exp2=(tree.each[1]).each[0].name;
      if(exp2=="INT")exp2="int";
      if(exp2=="INT STAR")exp2="int*";
      if(exp1=="true"&&exp2=="int"){
	return "true";
      }else{
	string err="ERROR: Invalid Type";
	throw err;
      }
    }else if(name=="dcls dcl BECOMES NULL SEMI"){
      string exp1=checktype(tree.each[0],variable);
      string exp2=(tree.each[1]).each[0].name;
      if(exp2=="INT")exp2="int";
      if(exp2=="INT STAR")exp2="int*";
      if(exp1=="true"&&exp2=="int*"){
	return "true";
      }else{
	string err="ERROR: Invalid Type";
	throw err;
      }
    }
  }
}






void maketable(Node data,map<string,string> &variables){
  //cout << data.name << endl;
  //if(data.key=="expr")cerr << "cao ni ma" << endl;
  if(data.key=="procedure"){
    string temp=data.each[1].name;
    //cout << "I am in prodcure " << data.each[1].name << endl;
    if(table.count(temp)!=0){
      string err="ERROR: Function "+ temp +" declared before";
      throw err;
    }
    vector<string> arg;
    table[temp]="int";
    cerr << temp;
    Node last=data.each[3];
    //cout << "here is name " << last.name << endl;
    if(last.child==0)cerr << endl;
    if(last.name=="paramlist"){
      last=last.each[0];
      while(last.child==3){
	Node temp3=last.each[0];
	string temp4=temp3.each[0].name;
	if(temp4=="INT")temp4="int";
	if(temp4=="INT STAR")temp4="int*";
	variables[(temp3.each[1]).name]=temp4;
	arg.push_back(temp4);
	cerr  << " " << temp4;
	last=last.each[2];;
      }
      string temp4=(last.each[0]).each[0].name;
      if(temp4=="INT")temp4="int";
      if(temp4=="INT STAR")temp4="int*";
      cerr << " " << temp4 << endl;
      arg.push_back(temp4);
      //map<string,string> variable;
      //for(int a=2;a<data.child;++a){
      //cout << "here is recursion " << data.name << endl;
      //  maketable(data.each[a],variable);
      //}
    }
    funcpa[temp]=arg;
    map<string,string> variable;
    for(int a=2;a<data.child;++a){
      //cout << "here is recursion " << data.name << endl;
      maketable(data.each[a],variable);
    }
    checktype(data,variable);
    cerr << endl;
    return;
  }
  if(data.key=="main"){
    if(table.count("wain")!=0){
      string err="ERROR: wain has been declared";
      throw err;
    }
    table["wain"]="int";
    string para1=((data.each[3]).each[0]).name;
    string para2=((data.each[5]).each[0]).name;
    string check1;
    string check2;
    if(para1=="INT")para1="int";
    if(para1=="INT STAR")para1="int*";
    if(para2=="INT")para2="int";
    if(para2=="INT STAR")para2="int*";
    cerr << "wain " << para1 << " " << para2 << endl;
    map<string,string> variable;
    variables=variable;
  }
  if(data.key=="dcl"){
    string temp=(data.each[0]).name;
    string temp1=(data.each[1]).name;
    if(variables.count(temp1)!=0){
      string err="ERROR: "+ temp1 +" declared before";
      throw err;
    }
    if(temp=="INT"){
      variables[temp1]="int";
      cerr << temp1 << " " << variables[temp1] << endl;
    }else{
      variables[temp1]="int*";
      cerr << temp1 << " " << variables[temp1] << endl;
    }
  }else if(data.key=="ID"){
    //cout << "here is id " << data.key << endl;
    //cout << "here is name " << data.name << endl;
    if(variables.count(data.name)==0){
      string err="ERROR: "+data.name+" was not declared";
      throw err;
    }
  }
  if(data.key=="factor"&&(data.name=="ID LPAREN RPAREN"||data.name=="ID LPAREN arglist RPAREN")){
    if(variables.count(data.each[0].name)!=0){
      string err="ERROR: "+data.each[0].name+" was declared before";
      throw err;
    }else if(table.count(data.each[0].name)==0){
      string err="ERROR:1 "+data.each[0].name+" was not declared";
      throw err;
    }else{
      for(int a=1;a<data.child;++a){
	maketable(data.each[a],variables);
      }
    }
    checktype(data,variables);
    return;
  }
  for(int a=0;a<data.child;++a){
    maketable(data.each[a],variables);
  }
  //cout << "I'm going into checktype" << endl;
  if(data.key=="term"||data.key=="expr"||data.key=="lvalue"|| data.key=="main" || data.key=="statement" || data.key=="statements" || data.key=="dcls"
     ||data.key=="procedure"||data.key=="test" ||(data.key=="factor"&&(data.name!="ID LPAREN RPAREN"||data.name!="ID LPAREN arglist RPAREN"))){
    checktype(data,variables);
  }
}



int main(){
  makestate();
  Node temp=readin();
  try{
    map <string,string> variable;
    maketable(temp,variable);
    if(table.count("wain")==0){
      string err="ERROR: Wain was not declared";
      throw err;
    }
  }catch(string err){
    cerr << err << endl;
  }
}
