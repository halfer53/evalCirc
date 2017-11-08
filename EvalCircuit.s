	.file	"EvalCircuit.c"
	.comm	pArgs,120,32
	.comm	fHead,64,32
	.section	.rodata
.LC0:
	.string	"n:w:a:b:i:f:c:s:g:z:o:q?"
	.data
	.align 8
	.type	optString, @object
	.size	optString, 8
optString:
	.quad	.LC0
	.section	.rodata
	.align 8
.LC1:
	.string	"EvalCircuit - evaluate a BPW circuit"
	.align 8
.LC2:
	.string	"  -n x, evaluate a circuit with n == (1<<x) gates"
	.align 8
.LC3:
	.string	"  -w x, evaluate a width-(1<<x) circuit"
	.align 8
.LC4:
	.string	"  -a x, evaluate a circuit with x inputs"
	.align 8
.LC5:
	.string	"  -b x, evaluate a circuit with x outputs"
	.align 8
.LC6:
	.string	"  -r x, generate a random circuit using rng #x"
	.align 8
.LC7:
	.string	"  -i x, use hexstring x as input to the circuit"
	.align 8
.LC8:
	.string	"  -f x, read a BPW circuit description from file x"
	.align 8
.LC9:
	.string	"  -c x, create file x with a random BPW circuit"
	.align 8
.LC10:
	.string	"  -g x, specify gate-format x, e.g. 08211.  Ignored if reading a bpw file."
	.align 8
.LC11:
	.string	"  -h x, gate-basis bitmap of circuit in hex e.g. -h16 for {2nand, 2nor}"
	.align 8
.LC12:
	.string	"  -s x, BPW file has blocksize (1<<x)"
	.align 8
.LC13:
	.string	"  -o 0, allocate file buffers on the program heap"
	.align 8
.LC14:
	.string	"  -o 1, allocate file buffers in the OS (win32) heap"
	.align 8
.LC15:
	.string	"  -z x, zippy: add cpu load if x<0, error-check if x==0, x==1 for fastest correct output"
	.align 8
.LC16:
	.string	"  -q, quiet: don't echo args, don't print circuit input and output"
	.text
	.globl	display_usage
	.type	display_usage, @function
display_usage:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC1, %edi
	call	puts
	movl	$.LC2, %edi
	call	puts
	movl	$.LC3, %edi
	call	puts
	movl	$.LC4, %edi
	call	puts
	movl	$.LC5, %edi
	call	puts
	movl	$.LC6, %edi
	call	puts
	movl	$.LC7, %edi
	call	puts
	movl	$.LC8, %edi
	call	puts
	movl	$.LC9, %edi
	call	puts
	movl	$.LC10, %edi
	call	puts
	movl	$.LC11, %edi
	call	puts
	movl	$.LC12, %edi
	call	puts
	movl	$.LC13, %edi
	call	puts
	movl	$.LC14, %edi
	call	puts
	movl	$.LC15, %edi
	call	puts
	movl	$.LC16, %edi
	call	puts
	movl	$1, %edi
	call	exit
	.cfi_endproc
.LFE2:
	.size	display_usage, .-display_usage
	.section	.rodata
	.align 8
.LC17:
	.string	"Argument %s of -n must be numeric"
	.align 8
.LC18:
	.string	"Argument %s of -w must be numeric"
	.align 8
.LC19:
	.string	"Argument %s of -a must be numeric"
	.align 8
.LC20:
	.string	"Argument %s of -b must be numeric"
	.align 8
.LC21:
	.string	"Argument %s of -g must be four or five decimal digits"
	.align 8
.LC22:
	.string	"Argument %s of -h must be one to four hex digits"
	.align 8
.LC23:
	.string	"Argument %s of -s must be numeric"
.LC24:
	.string	"default.bpw"
	.align 8
.LC25:
	.string	"Argument %s of -z must be numeric"
	.align 8
.LC26:
	.string	"Argument %s of -o must be 0 or 1"
	.align 8
.LC27:
	.string	"This implementation can't evaluate a width-%lu circuit\n"
	.align 8
.LC28:
	.string	"Irregular width %lu in this %lu-gate circuit\n"
	.align 8
.LC29:
	.string	"Can't evaluate a circuit with %lu inputs and %lu outputs\n"
	.align 8
