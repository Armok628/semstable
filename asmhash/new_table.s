.type	new_table, @function
new_table:
	pushq	%rbx
	movq	%rdi, %rbx
	movq	$16, %rdi
	call	malloc@plt
	movq	%rbx, (%rax)
	movq	%rbx, %rdx
	movq	%rax, %rbx
	movq	$24, %rax
	mulq	%rdx
	movq	%rax, %rdi
	call	malloc@plt
	movq	%rax, 8(%rbx)
	movq	%rbx, %rax
	popq	%rbx
	ret
