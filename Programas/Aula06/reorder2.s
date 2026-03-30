	.file	"reorder.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Digite um n\303\272mero inteiro: "
.LC1:
	.string	"%d"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
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
	leaq	.LC0(%rip), %rsi
	movl	$2, %edi
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	call	__printf_chk@PLT
	leaq	4(%rsp), %rsi
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	call	__isoc99_scanf@PLT
	movl	4(%rsp), %esi
	testl	%esi, %esi
	jle	.L5
	leal	1(%rsi), %edx
	movl	$1, %eax
	xorl	%ecx, %ecx
	testb	$1, %sil
	je	.L3
	movl	$2, %eax
	movl	$1, %ecx
	cmpl	%edx, %eax
	je	.L2
	.p2align 4,,10
	.p2align 3
.L3:
	leal	1(%rcx,%rax,2), %ecx
	addl	$2, %eax
	cmpl	%edx, %eax
	jne	.L3
.L2:
	movl	%esi, %edx
	xorl	%eax, %eax
	leaq	.LC2(%rip), %rsi
	movl	$2, %edi
	call	__printf_chk@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L14
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L5:
	.cfi_restore_state
	xorl	%ecx, %ecx
	jmp	.L2
.L14:
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
	testl	%edi, %edi
	jle	.L18
	leal	1(%rdi), %ecx
	xorl	%edx, %edx
	andl	$1, %edi
	movl	$1, %eax
	je	.L17
	movl	$2, %eax
	movl	$1, %edx
	cmpl	%ecx, %eax
	je	.L15
	.p2align 4,,10
	.p2align 3
.L17:
	leal	1(%rdx,%rax,2), %edx
	addl	$2, %eax
	cmpl	%ecx, %eax
	jne	.L17
.L15:
	movl	%edx, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L18:
	xorl	%edx, %edx
	movl	%edx, %eax
	ret
	.cfi_endproc
.LFE24:
	.size	somatorio, .-somatorio
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
