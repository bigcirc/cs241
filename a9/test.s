INT int
WAIN wain
LPAREN (
INT int
ID a
COMMA ,
INT int
ID b
RPAREN )
LBRACE {
INT int
ID x
BECOMES =
NUM 0
SEMI ;
WHILE while
LPAREN (
ID x
LT <
ID b
PLUS +
ID a
PLUS +
NUM 5
RPAREN )
LBRACE {
IF if
LPAREN (
ID x
LT <
ID b
PLUS +
ID a
RPAREN )
LBRACE {
PRINTLN println
LPAREN (
ID x
PLUS +
ID b
RPAREN )
SEMI ;
IF if
LPAREN (
ID x
GT >
ID a
RPAREN )
LBRACE {
PRINTLN println
LPAREN (
ID x
MINUS -
ID a
RPAREN )
SEMI ;
RBRACE }
ELSE else
LBRACE {
PRINTLN println
LPAREN (
ID x
MINUS -
ID b
RPAREN )
SEMI ;
RBRACE }
RBRACE }
ELSE else
LBRACE {
PRINTLN println
LPAREN (
ID x
STAR *
ID b
RPAREN )
SEMI ;
IF if
LPAREN (
ID x
GT >
ID b
PLUS +
ID a
RPAREN )
LBRACE {
PRINTLN println
LPAREN (
ID x
MINUS -
ID a
PLUS +
NUM 5
RPAREN )
SEMI ;
RBRACE }
ELSE else
LBRACE {
PRINTLN println
LPAREN (
ID x
MINUS -
ID b
PLUS +
NUM 3
RPAREN )
SEMI ;
RBRACE }
RBRACE }
IF if
LPAREN (
ID x
EQ ==
ID a
RPAREN )
LBRACE {
PRINTLN println
LPAREN (
ID a
STAR *
ID b
MINUS -
ID x
RPAREN )
SEMI ;
RBRACE }
ELSE else
LBRACE {
PRINTLN println
LPAREN (
ID x
RPAREN )
SEMI ;
RBRACE }
ID x
BECOMES =
ID x
PLUS +
NUM 1
SEMI ;
RBRACE }
RETURN return
LPAREN (
ID x
RPAREN )
SEMI ;
RBRACE }
