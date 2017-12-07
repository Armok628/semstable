.type	new_bucket, @function
new_bucket:
	# Allocate bucket
	pushq	%rsi
	pushq	%rdi
	movq	$24, %rdi
	call	malloc@plt
	# Set bucket's values
	popq	%rdi
	popq	%rsi
	movq	%rdi, (%rax)
	movq	%rsi, 8(%rax)
	movq	$0, 16(%rax)
	# Return bucket
	ret
