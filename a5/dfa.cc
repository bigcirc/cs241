#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
typedef vector<string> vec;
int main(){
  int a;
  string b;
  string start;
  cin >> a;
  vec alphabet;
  vec state;
  vec final;
  for(int i = 0; i < a; i++){
    cin >> b;
    alphabet.push_back(b);
  }
  cin >> a;
  for( int i = 0; i < a; i++){
    cin >> b;
    state.push_back(b);
  }
  cin >> start;
  cin >> a;
  for( int i =0; i < a; i++){
    cin >> b;
    final.push_back(b);
  }
  vec route;
  cin >> a;
  for( int i = 0; i < a; i++){
    getline(cin, b);
    route.push_back(b);
  }
  while(getline(cin,b)){
    stringstream ss;
    ss << b;
    
  }
    
  
}
