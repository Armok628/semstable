.type	hash_key, @function
hash_key:
	pushq	%rdi	
	xorq	%rax, %rax
	xorq	%rdx, %rdx
.hash_key_l:
	movsbq	(%rdi), %rcx
	cmpb	$0, %cl
	jz	.hash_key_q
	addq	%rcx, %rax
	mulq	%rcx
	shlq	$8, %rax
	divq	%rcx
	incq	%rdi
	jmp	.hash_key_l
.hash_key_q:
	popq	%rdi
	ret
