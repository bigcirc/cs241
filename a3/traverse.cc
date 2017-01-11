#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int indic=0;//this is a indicator variable.

struct node{
  int nodes;
  int numofchild;
  int counter;//both store num of children
  int parentcounter;//store parent's numofchild.
  node *list;//store all siblings
};


void post_traverse(node* t){//this is for print.
  if(t==NULL){
    return;//if NULL return
  }
  if(indic==0){//if indicator has been changed, run for loop
    for(int i=0;i<t->numofchild;i++){
      if(i!=t->numofchild-1){
	t->list--;
      }
    }
  }
  post_traverse(t->list);//this is for recursion
  if(t->parentcounter==1 || t->parentcounter==0){
    cout << t->nodes << " " << t->numofchild << endl;
    //only cases are node is root node or node has only one child
  }else{
    indic=1;//otherwise, run this loop
    for(int i=0;i<t->parentcounter;i++){
      if(i>=1){
	if(t->numofchild >1){
	  for(int i=0;i<t->numofchild;i++){
	    if(i!=t->numofchild-1){
	      t->list--;//if not the first time, make the list be the first node
	      //and then run loop.
	      //otherwise, siblings are not the index 0 of array list
	    }
	  }
	}
	post_traverse(t->list);//if we run the first time, recursion
      }
      cout << t->nodes << " " << t->numofchild<< endl;//after all, print
      if(i != t->parentcounter-1){
	t++;//continue.
      }
    }
  }
}

node *addnode(node *t,int a,int b,int c){//this is for add node
  if(t!=NULL){
    if(b==0){//if we can determine there is no child
      t->nodes=a;
      t->numofchild=b;
      t->counter=b;
      t->parentcounter=c;
      t->list =NULL;//do so
    }
    else{//otherwise
      t->nodes=a;
      t->numofchild=b;
      t->counter=b;
      t->parentcounter=c;
      t->list =new node[b];
      for(int i=0;i<t->numofchild;i++){//run loop to add child
	cin >> a >> b;
	t->list = addnode(t->list,a,b,t->numofchild);
	if(t->counter >1){
	  t->list++;
	  t->counter--;//coutner for determine how many space left
	}
      }//end for
    }
  }//end t!=NULL
  else{//t==NULL
    t=new node();
    t->nodes=a;
    t->counter=b;
    t->numofchild=b;
    t->parentcounter=c;//add stuff
    if(b==0){//determine child numbers
      t->list = NULL;
    }
    if(b==1){
      t->list=new node();
      cin >>a >> b;
      t->list=addnode(t->list,a,b,t->numofchild);
    }
    if(b>1){
      t->list = new node[b];
      for(int i=0;i<t->numofchild;i++){
	cin >> a >>b;
	t->list=addnode(t->list,a,b,t->numofchild);
	if(t->counter >1){
	  t->list++;
	  t->counter--;
	}
      }
    }//select one
    
  }
  return t;//return
}


void freenode(node*t){
  if(t==NULL){
    return;
  }
  if(t->numofchild==0){
    delete t;
    return;
  }
  if(t->numofchild==1){
    freenode(t->list);
    delete t->list;
  }
  else{
    for(int i=0;i<t->numofchild;i++){
      freenode(t->list);
      if(i!=t->numofchild-1){
	t->list++;
      }
    }
  }
  if(t->numofchild==1){
    delete t->list;
  }
  else{
    for(int i=0;i<t->numofchild;i++){
      delete t->list;
      if(i!=t->numofchild-1){
	t->list++;
      }
    }
  }
  delete t;

}//free space




int main(){
  int nodes;
  int num;
  node *init=NULL;
  while(true){

    cin >> nodes;
    cin >> num;
    if(cin.eof()) break;
    init=addnode(init,nodes,num,0);
  }//run loop to add nodes
  post_traverse(init);//print with order
  freenode(init);//free
}
