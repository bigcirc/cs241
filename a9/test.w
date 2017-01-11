start BOF procedures EOF
BOF BOF
procedures main
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
INT int
WAIN wain
LPAREN (
dcl type ID
type INT
INT int
ID a
COMMA ,
dcl type ID
type INT
INT int
ID b
RPAREN )
LBRACE {
dcls dcls dcl BECOMES NUM SEMI
dcls
dcl type ID
type INT
INT int
ID x
BECOMES =
NUM 0
SEMI ;
statements statements statement
statements
statement WHILE LPAREN test RPAREN LBRACE statements RBRACE
WHILE while
LPAREN (
test expr LT expr
expr term
term factor
factor ID
ID x
LT <
expr expr PLUS term
expr expr PLUS term
expr term
term factor
factor ID
ID b
PLUS +
term factor
factor ID
ID a
PLUS +
term factor
factor NUM
NUM 5
RPAREN )
LBRACE {
statements statements statement
statements statements statement
statements statements statement
statements
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
IF if
LPAREN (
test expr LT expr
expr term
term factor
factor ID
ID x
LT <
expr expr PLUS term
expr term
term factor
factor ID
ID b
PLUS +
term factor
factor ID
ID a
RPAREN )
LBRACE {
statements statements statement
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr expr PLUS term
expr term
term factor
factor ID
ID x
PLUS +
term factor
factor ID
ID b
RPAREN )
SEMI ;
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
IF if
LPAREN (
test expr GT expr
expr term
term factor
factor ID
ID x
GT >
expr term
term factor
factor ID
ID a
RPAREN )
LBRACE {
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr expr MINUS term
expr term
term factor
factor ID
ID x
MINUS -
term factor
factor ID
ID a
RPAREN )
SEMI ;
RBRACE }
ELSE else
LBRACE {
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr expr MINUS term
expr term
term factor
factor ID
ID x
MINUS -
term factor
factor ID
ID b
RPAREN )
SEMI ;
RBRACE }
RBRACE }
ELSE else
LBRACE {
statements statements statement
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr term
term term STAR factor
term factor
factor ID
ID x
STAR *
factor ID
ID b
RPAREN )
SEMI ;
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
IF if
LPAREN (
test expr GT expr
expr term
term factor
factor ID
ID x
GT >
expr expr PLUS term
expr term
term factor
factor ID
ID b
PLUS +
term factor
factor ID
ID a
RPAREN )
LBRACE {
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr expr PLUS term
expr expr MINUS term
expr term
term factor
factor ID
ID x
MINUS -
term factor
factor ID
ID a
PLUS +
term factor
factor NUM
NUM 5
RPAREN )
SEMI ;
RBRACE }
ELSE else
LBRACE {
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr expr PLUS term
expr expr MINUS term
expr term
term factor
factor ID
ID x
MINUS -
term factor
factor ID
ID b
PLUS +
term factor
factor NUM
NUM 3
RPAREN )
SEMI ;
RBRACE }
RBRACE }
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
IF if
LPAREN (
test expr EQ expr
expr term
term factor
factor ID
ID x
EQ ==
expr term
term factor
factor ID
ID a
RPAREN )
LBRACE {
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr expr MINUS term
expr term
term term STAR factor
term factor
factor ID
ID a
STAR *
factor ID
ID b
MINUS -
term factor
factor ID
ID x
RPAREN )
SEMI ;
RBRACE }
ELSE else
LBRACE {
statements statements statement
statements
statement PRINTLN LPAREN expr RPAREN SEMI
PRINTLN println
LPAREN (
expr term
term factor
factor ID
ID x
RPAREN )
SEMI ;
RBRACE }
statement lvalue BECOMES expr SEMI
lvalue ID
ID x
BECOMES =
expr expr PLUS term
expr term
term factor
factor ID
ID x
PLUS +
term factor
factor NUM
NUM 1
SEMI ;
RBRACE }
RETURN return
expr term
term factor
factor LPAREN expr RPAREN
LPAREN (
expr term
term factor
factor ID
ID x
RPAREN )
SEMI ;
RBRACE }
EOF EOF
