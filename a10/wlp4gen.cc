#include <map>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
map<string, map<string, int> > off;//offset
string offset;
int counter = 0;
int indic = 0;
map<string, vector<pair<string, string> > > proc;
map<string, string> sym_tab;
map<string, map<string, string> > ty;
string funname;
int numele = 0;
string id;
vector< pair<string, string> > now;
vector< pair<string, int> > numpara;

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
  if(line == "INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
    
  }
  tmp->rule = line;                                                                            
  istringstream iss(line);
  iss >> tok;
  if(tok == ""){
    idc = 1;
    return;
  }
  if(tok == "BOF" || tok == "BECOMES" || tok == "COMMA" || tok == "ELSE" ||
     tok == "EOF" || tok == "EQ" || tok == "GE"  || tok == "ID" || tok == "IF" ||
     tok == "GT" || tok == "INT" || tok == "LBRACE" || tok == "LE" || tok == "LPAREN" || tok == "WAIN" ||                              
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

   
string key(Tree *tmp){
  string a;
  istringstream ss(tmp->rule);
  ss >> a;
  return a;
}

string name(Tree * tmp){
  string name, b;
  istringstream ss(tmp->rule);
  ss >> b;
  while(ss >> b){
    name += b;
  }
  return name;
}
int total = 0;
int number(Tree *tmp){
  if(tmp->rule == "arglist expr COMMA arglist"){
    total++;
    number(tmp->children[2]);
  } else if(tmp->rule == "arglist expr"){
    total++;
    return total;
  }
}

string whatty(Tree *tmp){
  if(tmp->rule == "expr term"){
    string result = whatty(tmp->children[0]);
    return result;
  } else if(tmp->rule == "term factor"){
    string result = whatty(tmp->children[0]);
    return result;
  } else if(tmp->rule == "factor ID"){
    istringstream ss(tmp->children[0]->rule);
    string result;
    ss >> result >> result;
    return result;
  } else if(tmp->rule == "factor ID LPAREN RPAREN"){
    istringstream ss(tmp->children[0]->rule);
    string n;
    ss >> n >> n;
    if(proc.count(n) > 0) return "int";
  } else if(tmp->rule == "factor NULL"){
    return "int*";
  } else if(tmp->rule == "factor NUM"){
    return "int";
  }  else if(tmp->rule == "expr expr PLUS term"){
    string expr1 = whatty(tmp->children[0]);
    string expr2 = whatty(tmp->children[2]);
    cerr << expr1 << " " << expr2 << " they are" << endl;
    if(expr1 != "int" && expr1 != "int*"){
      if(id != "WAIN"){
	for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
	  if((*it).second == expr1)
	    expr1 = (*it).first;
	}
      } else if(id == "WAIN"){
	expr1 = ty[id][expr1];
      }
    }
    if(expr2 != "int" && expr2 != "int*"){
      if(id != "WAIN"){
	for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
	  if((*it).second== expr2)
	    expr2 = (*it).first;
	}
      } else if(id == "WAIN"){
	expr2 = ty[id][expr2];
      }
    }
    if (expr1 =="int" && expr2 == "int"){
      return "int";
    } else if((expr1 =="int*" && expr2 == "int") ||(expr1 =="int" && expr2 == "int*")){
      return "int*";
    } else{
      string err = "ERROR plus";
      throw err;
    }
  } else if(tmp->rule == "expr expr MINUS term"){
    string expr1 = whatty(tmp->children[0]);
    string expr2 = whatty(tmp->children[2]);
    if(expr1 != "int" && expr1 != "int*") {
      if(id != "WAIN"){
        for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
          if((*it).second== expr1)
            expr1 = (*it).first;
        }
      } else{
	expr1 = ty[id][expr1];
      }
    }
    if(expr2 != "int" && expr2 != "int*"){
      if(id != "WAIN"){
        for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
          if((*it).second== expr2)
            expr2 = (*it).first;
        }
      } else{
	expr2 = ty[id][expr2];
      }
    }
    cerr << expr1 << expr2 << endl;
    if(expr1 == "int" && expr2 == "int") return "int";
    else if(expr1 == "int*" && expr2 == "int") return "int*";
    else if(expr1 == "int" && expr2 == "int*") return "int*";
    else{
      string err = "ERROR";
      throw err;
    }
  } else if(tmp->rule == "factor STAR factor"){
    string result = whatty(tmp->children[1]);
    cerr << "resulr " << result << "is wain>? "<< id<< endl;
    if(result != "int" && result != "int*"){
      if(id != "WAIN"){
        for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
          if((*it).second== result)
            result = (*it).first;
        }
      } else if(id == "WAIN"){
        result = ty[id][result];
      }
    }
    if(result == "int*")
      return "int";
  } else if(tmp->rule == "term term STAR factor"){
    string expr1 = whatty(tmp->children[0]);
    string expr2 = whatty(tmp->children[2]);
    cerr << expr1 << " star "<<expr2 << endl;
    if(expr1 != "int"){
      for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
        if((*it).second == expr1){
          expr1 = "int";
          break;
        }
      }
    }
    if(expr2 != "int"){
      for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
        if((*it).second == expr2){
          expr2 = "int";
          break;
        }
      }
    }
    if(expr1 == "int" && expr2 == "int") return "int";
  } else if(tmp->rule == "term term SLASH factor"){
    string expr1 = whatty(tmp->children[0]);
    string expr2 = whatty(tmp->children[2]);
    cerr << expr1 << " slash "<<expr2 << endl;
    if(expr1 != "int"){
      for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
	if((*it).second == expr1){
	  expr1 = "int";
	  break;
	}
      }
    }
    if(expr2 != "int"){
      for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
        if((*it).second == expr2){
          expr2 = "int";
          break;
        }
      }
    }
    if(expr1 =="int" && expr2 == "int") return"int";
  } else if(tmp->rule == "term term PCT factor"){
    string expr1 = whatty(tmp->children[0]);
    string expr2 = whatty(tmp->children[2]);
    if(expr1 != "int"){
      for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
        if((*it).second == expr1){
          expr1= "int";
          break;
        }
      }
    }
    if(expr2 != "int"){
      for(vector<pair<string, string> >::iterator it = proc[id].begin(); it != proc[id].end(); it++){
        if((*it).second == expr2){
          expr2 = "int";
          break;
        }
      }
    }
    if(expr1 =="int" && expr2 == "int") return"int";
  } else if(tmp->rule == "factor ID LPAREN arglist RPAREN"){
    total = 0;
    istringstream ss(tmp->children[0]->rule);
    string a;
    ss >> a >> a;
    int n = number(tmp->children[2]);
    cerr << "n is ? " << n << endl;
    for(vector<pair<string, int> >::iterator it = numpara.begin(); it != numpara.end(); it++){
      if((*it).first == a){
	if(n != (*it).second){
	  string err = "ERROR less or more para";
	  throw err;
	}
      }
    }
    return "int";
  } else if(tmp->rule == "factor LPAREN expr RPAREN"){
    string a = whatty(tmp->children[1]);
    return a;
  }
}



