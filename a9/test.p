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
test expr GT expr
expr term
term factor
factor ID
ID x
GT >
expr term
term factor
factor NUM
NUM 0
RPAREN )
LBRACE {
statements statements statement
statements
statement lvalue BECOMES expr SEMI
lvalue ID
ID x
BECOMES =
expr expr MINUS term
expr term
term factor
factor ID
ID x
MINUS -
term factor
factor NUM
NUM 1
SEMI ;
RBRACE }
RETURN return
expr term
term factor
factor ID
ID x
SEMI ;
RBRACE }
EOF EOF
