.type	free_bucket, @function
free_bucket:
	pushq	%rdi
	movq	8(%rdi), %rdi
	call	free@plt
	popq	%rdi
	call	free@plt
	ret
