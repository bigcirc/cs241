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
;;----------------initialize c to NULL-------------
sw $11, -8($29)
sub $30, $30, $4
;;----------------initialize d to NULL-------------
sw $11, -12($29)
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
.word 10
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
;;---------------Assignment----------------
lw $3, -8($29)
;;----->check for NULL
beq $3, $11, NULL2
add $6, $3, $0
lis $3
.word 2
sw $3, 0($6)
NULL2:
;;---------------Assignment----------------
;;------->PLUS<-------
lw $3, -8($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
lw $5, 0($30)
add $30, $30, $4
;;----->check for NULL
beq $5, $11, NULL3
mult $3, $4
mflo $3
add $3, $5, $3
NULL3:
;;----->check for NULL
beq $3, $11, NULL4
add $6, $3, $0
lis $3
.word 3
sw $3, 0($6)
NULL4:
;;---------------Assignment----------------
;;------->PLUS<-------
lw $3, -8($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 2
lw $5, 0($30)
add $30, $30, $4
;;----->check for NULL
beq $5, $11, NULL5
mult $3, $4
mflo $3
add $3, $5, $3
NULL5:
;;----->check for NULL
beq $3, $11, NULL6
add $6, $3, $0
lis $3
.word 5
sw $3, 0($6)
NULL6:
;;---------------Assignment----------------
;;------->PLUS<-------
lw $3, -8($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 3
lw $5, 0($30)
add $30, $30, $4
;;----->check for NULL
beq $5, $11, NULL7
mult $3, $4
mflo $3
add $3, $5, $3
NULL7:
;;----->check for NULL
beq $3, $11, NULL8
add $6, $3, $0
lis $3
.word 7
sw $3, 0($6)
NULL8:
;;---------------Assignment----------------
;;------->PLUS<-------
lw $3, -8($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 4
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
.word 11
sw $3, 0($6)
NULL10:
;;---------------Assignment----------------
lis $3
.word -12
add $3, $29, $3
;;----->check for NULL
beq $3, $11, NULL11
add $6, $3, $0
lw $3, -8($29)
sw $3, 0($6)
NULL11:
;;---------------->PRINT<----------------
lw $3, -12($29)
;;----->check for NULL
beq $3, $11, NULL12
lw $3, 0($3)
NULL12:
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
;;---------------->PRINT<----------------
;;------->PLUS<-------
lw $3, -12($29)
;;----->check for NULL
beq $3, $11, NULL13
lw $3, 0($3)
NULL13:
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 2
lw $5, 0($30)
add $30, $30, $4
add $3, $5, $3
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
lw $31, 0($30)
add $30, $30, $4
;;---------->Return
lis $3
.word 0
add$30, $29, $4
jr $31
