.type	add_entry, @function
add_entry: # (table_t *table,char *str,void *val)
	pushq	%rdi
	pushq	%rdx
	movq	%rsi, %rdi
	call	hash_key
	movq	%rax, %rdi
	popq	%rsi
	call	new_bucket
	movq	%rax, %rsi
	popq	%rdi
	call	add_bucket
	ret
