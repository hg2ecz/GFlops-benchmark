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
	mov	x9, 18432
	add	x3, x0, 16
	add	x8, x1, 16
	mov	x2, 0
	movk	x9, 0x1e8, lsl 16
	.p2align 3,,7
.L2:
	ldr	q2, [x0, x2]
	add	x4, x2, 32
	ldr	q1, [x1, x2]
	add	x7, x2, 64
	ldr	q0, [x3, x2]
	add	x6, x2, 96
	fmul	v4.4s, v2.4s, v1.4s
	add	x5, x2, 128
	ldr	q3, [x8, x2]
	fmls	v4.4s, v0.4s, v3.4s
	str	q4, [x0, x2]
	ldr	q5, [x1, x2]
	fmul	v6.4s, v0.4s, v5.4s
	fmla	v6.4s, v3.4s, v2.4s
	str	q6, [x3, x2]
	add	x2, x2, 160
	cmp	x2, x9
	ldr	q7, [x0, x4]
	ldr	q16, [x1, x4]
	ldr	q17, [x3, x4]
	fmul	v18.4s, v7.4s, v16.4s
	ldr	q19, [x8, x4]
	fmls	v18.4s, v17.4s, v19.4s
	str	q18, [x0, x4]
	ldr	q20, [x1, x4]
	fmul	v21.4s, v17.4s, v20.4s
	fmla	v21.4s, v19.4s, v7.4s
	str	q21, [x3, x4]
	ldr	q22, [x0, x7]
	ldr	q23, [x1, x7]
	ldr	q24, [x3, x7]
	fmul	v25.4s, v22.4s, v23.4s
	ldr	q26, [x8, x7]
	fmls	v25.4s, v24.4s, v26.4s
	str	q25, [x0, x7]
	ldr	q27, [x1, x7]
	fmul	v28.4s, v24.4s, v27.4s
	fmla	v28.4s, v26.4s, v22.4s
	str	q28, [x3, x7]
	ldr	q29, [x0, x6]
	ldr	q30, [x1, x6]
	ldr	q31, [x3, x6]
	fmul	v2.4s, v29.4s, v30.4s
	ldr	q4, [x8, x6]
	fmls	v2.4s, v31.4s, v4.4s
	str	q2, [x0, x6]
	ldr	q1, [x1, x6]
	fmul	v0.4s, v31.4s, v1.4s
	fmla	v0.4s, v4.4s, v29.4s
	str	q0, [x3, x6]
	ldr	q5, [x0, x5]
	ldr	q3, [x1, x5]
	ldr	q6, [x3, x5]
	fmul	v7.4s, v5.4s, v3.4s
	ldr	q16, [x8, x5]
	fmls	v7.4s, v6.4s, v16.4s
	str	q7, [x0, x5]
	ldr	q17, [x1, x5]
	fmul	v18.4s, v6.4s, v17.4s
	fmla	v18.4s, v16.4s, v5.4s
	str	q18, [x3, x5]
	bne	.L2
	ret
	.cfi_endproc
.LFE3987:
	.size	rotate, .-rotate
	.ident	"GCC: (Debian 9.2.1-21) 9.2.1 20191130"
	.section	.note.GNU-stack,"",@progbits
