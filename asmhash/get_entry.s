.type	get_entry, @function
get_entry: # (table_t *table,char *str)
	pushq	%rdi
	movq	%rsi, %rdi
	call	hash_key
	movq	%rax, %rsi # key
	popq	%rdi # table
	movq	(%rdi), %rax # size
	xorq	%rdx, %rdx # 0
	divq	%rsi
	movq	8(%rdi), %rdi # mem
	movq	(%rdi,%rdx,8), %rcx # bucket_t *def
	xorq	%rax, %rax # 0
	.get_entry_l:
	cmpq	$0, %rcx # !def
	jz	.get_entry_e
	cmpq	%rsi, (%rcx) # def->key==key
	je	.get_entry_ret
	movq	16(%rcx), %rcx # def=def->cdr
	jmp	.get_entry_l
	.get_entry_ret:
	movq	8(%rcx), %rax
	.get_entry_e:
	ret