.LC30:
	.string	"Unable to allocate enough working memory"
	.align 8
.LC31:
	.string	"Too many inputs %lu for this incomplete implementation\n"
	.align 8
.LC32:
	.string	"Evaluating an %lu-input circuit\n"
	.align 8
.LC33:
	.string	"Too many outputs %lu for this incomplete implementation\n"
	.align 8
.LC34:
	.string	"Evaluating an %lu-output circuit\n"
.LC35:
	.string	"Excessive blocksize %lx\n"
	.align 8
.LC36:
	.string	"Unimplemented basis set for circuit: %x\n"
	.align 8
.LC37:
	.string	"Stroul error %d: argument %s of -i must be hex, at most 16 digits"
.LC38:
	.string	"Creating BPW file %s\n"
	.align 8
.LC39:
	.string	"Filebuffers are allocated from the OS heap."
	.align 8
.LC43:
	.string	"n = %.1f Mgates, w = %.1fMiB (= %.3fKiB), blocksize = %.1f KiB, "
.LC44:
	.string	"format = %lu, file = %s\n"
.LC45:
	.string	"Zippy = %d\n"
	.align 8
.LC46:
	.string	"Circuit %s will be evaluated.\n"
	.align 8
.LC47:
	.string	"A random circuit will be evaluated."
.LC48:
	.string	"Circuit input: "
.LC49:
	.string	"%x%x"
	.align 8
.LC50:
	.string	"This implementation can't parse gates in g%lu format.\n"
.LC51:
	.string	"Circuit output: "
