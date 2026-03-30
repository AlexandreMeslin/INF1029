#include <stdio.h>

int somatorio(int n);
int fibonacci(int n);
int somafibo(int n);

int main(void) {
    int n;
    printf("Digite um número inteiro: ");
    scanf("%d", &n);
    printf("O somatório de 1 a %d é: %d\n", n, somatorio(n));
    printf("O %d-ésimo número de Fibonacci é: %d\n", n, fibonacci(n));
    printf("A soma do somatório e do %d-ésimo número de Fibonacci é: %d\n", n, somatorio(n) + fibonacci(n));
    printf("A soma do somatório e do %d-ésimo número de Fibonacci é: %d\n", n, somafibo(n));
    return 0;
}

int somatorio(int n) {
    int soma = 0;
    for (int i = 1; i < n; i++) {
        soma += i;
    }
    return soma;
}

int fibonacci(int n) {
    int a = 0, b = 1, temp;
    for (int i = 0; i < n; i++) {
        temp = a;
        a = b;
        b = temp + b;
    }
    return a;
}

int somafibo(int n) {
    int soma = 0;
    int a = 0, b = 1, temp;
    for (int i = 1; i < n; i++) {
        soma += i;
        temp = a;
        a = b;
        b = temp + b;
    }
    return soma + a; // soma dos inteiros + n-ésimo número de Fibonacci
}

/*
EAX a
EBX temp
ECX b
EDI n
ESI soma
EDX i

somafibo:
	movl	$1, %ecx
	movl	$0, %esi
	movl	$0, %eax
	movl	$1, %edx
.L1    
	cmpl	%edi, %edx
	jge	.L12
    addl    %edx, %esi
    movl    %eax, %ebx
    movl    %ecx, %eax
    addl    %ebx, %eax
    incl	%edx
    jmp    .L1
.L12:
	addl	%esi, %eax
	ret




EDI n
EBX i
EDX soma
EAX temp

somatorio:
	movl	$1, %ebx    # i = 1
	movl	$0, %edx    # soma = 0
.L2:
	cmpl	%edi, %ebx  # while (i < n)
	jge	.L3
    movl    %ebx, %eax  # temp = i
	addl	%eax, %edx
    movl    %eax, %edx
	addl	$1, %eax
    jmp	.L2
    .L3:
	movl	%edx, %eax
	ret

fibonacci:
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
	movl	%ecx, %eax
	ret
*/