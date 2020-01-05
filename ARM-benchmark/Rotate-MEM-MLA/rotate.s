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
	movi	v1.4s, 0
	mov	x12, 18432
	sub	sp, sp, #64
	.cfi_def_cfa_offset 64
	add	x4, x0, 16
	add	x3, x1, 16
	mov	x2, 0
	movk	x12, 0x1e8, lsl 16
	mov	v0.16b, v1.16b
	.p2align 3,,7
.L2:
	add	x5, x2, 32
	add	x11, x2, 64
	ldr	q7, [x0, x2]
	add	x10, x2, 96
	ldr	q2, [x1, x2]
	add	x9, x2, 128
	ldr	q5, [x4, x2]
	add	x8, x2, 160
	ldr	q18, [x1, x5]
	add	x7, x2, 192
	ldr	q19, [x4, x5]
	add	x6, x2, 224
	fmul	v17.4s, v7.4s, v2.4s
	fmul	v3.4s, v2.4s, v5.4s
	fmul	v20.4s, v18.4s, v19.4s
	ldr	q4, [x3, x2]
	add	x2, x2, 256
	ldr	q6, [x0, x5]
	cmp	x2, x12
	fmla	v3.4s, v7.4s, v4.4s
	fmls	v17.4s, v5.4s, v4.4s
	fmul	v16.4s, v6.4s, v18.4s
	mov	v26.16b, v20.16b
	ldr	q21, [x3, x5]
	ldr	q22, [x0, x11]
	ldr	q23, [x1, x11]
	ldr	q24, [x4, x11]
	fmls	v16.4s, v19.4s, v21.4s
	fmla	v26.4s, v6.4s, v21.4s
	fmul	v27.4s, v22.4s, v23.4s
	fmul	v28.4s, v23.4s, v24.4s
	fadd	v1.4s, v1.4s, v3.4s
	fadd	v0.4s, v0.4s, v17.4s
	ldr	q29, [x3, x11]
	ldr	q30, [x0, x10]
	ldr	q31, [x1, x10]
	ldr	q4, [x4, x10]
	fadd	v3.4s, v1.4s, v26.4s
	fmls	v27.4s, v24.4s, v29.4s
	fmla	v28.4s, v22.4s, v29.4s
	fadd	v17.4s, v0.4s, v16.4s
	fmul	v7.4s, v30.4s, v31.4s
	fmul	v2.4s, v31.4s, v4.4s
	ldr	q6, [x3, x10]
	ldr	q5, [x0, x9]
	ldr	q18, [x1, x9]
	ldr	q20, [x4, x9]
	fadd	v16.4s, v17.4s, v27.4s
	fadd	v21.4s, v3.4s, v28.4s
	fmls	v7.4s, v4.4s, v6.4s
	fmla	v2.4s, v30.4s, v6.4s
	fmul	v22.4s, v5.4s, v18.4s
	fmul	v23.4s, v18.4s, v20.4s
	ldr	q24, [x3, x9]
	ldr	q25, [x0, x8]
	ldr	q1, [x1, x8]
	ldr	q19, [x4, x8]
	fadd	v0.4s, v16.4s, v7.4s
	fmls	v22.4s, v20.4s, v24.4s
	fmla	v23.4s, v5.4s, v24.4s
	fmul	v27.4s, v25.4s, v1.4s
	fadd	v26.4s, v21.4s, v2.4s
	fmul	v28.4s, v1.4s, v19.4s
	ldr	q29, [x3, x8]
	ldr	q30, [x0, x7]
	ldr	q31, [x1, x7]
	ldr	q17, [x4, x7]
	fadd	v3.4s, v0.4s, v22.4s
	fadd	v2.4s, v26.4s, v23.4s
	fmla	v28.4s, v25.4s, v29.4s
	fmls	v27.4s, v19.4s, v29.4s
	fmul	v4.4s, v30.4s, v31.4s
	fmul	v20.4s, v31.4s, v17.4s
	ldr	q6, [x3, x7]
	ldr	q18, [x0, x6]
	ldr	q7, [x1, x6]
	ldr	q16, [x4, x6]
	fmul	v22.4s, v18.4s, v7.4s
	fadd	v5.4s, v3.4s, v27.4s
	fadd	v21.4s, v2.4s, v28.4s
	fmls	v4.4s, v17.4s, v6.4s
	fmla	v20.4s, v30.4s, v6.4s
	fmul	v7.4s, v7.4s, v16.4s
	ldr	q23, [x3, x6]
	fadd	v24.4s, v5.4s, v4.4s
	fadd	v25.4s, v21.4s, v20.4s
	fmls	v22.4s, v16.4s, v23.4s
	fmla	v7.4s, v18.4s, v23.4s
	fadd	v0.4s, v24.4s, v22.4s
	fadd	v1.4s, v25.4s, v7.4s
	bne	.L2
	add	sp, sp, 64
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3987:
	.size	rotate, .-rotate
	.ident	"GCC: (Debian 9.2.1-21) 9.2.1 20191130"
	.section	.note.GNU-stack,"",@progbits
