import java.util.*;
import java.io.*;
/**
 * Starter code for CS241 assignments 9-11 for Spring 2011.
 * 
 * Based on Scheme code by Gord Cormack. Java translation by Ondrej Lhotak.
 * 
 * Version 20081105.1
 *
 * Modified June 30, 2011 by Brad Lushman
 */
public class WLPPGen {
    Scanner in = new Scanner(System.in);

    // The set of terminal symbols in the WLPP grammar.
    Set<String> terminals = new HashSet<String>(Arrays.asList("BOF", "BECOMES", 
         "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF", "INT", "LBRACE", 
         "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN",
         "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE",
         "AMP", "LBRACK", "RBRACK", "NEW", "DELETE", "NULL"));
	
	int off; //offset
    List<String> symbols; //symbol table
	List<String> offsets; //variable offset table
	List<String> code;    //the code
	
    // Data structure for storing the parse tree.
    public class Tree {
        List<String> rule;

        ArrayList<Tree> children = new ArrayList<Tree>();

        // Does this node's rule match otherRule?
        boolean matches(String otherRule) {
            return tokenize(otherRule).equals(rule);
        }
    }

    // Divide a string into a list of tokens.
    List<String> tokenize(String line) {
        List<String> ret = new ArrayList<String>();
        Scanner sc = new Scanner(line);
        while (sc.hasNext()) {
            ret.add(sc.next());
        }
        return ret;
    }

    // Read and return wlppi parse tree
    Tree readParse(String lhs) {
        String line = in.nextLine();
        List<String> tokens = tokenize(line);
        Tree ret = new Tree();
        ret.rule = tokens;
        if (!terminals.contains(lhs)) {
            Scanner sc = new Scanner(line);
            sc.next(); // discard lhs
            while (sc.hasNext()) {
                String s = sc.next();
                ret.children.add(readParse(s));
            }
        }
        return ret;
    }

    // Compute symbols defined in t
    List<String> genSymbols(Tree t) {
	int len=t.rule.size();
       
	if(len==3){
	    String s=t.rule.get(0)+" "+t.rule.get(1)+" "+t.rule.get(2);
	    
	    if(s.equals("dcl type ID")){
		//System.out.println(s);
		String str="";
		for(int i=0;i<t.children.size();i++){
		    Tree child=t.children.get(i);
		    if(!child.rule.get(0).equals("type")){
			//System.out.println(child.rule.get(1));
			str=str+child.rule.get(1);
		    }
		    if(child.children.size()>0){
			Tree ch=child.children.get(0);
			//System.out.print(ch.rule.get(1)+" ");
			str=str+ch.rule.get(1);
			if(child.children.size()==2){
			    ch=child.children.get(1);
			    str=str+ch.rule.get(1);
			}
			str=str+" ";
		    }
		}
		String strArr[]=str.split(" ");
		if(checkDup(strArr[1],symbols)){
		    bail("Duplicate");
		}
		symbols.add(strArr[1]+" "+strArr[0]);
		
		//add to code 
		if(off==0){
			code.add("sw $1,"+off+"($30)");
		}else if(off==-4){
			code.add("sw $2,"+off+"($30)");
		}else{
			code.add("sw $0,"+off+"($30)");
		}
		
		offsets.add(strArr[1]+" "+off); //offset table
		off=off-4;
	    }
	}
	if(t.rule.size()==2&&t.rule.get(0).equals("ID")&&!checkDup(t.rule.get(1),symbols)){
	    bail("Not Defined");
	}
	for(int i=0;i<t.children.size();i++){
	    genSymbols(t.children.get(i));
	}
    
        return null;
    }
    
    boolean checkDup(String s,List<String> str){
	for(int i=0;i<str.size();i++){
	    String a[]=str.get(i).split(" ");
	    if(a[0].equals(s)){
		return true;
	    }
	}
	return false;
    }

