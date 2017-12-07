chomp:
	pushq	%rdi
.chomp_l:
	cmpb	$10, (%rdi)
	je	.chomp_x
	cmpb	$0, (%rdi)
	je	.chomp_q
	incq	%rdi
	jmp	.chomp_l
.chomp_x:
	movq	$0, (%rdi)
.chomp_q:
	popq	%rax
	ret
