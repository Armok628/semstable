.type	free_table, @function
free_table:
	pushq	%r15
	pushq	%r14
	pushq	%rdi	
	# Initialize non-volatile registers
	movq	(%rdi), %r14 # size
	movq	8(%rdi), %r15 # mem
	pushq	%r15
.free_table_l:
	# Update bucket address
	movq	(%r15), %rdi
	# Call free if there is a bucket there
	cmpq	$0, %rdi
	jz	.free_table_lc
	call	free_bucket
.free_table_lc:
	# If there aren't any more entries, stop
	cmpq	$0, %r14
	jz	.free_table_lb
	# Otherwise, count down and increment the memory pointer
	decq	%r14
	add	$8, %r15
	jmp	.free_table_l
.free_table_lb:
	# Free table memory
	popq	%rdi
	call	free@plt
	# Free table
	popq	%rdi
	call	free@plt
	popq	%r14
	popq	%r15
	ret
