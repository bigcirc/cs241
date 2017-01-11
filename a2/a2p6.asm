add $5,$0,$0
lis $28
.word 0xffff000c
lis $27
.word 10
lis $26
.word 48
lis $25
.word 45			;'-'
lis $4
.word 4
lis $6
.word 1
lis $12
.word 10
slt $3,$1,$0
beq $3,$0,go
sw $25,0($28)
sub $1,$0,$1
go:
divu $1,$27
mflo $1
mfhi $8
sw $8,-4($30)
sub $30,$30,$4
add $5,$5,$6
bne $1,$0,go
loop:
add $30,$30,$4
lw $9,-4($30)
add $9,$9,$26
sw $9,0($28)
sub $5,$5,$6
bne $5,$0,loop
sw $12,0($28)
jr $31