void push(string reg){
  cout << "sw " << reg << ", -4($30)" << endl;
  cout << "sub $30, $30, $4" << endl;
}

void pop(string reg){
  cout << "add $30, $30, $4" << endl;
  cout << "lw " << reg << ", -4($30)" << endl;
}

int q = 0;
void test(Tree * tmp){
  if(tmp->rule == "start BOF procedures EOF"){
    test(tmp->children[1]);
  } else if(tmp->rule == "procedures main"){
    test(tmp->children[0]);
  } else if(tmp->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
    //string funname;
    counter = 0;
    istringstream ss(tmp->rule);
    ss >> funname >> funname >> funname;
    id = funname;
    cout << "lis $11" << endl;
    cout << ".word 1" << endl;
    cout << "lis $4" << endl;
    cout << ".word 4" << endl;
    cout << "sub $29, $30, $4" << endl;
    string type, name;
    istringstream ss2(tmp->children[3]->children[0]->rule);
    ss2 >> name >> name;
    if(ss2 >> name) name = "int*";
    else{
      name = "int";
    }
    istringstream ss1(tmp->children[3]->children[1]->rule);
    ss1 >> type >> type;
    ty[funname][type] = name;
    off[funname][type] = 0;
    counter++;
    cout << "sw $1, 0($29)" << endl;
    string type1, name1;
    istringstream ss3(tmp->children[5]->children[1]->rule);
    ss3 >> type1 >> type1;
    istringstream ss4(tmp->children[5]->children[0]->rule);
    ss2 >> name1 >> name1;
    if(ss2 >> name1) name1 = "int*";
    else{
      name1 = "int";
    }
    off[funname][type1] = -4;
    ty[funname][type1] = name1;
    counter++;
    cout << "sw $2, -4($29)" << endl;
    cout << "sub $30, $30, $4" << endl;
    cout << "sub $30, $30, $4" << endl;
    if(name == "int") cout << "add $2, $0, $0" << endl;
    push("$31");
    cout << "lis $9" << endl;
    cout << ".word init" << endl;
    cout << "jalr $9" << endl;
    pop("$31");
    test(tmp->children[8]);
    test(tmp->children[9]);
    test(tmp->children[11]);
    cout << "jr $31" << endl;
  } else if(tmp->rule == "expr term"){
    test(tmp->children[0]);
  } else if(tmp->rule == "term factor"){
    test(tmp->children[0]);
  } else if(tmp->rule == "factor ID"){
    string name;
    istringstream ss(tmp->children[0]->rule);
    ss >> name >> name;
    cout << "lw $3, " << off[id][name] << "($29)" << endl;
  } else if(tmp->rule == "factor LPAREN expr RPAREN"){
    test(tmp->children[1]);
  } else if(tmp->rule == "expr expr PLUS term"){
    string typ1 = whatty(tmp->children[0]);
    string typ2 = whatty(tmp->children[2]);
    cerr<< "meici "<< typ1 << " " << typ2 << endl;
    if(typ1 != "int" && typ1 != "int*"){
      string t = ty[id][typ1];
      typ1 = t;
    }
    if(typ2 != "int" && typ2 !="int*"){
      string t = ty[id][typ2];
      typ2 = t;
    }
    cerr << "typ1 " << typ1 << " t yp2 " << typ2 << endl;
    if(typ1 == "int" && typ2 == "int"){
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$8");
      cout << "add $3, $8, $3" << endl;
    } else if(typ1 == "int" && typ2 == "int*"){
      q++;
      int tempq = q;
      cerr << tempq << endl;
      cout << "beq $3, $11, " << "plus" << tempq << endl;
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$8");
      cout << "mult $8, $4" << endl;
      cout << "mflo $8" << endl;
      cout << "add $3, $8, $3" << endl;
      cout << "plus" << tempq << ":" << endl;
    } else if(typ1 == "int*" && typ2 == "int"){
      cout << "; jinlaile," << endl;
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$8");
      cout << "mult $3, $4" << endl;
      cout << "mflo $3" << endl;
      cout << "add $3, $8, $3" << endl;
    }
  } else if(tmp->rule == "expr expr MINUS term"){
    cerr << "minus seg " << endl;
    string typ1 = whatty(tmp->children[0]);
    string typ2 = whatty(tmp->children[2]);
    cerr << typ1 << " typ1 minus " << typ2 << " typ2" << endl;
    if(typ1 != "int" && typ1 != "int*"){
      string t = ty[id][typ1];
      typ1 = t;
    }
    if(typ2 != "int" && typ2 !="int*"){
      string t = ty[id][typ2];
      typ2 = t;
    }
    if(typ1 == "int" && typ2 == "int"){
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$8");
      cout << "sub $3, $8, $3" << endl;
    } else if(typ1 == "int*" && typ2 == "int"){
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$8");
      cout << "mult $3, $4" << endl;
      cout << "mflo $3" << endl;
      cout << "sub $3, $8, $3" << endl;
    } else if(typ1 == "int*" && typ2 == "int*"){
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$8");
      cout << "sub $3, $8, $3" << endl;
      cout << "div $3, $4" << endl;
      cout << "mflo $3" << endl;
    }
  } else if(tmp->rule == "term term STAR factor"){
    cerr << "times seg fal" << endl;
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "mult $3, $8" << endl;
    cout << "mflo $3" << endl;
  } else if(tmp->rule == "term term SLASH factor"){
    cerr << "div seg fal" << endl;
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "div $8, $3" << endl;
    cout << "mflo $3" << endl;
  } else if(tmp->rule == "term term PCT factor"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$8");
    cout << "div $8, $3" << endl;
    cout << "mfhi $3" << endl;
  } else if(tmp->rule == "factor NUM"){
    string num;
    istringstream ss(tmp->children[0]->rule);
    ss >> num >> num;
    cout << "lis $3" << endl;
    cout << ".word " << num << endl;
  } else if(tmp->rule == "statements statements statement"){
    test(tmp->children[0]);
    test(tmp->children[1]);
  } else if(tmp->rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
    test(tmp->children[2]);
    //push("$1");
    cout << "add $1, $3, $0" << endl;
    push("$31");
    cout << "lis $9" << endl;
    cout << ".word print" << endl;
    cout << "jalr $9" << endl;
    pop("$31");
    //    pop("$1");
  } else if(tmp->rule == "dcls dcls dcl BECOMES NUM SEMI"){
    test(tmp->children[0]);
    string type1, name1;
    istringstream ss2(tmp->children[1]->children[1]->rule);
    istringstream ss5(tmp->children[1]->children[0]->rule);
    ss5 >> name1 >> name1;
    if(ss5 >> name1) name1 = "int*";
    else name1 = "int";
    ss2 >> type1 >> type1;
    off[id][type1] = -4 * (counter) + numele * 4;
    cerr << id << type1 << " xiaojiba" << endl;
    ty[id][type1] = name1;
    counter++;
    cout << "lis $7" << endl;
    string num;
    istringstream ss(tmp->children[3]->rule);
    ss >> num >> num;
    cout << ".word " << num << endl;
    cout << "sw $7, " << off[id][type1] << "($29)" << endl;
    cout << "sub $30, $30, $4" << endl;
  } else if(tmp->rule == "statement lvalue BECOMES expr SEMI"){
    test(tmp->children[0]);
    cout << "add $5, $3, $0" << endl;
    test(tmp->children[2]);
    cout << "sw $3, 0($5)" << endl;
    //    cout << "sw $3, " << off[funname][offset] << "($29)" << endl;
  } else if(tmp->rule == "lvalue ID"){
    istringstream ss1(tmp->children[0]->rule);
    ss1 >> offset >> offset;
    cout << "lis $3" << endl;
    cout << ".word " << off[id][offset] << endl;
    cout << "add $3, $3, $29" << endl;
  } else if(tmp->rule == "lvalue LPAREN lvalue RPAREN"){
    test(tmp->children[1]);
  } else if(tmp->rule == "test expr LT expr"){
    string a = whatty(tmp->children[0]);
    string b = whatty(tmp->children[2]);
    if(a != "int" && a != "int*") a = ty[id][a];
    if(b != "int" && b != "int*") b = ty[id][b];
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$5");
    if(a == "int" && b == "int"){
      cout << "slt $3, $5, $3" << endl;
    } else if( a == "int*" && b == "int*"){
      cout << "sltu $3, $5, $3" << endl;
    }
  } else if(tmp->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
    q++;
    int tempq = q;
    cout << "loop" << tempq << ":" << endl;
    test(tmp->children[2]);
    cout << "beq $3, $0, endloop" << tempq << endl;
    test(tmp->children[5]);
    cout << "beq $0, $0, loop" << tempq << endl;
    cout << "endloop" << tempq << ":" << endl;
  } else if(tmp->rule == "test expr EQ expr"){
    string a = whatty(tmp->children[0]);
    string b = whatty(tmp->children[2]);
    if(a != "int" && a != "int*") a = ty[id][a];
    if(b != "int" && b != "int*") b = ty[id][b];
    test(tmp->children[0]);
    cout << "add $5, $3, $0" <<endl;
    test(tmp->children[2]);
    if(a == "int" && b== "int"){
      cout << "slt $6, $3, $5" << endl;
      cout << "slt $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
      cout << "sub $3, $11, $3" << endl;
    } else if(a == "int*" && b == "int*"){
      cout << "sltu $6, $3, $5" << endl;
      cout << "sltu $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
      cout << "sub $3, $11, $3" << endl;
    }
  } else if(tmp->rule == "test expr NE expr"){
    string a = whatty(tmp->children[0]);
    string b = whatty(tmp->children[2]);
    if(a != "int" && a != "int*") a = ty[id][a];
    if(b != "int" && b != "int*") b = ty[id][b];
    test(tmp->children[0]);
    cout << "add $5, $3, $0" << endl;
    test(tmp->children[2]);
    if( a == "int" && b == "int"){
      cout << "slt $6, $3, $5" << endl;
      cout << "slt $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
  } else if(a == "int*" && b == "int*"){
      cout << "sltu $6, $3, $5" << endl;
      cout << "sltu $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
    }
  } else if(tmp->rule == "test expr LE expr"){
    string a = whatty(tmp->children[0]);
    string b = whatty(tmp->children[2]);
    if(a != "int" && a != "int*") a = ty[id][a];
    if(b != "int" && b != "int*") b = ty[id][b];
    test(tmp->children[0]);
    cout << "add $5, $3, $0" << endl;
    test(tmp->children[2]);
    if( a == "int" && b == "int"){
      cout << "slt $6, $3, $5" <<endl;
      cout << "slt $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
      cout << "sub $12, $11, $3" << endl;
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$5");
      cout << "slt $3, $5, $3" << endl;
      cout << "add $3, $3, $12" << endl;
    } else if(a == "int*" && b == "int*"){
      cout << "sltu $6, $3, $5" <<endl;
      cout << "sltu $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
      cout << "sub $12, $11, $3" << endl;
      test(tmp->children[0]);
      push("$3");
      test(tmp->children[2]);
      pop("$5");
      cout << "sltu $3, $5, $3" << endl;
      cout << "add $3, $3, $12" << endl;
    }
  } else if(tmp->rule == "test expr GE expr"){
    string a = whatty(tmp->children[0]);
    string b = whatty(tmp->children[2]);
    if(a != "int" && a != "int*") a = ty[id][a];
    if(b != "int" && b != "int*") b = ty[id][b];
    test(tmp->children[0]);
    cout << "add $5, $3, $0" <<endl;
    test(tmp->children[2]);
    if(a == "int" && b == "int"){
      cout << "slt $6, $3, $5" <<endl;
      cout << "slt $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
      cout << "sub $12, $11, $3" << endl;
      test(tmp->children[2]);
      push("$3");
      test(tmp->children[0]);
      pop("$5");
      cout << "slt $3, $5, $3" << endl;
      cout << "add $3, $3, $12" << endl;
  } else if(a == "int*" && b == "int*"){
      cout << "sltu $6, $3, $5" <<endl;
      cout << "sltu $13, $5, $3" << endl;
      cout << "add $3, $13, $6" << endl;
      cout << "sub $12, $11, $3" << endl;
      test(tmp->children[2]);
      push("$3");
      test(tmp->children[0]);
      pop("$5");
      cout << "sltu $3, $5, $3" << endl;
      cout << "add $3, $3, $12" << endl;
    }
  } else if(tmp->rule == "test expr GT expr"){
    string a = whatty(tmp->children[0]);
    string b = whatty(tmp->children[2]);
    if(a != "int" && a != "int*") a = ty[id][a];
    if(b != "int" && b != "int*") b = ty[id][b];
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
    pop("$5");
    if(a == "int" && b == "int"){
      cout << "slt $3, $3, $5" << endl;
    } else if( a == "int*" && b == "int*"){
      cout << "sltu $3, $3, $5" << endl;
    }
  } else if(tmp->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
    q++;
    int tempq = q;
    test(tmp->children[2]);
    cout << "beq $3, $0, else" << tempq << endl;
    test(tmp->children[5]);
    cout << "beq $0, $0, endif" << tempq << endl;
    cout << "else" << tempq <<  ":" << endl;
    test(tmp->children[9]);
    cout << "endif" << tempq << ":" << endl;
  }  else if(tmp->rule == "dcls dcls dcl BECOMES NULL SEMI"){
    test(tmp->children[0]);
    string type1, name1;
    istringstream ss2(tmp->children[1]->children[1]->rule);
    istringstream ss5(tmp->children[1]->children[0]->rule);
    ss5 >> name1 >> name1;
    if(ss5 >> name1) name1 = "int*";
    else name1 = "int";
    ss2 >> type1 >> type1;
    off[id][type1] = -4 * (counter) + 4 * numele;
    ty[id][type1] = name1;
    counter++;
    cout << "add $7, $11, $0" << endl;
    cout << "sw $7, " << off[id][type1] << "($29)" << endl;
    cout << "sub $30, $30, $4" << endl;
  } else if(tmp->rule == "factor NULL"){
    cout << "add $3, $11, $0" << endl;
  } else if(tmp->rule == "factor AMP lvalue"){
    test(tmp->children[1]);
  } else if(tmp->rule == "factor STAR factor"){
    test(tmp->children[1]);
    cout << "lw $3, 0($3)" << endl;
  } else if(tmp->rule == "lvalue STAR factor"){
    test(tmp->children[1]);
  } else if(tmp->rule == "factor NEW INT LBRACK expr RBRACK"){
    q++;
    int tempq = q;
    test(tmp->children[3]);
    cout << "add $1, $3, $0" << endl;
    push("$31");
    cout << "lis $6" << endl;
    cout << ".word " << "new" << endl;
    cout << "jalr $6" << endl;
    pop("$31");
    cout << "bne $3, $0, " << "renew" << tempq << endl;
    cout << "add $3, $11, $0" << endl;
    cout << "renew" << tempq << ":" << endl;
  } else if(tmp->rule == "statement DELETE LBRACK RBRACK expr SEMI"){
    q++;
    int tempq = q;
    test(tmp->children[3]);
    cout << "beq $3, $11, " << "renew" << tempq << endl;
    cout << "add $1, $3, $0" <<endl;
    push("$31");
    cout << "lis $6" <<endl;
    cout << ".word " <<"delete" << endl;
    cout << "jalr $6" << endl;
    pop("$31");
    cout << "renew" << tempq << ":" << endl;
  } else if(tmp->rule == "procedures procedure procedures"){
    test(tmp->children[1]);
    test(tmp->children[0]);
  } else if(tmp->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
    counter = 0;
    istringstream ss(tmp->children[1]->rule);
    ss >> id >> id;
    for(vector<pair<string, int> >::iterator it = numpara.begin(); it != numpara.end(); it++){
      if((*it).first == id){
        numele = (*it).second;
        break;
      }
    }
    cerr << numele << " numele" << endl;
    test(tmp->children[3]);
    cout << "; prologue" << id << endl;
    cout << id << ":" << endl;
    cout << "sub $29, $30, $4" << endl;
    test(tmp->children[6]);
    push("$7");
    test(tmp->children[7]);
    test(tmp->children[9]);
    pop("$7");
    cout << "add $30, $29, $4" << endl;
    cout << "jr $31" << endl;
  } else if(tmp->rule == "params"){
    ty[id][""] = "";
    //off[id][""] = 0;
    counter++;
  } else if(tmp->rule == "params paramlist"){
    test(tmp->children[0]);
  } else if(tmp->rule == "paramlist dcl"){
    istringstream ss(tmp->children[0]->children[0]->rule);
    string n;
    ss >> n >> n;
    if(ss >> n) n = "int*";
    else n = "int";
    istringstream ss1(tmp->children[0]->children[1]->rule);
    string n1;
    ss1 >> n1 >> n1;
    off[id][n1] = -4 * counter + numele * 4;
    ty[id][n1] = n;
    counter++;
  } else if(tmp->rule == "paramlist dcl COMMA paramlist"){
    istringstream ss(tmp->children[0]->children[0]->rule);
    string n;
    ss >> n >> n;
    if(ss >> n) n = "int*";
    else n = "int";
    istringstream ss1(tmp->children[0]->children[1]->rule);
    string n1;
    ss1 >> n1 >> n1;
    off[id][n1] = -4 * counter + numele * 4;
    cerr << n1 << " n1 " << -4*counter << endl;
    ty[id][n1] = n;
    counter++;
    test(tmp->children[2]);
  } else if(tmp->rule == "factor ID LPAREN RPAREN"){
    cerr <<tmp->children[0]->rule << endl;
    istringstream ss(tmp->children[0]->rule);
    string n;
    ss >> n >> n;
    push("$29");
    push("$31");
    cout << "lis $9" << endl;
    cout << ".word " << n << endl;
    cout << "jalr $9" << endl;
    pop("$31");
    pop("$29");
  } else if(tmp->rule == "factor ID LPAREN arglist RPAREN"){
    istringstream ss(tmp->children[0]->rule);
    string n;
    ss >> n >> n;
    for(vector<pair<string, int> >::iterator it = numpara.begin(); it != numpara.end(); it++){
      if((*it).first == n){
	numele = (*it).second;
	break;
      }
    }
    //    if(id != "WAIN"){
    push("$29");
    push("$31");
    test(tmp->children[2]);
    cout << "lis $9" << endl;
    cout << ".word " << n << endl;
    cout << "jalr $9" << endl;
    for(int i = 0; i < numele; i++){
      pop("$9");
    }
    pop("$31");
    pop("$29");
      // }
  } else if(tmp->rule == "arglist expr"){
    test(tmp->children[0]);
    push("$3");
  } else if(tmp->rule == "arglist expr COMMA arglist"){
    test(tmp->children[0]);
    push("$3");
    test(tmp->children[2]);
  }
}