.LC52:
	.string	"Released all win32 resources."
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$120, %rsp
	.cfi_offset 3, -24
	movl	%edi, -116(%rbp)
	movq	%rsi, -128(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	$22, pArgs+8(%rip)
	movq	pArgs+8(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, fHead+24(%rip)
	movq	$8, pArgs(%rip)
	movq	pArgs(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, fHead+16(%rip)
	movq	$256, pArgs+16(%rip)
	movq	pArgs+16(%rip), %rax
	movq	%rax, fHead+32(%rip)
	movq	$32, pArgs+24(%rip)
	movq	pArgs+24(%rip), %rax
	movq	%rax, fHead+40(%rip)
	movq	$18, pArgs+32(%rip)
	movq	pArgs+32(%rip), %rax
	movl	%eax, fHead+12(%rip)
	movq	$8244, pArgs+48(%rip)
	movq	$8244, fHead+48(%rip)
	movl	$68, pArgs+56(%rip)
	movl	$24, pArgs+40(%rip)
	movl	pArgs+40(%rip), %eax
	movl	%eax, fHead+56(%rip)
	movq	$0, pArgs+64(%rip)
	movb	$0, pArgs+80(%rip)
	movl	$1, pArgs+84(%rip)
	movb	$0, pArgs+88(%rip)
	movq	$0, pArgs+72(%rip)
	movb	$0, pArgs+104(%rip)
	movl	$0, fHead+4(%rip)
	movl	$64, fHead+8(%rip)
	movl	$0, -108(%rbp)
	movq	optString(%rip), %rdx
	movq	-128(%rbp), %rcx
	movl	-116(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	getopt
	movl	%eax, -108(%rbp)
	jmp	.L3
.L38:
	movl	-108(%rbp), %eax
	subl	$63, %eax
	cmpl	$59, %eax
	ja	.L4
	movl	%eax, %eax
	movq	.L6(,%rax,8), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L6:
	.quad	.L5
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L7
	.quad	.L8
	.quad	.L9
	.quad	.L4
	.quad	.L4
	.quad	.L10
	.quad	.L11
	.quad	.L12
	.quad	.L13
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L14
	.quad	.L15
	.quad	.L4
	.quad	.L16
	.quad	.L4
	.quad	.L17
	.quad	.L4
	.quad	.L4
	.quad	.L4
	.quad	.L18
	.quad	.L4
	.quad	.L4
	.quad	.L19
	.text
.L14:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, pArgs+8(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L20
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC17, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L20:
	movq	pArgs+8(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, fHead+24(%rip)
	jmp	.L22
.L18:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, pArgs(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L23
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC18, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L23:
	movq	pArgs(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, fHead+16(%rip)
	jmp	.L22
.L7:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, pArgs+16(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L24
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC19, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L24:
	movq	pArgs+16(%rip), %rax
	movq	%rax, fHead+32(%rip)
	jmp	.L22
.L8:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, pArgs+24(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L25
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC20, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L25:
	movq	pArgs+24(%rip), %rax
	movq	%rax, fHead+40(%rip)
	jmp	.L22
.L11:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, pArgs+48(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L26
	movq	pArgs+48(%rip), %rax
	cmpq	$99999, %rax
	ja	.L26
	movq	pArgs+48(%rip), %rax
	cmpq	$999, %rax
	ja	.L27
.L26:
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC21, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L27:
	movq	pArgs+48(%rip), %rax
	movq	%rax, fHead+48(%rip)
	jmp	.L22
.L12:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$16, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movl	%eax, pArgs+40(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L28
	movl	pArgs+40(%rip), %eax
	testl	%eax, %eax
	je	.L28
	movl	pArgs+40(%rip), %eax
	cmpl	$65535, %eax
	jbe	.L29
.L28:
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC22, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L29:
	movl	pArgs+40(%rip), %eax
	movl	%eax, fHead+56(%rip)
	jmp	.L22
.L17:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, pArgs+32(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L30
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC23, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L30:
	movq	pArgs+32(%rip), %rax
	movl	%eax, fHead+12(%rip)
	jmp	.L22
.L13:
	movq	optarg(%rip), %rax
	movq	%rax, pArgs+64(%rip)
	jmp	.L22
.L10:
	movq	optarg(%rip), %rax
	movq	%rax, pArgs+72(%rip)
	movq	optarg(%rip), %rax
	testq	%rax, %rax
	je	.L31
	movq	optarg(%rip), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L103
.L31:
	movq	$.LC24, pArgs+72(%rip)
	jmp	.L103
.L19:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movl	%eax, pArgs+84(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L104
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC25, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L16:
	movb	$1, pArgs+88(%rip)
	jmp	.L22
.L15:
	movq	optarg(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$10, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	testq	%rax, %rax
	setne	%al
	movb	%al, pArgs+104(%rip)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L34
	movzbl	pArgs+104(%rip), %eax
	movzbl	%al, %eax
	cmpl	$1, %eax
	jle	.L105
.L34:
	movq	optarg(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC26, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L9:
	movq	optarg(%rip), %rax
	movq	%rax, pArgs+72(%rip)
	movb	$1, pArgs+80(%rip)
	movq	optarg(%rip), %rax
	testq	%rax, %rax
	je	.L36
	movq	optarg(%rip), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L106
.L36:
	movq	$.LC24, pArgs+72(%rip)
	jmp	.L106
.L5:
	call	display_usage
	jmp	.L22
.L4:
	call	display_usage
	movl	$1, %eax
	jmp	.L101
.L103:
	nop
	jmp	.L22
.L104:
	nop
	jmp	.L22
.L105:
	nop
	jmp	.L22
.L106:
	nop
.L22:
	movq	optString(%rip), %rdx
	movq	-128(%rbp), %rcx
	movl	-116(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	getopt
	movl	%eax, -108(%rbp)
.L3:
	cmpl	$-1, -108(%rbp)
	jne	.L38
	movzbl	pArgs+80(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L39
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L39
	movl	$0, %eax
	call	open_file
.L39:
	movq	pArgs(%rip), %rax
	cmpq	$30, %rax
	ja	.L40
	movq	fHead+16(%rip), %rax
	testq	%rax, %rax
	je	.L40
	movq	fHead+16(%rip), %rax
	andl	$7, %eax
	testq	%rax, %rax
	jne	.L40
	movq	fHead+16(%rip), %rax
	movq	pArgs(%rip), %rdx
	movl	$1, %esi
	movl	%edx, %ecx
	salq	%cl, %rsi
	movq	%rsi, %rdx
	cmpq	%rdx, %rax
	je	.L41
.L40:
	movq	fHead+16(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC27, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L41:
	movq	fHead+24(%rip), %rax
	movq	fHead+16(%rip), %rcx
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, %rax
	testq	%rax, %rax
	je	.L42
	movq	fHead+24(%rip), %rdx
	movq	fHead+16(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC28, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L42:
	movzbl	pArgs+80(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	jne	.L43
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	jne	.L44
.L43:
	movq	pArgs+16(%rip), %rax
	testq	%rax, %rax
	je	.L45
	movq	pArgs+24(%rip), %rax
	testq	%rax, %rax
	jne	.L44
.L45:
	movq	pArgs+24(%rip), %rdx
	movq	pArgs+16(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC29, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L44:
	movq	fHead+16(%rip), %rax
	salq	$2, %rax
	shrq	$3, %rax
	movq	%rax, -64(%rbp)
	cmpq	$31, -64(%rbp)
	ja	.L46
	movq	$32, -64(%rbp)
.L46:
	movq	-64(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, pArgs+96(%rip)
	movq	pArgs+96(%rip), %rax
	testq	%rax, %rax
	jne	.L47
	movl	$.LC30, %edi
	call	puts
	movl	$1, %eax
	jmp	.L101
.L47:
	movq	pArgs+16(%rip), %rdx
	movq	fHead+16(%rip), %rax
	cmpq	%rax, %rdx
	jbe	.L48
	movq	pArgs+16(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC31, %edi
	movl	$0, %eax
	call	printf
	movq	fHead+16(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC32, %edi
	movl	$0, %eax
	call	printf
	movq	fHead+16(%rip), %rax
	movq	%rax, pArgs+16(%rip)
.L48:
	movq	pArgs+24(%rip), %rdx
	movq	fHead+16(%rip), %rax
	cmpq	%rax, %rdx
	jbe	.L49
	movq	pArgs+24(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC33, %edi
	movl	$0, %eax
	call	printf
	movq	fHead+16(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC34, %edi
	movl	$0, %eax
	call	printf
	movq	fHead+16(%rip), %rax
	movq	%rax, pArgs+24(%rip)
	movl	$1, %eax
	jmp	.L101
.L49:
	movq	pArgs+32(%rip), %rax
	cmpq	$30, %rax
	jbe	.L50
	movq	pArgs+32(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, %rsi
	movl	$.LC35, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L50:
	movl	pArgs+40(%rip), %eax
	cmpl	$24, %eax
	je	.L51
	movl	pArgs+40(%rip), %eax
	movl	%eax, %esi
	movl	$.LC36, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L51:
	movq	pArgs+48(%rip), %rax
	movl	%eax, -96(%rbp)
	movl	-96(%rbp), %ecx
	movl	$274877907, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$6, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -92(%rbp)
	movl	-92(%rbp), %eax
	imull	$-1000, %eax, %eax
	addl	%eax, -96(%rbp)
	movl	-96(%rbp), %ecx
	movl	$1374389535, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$5, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -88(%rbp)
	movl	-88(%rbp), %eax
	imull	$-100, %eax, %eax
	addl	%eax, -96(%rbp)
	movl	-96(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -84(%rbp)
	movl	-84(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	negl	%eax
	addl	%eax, -96(%rbp)
	movl	-96(%rbp), %eax
	movl	%eax, -80(%rbp)
	movl	-92(%rbp), %eax
	imull	-88(%rbp), %eax
	imull	-84(%rbp), %eax
	movl	%eax, %edx
	movl	-92(%rbp), %eax
	imull	-80(%rbp), %eax
	leal	7(%rax), %ecx
	testl	%eax, %eax
	cmovs	%ecx, %eax
	sarl	$3, %eax
	addl	%edx, %eax
	movl	%eax, pArgs+56(%rip)
	movzbl	pArgs+80(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L52
	movq	pArgs+64(%rip), %rax
	testq	%rax, %rax
	jne	.L53
	movl	$0, -104(%rbp)
	jmp	.L54
.L55:
	movq	pArgs+96(%rip), %rdx
	movl	-104(%rbp), %eax
	cltq
	leaq	(%rdx,%rax), %rbx
	movl	$8, %edi
	call	randomRDk
	movb	%al, (%rbx)
	addl	$1, -104(%rbp)
.L54:
	movl	-104(%rbp), %eax
	cltq
	movq	pArgs+16(%rip), %rdx
	subq	$1, %rdx
	shrq	$3, %rdx
	addq	$1, %rdx
	cmpq	%rdx, %rax
	jb	.L55
	jmp	.L52
.L53:
	movq	pArgs+64(%rip), %rax
	leaq	-72(%rbp), %rcx
	movl	$16, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strtoul
	movq	%rax, -56(%rbp)
	call	__errno_location
	movl	(%rax), %eax
	movl	%eax, -76(%rbp)
	movq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L56
	cmpl	$0, -76(%rbp)
	je	.L57
.L56:
	movq	pArgs+64(%rip), %rdx
	movl	-76(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC37, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	jmp	.L101
.L57:
	movl	$0, -100(%rbp)
	jmp	.L58
.L59:
	movq	pArgs+96(%rip), %rdx
	movl	-100(%rbp), %eax
	cltq
	addq	%rdx, %rax
	movq	-56(%rbp), %rdx
	movb	%dl, (%rax)
	shrq	$8, -56(%rbp)
	addl	$1, -100(%rbp)
.L58:
	movl	-100(%rbp), %eax
	cltq
	movq	pArgs+16(%rip), %rdx
	subq	$1, %rdx
	shrq	$3, %rdx
	addq	$1, %rdx
	cmpq	%rdx, %rax
	jb	.L59
.L52:
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L60
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L61
	movq	pArgs+72(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC38, %edi
	movl	$0, %eax
	call	printf
.L61:
	movl	$0, %eax
	call	create_file
.L60:
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L62
	movl	$0, %eax
	call	alloc_fbuffers
.L62:
	movzbl	pArgs+104(%rip), %eax
	testb	%al, %al
	je	.L63
	movl	$.LC39, %edi
	movl	$0, %eax
	call	printf
.L63:
	movq	pArgs+32(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rax, %xmm0
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC40(%rip), %xmm1
	movapd	%xmm0, %xmm3
	divsd	%xmm1, %xmm3
	movq	fHead+16(%rip), %rax
	testq	%rax, %rax
	js	.L64
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rax, %xmm0
	jmp	.L65
.L64:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rdx, %xmm0
	addss	%xmm0, %xmm0
.L65:
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC40(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	.LC41(%rip), %xmm1
	movapd	%xmm0, %xmm2
	divsd	%xmm1, %xmm2
	movq	fHead+16(%rip), %rax
	testq	%rax, %rax
	js	.L66
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rax, %xmm0
	jmp	.L67
.L66:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rdx, %xmm0
	addss	%xmm0, %xmm0
.L67:
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC40(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	.LC40(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	.LC41(%rip), %xmm1
	movapd	%xmm0, %xmm4
	divsd	%xmm1, %xmm4
	movq	fHead+24(%rip), %rax
	testq	%rax, %rax
	js	.L68
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rax, %xmm0
	jmp	.L69
.L68:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rdx, %xmm0
	addss	%xmm0, %xmm0
.L69:
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC42(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movapd	%xmm4, %xmm1
	movl	$.LC43, %edi
	movl	$4, %eax
	call	printf
	movq	pArgs+72(%rip), %rdx
	movq	pArgs+48(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC44, %edi
	movl	$0, %eax
	call	printf
	movl	pArgs+84(%rip), %eax
	cmpl	$1, %eax
	je	.L70
	movl	pArgs+84(%rip), %eax
	movl	%eax, %esi
	movl	$.LC45, %edi
	movl	$0, %eax
	call	printf
.L70:
	movzbl	pArgs+80(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L71
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L72
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L73
	movq	pArgs+72(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC46, %edi
	movl	$0, %eax
	call	printf
	jmp	.L72
.L73:
	movl	$.LC47, %edi
	call	puts
.L72:
	movl	$.LC48, %edi
	movl	$0, %eax
	call	printf
	movq	$0, -48(%rbp)
	jmp	.L74
.L75:
	movq	pArgs+96(%rip), %rdx
	movq	pArgs+16(%rip), %rax
	subq	-48(%rbp), %rax
	subq	$1, %rax
	shrq	$3, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -110(%rbp)
	movzbl	-110(%rbp), %eax
	andl	$15, %eax
	movl	%eax, %edx
	movzbl	-110(%rbp), %eax
	shrb	$4, %al
	movzbl	%al, %eax
	andl	$15, %eax
	movl	%eax, %esi
	movl	$.LC49, %edi
	movl	$0, %eax
	call	printf
	addq	$8, -48(%rbp)
.L74:
	movq	pArgs+16(%rip), %rax
	cmpq	-48(%rbp), %rax
	ja	.L75
	movl	$10, %edi
	call	putchar
.L71:
	movq	fHead+16(%rip), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movq	%rax, -40(%rbp)
	movq	pArgs+48(%rip), %rax
	cmpq	$8211, %rax
	jne	.L76
	call	evalOrCreateCirc8211
	movq	%rax, -40(%rbp)
	jmp	.L77
.L76:
	movq	pArgs+48(%rip), %rax
	cmpq	$8214, %rax
	jne	.L78
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L79
	call	createCirc8214
	jmp	.L77
.L79:
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L81
	call	evalCirc8214
	movq	%rax, -40(%rbp)
	jmp	.L77
.L81:
	call	evalOrCreateCirc8221
	movq	%rax, -40(%rbp)
	jmp	.L77
.L78:
	movq	pArgs+48(%rip), %rax
	cmpq	$8221, %rax
	jne	.L82
	call	evalOrCreateCirc8221
	movq	%rax, -40(%rbp)
	jmp	.L77
.L82:
	movq	pArgs+48(%rip), %rax
	cmpq	$8231, %rax
	jne	.L83
	call	evalOrCreateCirc8231
	movq	%rax, -40(%rbp)
	jmp	.L77
.L83:
	movq	pArgs+48(%rip), %rax
	cmpq	$8241, %rax
	jne	.L84
	call	evalOrCreateCirc8241
	movq	%rax, -40(%rbp)
	jmp	.L77
.L84:
	movq	pArgs+48(%rip), %rax
	cmpq	$8224, %rax
	jne	.L85
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L86
	call	createCirc8224
	jmp	.L77
.L86:
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L88
	call	evalCirc8224
	movq	%rax, -40(%rbp)
	jmp	.L77
.L88:
	call	evalOrCreateCirc8221
	movq	%rax, -40(%rbp)
	jmp	.L77
.L85:
	movq	pArgs+48(%rip), %rax
	cmpq	$8234, %rax
	jne	.L89
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L90
	call	createCirc8234
	jmp	.L77
.L90:
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L92
	call	evalCirc8234
	movq	%rax, -40(%rbp)
	jmp	.L77
.L92:
	call	evalOrCreateCirc8221
	movq	%rax, -40(%rbp)
	jmp	.L77
.L89:
	movq	pArgs+48(%rip), %rax
	cmpq	$8244, %rax
	jne	.L93
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L94
	call	createCirc8244
	jmp	.L77
.L94:
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L96
	call	evalCirc8244
	movq	%rax, -40(%rbp)
	jmp	.L77
.L96:
	call	evalOrCreateCirc8221
	movq	%rax, -40(%rbp)
	jmp	.L77
.L93:
	movq	pArgs+48(%rip), %rax
	movq	%rax, %rsi
	movl	$.LC50, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L77:
	movzbl	pArgs+80(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L97
	movl	$.LC51, %edi
	movl	$0, %eax
	call	printf
	movq	$0, -32(%rbp)
	jmp	.L98
.L99:
	movq	pArgs+96(%rip), %rdx
	movq	-40(%rbp), %rcx
	movq	-32(%rbp), %rax
	addq	%rcx, %rax
	leaq	7(%rax), %rcx
	testq	%rax, %rax
	cmovs	%rcx, %rax
	sarq	$3, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -109(%rbp)
	movzbl	-109(%rbp), %eax
	andl	$15, %eax
	movl	%eax, %edx
	movzbl	-109(%rbp), %eax
	shrb	$4, %al
	movzbl	%al, %eax
	andl	$15, %eax
	movl	%eax, %esi
	movl	$.LC49, %edi
	movl	$0, %eax
	call	printf
	addq	$8, -32(%rbp)
.L98:
	movq	pArgs+24(%rip), %rdx
	movq	-32(%rbp), %rax
	cmpq	%rax, %rdx
	ja	.L99
	movl	$10, %edi
	call	putchar
.L97:
	movq	pArgs+72(%rip), %rax
	testq	%rax, %rax
	je	.L100
	movl	$0, %eax
	call	close_file
	movl	$0, %eax
	call	free_handles
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L100
	movl	$.LC52, %edi
	call	puts
.L100:
	movl	$0, %eax
.L101:
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L102
	call	__stack_chk_fail
.L102:
	addq	$120, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC40:
	.long	0
	.long	1083179008
	.align 8
.LC41:
	.long	0
	.long	1075838976
	.align 8
.LC42:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
