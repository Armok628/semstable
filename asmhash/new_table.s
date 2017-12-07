.type	new_table, @function
new_table:
	// Allocate table
	pushq	%rdi
	movq	$16, %rdi
	call	malloc@plt
	// Allocate memory
	popq	%rdx
	pushq	%rax
	movq	$24, %rax
	mulq	%rdx
	movq	%rax, %rdi
	call	malloc@plt
	// Assign memory to table
	popq	%rdi
	movq	%rax, 8(%rdi)
	// Return table
	movq	%rdi, %rax
	ret
