;;------------->one part of prologue<-------------
.import print
lis $4
.word 4
lis $11
.word 1
sub $29, $30, $4
;;------------------>end of part<-------------------
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
;;------------->another part of prologue<-------------
add $2, $0, $0
.import init
.import new
.import delete
;;---call init
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word init
jalr $5
lw $31, 0($30)
add $30, $30, $4
;;------------------>START OF CODE BODY<-------------------
;;----------------initialize arr to NULL-------------
sw $11, -8($29)
sub $30, $30, $4
;;---------------Assignment----------------
lis $3
.word -8
add $3, $29, $3
;;----->check for NULL
beq $3, $11, NULL0
add $6, $3, $0
;;---------->new
lis $3
.word 40
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word new
jalr $5
lw $31, 0($30)
add $30, $30, $4
;;----->check for NULL
bne $3, $0, NULL1
add $3, $11, $0
NULL1:
sw $3, 0($6)
NULL0:
;;---------->Return
;;------call procedure foo------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 0($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, -8($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 0($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, -4($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 0
add $3, $29, $3
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ffoo
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
add$30, $29, $4
jr $31
;;-------------procedure foo-------------
;;-----Prologue of foo-------
Ffoo:
sub $29, $30, $4
sw $6, -4($30)
sub $30, $30, $4
;;------call procedure f------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ff
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure g------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Fg
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
slt $3, $5, $3
bne $3, $0, true0
beq $0, $0, endif0
true0:
;;---------------->PRINT<----------------
lis $3
.word 0
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
endif0:
;;------call procedure f------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ff
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure g------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Fg
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
slt $3, $3, $5
bne $3, $0, true1
beq $0, $0, endif1
true1:
;;---------------->PRINT<----------------
lis $3
.word 1
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
endif1:
;;------call procedure f------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ff
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure g------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Fg
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
slt $6, $3, $5
slt $7, $5, $3
add $3, $6, $7
sub $3, $11, $3
bne $3, $0, true2
beq $0, $0, endif2
true2:
;;---------------->PRINT<----------------
lis $3
.word 2
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
endif2:
;;------call procedure f------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ff
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure g------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Fg
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
slt $6, $3, $5
slt $7, $5, $3
add $3, $6, $7
bne $3, $0, true3
beq $0, $0, endif3
true3:
;;---------------->PRINT<----------------
lis $3
.word 3
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
endif3:
;;------call procedure f------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ff
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure g------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Fg
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
slt $3, $3, $5
sub $3, $11, $3
bne $3, $0, true4
beq $0, $0, endif4
true4:
;;---------------->PRINT<----------------
lis $3
.word 4
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
endif4:
;;------call procedure f------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ff
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure g------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Fg
jalr $5
lw $5, 0($30)
add $30, $30, $4
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
slt $3, $5, $3
sub $3, $11, $3
bne $3, $0, true5
beq $0, $0, endif5
true5:
;;---------------->PRINT<----------------
lis $3
.word 5
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
endif5:
;;---------------Assignment----------------
;;------->PLUS<-------
lw $3, 16($29)
sw $3, -4($30)
sub $30, $30, $4
;;------call procedure bar------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word Fbar
jalr $5
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
;;----->check for NULL
beq $5, $11, NULL2
mult $3, $4
mflo $3
add $3, $5, $3
NULL2:
;;----->check for NULL
beq $3, $11, NULL3
add $6, $3, $0
;;------call procedure bar------
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word Fbar
jalr $5
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
sw $3, 0($6)
NULL3:
;;----foo return----
;;------->PLUS<-------
;;------->PLUS<-------
;;------->PLUS<-------
;;------->PLUS<-------
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
;;------->PLUS<-------
lw $3, 16($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 10
lw $5, 0($30)
add $30, $30, $4
;;----->check for NULL
beq $5, $11, NULL4
mult $3, $4
mflo $3
add $3, $5, $3
NULL4:
;;----->check for NULL
beq $3, $11, NULL5
lw $3, 0($3)
NULL5:
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
sw $3, -4($30)
sub $30, $30, $4
lw $3, 12($29)
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
sw $3, -4($30)
sub $30, $30, $4
lw $3, 8($29)
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
sw $3, -4($30)
sub $30, $30, $4
lw $3, 4($29)
;;----->check for NULL
beq $3, $11, NULL6
lw $3, 0($3)
NULL6:
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
lw $6, 0($30)
add $30, $30, $4
add $30, $29, $4
jr $31
;;-------------procedure bar-------------
;;-----Prologue of bar-------
Fbar:
sub $29, $30, $4
;;----------------initialize arr to NULL-------------
sw $11, 0($29)
sub $30, $30, $4
sw $6, -4($30)
sub $30, $30, $4
;;---------------Assignment----------------
lis $3
.word 0
add $3, $29, $3
;;----->check for NULL
beq $3, $11, NULL7
add $6, $3, $0
;;---------->new
lis $3
.word 5
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word new
jalr $5
lw $31, 0($30)
add $30, $30, $4
;;----->check for NULL
bne $3, $0, NULL8
add $3, $11, $0
NULL8:
sw $3, 0($6)
NULL7:
;;---------------Assignment----------------
;;------->PLUS<-------
lw $3, 0($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 3
lw $5, 0($30)
add $30, $30, $4
;;----->check for NULL
beq $5, $11, NULL9
mult $3, $4
mflo $3
add $3, $5, $3
NULL9:
;;----->check for NULL
beq $3, $11, NULL10
add $6, $3, $0
lis $3
.word 2
sw $3, 0($6)
NULL10:
;;--------------->delete
lw $3, 0($29)
;;----->check for NULL
beq $3, $11, NULL11
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word delete
jalr $5
lw $31, 0($30)
add $30, $30, $4
NULL11:
;;----bar return----
lis $3
.word 10
lw $6, 0($30)
add $30, $30, $4
add $30, $29, $4
jr $31
;;-------------procedure g-------------
;;-----Prologue of g-------
Fg:
sub $29, $30, $4
sw $6, -4($30)
sub $30, $30, $4
;;----g return----
;;------->PLUS<-------
lw $3, 4($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 9
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
lw $6, 0($30)
add $30, $30, $4
add $30, $29, $4
jr $31
;;-------------procedure f-------------
;;-----Prologue of f-------
Ff:
sub $29, $30, $4
sw $6, -4($30)
sub $30, $30, $4
;;----f return----
;;------->PLUS<-------
lw $3, 4($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 4
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
lw $6, 0($30)
add $30, $30, $4
add $30, $29, $4
jr $31
