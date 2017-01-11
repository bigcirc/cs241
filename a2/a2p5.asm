lis $27
.word 0xffff000c
lis $24
.word 32
lis $25
.word 64			;'A'-1
lis $5
.word 1
lis $4
.word 4
beq $2,$0,end
loop:	
sub $2,$2,$5
;add $1,$1,$25			
lw $3,0($1)
add $3,$3,$25
bne $3,$25,nspace
sub $3,$3,$24
nspace:	
sw $3,0($27)
add $1,$1,$4
bne $2,$0,loop
jr $31
end:
jr $31