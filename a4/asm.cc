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
} //used for print machine code

 
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
    map<string,int> symbol_tab;//symbol table
    unsigned int numoflines=0;//record lines
    int capa=0;
    vector<vector<string> > print;//use for print
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;
      
      for(it2 = it->begin(); it2 != it->end(); ++it2){
	string transfer = (*it2)->toString();//this is type of instruction, either id, dotword or label
	string lexeme = (*it2)->getLexeme();//this is exactly string of instruction which is ben, add, etc
	int length = lexeme.length();//length of instruction
	int size = it->end()-it->begin();//this is length of vector of tokends;
	if(transfer == "DOTWORD"){
	  if(size<2 || it2!=it->end()-2){
	    throw string("ERROR");//if dotword, then we have to check if .word instruction has length or at least two items from back counting
	  }//if not, throw error.
	  string next= (*(it2+1))->toString();
	  string nextlex = (*(it2+1))->getLexeme();
	   if(next=="ID"||next =="INT" || next == "HEXINT"){//this is to check if next is int id or hexint
	    if(next=="ID"){
	      symbol_tab.insert(pair<string,int>(next,numoflines*4));//if id, insert into symbol table
	    }
	    numoflines++;//no matter what type, add lines by one
	    capa++;
	    print.resize(capa);//and resize print
	    print[capa-1].push_back(transfer);//insert type of instruction
	    print[capa-1].push_back(next);//next word instruction
	    print[capa-1].push_back(nextlex);//and next wor type
	    break;
	  }
	  else{
	    throw string("ERROR");//other case,break
	  }
	}
	else if(transfer == "LABEL"){//if lable, insert into symbol table
	  string content = lexeme.substr(0,lexeme.length()-1);
	  int occur= symbol_tab.count(content);
	  if(occur==0){
	    symbol_tab.insert(pair<string,int>(content,numoflines*4));
	    //cerr<< symbol_tab.first()<<symbol_tab.second()<<endl;
	    //if necessary, cerr symbol table here.
	  }else{
	    throw string("ERROR");
	  }
	}//this is for checking lable
	else if(transfer!="LABEL"&&transfer!="DOTWORD"&&transfer=="ID"){//other case
	  if(lexeme=="jr"||lexeme=="jalr"){
	    if(size<2 || it2!=it->end()-2){//jalr and jr have length 2
	      throw string("ERROR");
	    }
	    if((*(it2+1))->toString()=="REGISTER"){//check if second item is register
	      numoflines++;//if so, true
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);//insert instruction and string
	      string regis= ((*(it2+1))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(regis);//and string of register without $ sign
	      break;
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  else if(lexeme=="lw"||lexeme=="sw"){
	    if(size<7||it2!=it->end()-7){//load word save word have length 7
	      throw string("ERROR1");
	    }
	    if((*(it2+1))->toString()=="REGISTER"&&
	       (*(it2+2))->toString()=="COMMA"&&
	       ((*(it2+3))->toString()=="INT"||(*(it2+3))->toString()=="HEXINT")&&
	       (*(it2+4))->toString()=="LPAREN"&&
	       (*(it2+5))->toString()=="REGISTER"&&
	       (*(it2+6))->toString()=="RPAREN"){//check one by one
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
              print[capa-1].push_back(lexeme);//insert 
	      string t=((*(it2+1))->getLexeme()).substr(1,length);
	      string s=((*(it2+5))->getLexeme()).substr(1,length);
	      string i=((*(it2+3))->getLexeme());//get rid of $ sign
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
	      throw string("ERROR");//length 6
	    }
	    if((*(it2+1))->toString()=="REGISTER"&&(*(it2+2))->toString()=="COMMA"&&
	       (*(it2+3))->toString()=="REGISTER"&&(*(it2+4))->toString()=="COMMA"&&
	       (*(it2+5))->toString()=="REGISTER"){
	      numoflines++;//check types
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string d=((*(it2+1))->getLexeme()).substr(1,length);
	      string s=((*(it2+3))->getLexeme()).substr(1,length);
	      string t=((*(it2+5))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(d);
	      print[capa-1].push_back(s);
	      print[capa-1].push_back(t);//insert 
	      break;
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  else if(lexeme=="beq"||lexeme=="bne"){
	    if(size<6||it2!=it->end()-6){
	      throw string("ERROR1");//length 6
	    }
	    else if((*(it2+1))->toString()=="REGISTER"&&
		    (*(it2+2))->toString()=="COMMA"&&
		    (*(it2+3))->toString()=="REGISTER"&&
		    (*(it2+4))->toString()=="COMMA"&&
		    (*(it2+5))->toString()=="INT"||
		    (*(it2+5))->toString()=="HEXINT"||
		    (*(it2+5))->toString()=="ID"){
	      numoflines++;//types
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
	      print[capa-1].push_back(num);//insert current num of lines
	      break;//insert
	    }
	    else{
	      throw string("ERROR2");
	    }
	  }
	  else if(lexeme=="mult"||lexeme=="multu"||lexeme=="div"||lexeme=="divu"){
	    if(size<4||it2!=it->end()-4){
	      throw string("ERROR");//length 4
	    }
	    else if((*(it2+1))->toString()=="REGISTER"&&
	       (*(it2+2))->toString()=="COMMA"&&
	       (*(it2+3))->toString()=="REGISTER"){
	      numoflines++;//check types
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string s=((*(it2+1))->getLexeme()).substr(1,length);
	      string t=((*(it2+3))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(s);
	      print[capa-1].push_back(t);
	      break;
	    }//insert
	    else{
	      throw string("ERROR");
	    }
	  }
	  else if(lexeme=="mfhi"||lexeme=="mflo"||lexeme=="lis"){
	    if(size<2||it2!=it->end()-2){
	      throw string("ERROR");
	    }//length 2
	    else if((*(it2+1))->toString()=="REGISTER"){//check types
	      numoflines++;
	      capa++;
	      print.resize(capa);
	      print[capa-1].push_back(transfer);
	      print[capa-1].push_back(lexeme);
	      string d=((*(it2+1))->getLexeme()).substr(1,length);
	      print[capa-1].push_back(d);
	      break;//insert
	    }
	    else{
	      throw string("ERROR");
	    }
	  }
	  else{
	    throw string("ERROR");
	  }//otherwise throw error
	}
      }
    }
    
    vector<vector<string> >::iterator out;//print procedure
    for(out=print.begin();out!=print.end();out++){
      string tansfer1 = (*out)[0];//type, either id, dotword or lable
      string next1 = (*out)[1];//details, either beq or add or sw, etc
      if(tansfer1=="DOTWORD"){//dotword
	if(next1=="ID"){//if id
	  if(symbol_tab.count((*out)[2])==1){
	    outp(symbol_tab.find((*out)[2])->second);
	  }//out put lines of .word
	  if(symbol_tab.count((*out)[2])!=1){
	    throw string("ERROR");
	  }
	}
	else if(next1 =="INT"){
	  int s= atoi((*out)[2].c_str());
	  outp(s);
	}//if int, print
	else if(next1=="HEXINT"){
	  stringstream str;
	  string s1=(*out)[2].substr(2);
	  str <<s1;
	  int value;
	  str >>std::hex >> value;
	  outp(value);//if hex, convert into hex, then print
	}
	else{
	  throw string("ERROR");
	}	
      }
      else if(tansfer1!="DOTWORD"&&tansfer1=="ID"){
	if(next1=="jr"||next1=="jalr"){//jalr or jr
	  int convert=atoi((*out)[2].c_str());//convert register
	  if(convert <0||convert>31){
	    throw string("ERROR");//if out of bound
	  }
	  if(next1=="jr"){
	    outp(0<<26|convert<<21|8);
	  }
	  else if(next1=="jalr"){
	    outp(0<<26|convert<<21|9);
	  }//otherwise, print 
	  else{
	    throw string("ERROR");
	  }
	}
	else if(next1=="mult"||next1=="multu"||next1=="div"||next1=="divu"){
	  int s=atoi((*out)[2].c_str());
          int t=atoi((*out)[3].c_str()); 
	  if(s<0||s>31||t<0||t>31){
	    throw string("ERROR");
	  }//two registers and see if out of bound
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
	  }//print
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
	  }//three registers,check out of bound
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
	  }//print
	  else{
            throw string("ERROR");
          }
	}
	else if(next1=="mflo"||next1=="mfhi"||next1=="lis"){
	  int d=atoi((*out)[2].c_str());
	  if(d<0||d>31){
	    throw string("ERROR");
	  }//only one reigster
	  if(next1=="mflo"){
	    outp(d<<11|18);
	  }
	  else if(next1=="mfhi"){
	    outp(d<<11|16);
	  }
	  else if(next1=="lis"){
	    outp(d<<11|20);
	  }//print 
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
	  }//two registers
	  if((*out)[5]=="INT"&& (i>32767)||(i<-32768)){
            throw string("ERROR4");
          }//check i if out of bound
          if((*out)[5]=="HEXINT"&& (i>65535)){
            throw string("ERROR5");
          }//hex also check out of bound
	  if(next1=="lw"){
	    outp(35<<26|s<<21|t<<16|(i&0xffff));
	  }
	  else if(next1=="sw"){
	    outp(43<<26|s<<21|t<<16|(i&0xffff));
	  }
	  else{
	    throw string("ERROR6");
	  }//print
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
	    }//check types and convert into int
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
	  }//check out of bound
	  if((*out)[5]=="INT"&&( (i>32767)||(i<-32768))){
	    throw string("ERROR5");
	  }
	  if((*out)[5]=="HEXINT"&& (i>65535)){
	    throw string("ERROR6");
	  }
	  if((*out)[5]=="ID"&&( (i>32767)||(i<-32768))){
            throw string("ERROR5");
          }//check out of bound for memory.
	  if(next1=="beq"){
	    outp(4<<26|s<<21|t<<16|(0xffff&i));
	  }
	  else if(next1=="bne"){
	    outp(5<<26|s<<21|t<<16|(0xffff&i));
	  }//print
	  else{
	    throw string("ERROR7");
	  }
	}
      }
      
    }
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
