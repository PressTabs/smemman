
%ifndef NULL_PTR
	%define NULL_PTR 0
%endif

;	A memory block is laid out as follows:
;		next:	8 bytes - address of the next mblk's (next). NULL_PTR if there is no next mblk
;		size:	8 bytes - the size of (data)
;		data:	(size) bytes - this is the actual data

;	Memory blocks exist within two lists, the free list and the alloc'd list

section .text
	global mblk_new
	global mblk_next
	global mblk_size
	global mblk_data
	global mblk_merge
	global mblk_split
	global mblk_zero

; void mblk_new(void *addr, void *next, size_t size)
;	Creates a new mblk at %rdi with %rsi (next), and %rdx (size)
mblk_new:
	mov [rdi], rsi		;	Sets (next)
	mov [rdi+8], rdx	;	Sets (size)
	ret			;	Returns

; void *mblk_next(void *addr)
;	Returns the (next) of an mblk at %rdi in %rax
mblk_next:
	mov rax, rdi		;	Gets (next)
	ret			;	Returns

; size_t mblk_size(void *addr)
;	Returns the (size) of an mblk at %rdi in %rax
mblk_size:
	mov rax, [rdi+8]	;	Gets (size)
	ret			;	Returns

; void *mblk_data(void *addr)
;	Returns the (data) of an mblk at %rdi in %rax
mblk_data:
	lea rax, [rdi+16]	;	Calculates addr (data)
	ret			;	Returns

; void *mblk_merge(void *addr)
;	Merges the %rdi mblk with the (next) mblk. Memory blocks must be contiguous. Does not perform list operations. Returns (mblk) in %rax
;	Uses %rsi
mblk_merge:
  mov rax, [rdi]		;	Gets next (mblk)
  push rax          ; Protects %rax
  mov rax, [rax]    ; Gets next (next) addr
  mov [rdi], rax    ; Sets current (next) to next (next)
  pop rax           ; Deprotects %rax
  mov rax, [rax+8]  ; Gets next (size)
  add rax, 16       ; Adds 16 (HEADER_SIZE) to intermediate %rax
  mov [rdi+8], rax  ; Sets current (size) to intermediate %rax
	mov rax, rdi		;	Sets (mblk)
	ret			;	Returns

; void *mblk_split(void *addr, size_t resize)
;	Splits the %rdi mblk into two contiguous mblk(s). New size of %rdi mblk specified by %rsi. Returns the new (mblk) in %rax
;	The %rdi mblk must originally have a (size) of at least 17 for the %rax mblk to form without memory corruption
mblk_split:
	lea rax, [rdi+16+rsi]	;	Calculates next (mblk)
  push rdi          ; Protects %rdi
  mov rdi, [rdi]    ; Gets current (next)
	mov [rax], rdi	;	Sets next (next) to current (mblk)
  pop rdi         ; Deprotects %rdi
	mov [rdi], rax		;	Sets current (next) to next (mblk)
  push rdi          ; Protects %rdi
  mov rdi, [rdi+8]  ; Gets current (size)
	mov [rax+8], rdi	;	Sets next (size) to current (size)
	sub [rax+8], rsi	;	Subtracts %rsi (resize) from next (size)
	sub [rax+8], 16		;	Subtracts 16 (HEADER_SIZE) from next (size)
  pop rdi           ; Deprotects %rdi
	mov [rdi+8], rsi	;	Sets current (size) to %rsi (size)
	ret			;	Returns

; void *mblk_zero(void *addr)
;	Zeroes out the (data) in the %rdi mblk. Returns the end of the mblk in %rax. Requires (size) > 0. Zeroes %rcx
mblk_zero:
	lea rax, [rdi+16]	;	Calculates (data)
	mov rcx, [rdi+8]	;	Sets %rcx to (size)
	.loop:			;	Initializes loop
	mov byte [rax], 0	;	Zeroes the byte at %rax
	inc rax			;	Moves %rax to the next byte
	loop .loop		;	Loops
	ret			;	Returns
