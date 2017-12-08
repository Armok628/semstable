.include "chomp.s"
.include "hash_key.s"
.include "new_table.s"
.include "free_table.s"
.include "new_bucket.s"
.include "free_bucket.s"
.include "get_entry.s"
.include "set_entry.s"
.include "add_bucket.s"
.include "add_entry.s"

form:
	.string	"%s -> %lu\n"
ss_add:
	.string "add %s %s"
ss_set:
	.string	"set %s %s"
ss_get:
	.string "get %s"

fs_undef:
	.string "%s is undefined\n"

teststr:
	.string "test"
test2str:
	.string "test2"

.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	# Allocate memory for strings and table
	movq	$100, %rdi
	call	new_table@plt
	movq	%rax, %r15
	movq	$100, %rdi
	call	malloc@plt
	movq	%rax, %r12
	movq	$100, %rdi
	call	malloc@plt
	movq	%rax, %r13
	movq	$100, %rdi
	call	malloc@plt
	movq	%rax, %r14
.main_loop:
	# Read input
	xorq	%rdi, %rdi
	movq	%r12, %rsi
	movq	$99, %rdx
	call	read@plt
	# Scan for appropriate commands
	movq	%r12, %rdi
	leaq	ss_add(%rip), %rsi
	movq	%r13, %rdx
	movq	%r14, %rcx
	call	sscanf@plt
	cmpq	$2, %rax
	je	.main_add
	movq	%r12, %rdi
	leaq	ss_set(%rip), %rsi
	movq	%r13, %rdx
	movq	%r14, %rcx
	call	sscanf@plt
	cmpq	$2, %rax
	je	.main_set
	movq	%r12, %rdi
	leaq	ss_get(%rip), %rsi
	movq	%r13, %rdx
	call	sscanf@plt
	cmpq	$1, %rax
	je	.main_get
	jmp	.main_loop
.main_add:
	movq	%r15, %rdi
	movq	%r13, %rsi
	movq	%r14, %rdx
	call	add_entry
	jmp	.main_loop
.main_set:
	movq	%r15, %rdi
	movq	%r13, %rsi
	movq	%r14, %rdx
	call	set_entry
	jmp	.main_loop
.main_get:
	movq	%r15, %rdi
	movq	%r13, %rsi
	call	get_entry
	cmpq	$0, %rax
	jz	.main_undef
	movq	%rax, %rdi
	call	puts@plt
	jmp	.main_loop
.main_undef:
	leaq	fs_undef(%rip), %rdi
	movq	%r13, %rsi
	call	printf@plt
	jmp	.main_loop

/*	# Functions test
	# Make a new table, send to rbx
	movq	$1, %rdi
	call	new_table
	movq	%rax, %rbx
	# Add an entry as teststr -> "test"
	movq	%rbx, %rdi
	leaq	teststr(%rip), %rsi
	movq	%rsi, %rdx
	call	add_entry
	# Retrieve the entry for teststr and print
	movq	%rbx, %rdi
	leaq	teststr(%rip), %rsi
	call	get_entry
	movq	%rax, %rdi
	call	puts@plt
	# Set the entry for teststr to test2str
	movq	%rbx, %rdi
	leaq	teststr(%rip), %rsi
	leaq	test2str(%rip), %rdx
	call	set_entry
	# Retrieve the entry for teststr and print (copy)
	movq	%rbx, %rdi
	leaq	teststr(%rip), %rsi
	call	get_entry
	movq	%rax, %rdi
	call	puts@plt
	# Not freeing memory; free functions expect malloc'd vals
	# Must free manually here; doesn't serve testing purposes
*/

/*	# Old key test
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
	leave
	ret
