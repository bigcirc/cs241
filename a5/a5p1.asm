beq $0,$0,2

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
.word 0x01			;0x70
.word 0x60
.word 0x05
.word 0x54
.word 3
.word 67
.word 68
.word 69
.word 0x05
.word 0x58
.word 3
.word 64
.word 65
.word 66
.word 0x05
.word 0x6c
.word 3
.word 61
.word 62
.word 63
.word 0x01
.word def
.word 0x01
.word abc
.word 0x01
.word def
.word 0x01
.word ghi



