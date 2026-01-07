
%ifndef SYS_BRK
	%define SYS_BRK 0x0c
%endif

%ifndef NULL_PTR
	%define NULL_PTR 0x00
%endif

section .text
	global sbrk
	
; Increments the brk and returns the previous brk
; Registers used - rax, rdi
; rdi - increment
; rax - return of previous brk

sbrk:
	;	brk(0)	

	mov rax, SYS_BRK	;	Setup for sys_brk call
	push rdi		;	Prot brk inc in rdi
	mov rdi, NULL_PTR	;	brk(0) ret brk addr
	syscall			;	brk(0)
	
	;	brk(new_addr)
	pop rdi			;	Deprot brk inc in rdi
	add rdi, rax		;	Calc new brk addr
	push rax		;	Prot curr brk addr
	mov rax, SYS_BRK	;	Setup for sys_brk call
	syscall			;	brk(curr + inc)

	;	ret prev_addr
	pop rax			;	Deprot prev brk addr in rax
	ret			;	Return to caller
