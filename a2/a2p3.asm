beq $2,$0,return
lis $4
.word 4
lis $5
.word 1
sub $2,$2,$5
mult $2,$4
mflo $4
add $1,$4,$1
lw $3,0($1)
jr $31
return:
lis $4
.word 1
sub $3,$0,$4
jr $31
