.type	add_bucket, @function
add_bucket: # (table_t *table, bucket_t *bucket)
	pushq	%rbx
	movq	(%rsi), %rax # bucket->key
	xorq	%rdx, %rdx
	divq	(%rdi) # size
	movq	8(%rdi), %rdi # mem
	leaq	(%rdi,%rdx,8), %rbx # bucket_t **def2
	movq	(%rbx), %rcx # bucket_t *def1
	movq	(%rsi), %rax # bucket->key
	.add_bucket_l:
	cmpq	$0, %rcx # !def1
	jz	.add_bucket_a
	cmpq	%rax, (%rcx) # def1->key==bucket->key
	je	.add_bucket_e
	leaq	16(%rcx), %rbx # def2=&(def1->cdr)
	movq	(%rbx), %rcx # def1=*def2
	jmp	.add_bucket_l
	.add_bucket_e:
	movq	8(%rsi), %rax # bucket->val
	movq	%rax, 8(%rcx) # def1->val=bucket->val
	movq	%rsi, %rax # bucket
	call	free@plt
	movq	%rcx, %rax # def1
	jmp	.add_bucket_ret
	.add_bucket_a:
	movq	%rsi, (%rbx) # *def2=bucket
	movq	%rsi, %rax
	.add_bucket_ret:
	popq	%rbx
	ret
