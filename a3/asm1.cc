#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <map>
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


void outp(unsigned int in){
  putchar(in >> 24);
  putchar(in >> 16);
  putchar(in >>8 );
  putchar(in);
} 


int main(int argc, char* argv[]){
  // Nested vector representing lines of Tokens
  // Needs to be used here to cleanup in the case
  // of an exception
  vector< vector<Token*> > tokLines;
  try{
    // Create a MIPS recognizer to tokenize
    // the input lines
    Lexer lexer;
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
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;
      
      for(it2 = it->begin(); it2 != it->end(); ++it2){
	string transfer = (*it2)->toString();
	string lexeme = (*it2)->getLexeme();
	if(transfer == "DOTWORD"){
	  if((*(it2+1))->toString() =="INT" || (*(it2+1))->toString() == "HEXINT"){
	    //	    cout<<lexeme<<" " << "0x" <<std::hex<<(*(it2+1))->toInt();
	    numoflines++;
	    outp((*(it2+1))->toInt());
	    //	    cout << lexeme << endl;
	  }
	  
	  else{
	    cerr <<"ERROR" << endl;
	  }
	}
	if(transfer == "LABEL"){
	  string content = lexeme.substr(0,lexeme.length()-1);
	  int occur= symbol_tab.count(content);
	  if(occur==0){
	    symbol_tab.insert(pair<string,int>(content,numoflines*4));
	    cerr << content << " " << numoflines*4 << endl;
	  }else{
	    cerr << "ERROR" << endl;
	  }
	}
	//        cerr << "  Token: "  << *(*it2) << endl;
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
