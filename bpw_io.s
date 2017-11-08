	.file	"bpw_io.c"
	.local	resSize
	.comm	resSize,4,4
	.section	.rodata
.LC0:
	.string	"BPW"
	.data
	.align 8
	.type	magicStr, @object
	.size	magicStr, 8
magicStr:
	.quad	.LC0
	.local	fb0
	.comm	fb0,48,32
	.local	fb1
	.comm	fb1,48,32
	.local	fb2
	.comm	fb2,48,32
	.local	curr_fb
	.comm	curr_fb,8,8
	.local	next_fb
	.comm	next_fb,8,8
	.text
	.globl	pCfbI
	.type	pCfbI, @function
pCfbI:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	curr_fb(%rip), %rax
	movq	16(%rax), %rdx
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rax
	addq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	pCfbI, .-pCfbI
	.globl	get_err
	.type	get_err, @function
get_err:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	curr_fb(%rip), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	aio_error
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	jne	.L4
	call	__errno_location
	movl	(%rax), %eax
	jmp	.L6
.L4:
	movl	-4(%rbp), %eax
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	get_err, .-get_err
	.section	.rodata
.LC1:
	.string	"bpw_io.c"
	.align 8
.LC2:
	.string	"Error in `fstat(fb0.cb->aio_fildes, &st)` syscall in line %d in %s\n"
	.text
	.globl	get_file_size
	.type	get_file_size, @function