	//check syntax error
    String checkSyntax(Tree t){
	int len=t.rule.size();
	String type="";
	List<String> r=t.rule; //list of rule tokens
	
	/*
	for(int i=0;i<len;i++){
		System.out.print(t.rule.get(i)+" ");
	}
	System.out.println();
	*/
	
	if(r.size()==2&&r.get(0).equals("expr")&&r.get(1).equals("term")){
	    type=checkSyntax(t.children.get(0));
		
	}else if(r.size()==4&&r.get(0).equals("expr")&&r.get(1).equals("expr")&&r.get(2).equals("PLUS")&&r.get(3).equals("term")){ //plus
	    String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
		//System.out.println(type1+" "+type2);
		
	    if(type1.equals("int")&&type2.equals("int")){
		type="int";
	    }else if(type1.equals("int*")&&type2.equals("int")){
		type="int*";
	    }else if(type1.equals("int")&&type2.equals("int*")){
		type="int*";
	    }else{
			bail(" ");
		}
	}else if(r.size()==4&&r.get(0).equals("expr")&&r.get(1).equals("expr")&&r.get(2).equals("MINUS")&&r.get(3).equals("term")){
	    String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
		//System.out.println(type1+" "+type2);
		
	    if(type1.equals("int")&&type2.equals("int")){
		type="int";
	    }else if(type1.equals("int*")&&type2.equals("int")){
		type="int*";
	    }else if(type1.equals("int*")&&type2.equals("int*")){
		type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==2&&r.get(0).equals("term")&&r.get(1).equals("factor")){
		type=checkSyntax(t.children.get(0));
	}else if(r.size()==4&&r.get(0).equals("term")&&r.get(1).equals("term")&&r.get(2).equals("STAR")&&r.get(3).equals("factor")){
		String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
	    if(type1.equals("int")&&type2.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==4&&r.get(0).equals("term")&&r.get(1).equals("term")&&r.get(2).equals("SLASH")&&r.get(3).equals("factor")){
		String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
	    if(type1.equals("int")&&type2.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==4&&r.get(0).equals("term")&&r.get(1).equals("term")&&r.get(2).equals("PCT")&&r.get(3).equals("factor")){
		String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
	    if(type1.equals("int")&&type2.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==2&&r.get(0).equals("factor")&&r.get(1).equals("ID")){
		type=checkSyntax(t.children.get(0));
	}else if(r.size()==2&&r.get(0).equals("factor")&&r.get(1).equals("NUM")){
		type="int";
	}else if(r.size()==2&&r.get(0).equals("factor")&&r.get(1).equals("NULL")){
		type="int*";
	}else if(r.size()==4&&r.get(0).equals("factor")&&r.get(1).equals("LPAREN")&&r.get(2).equals("expr")&&r.get(3).equals("RPAREN")){
		type=checkSyntax(t.children.get(1));
		
		//bail(" ");
	}else if(r.size()==3&&r.get(0).equals("factor")&&r.get(1).equals("AMP")&&r.get(2).equals("lvalue")){
		String type1=checkSyntax(t.children.get(1));
		if(type1.equals("int")){
			type="int*";
		}else{
			bail(" ");
		}
	}else if(r.size()==3&&r.get(0).equals("factor")&&r.get(1).equals("STAR")&&r.get(2).equals("factor")){
		String type1=checkSyntax(t.children.get(1));
		//System.out.println(type1);
		if(type1.equals("int*")){
			type="int";
		}else{	
			bail(" ");
		}
	}else if(r.size()==6&&r.get(0).equals("factor")&&r.get(1).equals("NEW")&&r.get(2).equals("INT")&&r.get(3).equals("LBRACK")&&r.get(4).equals("expr")&&r.get(5).equals("RBRACK")){
		String type1=checkSyntax(t.children.get(3));
		//System.out.println(type1);
		if(type1.equals("int")){
			type="int*";
		}else{	
			bail(" ");
		}
	}else if(r.size()==2&&r.get(0).equals("lvalue")&&r.get(1).equals("ID")){
		String type1=checkSyntax(t.children.get(0));
		type=type1;
	}else if(r.size()==3&&r.get(0).equals("lvalue")&&r.get(1).equals("STAR")&&r.get(2).equals("factor")){
		String type1=checkSyntax(t.children.get(1));
		if(type1.equals("int*")){
			type="int";
		}else{
			bail(" ");
		}
	}else if(r.size()==4&&r.get(0).equals("LPAREN")&&r.get(1).equals("lvalue")&&r.get(2).equals("RPAREN")){
		String type1=checkSyntax(t.children.get(1));
		type=type1;
	}else if(r.size()==2&&r.get(0).equals("ID")){
		type=typeCheck(r.get(1),symbols);
	}else if(r.size()==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&(r.get(2).equals("EQ")||r.get(2).equals("LT")||r.get(2).equals("NE")||r.get(2).equals("LE")||r.get(2).equals("GE")||r.get(2).equals("GT"))&&r.get(3).equals("expr")){
		String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
	    if(type1.equals(type2)){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==8&&r.get(0).equals("statement")&&r.get(1).equals("WHILE")&&r.get(2).equals("LPAREN")&&r.get(3).equals("test")&&r.get(4).equals("RPAREN")&&r.get(5).equals("statements")&&r.get(6).equals("RBRACE")){
		String type1=checkSyntax(t.children.get(2));
	    String type2=checkSyntax(t.children.get(5));
	    if(type1.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==12&&r.get(0).equals("statement")&&r.get(1).equals("LPAREN")&&r.get(2).equals("test")&&r.get(3).equals("RPAREN")&&r.get(4).equals("LBRACE")&&r.get(5).equals("statements")&&r.get(6).equals("RBRACE")&&r.get(7).equals("ELSE")&&r.get(8).equals("LBRACE")&&r.get(9).equals("statements")&&r.get(10).equals("RBRACE")){
		String type1=checkSyntax(t.children.get(2));
	    String type2=checkSyntax(t.children.get(5));
		String type3=checkSyntax(t.children.get(9));
	    if(type1.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==6&&r.get(0).equals("statement")&&r.get(1).equals("PRINTLN")&&r.get(2).equals("LPAREN")&&r.get(3).equals("expr")&&r.get(4).equals("RPAREN")&&r.get(5).equals("SEMI")){
		String type1=checkSyntax(t.children.get(2));
	    
	    if(type1.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==6&&r.get(0).equals("statement")&&r.get(1).equals("DELETE")&&r.get(2).equals("LBRACK")&&r.get(3).equals("RBRACK")&&r.get(4).equals("expr")&&r.get(5).equals("SEMI")){
		String type1=checkSyntax(t.children.get(3));
	    
	    if(type1.equals("int*")){
			type="int*";
	    }else{
			bail(" ");
		}
	}else if(r.size()==5&&r.get(0).equals("statement")&&r.get(1).equals("lvalue")&&r.get(2).equals("BECOMES")&&r.get(3).equals("expr")&&r.get(4).equals("SEMI")){
		String type1=checkSyntax(t.children.get(0));
	    String type2=checkSyntax(t.children.get(2));
	    if(type1.equals(type2)){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==6&&r.get(0).equals("dcls")&&r.get(1).equals("dcls")&&r.get(2).equals("dcl")&&r.get(3).equals("BECOMES")&&r.get(4).equals("NUM")&&r.get(5).equals("SEMI")){
		checkSyntax(t.children.get(0));
		String type1=checkSyntax(t.children.get(1));
	    String type2=checkSyntax(t.children.get(3));
		//System.out.println(type1+" "+type2);
	    if(type1.equals("int")&&type2.equals("int")){
			type="int";
	    }else{
			bail(" ");
		}
	}else if(r.size()==6&&r.get(0).equals("dcls")&&r.get(1).equals("dcls")&&r.get(2).equals("dcl")&&r.get(3).equals("BECOMES")&&r.get(4).equals("NULL")&&r.get(5).equals("SEMI")){
		checkSyntax(t.children.get(0));
		String type1=checkSyntax(t.children.get(1));
	    String type2=checkSyntax(t.children.get(3));
		//System.out.println(type1+" "+type2);
		
	    if(type1.equals("int*")&&type2.equals("int*")){
			type="int*";
	    }else{
			bail(" ");
		}
	}else if(r.size()==2&&r.get(0).equals("NULL")&&r.get(1).equals("NULL")){
		type="int*";
	}else if(r.size()==2&&r.get(0).equals("NUM")){
		type="int";
	}else{
		int count=0;
		for(int i=0;i<t.children.size();i++){
			String s=t.children.get(i).rule.get(0);
			type=checkSyntax(t.children.get(i));
			
			if(t.rule.get(0).equals("procedure")&&s.equals("dcl")){
				count++;
			}
		
			if(t.rule.get(0).equals("procedure")&&s.equals("dcl")&&count==2){
				if(type.equals("int*")){
					bail(" ");
				}
			}
			
			if(i>0&&t.children.get(i-1).rule.get(0).equals("RETURN")){
				if(type.equals("int*")){
					bail(" ");
				}
			}
			
		}
	}

	return type;
    }
	
    //return the corresponding type for given string ID
    String typeCheck(String s,List<String> str){
	for(int i=0;i<str.size();i++){
	    String a[]=str.get(i).split(" ");
	    if(a[0].equals(s)){
		return a[1];
	    }
	}
	return " ";
    }
	
	//offset check
	String offsetCheck(String s,List<String> str){
	for(int i=0;i<str.size();i++){
	    String a[]=str.get(i).split(" ");
	    if(a[0].equals(s)){
		return a[1];
	    }
	}
	return " ";
    }
	
    // Print an error message and exit the program.
    void bail(String msg) {
        System.err.println("ERROR: " + msg);
        System.exit(0);
    }

    // Generate the code for the parse tree t.
    String genCode(Tree t) {
		int len=t.rule.size();
		List<String> r=t.rule;
		ArrayList<Tree> ch=t.children;
		
		//System.err.println(r.get(0)+" "+r.get(1));
		
		if(t.rule.get(0).equals("ID")){
			off=off-4; //update offset
			String s=offsetCheck(t.rule.get(1),offsets);
			code.add("lw $3,"+s+"($29)");
			
			return t.rule.get(1);
		}else if(t.rule.get(0).equals("NUM")){
			off=off-4; //update offset
			code.add("lis $3");
			code.add(".word "+r.get(1));
			
		}else if(t.rule.get(0).equals("factor")&&t.rule.get(1).equals("NULL")){
			code.add("add $3,$0,$0");
		}else if(r.get(0).equals("expr")&&r.get(1).equals("term")){
			return genCode(ch.get(0));
		}else if(r.get(0).equals("term")&&r.get(1).equals("factor")){
			return genCode(ch.get(0));
		}else if(r.get(0).equals("expr")&&r.get(1).equals("expr")&&r.get(2).equals("PLUS")&&r.get(3).equals("term")){
			
			off=off-4; //update offset
			String sy1=genCode(ch.get(0));
			String s1=typeCheck(sy1,symbols);
			
			int f=off;
			code.add("sw $3,"+off+"($30)");
			String sy2=genCode(ch.get(2));
			String s2=typeCheck(sy2,symbols);
			
			off=f;
			code.add("lw $4,"+off+"($30)");
			
			
			if(s1.equals("int*")){
				code.add("lis $14");
				code.add(".word 4");
				code.add("mult $3,$14");
				code.add("mflo $3");
				//System.err.println("s1");
			}else if(s2.equals("int*")){
				code.add("lis $14");
				code.add(".word 4");
				code.add("mult $4,$14");
				code.add("mflo $4");
				//System.err.println("s2");
			}
			
			code.add("add $3,$3,$4");
			
			if(s1.equals("int*")){
				return sy1;
			}if(s2.equals("int*")){
				return sy2;
			}
		}else if(r.get(0).equals("expr")&&r.get(1).equals("expr")&&r.get(2).equals("MINUS")&&r.get(3).equals("term")){
			off=off-4; //update offset
			String sy1=genCode(ch.get(0));
			String s1=typeCheck(sy1,symbols);
			
			int f=off;
			code.add("sw $3,"+off+"($30)");
			String sy2=genCode(ch.get(2));
			String s2=typeCheck(sy2,symbols);
			
			off=f;
			code.add("lw $4,"+off+"($30)");
			
			if(s1.equals("int*")&&!s2.equals("int*")){
				code.add("lis $14");
				code.add(".word 4");
				code.add("mult $3,$14");
				code.add("mflo $3");
				
				code.add("sub $3,$4,$3");
				return sy1;
			}else if(s1.equals("int*")&&s2.equals("int*")){
				code.add("lis $14");
				code.add(".word 4");
				code.add("sub $3,$4,$3");
				code.add("div $3,$14");
				code.add("mflo $3");
			}else{
				code.add("sub $3,$4,$3");
			}
		}else if(r.get(0).equals("term")&&r.get(1).equals("term")&&r.get(2).equals("STAR")&&r.get(3).equals("factor")){
			off=off-4; //update offset
			genCode(ch.get(0));
			int f=off;
			code.add("sw $3,"+off+"($30)");
			genCode(ch.get(2));
			off=f;
			code.add("lw $4,"+off+"($30)");
			code.add("mult $3,$4");
			code.add("mflo $3");
		}else if(r.get(0).equals("term")&&r.get(1).equals("term")&&r.get(2).equals("SLASH")&&r.get(3).equals("factor")){
			off=off-4; //update offset
			genCode(ch.get(0));
			int f=off;
			code.add("sw $3,"+off+"($30)");
			genCode(ch.get(2));
			off=f;
			code.add("lw $4,"+off+"($30)");
			code.add("div $4,$3");
			code.add("mflo $3");
		}else if(r.get(0).equals("term")&&r.get(1).equals("term")&&r.get(2).equals("PCT")&&r.get(3).equals("factor")){
			off=off-4; //update offset
			genCode(ch.get(0));
			int f=off;
			code.add("sw $3,"+off+"($30)");
			genCode(ch.get(2));
			off=f;
			code.add("lw $4,"+off+"($30)");
			code.add("div $4,$3");
			code.add("mfhi $3");
		}else if(r.get(0).equals("statement")&&r.get(1).equals("PRINTLN")){
			genCode(ch.get(2));
			code.add("add $1,$3,$0");
			code.add("lis $8 ;print function address");
			code.add(".word print");
			code.add("sw $31,-4($30) ;push");
			code.add("lis $9");
			code.add(".word 4");
			code.add("sub $30,$30,$9");
			code.add("jalr $8 ;execute helper function");
			code.add("lw $31,0($30)");
			code.add("lis $9");
			code.add(".word 4");
			code.add("add $30,$30,$9");
		}else if(len>3&&r.get(0).equals("dcls")&&r.get(1).equals("dcls")&&r.get(2).equals("dcl")&&r.get(4).equals("NUM")){
			genCode(ch.get(0)); //recursion on dcls
			Tree c=ch.get(1);
			String s=offsetCheck(c.children.get(1).rule.get(1),offsets);
			String n=ch.get(3).rule.get(1);
			code.add("lis $6");
			code.add(".word "+n);
			code.add("sw $6,"+s+"($29)");
		}else if(r.get(0).equals("lvalue")&&r.get(1).equals("ID")){
			return genCode(ch.get(0));
		
		}else if(len==4&&r.get(0).equals("lvalue")&&r.get(1).equals("LPAREN")&&r.get(2).equals("lvalue")&&r.get(3).equals("RPAREN")){
			return genCode(ch.get(1));
		}else if(len>3&&r.get(0).equals("statement")&&r.get(1).equals("lvalue")&&r.get(2).equals("BECOMES")){
			code.add(" ");
			String sym1=genCode(ch.get(0));
			String s1=typeCheck(sym1,symbols);
			code.add("add $18,$3,$0");
			String sym2=genCode(ch.get(2));
			String s2=typeCheck(sym2,symbols);
			
			//System.err.println(s1+" "+s2);
			if(ch.get(2).children.get(0).children.get(0).rule.get(1).equals("AMP")){
				s2="int*";
			}
			
			if(ch.get(2).children.get(0).children.get(0).rule.get(1).equals("NEW")){
				s2="int*";
				
			}
			System.err.println(s1+" "+s2);
			if(s1.equals("int*")&&!s2.equals("int*")){
				
				code.add("sw $3,0($18)");
			}else{
				s1=offsetCheck(sym1,offsets);
				//System.err.println(s1);
				code.add("sw $3,"+s1+"($29)");
			}
			code.add(" ");
		}else if(len==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&r.get(2).equals("LT")&&r.get(1).equals("expr")){
			//less than
			String symbol=genCode(ch.get(0));
			String s=typeCheck(symbol,symbols);
			if(ch.get(0).children.get(0).children.get(0).rule.get(1).equals("AMP")){
				s="int*";
			}
			//System.err.println(s);
			code.add("add $10,$3,$0 ;expr1");
			genCode(ch.get(2));
			code.add("add $11,$3,$0 ;expr2");
			if(s.equals("int*")){
				code.add("sltu $12,$10,$11 ;test result");
			}else{
				code.add("slt $12,$10,$11 ;test result");
			}
			
		}else if(len==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&r.get(2).equals("EQ")&&r.get(1).equals("expr")){
			//equal to
			genCode(ch.get(0));
			code.add("add $10,$3,$0 ;expr1");
			genCode(ch.get(2));
			code.add("add $11,$3,$0 ;expr2");
			
			code.add("beq $10,$11,2");
			code.add("add $12,$0,$0 ;$12=0");
			code.add("bne $10,$11,3");
	
			code.add("lis $1");
			code.add(".word 1");
			code.add("add $12,$1,$0 ;$12=1");
		}else if(len==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&r.get(2).equals("NE")&&r.get(1).equals("expr")){
			//not equal
			genCode(ch.get(0));
			code.add("add $10,$3,$0 ;expr1");
			genCode(ch.get(2));
			code.add("add $11,$3,$0 ;expr2");
			
			code.add("bne $10,$11,2");
			code.add("add $12,$0,$0 ;$12=0");
			code.add("beq $10,$11,3");
	
			code.add("lis $1");
			code.add(".word 1");
			code.add("add $12,$1,$0 ;$12=1");
		}else if(len==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&r.get(2).equals("LE")&&r.get(1).equals("expr")){
			//less than or equal
			String symbol=genCode(ch.get(0));
			String s=typeCheck(symbol,symbols);
			if(ch.get(0).children.get(0).children.get(0).rule.get(1).equals("AMP")){
				s="int*";
			}
			code.add("add $10,$3,$0 ;expr1");
			genCode(ch.get(2));
			code.add("add $11,$3,$0 ;expr2");
			
			if(s.equals("int*")){
				code.add("sltu $12,$10,$11 ;test result");
			}else{
				code.add("slt $12,$10,$11 ;test result");
			}
			
			code.add("bne $12,$0,6");
			
			code.add("beq $10,$11,2");
			code.add("add $12,$0,$0 ;$12=0");
			code.add("bne $10,$11,3");
	
			code.add("lis $1");
			code.add(".word 1");
			code.add("add $12,$1,$0 ;$12=1");
			
		}else if(len==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&r.get(2).equals("GE")&&r.get(1).equals("expr")){
			//greater than or equal
			String symbol=genCode(ch.get(0));
			String s=typeCheck(symbol,symbols);
			if(ch.get(0).children.get(0).children.get(0).rule.get(1).equals("AMP")){
				s="int*";
			}
			code.add("add $10,$3,$0 ;expr1");
			genCode(ch.get(2));
			code.add("add $11,$3,$0 ;expr2");
			
			if(s.equals("int*")){
				code.add("sltu $12,$11,$10 ;test result");
			}else{
				code.add("slt $12,$11,$10 ;test result");
			}
			
			code.add("bne $12,$0,6");
			
			code.add("beq $10,$11,2");
			code.add("add $12,$0,$0 ;$12=0");
			code.add("bne $10,$11,3");
	
			code.add("lis $1");
			code.add(".word 1");
			code.add("add $12,$1,$0 ;$12=1");
		}else if(len==4&&r.get(0).equals("test")&&r.get(1).equals("expr")&&r.get(2).equals("GT")&&r.get(1).equals("expr")){
			//greater than
			String symbol=genCode(ch.get(0));
			String s=typeCheck(symbol,symbols);
			if(ch.get(0).children.get(0).children.get(0).rule.get(1).equals("AMP")){
				s="int*";
			}
			code.add("add $10,$3,$0 ;expr1");
			genCode(ch.get(2));
			code.add("add $11,$3,$0 ;expr2");
			
			if(s.equals("int*")){
				code.add("sltu $12,$11,$10 ;test result");
			}else{
				code.add("slt $12,$11,$10 ;test result");
			}
			
		}else if(len>2&&r.get(0).equals("statement")&&r.get(1).equals("WHILE")){
			//while loop
			
			int i=0-off;
			
			genCode(ch.get(2)); //get code for test
			code.add("begin"+i+":");
			
			code.add("lis $1");
			code.add(".word 1");
			code.add("bne $12,$1,end"+i+";branch on not equal");
			
			genCode(ch.get(5));//code for statement
			genCode(ch.get(2));
			
			code.add("beq $0,$0,begin"+i+";return to begin");
			code.add("end"+i+":");
			
		}else if(len>2&&r.get(0).equals("statement")&&r.get(1).equals("IF")){
			int i=0-off;
			genCode(ch.get(2));
			
			code.add("lis $1");
			code.add(".word 1");
			code.add("bne $12,$1,cond"+i+";branch on not equal");
			
			genCode(ch.get(5));

			code.add("beq $0,$0,condEnd"+i+";branch on equal");
			code.add("cond"+i+":");
			
			genCode(ch.get(9));
			code.add("condEnd"+i+":");
			
		}else if(len==6&&r.get(0).equals("dcls")&&r.get(1).equals("dcls")&&r.get(2).equals("dcl")&&r.get(3).equals("BECOMES")&&r.get(4).equals("NULL")&&r.get(5).equals("SEMI")){
			genCode(ch.get(0)); //recursion on dcls
			
		}else if(len==3&&r.get(0).equals("factor")&&r.get(1).equals("AMP")&&r.get(2).equals("lvalue")){
			String str=ch.get(1).rule.get(1);
			
			if(str.equals("STAR")){
				return genCode(ch.get(1).children.get(1));
			}else{
				String symbol=genCode(ch.get(1));
				String s=offsetCheck(symbol,offsets);
				code.add("lis $1");			
				code.add(".word "+s);
				code.add("add $3,$29,$1");
			}
			
		}else if(len==3&&r.get(0).equals("factor")&&r.get(1).equals("STAR")&&r.get(2).equals("factor")){
			String s=genCode(ch.get(1));
			code.add("lw $3,0($3)");
			//return s;
		}else if(len==2&&r.get(0).equals("factor")&&r.get(1).equals("ID")){
			return genCode(ch.get(0));
		}else if(len==3&&r.get(0).equals("lvalue")&&r.get(1).equals("STAR")&&r.get(2).equals("factor")){
			String s=genCode(ch.get(1));
			//code.add("lw $3,0($3)");
			return s;
		}else if(len==4&&r.get(0).equals("factor")&&r.get(1).equals("LPAREN")&&r.get(2).equals("expr")&&r.get(3).equals("RPAREN")){
			return genCode(ch.get(1));
		}else if(len==6&&r.get(0).equals("factor")&&r.get(1).equals("NEW")&&r.get(4).equals("expr")){
			
			String s=genCode(ch.get(3));
			//call function new
			
			code.add("add $1,$3,$0");
			code.add("lis $8 ;  function new address");
			code.add(".word new");
			code.add("sw $31,-4($30) ;push");
			code.add("lis $9");
			code.add(".word 4");
			code.add("sub $30,$30,$9");
			code.add("jalr $8 ;execute helper function");
			code.add("lw $31,0($30)");
			code.add("lis $9");
			code.add(".word 4");
			code.add("add $30,$30,$9");
			
			return s;
		}else if(len==6&&r.get(0).equals("statement")&&r.get(1).equals("DELETE")&&r.get(4).equals("expr")){
			
			String s=genCode(ch.get(3));
			s=offsetCheck(s,offsets);
			
			//call function delete
			
			code.add("add $1,$3,$0");
			code.add("lis $8 ;  function delete address");
			code.add(".word delete");
			code.add("sw $31,-4($30) ;push");
			code.add("lis $9");
			code.add(".word 4");
			code.add("sub $30,$30,$9");
			code.add("jalr $8 ;execute helper function");
			code.add("lw $31,0($30)");
			code.add("lis $9");
			code.add(".word 4");
			code.add("add $30,$30,$9");
			
		}else if(r.get(0).equals("procedure")){
			String ty=symbols.get(0).split(" ")[1];
			
			if(ty.equals("int*")){
				code.add("lw $2,-4($29)");
			}else{
				code.add("add $2,$0,$0");
			}
			//code.add("add $1,$3,$0");
			code.add("lis $8 ; init function address");
			code.add(".word init");
			code.add("sw $31,-4($30) ;push");
			code.add("lis $9");
			code.add(".word 4");
			code.add("sub $30,$30,$9");
			code.add("jalr $8 ;execute helper function");
			code.add("lw $31,0($30)");
			code.add("lis $9");
			code.add(".word 4");
			code.add("add $30,$30,$9");
			
			genCode(ch.get(8));
			genCode(ch.get(9));
			genCode(ch.get(11));
			code.add("add $30,$29,$0");
			code.add("jr $31");
	
		}else{
			for(int i=0;i<ch.size();i++){	
				genCode(ch.get(i));
			}
		}

		return " ";
    }

    // Main program
    public static final void main(String args[]) {
        new WLPPGen().go();
    }

    public void go() {
	//add code
	off=0;
	code=new ArrayList<String>();
	offsets=new ArrayList<String>();
	code.add(".import print");
	code.add(".import init");
	code.add(".import new");
	code.add(".import delete");
	code.add(".import printFreeList");
	
	code.add("lis $4");
	code.add(".word 4");
	code.add("sub $29,$30,$4");
	code.add("sub $30,$30,$4");
	
	
	symbols=new ArrayList<String>();
	Tree parseTree = readParse("S");
	genSymbols(parseTree);
	
	off=off+4;
	code.add("lis $4");
	code.add(".word "+off);
	code.add("add $30,$30,$4");
	off=off-4;
	
	for(int i=0;i<symbols.size();i++){
	    System.err.println(symbols.get(i));
	}	
	
	//checkSyntax(parseTree);
	
	genCode(parseTree); //generate code
	/*
	try{
		File file=new File("print.asm");
		BufferedReader reader = new BufferedReader(new FileReader(file));
		String temp=null;

		while((temp=reader.readLine())!=null){
			code.add(temp);
			
		}
	}catch(Exception e){
	
	}
	
	try{
		File file=new File("alloc.asm");
		BufferedReader reader = new BufferedReader(new FileReader(file));
		String temp=null;

		while((temp=reader.readLine())!=null){
			code.add(temp);
			
		}
	}catch(Exception e){
	
	}*/
	
	for(int i=0;i<code.size();i++){
	    System.out.println(code.get(i));
	}	
	
    }
}
