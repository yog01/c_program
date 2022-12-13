	.file	"masking.c"
	.text
	.section	.rodata
.LC0:
	.string	"2nd bit %x\n"
.LC1:
	.string	"1 bit reset %x\n"
.LC2:
	.string	"1 bit is set"
.LC3:
	.string	"1 bit is not set"
.LC4:
	.string	"1 bit toggel %x\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movb	$1, -1(%rbp)
	orb	$2, -1(%rbp)
	movzbl	-1(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	andb	$-3, -1(%rbp)
	movzbl	-1(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	andb	$1, -1(%rbp)
	cmpb	$1, -1(%rbp)
	jne	.L2
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	jmp	.L3
.L2:
	leaq	.LC3(%rip), %rdi
	call	puts@PLT
.L3:
	xorb	$1, -1(%rbp)
	movzbl	-1(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
