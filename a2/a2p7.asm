beq $2,$0,go2			
lis $4
.word 4
lis $6
.word 1
lis $7
.word 0
lis $5
.word print
sw $31,-4($30)
lis $31
.word -4
add $30,$30,$31
bne $2,$0,go1
go2:	
jr $31
go1:
add $3,$0,$1
lw $1,0($1)
loop1:
jalr $5
sub $2,$2,$6
beq $2,$0,end
add $3,$3,$4
lw $1,0($3)
beq $0,$0,loop1
end:
lis $31
.word 4
add $30,$30,$31
lw $31,-4($30)
jr $31

