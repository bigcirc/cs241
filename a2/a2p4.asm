lis $4
.word 4
lis $5
.word 1
lw $3,0($1)
sub $2,$2,$5
beq $2,$5,return
bne $2,$5,return1
return:
jr $31
return1:	
add $6,$5,$0	
loop:
mult $4,$6
mflo $10
add $7,$10,$1
lw $7,0($7)
slt $8,$7,$3
beq $8,$5,plus
add $3,$7,$0
plus:
add $6,$6,$5
bne $2,$0,loop
jr $31















