#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cin;
using std::getline;
using ASM::Token;
using ASM::Lexer;
using std::cout;


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
    int indic = 0;
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;

      for(it2 = it->begin(); it2 != it->end(); ++it2){
	ASM::Kind mystring = (*it2)->getKind();
	string lex = (*it2)->getLexeme();
	if(mystring == ASM::ID){
	  if(lex == "return"){
	    cout << "RETURN" << " " << lex << endl;
	  } else if(lex == "if"){
	    cout << "IF" << " " << lex << endl;
	  } else if(lex == "else"){
	    cout << "ELSE" << " " << lex << endl;
	  } else if(lex == "while"){
	    cout << "WHILE" << " " << lex << endl;
	  } else if(lex == "println"){
	    cout << "PRINTLN" << " " << lex << endl;
	  } else if(lex == "wain"){
	    cout << "WAIN" << " " << lex << endl;
	  } else if(lex == "int"){
	    cout << "INT" << " " << lex << endl;
	  } else if(lex == "new"){
	    cout << "NEW" << " " << lex << endl;
	  } else if(lex == "delete"){
	    cout << "DELETE" << " " << lex << endl;
	  } else if(lex == "NULL"){
	    cout << "NULL" << " " << lex << endl;
	  } else{
	  cout << "ID" << " " << lex << endl;
	  }
	} else if(mystring == ASM::INT){
	  indic = 0;
	  string num = (*it2)->getLexeme();
	  if(num.size() > 1){
	    for(int i = 0; i < num.size(); i++){
	      if((num[i] != '0') && (num[i] != '1') && 
		 (num[i] != '2') && (num[i] != '3') &&
		 (num[i] != '4') && (num[i] != '5') && 
		 (num[i] != '6') && (num[i] != '7') &&
		 (num[i] != '8') && (num[i] != '9')){
		cerr << "ERROR not a real num, but a combo of letters and digits" << endl;
		indic = 1;
		break;
	      }
	    }
	  }
	  if(indic == 1){
	    break;
	  }
	  long long s = atoll(num.c_str());
	  if(num.substr(0,1) == "0" && num.size() > 1){
	    cerr << "ERROR zero in the first digit" << endl;
	  } else if(s >= 0 && s <= 2147483647){
	    cout << "NUM" << " " << lex << endl;
	  } else{
	    cerr << "ERROR num out of bound" << endl;
	  }
	} else if(mystring == ASM::LPAREN){
	  cout << "LPAREN" << " " << lex << endl;
	} else if(mystring == ASM::RPAREN){
          cout << "RPAREN" << " " << lex << endl;
        } else if(mystring == ASM::LBRACE){
 	  cout << "LBRACE" << " " << lex << endl;
	} else if(mystring == ASM::RBRACE){
	  cout << "RBRACE" << " " << lex << endl;
	} else if(mystring == ASM::BECOMES){
	  cout << "BECOMES" << " " << lex << endl;
	} else if(mystring == ASM::EQ){
	  cout << "EQ" << " " << lex << endl;
	} else if(mystring == ASM::NE){
	  cout << "NE" << " " << lex << endl;
	} else if(mystring == ASM::LT){
	  cout << "LT" << " " << lex << endl;
	} else if(mystring == ASM::GT){
	  cout << "GT" << " " << lex << endl;
	} else if(mystring == ASM::LE){
	  cout << "LE" << " " << lex << endl;
	} else if(mystring == ASM::GE){
	  cout << "GE" << " " << lex << endl;
	} else if(mystring == ASM::PLUS){
	  cout << "PLUS" << " " << lex << endl;
	} else if(mystring == ASM::MINUS){
	  cout << "MINUS" << " " << lex << endl;
	} else if(mystring == ASM::STAR){
	  cout << "STAR" << " " << lex << endl;
	} else if(mystring == ASM::PCT){
	  cout << "PCT" << " " << lex << endl;
	} else if(mystring == ASM::SLASH){
	  cout << "SLASH" << " " << lex << endl;
	} else if(mystring == ASM::COMMA){
	  cout << "COMMA" << " " << lex << endl;
	} else if(mystring == ASM::LBRACK){
	  cout << "LBRACK" << " " << lex << endl;
	} else if(mystring == ASM::RBRACK){
	  cout << "RBRACK" << " " << lex << endl;
	} else if(mystring == ASM::SEMI){
	  cout << "SEMI" << " " << lex << endl;
	} else if(mystring == ASM::AMP){
	  cout << "AMP" << " " << lex << endl;
	} else if(mystring == ASM::WHITESPACE){
	} else{
	  cerr << "ERROR  Token: "  << *(*it2) << "doesnot exist " << endl;
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
