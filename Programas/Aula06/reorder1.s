	.file	"reorder.c"
	.text
	.globl	somatorio
	.type	somatorio, @function
somatorio:
.LFB24:
	.cfi_startproc
	endbr64
	cmpl	$1, %edi
	jle	.L4
	movl	$1, %eax
	movl	$0, %edx
.L3:
	addl	%eax, %edx
	addl	$1, %eax
	cmpl	%eax, %edi
	jne	.L3
.L1:
	movl	%edx, %eax
	ret
.L4:
	movl	$0, %edx
	jmp	.L1
	.cfi_endproc
.LFE24:
	.size	somatorio, .-somatorio
	.globl	fibonacci
	.type	fibonacci, @function
fibonacci:
.LFB25:
	.cfi_startproc
	endbr64
	testl	%edi, %edi
	jle	.L9
	movl	$0, %edx
	movl	$1, %eax
	movl	$0, %esi
.L8:
	movl	%eax, %ecx
	addl	%esi, %eax
	addl	$1, %edx
	movl	%ecx, %esi
	cmpl	%edx, %edi
	jne	.L8
.L6:
	movl	%ecx, %eax
	ret
.L9:
	movl	$0, %ecx
	jmp	.L6
	.cfi_endproc
.LFE25:
	.size	fibonacci, .-fibonacci
	.globl	somafibo
	.type	somafibo, @function
somafibo:
.LFB26:
	.cfi_startproc
	endbr64
	cmpl	$1, %edi
	jle	.L14
	movl	$1, %edx
	movl	$1, %ecx
	movl	$0, %esi
	movl	$0, %eax
	jmp	.L13
.L15:
	movl	%r8d, %ecx
.L13:
	addl	%edx, %eax
	leal	(%rsi,%rcx), %r8d
	addl	$1, %edx
	movl	%ecx, %esi
	cmpl	%edx, %edi
	jne	.L15
.L12:
	addl	%ecx, %eax
	ret
.L14:
	movl	$0, %ecx
	movl	$0, %eax
	jmp	.L12
	.cfi_endproc
.LFE26:
	.size	somafibo, .-somafibo
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Digite um n\303\272mero inteiro: "
.LC1:
	.string	"%d"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"O somat\303\263rio de 1 a %d \303\251: %d\n"
	.align 8
.LC3:
	.string	"O %d-\303\251simo n\303\272mero de Fibonacci \303\251: %d\n"
	.align 8
.LC4:
	.string	"A soma do somat\303\263rio e do %d-\303\251simo n\303\272mero de Fibonacci \303\251: %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rsi
	movl	$2, %edi
	call	__printf_chk@PLT
	leaq	4(%rsp), %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	4(%rsp), %ebx
	movl	%ebx, %edi
	call	somatorio
	movl	%eax, %ecx
	movl	%ebx, %edx
	leaq	.LC2(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	4(%rsp), %ebx
	movl	%ebx, %edi
	call	fibonacci
	movl	%eax, %ecx
	movl	%ebx, %edx
	leaq	.LC3(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	4(%rsp), %ebp
	movl	%ebp, %edi
	call	somatorio
	movl	%eax, %ebx
	movl	%ebp, %edi
	call	fibonacci
	leal	(%rbx,%rax), %ecx
	movl	%ebp, %edx
	leaq	.LC4(%rip), %rbx
	movq	%rbx, %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	4(%rsp), %ebp
	movl	%ebp, %edi
	call	somafibo
	movl	%eax, %ecx
	movl	%ebp, %edx
	movq	%rbx, %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L19
	movl	$0, %eax
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L19:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE23:
	.size	main, .-main
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