get_file_size:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$160, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	fb0+8(%rip), %rax
	movl	(%rax), %eax
	leaq	-160(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	fstat
	cmpl	$-1, %eax
	jne	.L8
	movl	$85, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	err_sys
.L8:
	movq	-112(%rbp), %rax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L10
	call	__stack_chk_fail
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	get_file_size, .-get_file_size
	.section	.rodata
	.align 8
.LC3:
	.string	"Error in `fd = open(pArgs.circuitFileName, O_CREAT | accessflags, S_IRUSR | S_IWUSR)` syscall in line %d in %s\n"
	.text
	.globl	init_fbuffers
	.type	init_fbuffers, @function
init_fbuffers:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	$168, %esi
	movl	$1, %edi
	call	calloc
	movq	%rax, -32(%rbp)
	movl	$168, %esi
	movl	$1, %edi
	call	calloc
	movq	%rax, -24(%rbp)
	movl	$168, %esi
	movl	$1, %edi
	call	calloc
	movq	%rax, -16(%rbp)
	movzbl	pArgs+80(%rip), %eax
	movzbl	%al, %eax
	movl	%eax, -40(%rbp)
	movl	-40(%rbp), %eax
	orl	$64, %eax
	movl	%eax, %ecx
	movq	pArgs+72(%rip), %rax
	movl	$384, %edx
	movl	%ecx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open
	movl	%eax, -36(%rbp)
	cmpl	$-1, -36(%rbp)
	jne	.L12
	movl	$142, %edx
	movl	$.LC1, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	err_sys
.L12:
	movq	-32(%rbp), %rax
	movl	-36(%rbp), %edx
	movl	%edx, (%rax)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	dup
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	dup
	movl	%eax, %edx
	movq	-16(%rbp), %rax
	movl	%edx, (%rax)
	movl	fHead+8(%rip), %eax
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movq	%rdx, 128(%rax)
	movq	-32(%rbp), %rax
	movq	%rax, fb0+8(%rip)
	movq	-24(%rbp), %rax
	movq	%rax, fb1+8(%rip)
	movq	-16(%rbp), %rax
	movq	%rax, fb2+8(%rip)
	movl	$64, %esi
	movl	$1, %edi
	call	calloc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, fb0+16(%rip)
	movq	$64, fb0+24(%rip)
	movq	$0, fb1+16(%rip)
	movq	$0, fb1+24(%rip)
	movq	$0, fb2+16(%rip)
	movq	$0, fb2+24(%rip)
	movq	$0, fb0+32(%rip)
	movq	$0, fb1+32(%rip)
	movq	$0, fb2+32(%rip)
	movq	$0, fb0+40(%rip)
	movl	fHead+8(%rip), %eax
	movl	%eax, %eax
	movq	%rax, fb1+40(%rip)
	movq	$0, fb2+40(%rip)
	movq	$fb0, curr_fb(%rip)
	movq	$fb1, next_fb(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	init_fbuffers, .-init_fbuffers
	.section	.rodata
.LC4:
	.string	"Unable to allocate buffers."
.LC5:
	.string	"buf1 at %p\n"
.LC6:
	.string	"buf2 at %p\n"
	.text
	.globl	alloc_fbuffers
	.type	alloc_fbuffers, @function
alloc_fbuffers:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	pArgs+32(%rip), %rax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -8(%rbp)
	cmpq	$0, -16(%rbp)
	je	.L14
	cmpq	$0, -8(%rbp)
	jne	.L15
.L14:
	movl	$.LC4, %edi
	call	puts
	movl	$1, %edi
	call	exit
.L15:
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L16
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC6, %edi
	movl	$0, %eax
	call	printf
.L16:
	movq	-16(%rbp), %rax
	movq	%rax, fb1+16(%rip)
	movl	fHead+12(%rip), %eax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, fb1+24(%rip)
	movq	-8(%rbp), %rax
	movq	%rax, fb2+16(%rip)
	movq	fb1+24(%rip), %rax
	movq	%rax, fb2+24(%rip)
	movzbl	pArgs+80(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L18
	movl	$fb1, %edi
	call	read_fbuffer
.L18:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	alloc_fbuffers, .-alloc_fbuffers
	.section	.rodata
	.align 8
.LC7:
	.string	"curr_fb->fbuffer at %p for WriteFile\n"
	.align 8
.LC8:
	.string	"curr_fb->fbsize is %ld for WriteFile\n"
	.align 8
.LC9:
	.string	"Error in `aio_write(curr_fb->cb)` syscall in line %d in %s\n"
.LC10:
	.string	"Writing a block..."
	.align 8
.LC11:
	.string	"\nError code %d when writing file \"%s\" asynchronously.\n"
	.align 8
.LC12:
	.string	"WriteFile completed immediately!"
	.text
	.globl	write_fbuffer
	.type	write_fbuffer, @function
write_fbuffer:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L20
	movq	curr_fb(%rip), %rax
	movq	16(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	movq	curr_fb(%rip), %rax
	movq	24(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC8, %edi
	movl	$0, %eax
	call	printf
.L20:
	movq	curr_fb(%rip), %rax
	movq	8(%rax), %rax
	movq	curr_fb(%rip), %rdx
	movq	16(%rdx), %rdx
	movq	%rdx, 16(%rax)
	movq	curr_fb(%rip), %rax
	movq	8(%rax), %rax
	movq	curr_fb(%rip), %rdx
	movq	24(%rdx), %rdx
	movq	%rdx, 128(%rax)
	movq	curr_fb(%rip), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	aio_write
	cmpl	$-1, %eax
	jne	.L21
	movl	$318, %edx
	movl	$.LC1, %esi
	movl	$.LC9, %edi
	movl	$0, %eax
	call	err_sys
.L21:
	movl	$0, %eax
	call	get_err
	cmpl	$115, %eax
	setne	%al
	movb	%al, -5(%rbp)
	movzbl	-5(%rbp), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L22
	movl	$0, %eax
	call	get_err
	movl	%eax, -4(%rbp)
	cmpl	$115, -4(%rbp)
	jne	.L23
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L27
	movl	$.LC10, %edi
	movl	$0, %eax
	call	printf
	jmp	.L27
.L23:
	movq	pArgs+72(%rip), %rdx
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC11, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L22:
	movl	$.LC12, %edi
	call	puts
.L27:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	write_fbuffer, .-write_fbuffer
	.section	.rodata
.LC13:
	.string	"File I/O Completed"
	.align 8
.LC14:
	.string	"Timeout waiting for file I/O to complete"
.LC15:
	.string	"File I/O error %d\n"
	.text
	.globl	wait_on_fb
	.type	wait_on_fb, @function
wait_on_fb:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$1, -32(%rbp)
	movq	$0, -24(%rbp)
	movq	-56(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	leaq	-32(%rbp), %rdx
	leaq	-16(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	aio_suspend
	movl	%eax, -40(%rbp)
	movl	$0, %eax
	call	get_err
	movl	%eax, -36(%rbp)
	cmpl	$0, -40(%rbp)
	jne	.L29
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L34
	movl	$.LC13, %edi
	call	puts
	jmp	.L34
.L29:
	cmpl	$11, -40(%rbp)
	jne	.L31
	movl	$.LC14, %edi
	call	puts
	jmp	.L32
.L31:
	movl	-36(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC15, %edi
	movl	$0, %eax
	call	printf
.L32:
	movq	-56(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-56(%rbp), %rax
	movq	8(%rax), %rax
	movl	(%rax), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	aio_cancel
	movl	$1, %edi
	call	exit
.L34:
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L33
	call	__stack_chk_fail
.L33:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	wait_on_fb, .-wait_on_fb
	.section	.rodata
	.align 8
.LC16:
	.string	"pfb->fbuffer at %p for ReadFile\n"
	.align 8
.LC17:
	.string	"pfb->fbsize is %lx for ReadFile\n"
	.align 8
.LC18:
	.string	"Error in `aio_read(pfb->cb)` syscall in line %d in %s\n"
.LC19:
	.string	"Reading a block..."
	.align 8
.LC20:
	.string	"Error code %d: unable to read file \"%s\" asynchronously.\n"
	.align 8
.LC21:
	.string	"ReadFile completed immediately!"
	.text
	.globl	read_fbuffer
	.type	read_fbuffer, @function
read_fbuffer:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L36
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC16, %edi
	movl	$0, %eax
	call	printf
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC17, %edi
	movl	$0, %eax
	call	printf
.L36:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	16(%rdx), %rdx
	movq	%rdx, 16(%rax)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	24(%rdx), %rdx
	movq	%rdx, 128(%rax)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	aio_read
	cmpl	$-1, %eax
	jne	.L37
	movl	$394, %edx
	movl	$.LC1, %esi
	movl	$.LC18, %edi
	movl	$0, %eax
	call	err_sys
.L37:
	movl	$0, %eax
	call	get_err
	cmpl	$115, %eax
	setne	%al
	movb	%al, -5(%rbp)
	movzbl	-5(%rbp), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L38
	movl	$0, %eax
	call	get_err
	movl	%eax, -4(%rbp)
	cmpl	$115, -4(%rbp)
	jne	.L39
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L43
	movl	$.LC19, %edi
	movl	$0, %eax
	call	printf
	jmp	.L43
.L39:
	movq	pArgs+72(%rip), %rdx
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC20, %edi
	movl	$0, %eax
	call	printf
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	exit
.L38:
	movzbl	pArgs+88(%rip), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L43
	movl	$.LC21, %edi
	call	puts
.L43:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	read_fbuffer, .-read_fbuffer
	.section	.rodata
.LC22:
	.string	"resSize == 0"
	.align 8
.LC23:
	.string	"Improper format detected in input file"
	.text
	.globl	reserve_fbuffer
	.type	reserve_fbuffer, @function
reserve_fbuffer:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	resSize(%rip), %eax
	testl	%eax, %eax
	je	.L45
	movl	$__PRETTY_FUNCTION__.5020, %ecx
	movl	$432, %edx
	movl	$.LC1, %esi
	movl	$.LC22, %edi
	call	__assert_fail
.L45:
	movl	-20(%rbp), %eax
	movl	%eax, resSize(%rip)
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rdx
	movl	-20(%rbp), %eax
	cltq
	addq	%rax, %rdx
	movq	curr_fb(%rip), %rax
	movq	24(%rax), %rax
	cmpq	%rax, %rdx
	jbe	.L61
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L51
	jmp	.L48
.L49:
	movq	curr_fb(%rip), %rax
	movq	16(%rax), %rdx
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rdx
	addq	$1, %rdx
	movq	%rdx, 32(%rax)
.L48:
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rdx
	movq	curr_fb(%rip), %rax
	movq	24(%rax), %rax
	cmpq	%rax, %rdx
	jb	.L49
	movl	$0, %eax
	call	write_fbuffer
	jmp	.L50
.L53:
	movq	curr_fb(%rip), %rax
	movq	16(%rax), %rdx
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L52
	movl	$.LC23, %edi
	call	puts
	call	abort
.L52:
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rdx
	addq	$1, %rdx
	movq	%rdx, 32(%rax)
.L51:
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rdx
	movq	curr_fb(%rip), %rax
	movq	24(%rax), %rax
	cmpq	%rax, %rdx
	jb	.L53
	movq	curr_fb(%rip), %rax
	cmpq	$fb0, %rax
	jne	.L54
	movq	$fb2, curr_fb(%rip)
	movq	curr_fb(%rip), %rax
	movl	fHead+12(%rip), %edx
	movl	$1, %esi
	movl	%edx, %ecx
	salq	%cl, %rsi
	movq	%rsi, %rdx
	movq	%rdx, 24(%rax)
.L54:
	movq	curr_fb(%rip), %rax
	movq	$0, 32(%rax)
	movq	curr_fb(%rip), %rax
	movq	next_fb(%rip), %rdx
	movq	40(%rdx), %rcx
	movq	next_fb(%rip), %rdx
	movq	24(%rdx), %rdx
	addq	%rcx, %rdx
	movq	%rdx, 40(%rax)
	movq	curr_fb(%rip), %rax
	movq	%rax, %rdi
	call	read_fbuffer
.L50:
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L55
	movq	curr_fb(%rip), %rax
	cmpq	$fb0, %rax
	je	.L58
	movq	curr_fb(%rip), %rax
	movq	40(%rax), %rax
	cmpq	$64, %rax
	jne	.L57
	movl	$fb0, %edi
	call	wait_on_fb
	jmp	.L58
.L57:
	movq	next_fb(%rip), %rax
	movq	%rax, %rdi
	call	wait_on_fb
	jmp	.L58
.L55:
	movq	next_fb(%rip), %rax
	movq	%rax, %rdi
	call	wait_on_fb
.L58:
	movq	curr_fb(%rip), %rax
	cmpq	$fb0, %rax
	je	.L59
	movq	curr_fb(%rip), %rax
	jmp	.L60
.L59:
	movl	$fb2, %eax
.L60:
	movq	%rax, -8(%rbp)
	movq	next_fb(%rip), %rax
	movq	%rax, curr_fb(%rip)
	movq	-8(%rbp), %rax
	movq	%rax, next_fb(%rip)
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L61
	movq	next_fb(%rip), %rax
	movq	40(%rax), %rax
	testq	%rax, %rax
	je	.L61
	movq	curr_fb(%rip), %rax
	movq	$0, 32(%rax)
	movq	curr_fb(%rip), %rax
	movq	next_fb(%rip), %rdx
	movq	40(%rdx), %rcx
	movq	next_fb(%rip), %rdx
	movq	24(%rdx), %rdx
	addq	%rcx, %rdx
	movq	%rdx, 40(%rax)
.L61:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	reserve_fbuffer, .-reserve_fbuffer
	.globl	release_reservation
	.type	release_reservation, @function
release_reservation:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rcx
	movl	resSize(%rip), %edx
	movslq	%edx, %rdx
	addq	%rcx, %rdx
	movq	%rdx, 32(%rax)
	movl	$0, resSize(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	release_reservation, .-release_reservation
	.globl	write_64
	.type	write_64, @function
write_64:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, %eax
	call	pCfbI
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$8, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$8, %eax
	movl	%eax, resSize(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	write_64, .-write_64
	.globl	write_32
	.type	write_32, @function
write_32:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movl	%edi, -20(%rbp)
	movl	$0, %eax
	call	pCfbI
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	-20(%rbp), %edx
	movl	%edx, (%rax)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$4, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$4, %eax
	movl	%eax, resSize(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	write_32, .-write_32
	.globl	write_16
	.type	write_16, @function
write_16:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movl	%edi, %eax
	movw	%ax, -20(%rbp)
	movl	$0, %eax
	call	pCfbI
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movzwl	-20(%rbp), %edx
	movw	%dx, (%rax)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$2, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$2, %eax
	movl	%eax, resSize(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	write_16, .-write_16
	.globl	write_8
	.type	write_8, @function
write_8:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$0, %eax
	call	pCfbI
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	-20(%rbp), %edx
	movb	%dl, (%rax)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$1, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$1, %eax
	movl	%eax, resSize(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	write_8, .-write_8
	.globl	write_24
	.type	write_24, @function
write_24:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	write_16
	movl	-4(%rbp), %eax
	shrl	$16, %eax
	movzbl	%al, %eax
	movl	%eax, %edi
	call	write_8
	movl	resSize(%rip), %eax
	subl	$3, %eax
	movl	%eax, resSize(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	write_24, .-write_24
	.globl	create_file
	.type	create_file, @function
create_file:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	init_fbuffers
	movl	$64, %edi
	call	reserve_fbuffer
	movq	magicStr(%rip), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	write_32
	movl	$0, fHead+4(%rip)
	movl	fHead+4(%rip), %eax
	movl	%eax, %edi
	call	write_32
	movl	$64, fHead+8(%rip)
	movl	fHead+8(%rip), %eax
	movl	%eax, %edi
	call	write_32
	movl	fHead+12(%rip), %eax
	movl	%eax, %edi
	call	write_32
	movq	fHead+16(%rip), %rax
	movq	%rax, %rdi
	call	write_64
	movq	fHead+24(%rip), %rax
	movq	%rax, %rdi
	call	write_64
	movq	fHead+32(%rip), %rax
	movq	%rax, %rdi
	call	write_64
	movq	fHead+40(%rip), %rax
	movq	%rax, %rdi
	call	write_64
	movq	fHead+48(%rip), %rax
	movq	%rax, %rdi
	call	write_64
	movl	fHead+56(%rip), %eax
	movl	%eax, %edi
	call	write_32
	movl	$0, %edi
	call	write_32
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	create_file, .-create_file
	.section	.rodata
	.align 8
.LC24:
	.string	"Corrupt bpw file of length %ld\n"
.LC25:
	.string	"Incorrect file type: %x\n"
	.align 8
.LC26:
	.string	"This file is BPW version %d, but this evaluator is BPW version %d\n."
	.align 8
.LC27:
	.string	"This file has header length %d, unreadable by this circuit evaluator\n."
.LC28:
	.string	"Excessive blocksize %lx\n."
	.align 8
.LC29:
	.string	"Corrupt bpw file %s: %lu total size, with %u header and %f.4 blocks of size %lu.\n"
	.align 8
.LC30:
	.string	"Unsupported circuit width %lu\n"
.LC31:
	.string	"Unsupported circuit size %lu\n"
.LC32:
	.string	"Unsupported input size %lu\n"
.LC33:
	.string	"Unsupported output size %lu\n"
.LC34:
	.string	"Unsupported basis set: %u\n"
	.align 8
.LC35:
	.string	"Corrupt header, CRC32 is %u but should be %u\n"
	.align 8
.LC36:
	.string	"Implementation error in open_file(), aborting."
	.text
	.globl	open_file
	.type	open_file, @function
open_file:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	$0, %eax
	call	init_fbuffers
	movq	curr_fb(%rip), %rax
	movq	%rax, %rdi
	call	read_fbuffer
	movq	curr_fb(%rip), %rax
	movq	%rax, %rdi
	call	wait_on_fb
	movl	$0, %eax
	call	get_file_size
	movq	%rax, -64(%rbp)
	cmpq	$63, -64(%rbp)
	ja	.L70
	movq	-64(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC24, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L70:
	movl	$64, %edi
	call	reserve_fbuffer
	movl	$0, %eax
	call	read_32
	movl	%eax, -88(%rbp)
	movq	magicStr(%rip), %rax
	movl	(%rax), %eax
	cmpl	-88(%rbp), %eax
	je	.L71
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC25, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L71:
	movq	magicStr(%rip), %rax
	movq	%rax, %rsi
	movl	$fHead, %edi
	call	strcpy
	movl	$0, %eax
	call	read_32
	movl	%eax, -84(%rbp)
	cmpl	$0, -84(%rbp)
	je	.L72
	movl	-84(%rbp), %eax
	movl	$0, %edx
	movl	%eax, %esi
	movl	$.LC26, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L72:
	movl	-84(%rbp), %eax
	movl	%eax, fHead+4(%rip)
	movl	$0, %eax
	call	read_32
	movl	%eax, -80(%rbp)
	cmpl	$64, -80(%rbp)
	je	.L73
	movl	-80(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC27, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L73:
	movl	-80(%rbp), %eax
	movl	%eax, fHead+8(%rip)
	movl	$0, %eax
	call	read_32
	movl	%eax, -76(%rbp)
	cmpl	$30, -76(%rbp)
	jbe	.L74
	movl	-76(%rbp), %eax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movq	%rax, %rsi
	movl	$.LC28, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L74:
	movl	fHead+8(%rip), %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	subq	%rdx, %rax
	movq	%rax, %rdx
	movl	-76(%rbp), %eax
	movl	%eax, %ecx
	shrq	%cl, %rdx
	movq	%rdx, %rax
	testq	%rax, %rax
	js	.L75
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	jmp	.L76
.L75:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L76:
	movsd	%xmm0, -56(%rbp)
	movl	fHead+8(%rip), %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	subq	%rdx, %rax
	movq	%rax, %rdx
	movl	-76(%rbp), %eax
	movl	$1, %esi
	movl	%eax, %ecx
	salq	%cl, %rsi
	movq	%rsi, %rax
	subq	$1, %rax
	andq	%rdx, %rax
	movq	%rax, -48(%rbp)
	movl	-76(%rbp), %eax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	addq	$64, %rax
	cmpq	-64(%rbp), %rax
	ja	.L77
	cmpq	$0, -48(%rbp)
	je	.L78
.L77:
	movl	-76(%rbp), %eax
	movl	$1, %edx
	movq	%rdx, %rdi
	movl	%eax, %ecx
	salq	%cl, %rdi
	movl	fHead+8(%rip), %ecx
	movq	pArgs+72(%rip), %rax
	movq	-56(%rbp), %rsi
	movq	-64(%rbp), %rdx
	movq	%rdi, %r8
	movq	%rsi, -104(%rbp)
	movsd	-104(%rbp), %xmm0
	movq	%rax, %rsi
	movl	$.LC29, %edi
	movl	$1, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L78:
	movl	-76(%rbp), %eax
	movl	%eax, fHead+12(%rip)
	movl	fHead+12(%rip), %eax
	movl	%eax, %eax
	movq	%rax, pArgs+32(%rip)
	movl	$0, %eax
	call	read_64
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, fHead+16(%rip)
	movq	fHead+16(%rip), %rax
	bsrq	%rax, %rax
	xorq	$63, %rax
	movl	$63, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %eax
	movq	%rax, pArgs(%rip)
	movq	fHead+16(%rip), %rax
	movq	pArgs(%rip), %rdx
	movl	$1, %esi
	movl	%edx, %ecx
	salq	%cl, %rsi
	movq	%rsi, %rdx
	cmpq	%rdx, %rax
	je	.L79
	movq	-40(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC30, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L79:
	movl	$0, %eax
	call	read_64
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, fHead+24(%rip)
	movq	fHead+24(%rip), %rax
	bsrq	%rax, %rax
	xorq	$63, %rax
	movl	$63, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %eax
	movq	%rax, pArgs+8(%rip)
	movq	fHead+24(%rip), %rax
	movq	pArgs+8(%rip), %rdx
	movl	$1, %esi
	movl	%edx, %ecx
	salq	%cl, %rsi
	movq	%rsi, %rdx
	cmpq	%rdx, %rax
	je	.L80
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC31, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L80:
	movl	$0, %eax
	call	read_64
	movq	%rax, -24(%rbp)
	cmpq	$1073741824, -24(%rbp)
	jbe	.L81
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC32, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L81:
	movq	-24(%rbp), %rax
	movq	%rax, fHead+32(%rip)
	movq	fHead+32(%rip), %rax
	movq	%rax, pArgs+16(%rip)
	movl	$0, %eax
	call	read_64
	movq	%rax, -16(%rbp)
	cmpq	$1073741824, -16(%rbp)
	jbe	.L82
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC33, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L82:
	movq	-16(%rbp), %rax
	movq	%rax, fHead+40(%rip)
	movq	fHead+40(%rip), %rax
	movq	%rax, pArgs+24(%rip)
	movl	$0, %eax
	call	read_64
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, fHead+48(%rip)
	movq	fHead+48(%rip), %rax
	movq	%rax, pArgs+48(%rip)
	movl	$0, %eax
	call	read_32
	movl	%eax, -72(%rbp)
	cmpl	$24, -72(%rbp)
	je	.L83
	movl	-72(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC34, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L83:
	movl	-72(%rbp), %eax
	movl	%eax, fHead+56(%rip)
	movl	$0, %eax
	call	read_32
	movl	%eax, -68(%rbp)
	cmpl	$0, -68(%rbp)
	je	.L84
	movl	-68(%rbp), %eax
	movl	%eax, %edx
	movl	$0, %esi
	movl	$.LC35, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	exit
.L84:
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rax
	cmpq	$64, %rax
	jne	.L85
	movq	curr_fb(%rip), %rax
	movq	24(%rax), %rax
	cmpq	$64, %rax
	je	.L87
.L85:
	movl	$.LC36, %edi
	call	puts
	movl	$1, %edi
	call	exit
.L87:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	open_file, .-open_file
	.globl	read_8
	.type	read_8, @function
read_8:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	pCfbI
	movzbl	(%rax), %eax
	movb	%al, -1(%rbp)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$1, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$1, %eax
	movl	%eax, resSize(%rip)
	movzbl	-1(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	read_8, .-read_8
	.globl	read_16
	.type	read_16, @function
read_16:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	pCfbI
	movzwl	(%rax), %eax
	movw	%ax, -2(%rbp)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$2, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$2, %eax
	movl	%eax, resSize(%rip)
	movzwl	-2(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	read_16, .-read_16
	.globl	read_24
	.type	read_24, @function
read_24:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	pCfbI
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	andl	$16777215, -4(%rbp)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$3, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$3, %eax
	movl	%eax, resSize(%rip)
	movl	-4(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	read_24, .-read_24
	.globl	read_32
	.type	read_32, @function
read_32:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	pCfbI
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$4, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$4, %eax
	movl	%eax, resSize(%rip)
	movl	-4(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	read_32, .-read_32
	.globl	read_64
	.type	read_64, @function
read_64:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	pCfbI
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	curr_fb(%rip), %rax
	movq	curr_fb(%rip), %rdx
	movq	32(%rdx), %rdx
	addq	$8, %rdx
	movq	%rdx, 32(%rax)
	movl	resSize(%rip), %eax
	subl	$8, %eax
	movl	%eax, resSize(%rip)
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	read_64, .-read_64
	.section	.rodata
.LC38:
	.string	"%.2f Mbytes written.\n"
.LC39:
	.string	"%.2f Mbytes read.\n"
.LC40:
	.string	"Flushing writes to disk."
	.align 8
.LC41:
	.string	"Error in `close(curr_fb->cb->aio_fildes)` syscall in line %d in %s\n"
	.text
	.globl	close_file
	.type	close_file, @function
close_file:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L99
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rax
	testq	%rax, %rax
	je	.L100
	movl	fHead+12(%rip), %eax
	movl	$1, %edx
	movl	%eax, %ecx
	salq	%cl, %rdx
	movq	%rdx, %rax
	movl	%eax, %edx
	movq	curr_fb(%rip), %rax
	movq	32(%rax), %rax
	subl	%eax, %edx
	movl	%edx, %eax
	addl	$1, %eax
	movl	%eax, %edi
	call	reserve_fbuffer
.L100:
	movq	next_fb(%rip), %rax
	movq	%rax, %rdi
	call	wait_on_fb
.L99:
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L101
	movq	next_fb(%rip), %rax
	movq	40(%rax), %rdx
	movq	next_fb(%rip), %rax
	movq	24(%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2ssq	-16(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC37(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movl	$.LC38, %edi
	movl	$1, %eax
	call	printf
	jmp	.L102
.L101:
	movq	curr_fb(%rip), %rax
	movq	40(%rax), %rdx
	movq	curr_fb(%rip), %rax
	movq	24(%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2ssq	-8(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movsd	.LC37(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movl	$.LC39, %edi
	movl	$1, %eax
	call	printf
.L102:
	movzbl	pArgs+80(%rip), %eax
	testb	%al, %al
	je	.L103
	movq	fb1+8(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	fsync
	movl	$.LC40, %edi
	call	puts
.L103:
	movq	curr_fb(%rip), %rax
	movq	8(%rax), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	close
	cmpl	$-1, %eax
	jne	.L105
	movl	$789, %edx
	movl	$.LC1, %esi
	movl	$.LC41, %edi
	movl	$0, %eax
	call	err_sys
.L105:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	close_file, .-close_file
	.globl	free_handles
	.type	free_handles, @function
free_handles:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	fb1+8(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	close
	movq	fb2+8(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	close
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	free_handles, .-free_handles
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.5020, @object
	.size	__PRETTY_FUNCTION__.5020, 16
__PRETTY_FUNCTION__.5020:
	.string	"reserve_fbuffer"
	.align 8
.LC37:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
