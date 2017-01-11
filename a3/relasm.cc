#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <map>
#include <sstream>
// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::multimap;
using std::endl;
using std::cerr;
using std::cin;
using std::cout;
using std::getline;
using ASM::Token;
using ASM::Lexer;
using ASM::INT;
using ASM::HEXINT;


// reference from Piazza post
// Output the bytes of corresponding word, which is an unsigned int
void output_byte(unsigned int word) {
	  putchar(word >> 24);
  	putchar(word >> 16);
  	putchar(word >> 8);
  	putchar(word);
}

int output_one(unsigned int s, unsigned int bit_or) {
	return (s << 21 | bit_or);
}

int output_one_d(unsigned int d, unsigned int bit_or) {
  return (d << 11 | bit_or);
}

int output_two(unsigned int s, unsigned int t, unsigned int bit_or) {
  return ((s << 21) | (t << 16) | bit_or);
}

int output_two_with_i(unsigned int s, unsigned int t, 
  unsigned int bit_or1, unsigned int bit_or2) {
  return ((s << 21) | (t << 16) | ((bit_or1 << 16) >> 16) | bit_or2);
}

int output_three(unsigned int s, unsigned int t,
  unsigned int d, unsigned int bit_or) {
  return ((s << 21) | (t << 16) | (d << 11) | bit_or);
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

    unsigned int non_null_lines = 0; // keep track of number of non_null lines
    unsigned int len_out = 0; // length of output vector
    vector<vector<string> > output; // store value that need to output 
    map<string, int> sym_table; // store symbol table
    multimap<string, int> rel_table; // store relocation table

    for(it = tokLines.begin(); it != tokLines.end(); ++it){

      vector<Token*>::iterator it2;
      
      for(it2 = it->begin(); it2 != it->end(); ++it2){

        int size = it->end() - it->begin(); 
        string myString = (*it2)->toString();
        string myLexeme = (*it2)->getLexeme();

       	int len = myLexeme.length();  // length of myLexeme  

        if (myString == "DOTWORD") { // .word instruction
          if ((size >= 2) && (it2 == it->end()-2) && 
              (((*(it2+1))->toString() == "INT") || 
               ((*(it2+1))->toString() == "HEXINT") ||
               ((*(it2+1))->toString() == "ID"))) {
            if ((*(it2+1))->toString() == "ID") {
              rel_table.insert(pair<string, int>((*(it2+1))->getLexeme(),(non_null_lines+3)*4));
            } 
            ++non_null_lines;
            ++len_out;
            output.resize(len_out);
            string type = (*(it2+1))->toString();
            string v = (*(it2+1))->getLexeme();
            // store the value to output vector
            output[len_out-1].push_back(myString);
            output[len_out-1].push_back(type); 
            output[len_out-1].push_back(v);        
            break;
          } else {
            cerr << "ERROR: Invalid use of " << myLexeme << endl;
          }
        } else if (myString == "LABEL") { // label instruction   
        	string str = myLexeme.substr(0,len-1);   // substring without :
          	int count = sym_table.count(str); // occurrence of str in symbol table
          	if (count == 0) { 
         	  // add to symbol table
           	  sym_table.insert(pair<string, int>(str,non_null_lines*4));
           	 // output symbol table to standard error
           	  cerr << str << " " << non_null_lines*4+12 << endl;
        	} else { // check if it is a label and not already exist
         	   cerr << "ERROR: Invalid use of Label " << myLexeme << endl;
         	}
        } else if (myString == "ID") { 
        	if (myLexeme == "jr" || myLexeme == "jalr") {
        		if ((size >= 2) && (it2 == it->end()-2) &&
        			((*(it2+1))->toString() == "REGISTER")) {
              ++non_null_lines;
              ++len_out;
              output.resize(len_out);
              string s = ((*(it2+1))->getLexeme()).substr(1, len); // substring without $
              output[len_out-1].push_back(myLexeme);
              output[len_out-1].push_back(s);
              break;
        		} else {
        			cerr << "ERROR: Invalid use of " << myLexeme << endl;
        		} // inner if
        	} else if (myLexeme == "add" || myLexeme == "sub" ||
                     myLexeme == "slt" || myLexeme == "sltu") {
            if ((size >= 6) && (it2 == it->end()-6) &&
               ((*(it2+1))->toString() == "REGISTER") &&
               ((*(it2+2))->toString() == "COMMA") &&
               ((*(it2+3))->toString() == "REGISTER") &&
               ((*(it2+4))->toString() == "COMMA") && 
               ((*(it2+5))->toString() == "REGISTER")) {
              ++non_null_lines;
              ++len_out;   
              output.resize(len_out);
              string d = ((*(it2+1))->getLexeme()).substr(1, len); // substring without $
              string s = ((*(it2+3))->getLexeme()).substr(1, len); // substring without $
              string t = ((*(it2+5))->getLexeme()).substr(1, len); // substring without $
              output[len_out-1].push_back(myLexeme);
              output[len_out-1].push_back(d);
              output[len_out-1].push_back(s);
              output[len_out-1].push_back(t);
              break;
            } else {
              cerr << "ERROR: Invalid use of " << myLexeme << endl;
            }
          } else if(myLexeme == "beq" || myLexeme == "bne") {
            if ((size >= 6) && (it2 == it->end()-6) &&
               ((*(it2+1))->toString() == "REGISTER") &&
               ((*(it2+2))->toString() == "COMMA") &&
               ((*(it2+3))->toString() == "REGISTER") &&
               ((*(it2+4))->toString() == "COMMA") &&
               ((*(it2+5))->toString() == "INT" ||
                (*(it2+5))->toString() == "HEXINT" ||
                (*(it2+5))->toString() == "ID")) {
              ++non_null_lines;
              ++len_out;   
              output.resize(len_out);
              string s = ((*(it2+1))->getLexeme()).substr(1, len); // substring without $
              string t = ((*(it2+3))->getLexeme()).substr(1, len); // substring without $
              string i = (*(it2+5))->getLexeme(); // immediate operand
              string tmp;
              std::ostringstream convert;
              convert << non_null_lines;
              tmp = convert.str();

              output[len_out-1].push_back(myLexeme);
              output[len_out-1].push_back(s);
              output[len_out-1].push_back(t);
              output[len_out-1].push_back((*(it2+5))->toString());
              output[len_out-1].push_back(i);
              output[len_out-1].push_back(tmp);
              break;
            } else {
              cerr << "ERROR: Invalid use of " << myLexeme << endl;
            }
          } else if(myLexeme == "mfhi" || myLexeme == "mflo" || myLexeme == "lis") {
            if ((size >= 2) && (it2 == it->end()-2) &&
                ((*(it2+1))->toString() == "REGISTER")) {
              ++non_null_lines;
              ++len_out;
              output.resize(len_out);
              string d = ((*(it2+1))->getLexeme()).substr(1, len); // substring without $
              output[len_out-1].push_back(myLexeme);
              output[len_out-1].push_back(d);
              break;
            } else {
              cerr << "ERROR: Invalid use of " << myLexeme << endl;
            }

          } else if(myLexeme == "mult" || myLexeme == "multu" ||
                    myLexeme == "div" || myLexeme == "divu") {
            if ((size >= 4) && (it2 == it->end()-4) &&
                ((*(it2+1))->toString() == "REGISTER") &&
                ((*(it2+2))->toString() == "COMMA") &&
                ((*(it2+3))->toString() == "REGISTER")) {
              ++non_null_lines;
              ++len_out;
              output.resize(len_out);
              string s = ((*(it2+1))->getLexeme()).substr(1, len); // substring without $
              string t = ((*(it2+3))->getLexeme()).substr(1, len); // substring without $
              output[len_out-1].push_back(myLexeme);
              output[len_out-1].push_back(s);
              output[len_out-1].push_back(t);
              break;
            } else {
              cerr << "ERROR: Invalid use of " << myLexeme << endl;
            }
          } else if(myLexeme == "lw" || myLexeme == "sw") {
            if ((size >= 7) && (it2 == it->end()-7) &&
                ((*(it2+1))->toString() == "REGISTER") &&
                ((*(it2+2))->toString() == "COMMA") &&
                ((*(it2+3))->toString() == "INT" ||
                 (*(it2+3))->toString() == "HEXINT") &&
                ((*(it2+4))->toString() == "LPAREN") &&
                ((*(it2+5))->toString() == "REGISTER") &&
                ((*(it2+6))->toString() == "RPAREN")) {
              ++non_null_lines;
              ++len_out;
              output.resize(len_out);
              string t = ((*(it2+1))->getLexeme()).substr(1, len); // substring without $
              string s = ((*(it2+5))->getLexeme()).substr(1, len); // substring without $
              string i = (*(it2+3))->getLexeme();
              output[len_out-1].push_back(myLexeme);
              output[len_out-1].push_back(s);
              output[len_out-1].push_back(t);
              output[len_out-1].push_back((*(it2+3))->toString());
              output[len_out-1].push_back(i);
              break;
            } else {
              cerr << "ERROR: Invalid use of " << myLexeme << endl;
            }
          } else {
          cerr << "ERROR: Wrong Instruction" << endl;
          } // outter if
        }
         // cerr << "  Token: "  << *(*it2) << endl;
      } // inner for
    } // outter for

    // output MERL header
    output_byte(268435458);
    output_byte((3+non_null_lines+rel_table.size()*2)*4);
    output_byte((3+non_null_lines)*4);

    // output bytes to standard output
    vector<vector<string> >::iterator it3;
    for(it3 = output.begin(); it3 != output.end(); ++it3) {
    	string myString = (*it3)[0];
      string first = (*it3)[1];
    	// store transferred string to unsigned value
        if (myString == "DOTWORD") {
          unsigned int i;
          if (first == "INT") {
            i = atoi(((*it3)[2]).c_str());
            output_byte(i);
          } else if (first == "HEXINT") {
            i = (int)strtol(((*it3)[2]).c_str(), NULL, 0);
            output_byte(i);
          } else if (first == "ID") {
            if (sym_table.count((*it3)[2]) == 1) {
              i = sym_table.find((*it3)[2])->second;
              output_byte(i+12);
            } else {
              cerr << "ERROR: Label without definition" << endl;
              break;
            }
          }
        } else if (myString == "jr" || myString == "jalr") {
        	int s = atoi(first.c_str());
        	if (s < 0 || s > 31) {
            cerr << "ERROR: out of bound on REGISTER" << endl;
            break;
          }

        	if (myString == "jr") {
        			output_byte(output_one(s,8));
        	} else if (myString == "jalr") {
        			output_byte(output_one(s,9));
        	}
        } else if(myString == "add" || myString == "sub" || 
                  myString == "slt" || myString == "sltu") {
          int d = atoi(first.c_str());
          int s = atoi(((*it3)[2]).c_str());
          int t = atoi(((*it3)[3]).c_str());
          if (d < 0 || d > 31 || s < 0 || s > 31 || t < 0 || t > 31) {
            cerr << "ERROR: out of bound on REGISTER" << endl;
            break;
          }
          if (myString == "add") {
            output_byte(output_three(s,t,d,32));
          } else if (myString == "sub") {
            output_byte(output_three(s,t,d,34));
          } else if (myString == "slt") {
            output_byte(output_three(s,t,d,42));
          } else if (myString == "sltu") {
            output_byte(output_three(s,t,d,43));
          } 
        } else if(myString == "beq" || myString == "bne") {
          int s = atoi(first.c_str());
          int t = atoi(((*it3)[2]).c_str());
          int i;
          if (((*it3)[3]) == "INT") {
            i = atoi(((*it3)[4]).c_str());
          } else if (((*it3)[3]) == "HEXINT") {
            i = (int)strtol(((*it3)[4]).c_str(), NULL, 0);
          } else if (((*it3)[3]) == "ID") {
            if (sym_table.count((*it3)[4]) == 1) {
              int label_value = sym_table.find((*it3)[4])->second;
              int pc = atoi(((*it3)[5]).c_str());
              i = label_value/4 - pc;
            } else {
              cerr << "ERROR: Label without definition" << endl;
              break;
            }
          }
          
          if (s < 0 || s > 31 || t < 0 || t > 31 || 
             ((((*it3)[3]) == "INT" || ((*it3)[3]) == "ID")
               && ((i > 32767) || (i < -32768))) ||
              ((((*it3)[3]) == "HEXINT") && (i > 65535))) {
             cerr << "ERROR: out of bound" << endl;
             break;
          } 

          if (myString == "beq") {
            output_byte(output_two_with_i(s,t,i,268435456));
          } else if (myString == "bne") {
            output_byte(output_two_with_i(s,t,i,335544320));
          } 
            
        } else if(myString == "mfhi" || myString == "mflo" || myString == "lis") {
          int d = atoi(first.c_str());
          if (d < 0 || d > 31) {
            cerr << "ERROR: Out of bound on REGISTER" << endl;
          }
          if (myString == "mfhi") {
            output_byte(output_one_d(d,16));
          } else if (myString == "mflo") {
            output_byte(output_one_d(d,18));
          } else if (myString == "lis") {
            output_byte(output_one_d(d,20));
          }
        } else if(myString == "mult" || myString == "multu" ||
                  myString == "div" || myString == "divu") {
          int s = atoi(first.c_str());
          int t = atoi(((*it3)[2]).c_str());
          if (s < 0 || s > 31 || t < 0 || t > 31) {
            cerr << "ERROR: Out of bound on REGISTER" << endl;
          }
          if (myString == "mult") {
            output_byte(output_two(s,t,24));
          } else if (myString == "multu") {
            output_byte(output_two(s,t,25));
          } else if (myString == "div") {
            output_byte(output_two(s,t,26));
          } else if (myString == "divu") {
            output_byte(output_two(s,t,27));
          }
        } else if(myString == "lw" || myString == "sw") {
          int s = atoi(first.c_str());
          int t = atoi(((*it3)[2]).c_str());
          int i;
          if (((*it3)[3]) == "INT") {
            i = atoi(((*it3)[4]).c_str());
          } else { // (((*it3)[3]) == "HEXINT")
            i = (int)strtol(((*it3)[4]).c_str(), NULL, 0);
          }

          if (s < 0 || s > 31 || t < 0 || t > 31 || 
             ((((*it3)[3]) == "INT") && ((i > 32767) || (i < -32768))) ||
              ((((*it3)[3]) == "HEXINT") && (i > 65535))) {
             cerr << "ERROR: out of bound" << endl;
             break;
          } 

          if (myString == "lw") {
            output_byte(output_two_with_i(s,t,i,2348810240));
          } else if (myString == "sw") {
            output_byte(output_two_with_i(s,t,i,2885681152));
          } 
        } // outter if
    } // for

    // print out the relocation table
    multimap<string,int>::iterator it4;
    for (it4 = rel_table.begin(); it4 != rel_table.end(); ++it4) {     
      if (sym_table.count(it4->first) == 1) {
        output_byte(1);
        output_byte(it4->second);
      } else {
        cerr << "ERROR: Label without definition" << endl;
        break;
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


