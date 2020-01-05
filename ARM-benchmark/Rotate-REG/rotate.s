	.arch armv8-a+crc
	.file	"rotate.c"
	.text
	.align	2
	.p2align 3,,7
	.global	rotate
	.type	rotate, %function
rotate:
.LFB3987:
	.cfi_startproc
	mov	w0, 16960
	sub	sp, sp, #128
	.cfi_def_cfa_offset 128
	movk	w0, 0xf, lsl 16
	.p2align 3,,7
.L2:
	fmul	v4.4s, v2.4s, v0.4s
	subs	w0, w0, #8
	fmul	v5.4s, v2.4s, v1.4s
	fmls	v4.4s, v3.4s, v1.4s
	fmla	v5.4s, v3.4s, v0.4s
	fmul	v0.4s, v2.4s, v4.4s
	fmul	v1.4s, v2.4s, v5.4s
	fmls	v0.4s, v3.4s, v5.4s
	fmla	v1.4s, v3.4s, v4.4s
	fmul	v6.4s, v2.4s, v0.4s
	fmul	v7.4s, v2.4s, v1.4s
	fmls	v6.4s, v3.4s, v1.4s
	fmla	v7.4s, v3.4s, v0.4s
	fmul	v16.4s, v2.4s, v6.4s
	fmul	v17.4s, v2.4s, v7.4s
	fmls	v16.4s, v3.4s, v7.4s
	fmla	v17.4s, v3.4s, v6.4s
	fmul	v18.4s, v2.4s, v16.4s
	fmul	v19.4s, v2.4s, v17.4s
	fmls	v18.4s, v3.4s, v17.4s
	fmla	v19.4s, v3.4s, v16.4s
	fmul	v20.4s, v2.4s, v18.4s
	fmul	v21.4s, v2.4s, v19.4s
	fmls	v20.4s, v3.4s, v19.4s
	fmla	v21.4s, v3.4s, v18.4s
	fmul	v22.4s, v2.4s, v20.4s
	fmul	v23.4s, v2.4s, v21.4s
	fmls	v22.4s, v3.4s, v21.4s
	fmla	v23.4s, v3.4s, v20.4s
	fmul	v0.4s, v2.4s, v22.4s
	fmul	v1.4s, v2.4s, v23.4s
	fmls	v0.4s, v3.4s, v23.4s
	fmla	v1.4s, v3.4s, v22.4s
	bne	.L2
	add	sp, sp, 128
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3987:
	.size	rotate, .-rotate
	.ident	"GCC: (Debian 9.2.1-21) 9.2.1 20191130"
	.section	.note.GNU-stack,"",@progbits