int c = 0;
vector<pair<string, string> > list;
void check(Tree *tmp, map<string, string> &sym1, map<string, string> &sym_tab, map<string, vector<pair<string, string> > > &proc){
  string a = key(tmp);
  string second = name(tmp);
  if(a == "procedure"){
    list.clear();
    string t;
    istringstream ss1(tmp->children[1]->rule);
    ss1 >> t >> t;
    id = t;
    if(sym1.count(t) != 0){
      string err ="ERROR double defn";
      throw err;
    }
    sym1.insert(pair<string, string>(t, "int"));
    //vector<pair<string, string> > list;
    cerr << t;
    Tree* child = tmp->children[3];
    string begin = name(child);
    if(child->children[0]==NULL){
      //off[id][""] = 0;
      cerr << endl;
    }
    if(begin == "paramlist"){
      c = 0;
      counter = 0;
      child = child->children[0];
      while(child->children.size() == 3){
        c++;
	Tree *child1 = child->children[0];
        string type;
        istringstream ss11(child1->children[0]->rule);
        ss11 >> type >> type;
        if(ss11 >> type){
          type = "int*";
        } else{
          type = "int";
        }
        istringstream ss2(child1->children[1]->rule);
        string name;
        ss2 >> name >> name;
        sym_tab[name] = type;
	//	off[id][name] = counter * (-4);
	//counter++;
        list.push_back(pair<string, string>(type, name));
        cerr << " " << type;
        child = child->children[2];
      }
      c++;
      Tree *child2 = child->children[0];
      string type2, name2;
      istringstream ss12(child2->children[0]->rule);
      ss12 >> type2 >> type2;
      istringstream ss13(child2->children[1]->rule);
      cerr << "nima bi " << child2->children[1]->rule << endl;
      ss13 >> name2 >> name2;
      if(ss12 >> type2){
        type2 = "int*";
      } else{
	type2 = "int";
      }
      //off[id][name2] = -4 * counter;
      //counter++;
      list.push_back(pair<string, string>(type2, name2));
      cerr << " " << type2 << endl;
      numpara.push_back(pair<string, int>(t, c));
    }
    map<string, string> sym_tabs;
    for(int i = 2; i < tmp->children.size(); i++){
      check(tmp->children[i],sym1, sym_tabs, proc);
    }
    proc[t] = list;
    cerr << endl;
    return;
  } else if(a == "main"){
    counter = 0;
    if(sym1.count("wain") != 0){
      string err = "ERROR wain is declared before";
      throw err;
    }
    id = "WAIN";
    sym1["wain"] = "int";
    string name1, name2, ty1, ty2;
    istringstream n1(tmp->children[3]->children[0]->rule);
    istringstream t1(tmp->children[3]->children[1]->rule);
    t1 >> ty1 >> ty1;
    n1 >> name1 >> name1;
    if(n1 >> name1){
      name1 = "int*";
    } else{
      name1 = "int";
    }
    // off["WAIN"][ty1] = 0;
    //counter++;
    istringstream n2(tmp->children[5]->children[0]->rule);
    istringstream t2(tmp->children[5]->children[1]->rule);
    t2 >> ty2 >> ty2;
    n2 >> name2 >> name2;
    if(n2 >> name2){
      name2 = "int*";
    } else{
      name2 = "int";
    }
    //off["WAIN"][ty2] = -4;
    //counter++;
    cerr << "wain " <<  name1 << " " << name2 << endl;
    map<string, string> sym_tabs;
    sym_tab = sym_tabs;
  } else if(a == "dcl"){
    string t1, t2;
    istringstream iss(tmp->children[0]->rule);
    istringstream iss1(tmp->children[1]->rule);
    iss >> t1 >> t1;
    if(iss >> t1){
      t1 = "int*";
    } else{
      t1 = "int";
    }
    iss1 >> t2 >> t2;
    if(sym_tab.count(t2) != 0){
      string err = "ERROR duplicate defn";
      throw err;
    }
    if(t1 == "int"){
      sym_tab[t2] = t1;
      cerr << t2 << " " << t1 << endl;
    } else{
      sym_tab[t2] = t1;
      cerr << t2 << " " << t1 << endl;
    }
    ///off[id][t2] = counter* (-4);
    //counter++;
    list.push_back(pair<string,string>(t1, t2));
  } else if(a == "ID"){
    if(sym_tab.count(second) == 0){
	string err = "ERROR var never defn";
	throw err;
      }
  } else if(tmp->rule == "factor ID LPAREN RPAREN" || tmp->rule == "factor ID LPAREN arglist RPAREN"){
    string t1;
    istringstream iss(tmp->children[0]->rule);
    iss >> t1 >> t1;
    if(sym_tab.count(t1) != 0){
      string err =  "ERROR var defn before ";
      throw err;
    } else if(sym1.count(t1) == 0){
      string err = "ERROR never defn";
      throw err;
    } else{
      for(int i = 1; i < tmp->children.size(); i++){
        check(tmp->children[i], sym1, sym_tab, proc);
      }
    }
    return;
  }
  if(tmp->children.size() == 0) return;
  if(tmp->children[0] != NULL && tmp->children.size() != 0){
    for(int i = 0; i < tmp->children.size(); i++){
      check(tmp->children[i], sym1, sym_tab, proc);
    }
  }
}


int main(){
  map<string, string> sym1;
  vector<string> code;
  int idc = 0;
  Tree *tmp = new Tree();
  traverse(tmp, idc);
  int indic = 0;
  check(tmp, sym1, sym_tab, proc);
  //  find(tmp, sym_tab, vecofst);
  //print(tmp, sym_tab, indic);
  //if(indic == 0){
  // cerr << sym_tab.find("wain")->first << endl;
  // map<string, string>::iterator it = sym_tab.find("wain");
  // sym_tab.erase(it);
  // for(map<string, string>::iterator it = sym_tab.begin(); it != sym_tab.end(); it++){
  //   cerr << it->first << " " << it->second << endl;  
  // }
  // }
  try{
    cout << ".import print" << endl;
    cout << ".import init" << endl;
    cout << ".import new" << endl;
    cout << ".import delete" << endl;
    test(tmp);
    for(vector<string>::iterator it = code.begin(); it != code.end(); it++){
      cout << (*it) << endl;
    }
  } catch(string err){
    cerr << err << endl;
  }
}
    
