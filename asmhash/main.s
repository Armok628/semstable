.include "chomp.s"
.include "hash_key.s"
.include "new_table.s"
.include "free_table.s"
.include "new_bucket.s"
.include "free_bucket.s"
.include "get_entry.s" # Untested
.include "set_entry.s" # Untested

form:
	.string	"%s -> %lu\n"
teststr:
	.string "test"

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	# New table
	movq	$8, %rdi
	call	malloc@plt
	pushq	%rax
	movq	$1, %rdi
	call	new_table@plt
	movq	%rax, %r15 # table
	movq	8(%r15), %r14 # mem
	movq	$12345, %rdi
	popq	%rsi
	call	new_bucket@plt
	movq	%rax, (%r14)
	# Get rid of table
	movq	%r15, %rdi
	call	free_table@plt
/*
	# Allocate 100 bytes for string
	movq	$100, %rdi
	call	malloc@plt
	movq	%rax, %rbx # Save address in non-volatile register
	# Read string
	xorq	%rdi, %rdi
	movq	%rbx, %rsi
	movq	$99, %rdx
	call	read@plt
	# Remove trailing newline, get key
	movq	%rbx, %rdi
	call	chomp
	movq	%rbx, %rdi
	call	hash_key
	# Print string and resulting key
	leaq	form(%rip), %rdi
	movq	%rbx, %rsi
	movq	%rax, %rdx
	call	printf@plt
*/
	# Done
	popq	%rbp
	xorq	%rax, %rax
	ret
