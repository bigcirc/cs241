lis $3
.word 0x5c
sw $31,-4($30)
lis $31
.word 0x58
lw $31,0($31)
lw $31,0($31)
lw $31,0($31)
sub $30,$30,$31
jalr $3
lis $31
.word 0x58
lw $31,0($31)
lw $31,0($31)
lw $31,0($31)
add $30,$30,$31
lw $31,-4($30)
jr $31
ghi:	
.word 4
def:	
.word 0x70
abc:	
jr $31		
label:	.word 0x54




