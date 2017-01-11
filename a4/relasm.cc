#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <sstream>
// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cin;
using std::getline;
using ASM::Token;
using ASM::Lexer;
using std::pair;
using std::map;
using std::cout;
using std::stringstream;

void outp(unsigned int in){
  putchar(in >> 24&0xff);
  putchar(in >> 16 &0xff);
  putchar(in >>8 & 0xff);
  putchar(in & 0xff);
} 

 
int main(int argc, char* argv[]){
  // Nested vector representing lines of Tokens
  // Needs to be used here to cleanup in the case
    //  of an exception
    vector< vector<Token*> > tokLines;
  try{ 
    //  Create a MIPS recognizer to tokenize
    // t he input lines
    Lexer  lexer;
    // Tokenize each line of the input
    string line;
    while(getline(cin,line)){
      tokLines.push_back(lexer.scan(line));
    }
    
    // Iterate over the lines of tokens and print them
    // to standard error
    vector<vector<Token*> >::iterator it;
    map<string,int> symbol_tab;
    unsigned int numoflines=0;
    int capa=0;
    vector<vector<string> > print;
    std::multimap<string,int> relo;//this is a map for stroing relocatable
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;
      
      for(it2 = it->begin(); it2 != it->end(); ++it2){
	string transfer = (*it2)->toString();
	string lexeme = (*it2)->getLexeme();
	int length = lexeme.length();
	int size = it->end()-it->begin();
	if(transfer == "DOTWORD"){
	  if(size<2 || it2!=it->end()-2){
	    cerr<<"ERROR"<<endl;
	  }
	  string next= (*(it2+1))->toString();
	  string nextlex = (*(it2+1))->getLexeme();
	  if(next=="ID"||next =="INT" || next == "HEXINT"){
	    if(next=="ID"){
	      relo.insert(pair<string,int>(nextlex,numoflines*4+12));
	    }
	    numoflines++;
	    capa++;
	    print.resize(capa);
	    print[capa-1].push_back(transfer);
	    print[capa-1].push_back(next);
	    print[capa-1].push_back(nextlex);
	    break;
	  }
	  else{
	    cerr<<"ERROR"<<endl;
	  }
	}
	else if(transfer == "LABEL"){
	  string content = lexeme.substr(0,lexeme.length()-1);
	  int occur= symbol_tab.count(content);
	  if(occur==0){
	    symbol_tab.insert(pair<string,int>(content,numoflines*4+12));
	    //	    cerr << content << " " << numoflines*4 << endl;
	  }else{
	    cerr<<"ERROR"<<endl;
	  }
	}
	else if(transfer!="LABEL"&&transfer!="DOTWORD"&&transfer=="ID"){
	  if(lexeme=="jr"||lexeme=="jalr"){
	    if(size<2 || it2!=it->end()-2){
	      throw string("ERROR");
	    }
	    if((*(it2+1))->toString()=="REGISTER"){
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string regis= ((*(it2+1))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(regis);
	      break;
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  else if(lexeme=="lw"||lexeme=="sw"){
	    if(size<7||it2!=it->end()-7){
	      throw string("ERROR1");
	    }
	    if((*(it2+1))->toString()=="REGISTER"&&
	       (*(it2+2))->toString()=="COMMA"&&
	       ((*(it2+3))->toString()=="INT"||(*(it2+3))->toString()=="HEXINT")&&
	       (*(it2+4))->toString()=="LPAREN"&&
	       (*(it2+5))->toString()=="REGISTER"&&
	       (*(it2+6))->toString()=="RPAREN"){
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
              print[capa-1].push_back(lexeme);
	      string t=((*(it2+1))->getLexeme()).substr(1,length);
	      string s=((*(it2+5))->getLexeme()).substr(1,length);
	      string i=((*(it2+3))->getLexeme());
	      print[capa-1].push_back(t);
	      print[capa-1].push_back(s);
	      print[capa-1].push_back(i);
	      print[capa-1].push_back((*(it2+3))->toString());
	      break;
	    }
	    else{
	      throw string("ERROR2");
	    }
	  }
	  else if(lexeme=="add"||lexeme=="sub"||lexeme=="slt"||lexeme=="sltu"){
	    if(size<6 || it2!=it->end()-6){
	      throw string("ERROR");
	    }
	    if((*(it2+1))->toString()=="REGISTER"&&(*(it2+2))->toString()=="COMMA"&&
	       (*(it2+3))->toString()=="REGISTER"&&(*(it2+4))->toString()=="COMMA"&&
	       (*(it2+5))->toString()=="REGISTER"){
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string d=((*(it2+1))->getLexeme()).substr(1,length);
	      string s=((*(it2+3))->getLexeme()).substr(1,length);
	      string t=((*(it2+5))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(d);
	      print[capa-1].push_back(s);
	      print[capa-1].push_back(t);
	      break;
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  else if(lexeme=="beq"||lexeme=="bne"){
	    if(size<6||it2!=it->end()-6){
	      throw string("ERROR1");
	    }
	    else if((*(it2+1))->toString()=="REGISTER"&&
		    (*(it2+2))->toString()=="COMMA"&&
		    (*(it2+3))->toString()=="REGISTER"&&
		    (*(it2+4))->toString()=="COMMA"&&
		    (*(it2+5))->toString()=="INT"||
		    (*(it2+5))->toString()=="HEXINT"||
		    (*(it2+5))->toString()=="ID"){
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string s=((*(it2+1))->getLexeme()).substr(1,length);
	      string t=((*(it2+3))->getLexeme()).substr(1,length);
	      string i=((*(it2+5))->getLexeme());
	      print[capa-1].push_back(s);
	      print[capa-1].push_back(t);
	      print[capa-1].push_back(i);
	      print[capa-1].push_back((*(it2+5))->toString());
	      string num;
	      stringstream ss;
	      ss << numoflines;
	      num=ss.str();
	      print[capa-1].push_back(num);
	      break;
	    }
	    else{
	      throw string("ERROR2");
	    }
	  }
	  else if(lexeme=="mult"||lexeme=="multu"||lexeme=="div"||lexeme=="divu"){
	    if(size<4||it2!=it->end()-4){
	      throw string("ERROR");
	    }
	    else if((*(it2+1))->toString()=="REGISTER"&&
	       (*(it2+2))->toString()=="COMMA"&&
	       (*(it2+3))->toString()=="REGISTER"){
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string s=((*(it2+1))->getLexeme()).substr(1,length);
	      string t=((*(it2+3))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(s);
	      print[capa-1].push_back(t);
	      break;
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  else if(lexeme=="mfhi"||lexeme=="mflo"||lexeme=="lis"){
	    if(size<2||it2!=it->end()-2){
	      throw string("ERROR");
	    }
	    else if((*(it2+1))->toString()=="REGISTER"){
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string d=((*(it2+1))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(d);
	      break;
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  //cerr << "  Token: "  << *(*it2) << endl;
	}
      }
    }
    int beq002;
    stringstream ss;
    string s="0x10000002";
    ss<< s;
    ss>>std::hex >> beq002;
    outp(beq002);
    outp(numoflines*4+relo.size()*8+12);
    outp(numoflines*4+12);//first of all, print all three header lines
    vector<vector<string> >::iterator out;
    for(out=print.begin();out!=print.end();out++){
      string tansfer1 = (*out)[0];
      string next1 = (*out)[1];
      if(tansfer1=="DOTWORD"){
	if(next1=="ID"){
	  if(symbol_tab.count((*out)[2])==1){
	    outp(symbol_tab.find((*out)[2])->second);
	  }
	  else{
	    throw string("ERROR");
	  }
	}
	else if(next1 =="INT"){
	  int s= atoi((*out)[2].c_str());
	  outp(s);
	}
	else if(next1=="HEXINT"){
	  stringstream str;
	  string s1=(*out)[2].substr(2);
	  str <<s1;
	  int value;
	  str >>std::hex >> value;
	  outp(value);
	}
	else{
	  throw string("ERROR");
	}	
      }
      else if(tansfer1!="DOTWORD"&&tansfer1=="ID"){
	if(next1=="jr"||next1=="jalr"){
	  int convert=atoi((*out)[2].c_str());
	  if(convert <0||convert>31){
	    throw string("ERROR");
	  }
	  if(next1=="jr"){
	    outp(0<<26|convert<<21|8);
	  }
	  else if(next1=="jalr"){
	    outp(0<<26|convert<<21|9);
	  }
	  else{
	    throw string("ERROR");
	  }
	}
	else if(next1=="mult"||next1=="multu"||next1=="div"||next1=="divu"){
	  int s=atoi((*out)[2].c_str());
          int t=atoi((*out)[3].c_str()); 
	  if(s<0||s>31||t<0||t>31){
	    throw string("ERROR");
	  }
	  if(next1=="mult"){
	    outp(s<<21|t<<16|24);
	  }
	  else if(next1=="multu"){
	    outp(s<<21|t<<16|25);
	  }
	  else if(next1=="div"){
	    outp(s<<21|t<<16|26);
	  }
	  else if(next1=="divu"){
	    outp(s<<21|t<<16|27);
	  }
	  else{
	    throw string("ERROR");
	  }
	}
	else if(next1=="add"||next1=="sub"||next1=="slt"||next1=="sltu"){
	  int d=atoi((*out)[2].c_str());
	  int s=atoi((*out)[3].c_str());
	  int t=atoi((*out)[4].c_str());
	  if(d<0||d>31){
	    throw string("ERROR");
	  }
	  if(s<0||s>31){
            throw string("ERROR");
	  }
	  if(t<0||t>31){
            throw string("ERROR");
	  }
	  if(next1=="add"){
	    outp(0<<26|s<<21|t<<16|d<<11|32);
	  }
	  else if(next1=="sub"){
	    outp(0<<26|s<<21|t<<16|d<<11|34);
	  }
	  
	  else if(next1=="slt"){
	    outp(0<<26|s<<21|t<<16|d<<11|42);
	  }
	  else if(next1=="sltu"){
	    outp(0<<26|s<<21|t<<16|d<<11|43);
	  }
	  else{
            throw string("ERROR");
          }
	}
	else if(next1=="mflo"||next1=="mfhi"||next1=="lis"){
	  int d=atoi((*out)[2].c_str());
	  if(d<0||d>31){
	    throw string("ERROR");
	  }
	  if(next1=="mflo"){
	    outp(d<<11|18);
	  }
	  else if(next1=="mfhi"){
	    outp(d<<11|16);
	  }
	  else if(next1=="lis"){
	    outp(d<<11|20);
	  }
	  else{
	    throw string("ERROR");
	  }
	}
	else if(next1=="lw"||next1=="sw"){
	  int t=atoi((*out)[2].c_str());
	  int s=atoi((*out)[3].c_str());
	  int i;
	  if((*out)[5]=="INT"){
	    i=atoi((*out)[4].c_str());
	  }
	  else if((*out)[5]=="HEXINT"){
            stringstream str;
            string s1=(*out)[4].substr(2);
            str <<s1;
            str >>std::hex >> i;
          }
	  if(s<0||s>31|| t <0||t>31){
	    throw string("ERROR3");
	  }
	  if((*out)[5]=="INT"&& (i>32767)||(i<-32768)){
            throw string("ERROR4");
          }
          if((*out)[5]=="HEXINT"&& (i>65535)){
            throw string("ERROR5");
          }
	  if(next1=="lw"){
	    outp(35<<26|s<<21|t<<16|(i&0xffff));
	  }
	  else if(next1=="sw"){
	    outp(43<<26|s<<21|t<<16|(i&0xffff));
	  }
	  else{
	    throw string("ERROR6");
	  }
	}
	else if(next1=="bne"||next1=="beq"){
	  int s=atoi((*out)[2].c_str());
          int t=atoi((*out)[3].c_str());
	  int i;
	  if((*out)[5]=="INT"){
	    i=atoi((*out)[4].c_str());
	  }
	  else if((*out)[5]=="HEXINT"){
	    stringstream str;
	    string s1=(*out)[4].substr(2);
	    str <<s1;
	    str >>std::hex >> i;
	  }
	  else if((*out)[5]=="ID"){
	    if(symbol_tab.count((*out)[4])==1){
	      int address=symbol_tab.find((*out)[4])->second;
	      int tot = atoi((*out)[6].c_str());
	      i=address/4-tot;
	    }
	    else{
	      throw string("ERROR33");
	    }
	  }
	  if(s<0||s>31){
	    throw string("ERROR4");
	  }
	  if(t<0||t>31){
	    cerr<<"ERROR"<<endl;
	    break;
	  }
	  if((*out)[5]=="INT"&&( (i>32767)||(i<-32768))){
	    throw string("ERROR5");
	  }
	  if((*out)[5]=="HEXINT"&& (i>65535)){
	    throw string("ERROR6");
	  }
	  if(next1=="beq"){
	    outp(4<<26|s<<21|t<<16|(0xffff));
	  }
	  else if(next1=="bne"){
	    outp(5<<26|s<<21|t<<16|(0xffff));
	  }
	  else{
	    throw string("ERROR7");
	  }
	}
      }
    }
    std::multimap<string,int>::iterator relotable;
    for(relotable=relo.begin();relotable!=relo.end();relotable++){
      if(symbol_tab.count(relotable->first)>0){
	outp(1);
	outp(relotable->second);
      }
      else{
	throw string("ERROR111");
      }
    }//after doing all, print .word 1 and new relocatable from end of code till end of module
  } catch(const string& msg){
    // If an exception occurs print the message and end the program
    cerr << msg << endl;
  }
  // Delete the Tokens that have been made
  vector<vector<Token*> >::iterator it;
  for(it = tokLines.begin(); it != tokLines.end(); ++it){
    vector<Token*>::iterator it2;
    for(it2 = it->begin(); it2 != it->end(); ++it2){
      delete *it2;
    }
  }
}
