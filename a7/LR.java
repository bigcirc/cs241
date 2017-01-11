import java.util.*;
import java.math.*;

public class LR {
    public static void main(String[] args){
	new LR().run();
    }

    public void run(){
	Scanner in=new Scanner(System.in);
	//skip the number of terminals
	int num=in.nextInt();
	in.nextLine();
	
	for(int i=0;i<num;i++){
	    in.nextLine();
	}
	//skip the lines of non-terminals
	num=in.nextInt();
	in.nextLine();

	for(int i=0;i<num;i++){
	    in.nextLine();
	}
	// the start symbol
	String strSymbol=in.nextLine();

	//read number of production rules
	num=in.nextInt();
	in.nextLine();
	//make an array of production rules
	String production[]=new String[num];
	for(int i=0;i<num;i++){
	    production[i]=in.nextLine();
	}

	//number of states
	num=in.nextInt();
	in.nextLine();
	//System.out.println(num);
	//array of states
	ArrayList<ArrayList<String>> state=new ArrayList<ArrayList<String>>(num);

	for(int i=0;i<num;i++){
	    ArrayList<String> a=new ArrayList<String>();
	    state.add(a);
	}

	//number of reduce or shift rules
	num=in.nextInt();
	in.nextLine();

	//the list of reduce, shift operations
	for(int i=0;i<num;i++){
	    String s=in.nextLine();
	    //System.out.println(s.substring(0,1));
	    String str[]=s.split(" ");
	    int index=Integer.parseInt(str[0]);
	    //System.out.println(index);
	    state.get(index).add(s);
	}
       

	//input string
	String input=in.nextLine();
	while(in.hasNextLine()){
	    input=input+" "+in.nextLine();
	}	
	//System.out.println(input);

	String inputArray[]=input.split(" ");
	ArrayList<String> result=new ArrayList<String>();
	Stack symStack=new Stack();
	Stack stateStack=new Stack();
	symStack.push(inputArray[0]);
	String s[]=state.get(0).get(0).split(" ");
	stateStack.push(s[3]);
	
	for(int i=1;i<inputArray.length;i++){
	    //System.out.println(inputArray[i]);
	    ArrayList<String> tempState=state.get(Integer.parseInt(stateStack.peek().toString()));
	    String p=stackOP(tempState,inputArray[i],"reduce"); //production function if it's not null
	    while(p!=null){
		int n=Integer.parseInt(p);
		String s1[]=production[n].split(" ");
		result.add(production[n]);
		for(int c=s1.length-1;c>=1;c--){
		    symStack.pop(); //pop LHS
		    stateStack.pop();
		}
		symStack.push(s1[0]); //push RHS
		tempState=state.get(Integer.parseInt(stateStack.peek().toString()));
		stateStack.push(stackOP(tempState,s1[0],"shift")); //push state to stack
		
		tempState=state.get(Integer.parseInt(stateStack.peek().toString()));
		p=stackOP(tempState,inputArray[i],"reduce");
	    }
	    //push
	    symStack.push(inputArray[i]);
	    tempState=state.get(Integer.parseInt(stateStack.peek().toString()));
	    p=stackOP(tempState,inputArray[i],"shift");
	    if(p==null){
		System.err.println("ERROR at "+(i+1));
		System.exit(1);
	    }
	    stateStack.push(p);
	}
	    
	
	ArrayList<String> start=new ArrayList<String>();
	while(!symStack.empty()){
	    start.add(symStack.pop().toString());
	    }
	String arr[]=production[0].split(" ");
	
	boolean flag=true;
	if(start.size()!=arr.length-1){
	    flag=false;
	}
	if(flag){
	    for(int i=0;i<start.size();i++){
		if(!start.get(i).equals(arr[arr.length-1-i])){
		    flag=false;
		}
	    }
	}
	if(flag){
	    for(int i=0;i<result.size();i++){
		System.out.println(result.get(i));
	    }
	    
	    System.out.print(strSymbol+" ");
	    for(int i=start.size()-1;i>=0;i--){
		if(i>=1){
		    System.out.print(start.get(i)+" ");
		}else{
		    System.out.print(start.get(i));
		}
	    }
	    System.out.println();
	}
     
    }

    
    public String stackOP(ArrayList<String> a,String input,String operation){
	for(int i=0;i<a.size();i++){
	    String s[]=a.get(i).split(" ");
	    if(s[2].equals(operation)&&s[1].equals(input)){
		return s[3];
	    }
	}
	return null;
    }
}
