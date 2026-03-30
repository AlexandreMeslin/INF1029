	.file	"reorder.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Digite um n\303\272mero inteiro: "
.LC2:
	.string	"%d"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC4:
	.string	"O somat\303\263rio de 1 a %d \303\251: %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	endbr64
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	leaq	.LC1(%rip), %rsi
	movl	$2, %edi
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	call	__printf_chk@PLT
	leaq	4(%rsp), %rsi
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	call	__isoc99_scanf@PLT
	movl	4(%rsp), %edx
	testl	%edx, %edx
	jle	.L8
	leal	-1(%rdx), %eax
	cmpl	$9, %eax
	jbe	.L9
	movl	%edx, %ecx
	movdqa	.LC0(%rip), %xmm1
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movdqa	.LC3(%rip), %xmm3
	shrl	$2, %ecx
	.p2align 4,,10
	.p2align 3
.L4:
	movdqa	%xmm1, %xmm2
	addl	$1, %eax
	paddd	%xmm3, %xmm1
	paddd	%xmm2, %xmm0
	cmpl	%ecx, %eax
	jne	.L4
	movdqa	%xmm0, %xmm1
	movl	%edx, %eax
	psrldq	$8, %xmm1
	andl	$-4, %eax
	paddd	%xmm1, %xmm0
	addl	$1, %eax
	movdqa	%xmm0, %xmm1
	psrldq	$4, %xmm1
	paddd	%xmm1, %xmm0
	movd	%xmm0, %ecx
	testb	$3, %dl
	je	.L2
.L3:
	leal	1(%rax), %esi
	addl	%eax, %ecx
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	2(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	3(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	4(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	5(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	6(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	7(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	%esi, %ecx
	leal	8(%rax), %esi
	cmpl	%esi, %edx
	jl	.L2
	addl	$9, %eax
	addl	%esi, %ecx
	cmpl	%eax, %edx
	jl	.L2
	addl	%eax, %ecx
.L2:
	xorl	%eax, %eax
	leaq	.LC4(%rip), %rsi
	movl	$2, %edi
	call	__printf_chk@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L13
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	xorl	%ecx, %ecx
	jmp	.L2
.L9:
	movl	$1, %eax
	xorl	%ecx, %ecx
	jmp	.L3
.L13:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.text
	.p2align 4
	.globl	somatorio
	.type	somatorio, @function
somatorio:
.LFB24:
	.cfi_startproc
	endbr64
	movl	%edi, %edx
	testl	%edi, %edi
	jle	.L20
	leal	-1(%rdi), %eax
	cmpl	$9, %eax
	jbe	.L21
	movl	%edi, %ecx
	movdqa	.LC0(%rip), %xmm1
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movdqa	.LC3(%rip), %xmm3
	shrl	$2, %ecx
	.p2align 4,,10
	.p2align 3
.L17:
	movdqa	%xmm1, %xmm2
	addl	$1, %eax
	paddd	%xmm3, %xmm1
	paddd	%xmm2, %xmm0
	cmpl	%ecx, %eax
	jne	.L17
	movdqa	%xmm0, %xmm1
	movl	%edx, %ecx
	psrldq	$8, %xmm1
	andl	$-4, %ecx
	paddd	%xmm1, %xmm0
	addl	$1, %ecx
	movdqa	%xmm0, %xmm1
	psrldq	$4, %xmm1
	paddd	%xmm1, %xmm0
	movd	%xmm0, %eax
	testb	$3, %dl
	je	.L14
.L16:
	leal	1(%rcx), %esi
	addl	%ecx, %eax
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	2(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	3(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	4(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	5(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	6(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	7(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	leal	8(%rcx), %esi
	cmpl	%esi, %edx
	jl	.L14
	addl	%esi, %eax
	addl	$9, %ecx
	leal	(%rax,%rcx), %esi
	cmpl	%ecx, %edx
	cmovge	%esi, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	xorl	%eax, %eax
.L14:
	ret
.L21:
	movl	$1, %ecx
	xorl	%eax, %eax
	jmp	.L16
	.cfi_endproc
.LFE24:
	.size	somatorio, .-somatorio
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	1
	.long	2
	.long	3
	.long	4
	.align 16
.LC3:
	.long	4
	.long	4
	.long	4
	.long	4
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
