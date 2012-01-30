	.file	"bitmask.c"
	.text
	.p2align 2,,3
.globl bitmask_create
	.type	bitmask_create,@function
bitmask_create:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$24, %esp
	pushl	$12
	movl	8(%ebp), %edi
	movl	12(%ebp), %esi
	call	malloc
	movl	%eax, %ebx
	addl	$16, %esp
	xorl	%eax, %eax
	testl	%ebx, %ebx
	je	.L1
	leal	-1(%edi), %eax
	shrl	$5, %eax
	subl	$8, %esp
	incl	%eax
	pushl	$4
	movl	%esi, 4(%ebx)
	imull	%eax, %esi
	pushl	%esi
	movl	%edi, (%ebx)
	call	calloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, 8(%ebx)
	je	.L5
	movl	%ebx, %eax
.L1:
	leal	-12(%ebp), %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.L5:
	subl	$12, %esp
	pushl	%ebx
	call	free
	xorl	%eax, %eax
	jmp	.L1
.Lfe1:
	.size	bitmask_create,.Lfe1-bitmask_create
	.p2align 2,,3
.globl bitmask_free
	.type	bitmask_free,@function
bitmask_free:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
	movl	8(%ebp), %ebx
	pushl	8(%ebx)
	call	free
	movl	%ebx, 8(%ebp)
	addl	$16, %esp
	movl	-4(%ebp), %ebx
	leave
	jmp	free
.Lfe2:
	.size	bitmask_free,.Lfe2-bitmask_free
	.p2align 2,,3
.globl bitmask_clear
	.type	bitmask_clear,@function
bitmask_clear:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	8(%ebp), %edx
	movl	(%edx), %eax
	decl	%eax
	shrl	$5, %eax
	leal	4(,%eax,4), %eax
	imull	4(%edx), %eax
	pushl	%eax
	pushl	$0
	pushl	8(%edx)
	call	memset
	addl	$16, %esp
	leave
	ret
.Lfe3:
	.size	bitmask_clear,.Lfe3-bitmask_clear
	.p2align 2,,3
.globl bitmask_fill
	.type	bitmask_fill,@function
bitmask_fill:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	8(%ebp), %edx
	movl	(%edx), %eax
	decl	%eax
	shrl	$5, %eax
	leal	4(,%eax,4), %eax
	imull	4(%edx), %eax
	pushl	%eax
	pushl	$255
	pushl	8(%edx)
	call	memset
	addl	$16, %esp
	leave
	ret
.Lfe4:
	.size	bitmask_fill,.Lfe4-bitmask_fill
	.p2align 2,,3
.globl bitmask_overlap
	.type	bitmask_overlap,@function
bitmask_overlap:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$40, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %esi
	movl	16(%ebp), %ebx
	movl	20(%ebp), %ecx
	.p2align 2,,3
.L122:
	movl	(%edx), %edi
	cmpl	%edi, %ebx
	jge	.L138
	movl	4(%edx), %eax
	cmpl	%eax, %ecx
	movl	%eax, -40(%ebp)
	jge	.L138
	movl	4(%esi), %eax
	movl	%eax, -44(%ebp)
	negl	%eax
	cmpl	%eax, %ecx
	jle	.L138
	testl	%ebx, %ebx
	jns	.L148
	movl	%edx, %eax
	negl	%ebx
	movl	%esi, %edx
	negl	%ecx
	movl	%eax, %esi
	jmp	.L122
.L148:
	testl	%ecx, %ecx
	js	.L61
	movl	%ebx, %eax
	shrl	$5, %eax
	imull	-40(%ebp), %eax
	addl	%ecx, %eax
	movl	8(%edx), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	movl	-40(%ebp), %eax
	subl	%ecx, %eax
	cmpl	-44(%ebp), %eax
	jle	.L62
	movl	-44(%ebp), %eax
.L62:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %eax
	movl	8(%esi), %ecx
	movl	%eax, -20(%ebp)
.L147:
	movl	%ebx, %eax
	andl	$31, %eax
	movl	%ecx, -24(%ebp)
	movl	%eax, -28(%ebp)
	je	.L65
	movl	$32, -32(%ebp)
	subl	%eax, -32(%ebp)
	movl	(%esi), %eax
	decl	%edi
	decl	%eax
	shrl	$5, %edi
	shrl	$5, %ebx
	shrl	$5, %eax
	subl	%ebx, %edi
	leal	1(%eax), %esi
	cmpl	%edi, %esi
	jle	.L66
	movl	$0, -36(%ebp)
	cmpl	%edi, -36(%ebp)
	jge	.L124
	movl	-40(%ebp), %esi
	sall	$2, %esi
.L83:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	movl	-24(%ebp), %ecx
	cmpl	%eax, %edx
	movl	%ecx, -52(%ebp)
	jae	.L126
.L76:
	movl	(%edx), %eax
	movb	-28(%ebp), %cl
	shrl	%cl, %eax
	movl	-52(%ebp), %ecx
	andl	(%ecx), %eax
	addl	$4, %edx
	addl	$4, %ecx
	testl	%eax, %eax
	movl	%ecx, -52(%ebp)
	movl	$1, %ebx
	jne	.L57
	cmpl	-20(%ebp), %edx
	jb	.L76
.L126:
	addl	%esi, -16(%ebp)
	addl	%esi, -20(%ebp)
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	cmpl	%ecx, %edx
	movl	%eax, -52(%ebp)
	jae	.L128
.L82:
	movl	(%edx), %eax
	movb	-32(%ebp), %cl
	sall	%cl, %eax
	movl	-52(%ebp), %ecx
	andl	(%ecx), %eax
	addl	$4, %edx
	addl	$4, %ecx
	testl	%eax, %eax
	movl	%ecx, -52(%ebp)
	movl	$1, %ebx
	jne	.L57
	cmpl	-20(%ebp), %edx
	jb	.L82
.L128:
	movl	-24(%ebp), %edx
	movl	-44(%ebp), %eax
	incl	-36(%ebp)
	leal	(%edx,%eax,4), %edx
	cmpl	%edi, -36(%ebp)
	movl	%edx, -24(%ebp)
	jl	.L83
.L124:
	movl	-16(%ebp), %edx
	movl	-24(%ebp), %ecx
	cmpl	-20(%ebp), %edx
	movl	%ecx, -52(%ebp)
	jae	.L138
.L89:
	movl	(%edx), %eax
	movb	-28(%ebp), %cl
	shrl	%cl, %eax
	movl	-52(%ebp), %ecx
	andl	(%ecx), %eax
	addl	$4, %edx
	addl	$4, %ecx
	testl	%eax, %eax
	movl	%ecx, -52(%ebp)
	movl	$1, %ebx
	jne	.L57
	cmpl	-20(%ebp), %edx
	jb	.L89
.L138:
	xorl	%ebx, %ebx
.L57:
	addl	$40, %esp
	movl	%ebx, %eax
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.L66:
	movl	$0, -36(%ebp)
	cmpl	%esi, -36(%ebp)
	jge	.L138
	movl	-40(%ebp), %edi
	sall	$2, %edi
.L107:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	cmpl	%ecx, %edx
	movl	%eax, -52(%ebp)
	jae	.L134
.L100:
	movl	(%edx), %eax
	movb	-28(%ebp), %cl
	shrl	%cl, %eax
	movl	-52(%ebp), %ecx
	andl	(%ecx), %eax
	addl	$4, %edx
	addl	$4, %ecx
	testl	%eax, %eax
	movl	%ecx, -52(%ebp)
	movl	$1, %ebx
	jne	.L57
	cmpl	-20(%ebp), %edx
	jb	.L100
.L134:
	addl	%edi, -16(%ebp)
	addl	%edi, -20(%ebp)
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	cmpl	%ecx, %edx
	movl	%eax, -52(%ebp)
	jae	.L136
.L106:
	movl	(%edx), %eax
	movb	-32(%ebp), %cl
	sall	%cl, %eax
	movl	-52(%ebp), %ecx
	andl	(%ecx), %eax
	addl	$4, %edx
	addl	$4, %ecx
	testl	%eax, %eax
	movl	%ecx, -52(%ebp)
	movl	$1, %ebx
	jne	.L57
	cmpl	-20(%ebp), %edx
	jb	.L106
.L136:
	movl	-24(%ebp), %edx
	movl	-44(%ebp), %eax
	incl	-36(%ebp)
	leal	(%edx,%eax,4), %edx
	cmpl	%esi, -36(%ebp)
	movl	%edx, -24(%ebp)
	jl	.L107
	jmp	.L138
	.p2align 2,,3
.L65:
	movl	%edi, %eax
	subl	%ebx, %eax
	movl	(%esi), %edx
	cmpl	%edx, %eax
	jle	.L109
	movl	%edx, %eax
.L109:
	decl	%eax
	shrl	$5, %eax
	leal	1(%eax), %edi
	movl	$0, -36(%ebp)
	cmpl	%edi, -36(%ebp)
	jge	.L138
	movl	-44(%ebp), %ecx
	movl	-40(%ebp), %esi
	sall	$2, %ecx
	sall	$2, %esi
	movl	%ecx, -48(%ebp)
.L120:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	cmpl	%ecx, %edx
	movl	%eax, -52(%ebp)
	jae	.L140
	.p2align 2,,3
.L119:
	movl	-52(%ebp), %ecx
	movl	(%ecx), %eax
	andl	(%edx), %eax
	addl	$4, %ecx
	addl	$4, %edx
	testl	%eax, %eax
	movl	%ecx, -52(%ebp)
	movl	$1, %ebx
	jne	.L57
	cmpl	-20(%ebp), %edx
	jb	.L119
.L140:
	movl	-48(%ebp), %eax
	incl	-36(%ebp)
	addl	%esi, -16(%ebp)
	addl	%esi, -20(%ebp)
	addl	%eax, -24(%ebp)
	cmpl	%edi, -36(%ebp)
	jl	.L120
	jmp	.L138
.L61:
	movl	%ebx, %eax
	shrl	$5, %eax
	imull	-40(%ebp), %eax
	movl	8(%edx), %edx
	leal	(%edx,%eax,4), %eax
	movl	-44(%ebp), %edx
	movl	%eax, -16(%ebp)
	addl	%ecx, %edx
	movl	-40(%ebp), %eax
	cmpl	%edx, %eax
	jle	.L64
	movl	%edx, %eax
.L64:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	leal	0(,%ecx,4), %eax
	movl	8(%esi), %ecx
	subl	%eax, %ecx
	jmp	.L147
.Lfe5:
	.size	bitmask_overlap,.Lfe5-bitmask_overlap
	.p2align 2,,3
.globl bitmask_overlap_pos
	.type	bitmask_overlap_pos,@function
bitmask_overlap_pos:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$72, %esp
	movl	8(%ebp), %esi
	movl	16(%ebp), %ebx
	movl	(%esi), %ecx
	cmpl	%ecx, %ebx
	movl	20(%ebp), %edi
	jge	.L244
	movl	4(%esi), %eax
	cmpl	%eax, %edi
	movl	%eax, -48(%ebp)
	jge	.L244
	movl	12(%ebp), %edx
	movl	4(%edx), %edx
	movl	%edx, %eax
	negl	%eax
	cmpl	%eax, %edi
	movl	%edx, -52(%ebp)
	jle	.L244
	testl	%ebx, %ebx
	js	.L152
	movl	%ebx, %eax
	shrl	$5, %eax
	testl	%edi, %edi
	movl	%eax, -44(%ebp)
	js	.L153
	movl	-48(%ebp), %eax
	imull	-44(%ebp), %eax
	addl	%edi, %eax
	movl	8(%esi), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	movl	-48(%ebp), %eax
	subl	%edi, %eax
	cmpl	-52(%ebp), %eax
	jle	.L154
	movl	-52(%ebp), %eax
.L154:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %esi
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, -20(%ebp)
.L155:
	movl	%ebx, %eax
	andl	$31, %eax
	movl	%eax, -24(%ebp)
	je	.L157
	movl	$32, -28(%ebp)
	movl	12(%ebp), %edx
	subl	%eax, -28(%ebp)
	movl	(%edx), %eax
	decl	%ecx
	decl	%eax
	shrl	$5, %ecx
	shrl	$5, %ebx
	shrl	$5, %eax
	subl	%ebx, %ecx
	incl	%eax
	cmpl	%ecx, %eax
	movl	%ecx, -36(%ebp)
	movl	%eax, -40(%ebp)
	jle	.L158
	movl	$0, -32(%ebp)
	cmpl	%ecx, -32(%ebp)
	jge	.L248
.L185:
	movl	-16(%ebp), %ecx
	cmpl	%esi, %ecx
	movl	%ecx, -84(%ebp)
	movl	-20(%ebp), %edx
	jae	.L250
	movl	-44(%ebp), %eax
	addl	-32(%ebp), %eax
	movl	%eax, -56(%ebp)
	.p2align 2,,3
.L173:
	movb	-24(%ebp), %cl
	movl	(%edx), %eax
	sall	%cl, %eax
	movl	-84(%ebp), %ecx
	movl	(%ecx), %ebx
	andl	%eax, %ebx
	jne	.L283
	addl	$4, -84(%ebp)
	addl	$4, %edx
	cmpl	%esi, -84(%ebp)
	jb	.L173
.L250:
	movl	-48(%ebp), %eax
	sall	$2, %eax
	addl	%eax, -16(%ebp)
	addl	%eax, %esi
	movl	-16(%ebp), %eax
	cmpl	%esi, %eax
	movl	%eax, -84(%ebp)
	movl	-20(%ebp), %edx
	jae	.L254
	movl	-44(%ebp), %ecx
	addl	-32(%ebp), %ecx
	movl	%ecx, -60(%ebp)
	.p2align 2,,3
.L184:
	movb	-28(%ebp), %cl
	movl	(%edx), %eax
	shrl	%cl, %eax
	movl	-84(%ebp), %ecx
	movl	(%ecx), %ebx
	andl	%eax, %ebx
	jne	.L284
	addl	$4, -84(%ebp)
	addl	$4, %edx
	cmpl	%esi, -84(%ebp)
	jb	.L184
.L254:
	movl	-20(%ebp), %edx
	movl	-52(%ebp), %eax
	incl	-32(%ebp)
	movl	-36(%ebp), %ecx
	leal	(%edx,%eax,4), %edx
	cmpl	%ecx, -32(%ebp)
	movl	%edx, -20(%ebp)
	jl	.L185
.L248:
	movl	-16(%ebp), %eax
	cmpl	%esi, %eax
	movl	%eax, -84(%ebp)
	movl	-20(%ebp), %edx
	jae	.L244
	movl	-44(%ebp), %ecx
	addl	-36(%ebp), %ecx
	movl	%ecx, -64(%ebp)
.L196:
	movb	-24(%ebp), %cl
	movl	(%edx), %eax
	sall	%cl, %eax
	movl	-84(%ebp), %ecx
	movl	(%ecx), %ebx
	andl	%eax, %ebx
	jne	.L285
	addl	$4, -84(%ebp)
	addl	$4, %edx
	cmpl	%esi, -84(%ebp)
	jb	.L196
	.p2align 2,,3
.L244:
	xorl	%eax, %eax
.L149:
	leal	-12(%ebp), %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.L285:
	movl	-16(%ebp), %eax
	subl	%eax, %ecx
	sarl	$2, %ecx
	movl	%ecx, %eax
	addl	%edi, %eax
	movl	28(%ebp), %edx
	movl	-64(%ebp), %ecx
	movl	%eax, (%edx)
	sall	$5, %ecx
	xorl	%eax, %eax
	testl	$1, %ebx
	movl	%ebx, %edx
	jne	.L276
.L194:
	shrl	$1, %edx
	incl	%eax
	testl	$1, %edx
	je	.L194
.L276:
	leal	(%eax,%ecx), %eax
.L282:
	movl	24(%ebp), %ecx
	movl	%eax, (%ecx)
.L277:
	movl	$1, %eax
	jmp	.L149
.L284:
	movl	-16(%ebp), %eax
	subl	%eax, %ecx
	sarl	$2, %ecx
	movl	%ecx, %eax
	addl	%edi, %eax
	movl	28(%ebp), %edx
	movl	%eax, (%edx)
	movl	-60(%ebp), %ecx
	movl	%ebx, %eax
	sall	$5, %ecx
	xorl	%edx, %edx
	testl	$1, %eax
	jne	.L270
.L182:
	shrl	$1, %eax
	incl	%edx
	testl	$1, %eax
	je	.L182
.L270:
	leal	32(%edx,%ecx), %eax
	jmp	.L282
.L283:
	movl	-16(%ebp), %eax
	subl	%eax, %ecx
	sarl	$2, %ecx
	movl	%ecx, %eax
	addl	%edi, %eax
	movl	28(%ebp), %edx
	movl	-56(%ebp), %ecx
	movl	%eax, (%edx)
	sall	$5, %ecx
	xorl	%eax, %eax
	testl	$1, %ebx
	movl	%ebx, %edx
	jne	.L276
.L171:
	shrl	$1, %edx
	incl	%eax
	testl	$1, %edx
	je	.L171
	jmp	.L276
.L158:
	movl	$0, -32(%ebp)
	movl	-40(%ebp), %eax
	cmpl	%eax, -32(%ebp)
	jge	.L244
.L224:
	movl	-16(%ebp), %edx
	cmpl	%esi, -16(%ebp)
	movl	%edx, -84(%ebp)
	movl	-20(%ebp), %edx
	jae	.L264
	movl	-44(%ebp), %ecx
	addl	-32(%ebp), %ecx
	movl	%ecx, -68(%ebp)
	.p2align 2,,3
.L212:
	movb	-24(%ebp), %cl
	movl	(%edx), %eax
	sall	%cl, %eax
	movl	-84(%ebp), %ecx
	movl	(%ecx), %ebx
	andl	%eax, %ebx
	jne	.L286
	addl	$4, -84(%ebp)
	addl	$4, %edx
	cmpl	%esi, -84(%ebp)
	jb	.L212
.L264:
	movl	-48(%ebp), %eax
	sall	$2, %eax
	addl	%eax, -16(%ebp)
	addl	%eax, %esi
	movl	-16(%ebp), %eax
	cmpl	%esi, %eax
	movl	%eax, -84(%ebp)
	movl	-20(%ebp), %edx
	jae	.L268
	movl	-44(%ebp), %ecx
	addl	-32(%ebp), %ecx
	movl	%ecx, -72(%ebp)
	.p2align 2,,3
.L223:
	movb	-28(%ebp), %cl
	movl	(%edx), %eax
	shrl	%cl, %eax
	movl	-84(%ebp), %ecx
	movl	(%ecx), %ebx
	andl	%eax, %ebx
	jne	.L287
	addl	$4, -84(%ebp)
	addl	$4, %edx
	cmpl	%esi, -84(%ebp)
	jb	.L223
.L268:
	movl	-20(%ebp), %edx
	movl	-52(%ebp), %eax
	incl	-32(%ebp)
	movl	-40(%ebp), %ecx
	leal	(%edx,%eax,4), %edx
	cmpl	%ecx, -32(%ebp)
	movl	%edx, -20(%ebp)
	jl	.L224
	jmp	.L244
.L287:
	movl	-16(%ebp), %eax
	subl	%eax, %ecx
	sarl	$2, %ecx
	movl	%ecx, %eax
	addl	%edi, %eax
	movl	28(%ebp), %edx
	movl	%eax, (%edx)
	movl	-72(%ebp), %ecx
	movl	%ebx, %eax
	sall	$5, %ecx
	xorl	%edx, %edx
	testl	$1, %eax
	jne	.L270
.L221:
	shrl	$1, %eax
	incl	%edx
	testl	$1, %eax
	je	.L221
	jmp	.L270
.L286:
	movl	-16(%ebp), %eax
	subl	%eax, %ecx
	sarl	$2, %ecx
	movl	%ecx, %eax
	addl	%edi, %eax
	movl	28(%ebp), %edx
	movl	-68(%ebp), %ecx
	movl	%eax, (%edx)
	sall	$5, %ecx
	xorl	%eax, %eax
	testl	$1, %ebx
	movl	%ebx, %edx
	jne	.L276
.L210:
	shrl	$1, %edx
	incl	%eax
	testl	$1, %edx
	je	.L210
	jmp	.L276
	.p2align 2,,3
.L157:
	movl	%ecx, %eax
	movl	12(%ebp), %ecx
	subl	%ebx, %eax
	movl	(%ecx), %edx
	cmpl	%edx, %eax
	jle	.L226
	movl	%edx, %eax
.L226:
	decl	%eax
	shrl	$5, %eax
	incl	%eax
	movl	$0, -32(%ebp)
	cmpl	%eax, -32(%ebp)
	movl	%eax, -36(%ebp)
	jge	.L244
	movl	-48(%ebp), %eax
	sall	$2, %eax
	movl	%eax, -80(%ebp)
.L242:
	movl	-16(%ebp), %edx
	cmpl	%esi, -16(%ebp)
	movl	%edx, -84(%ebp)
	movl	-20(%ebp), %edx
	jae	.L274
	movl	-44(%ebp), %ecx
	addl	-32(%ebp), %ecx
	movl	%ecx, -76(%ebp)
	.p2align 2,,3
.L241:
	movl	-84(%ebp), %ecx
	movl	(%edx), %eax
	movl	(%ecx), %ebx
	andl	%eax, %ebx
	jne	.L288
	addl	$4, -84(%ebp)
	addl	$4, %edx
	cmpl	%esi, -84(%ebp)
	jb	.L241
.L274:
	movl	-80(%ebp), %eax
	addl	%eax, -16(%ebp)
	addl	%eax, %esi
	movl	-20(%ebp), %ecx
	movl	-52(%ebp), %edx
	incl	-32(%ebp)
	movl	-36(%ebp), %eax
	leal	(%ecx,%edx,4), %ecx
	cmpl	%eax, -32(%ebp)
	movl	%ecx, -20(%ebp)
	jl	.L242
	jmp	.L244
.L288:
	movl	-16(%ebp), %eax
	subl	%eax, %ecx
	sarl	$2, %ecx
	movl	%ecx, %eax
	addl	%edi, %eax
	movl	28(%ebp), %edx
	movl	-76(%ebp), %ecx
	movl	%eax, (%edx)
	sall	$5, %ecx
	xorl	%eax, %eax
	testl	$1, %ebx
	movl	%ebx, %edx
	jne	.L276
	.p2align 2,,3
.L239:
	shrl	$1, %edx
	incl	%eax
	testl	$1, %edx
	je	.L239
	jmp	.L276
.L153:
	movl	-48(%ebp), %edx
	imull	-44(%ebp), %edx
	movl	8(%esi), %eax
	leal	(%eax,%edx,4), %edx
	movl	%edx, -16(%ebp)
	movl	-52(%ebp), %edx
	addl	%edi, %edx
	movl	-48(%ebp), %eax
	cmpl	%edx, %eax
	jle	.L156
	movl	%edx, %eax
.L156:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %esi
	movl	12(%ebp), %edx
	leal	0(,%edi,4), %eax
	movl	8(%edx), %edx
	subl	%eax, %edx
	movl	%edx, -20(%ebp)
	xorl	%edi, %edi
	jmp	.L155
.L152:
	subl	$8, %esp
	pushl	28(%ebp)
	movl	%edi, %eax
	pushl	24(%ebp)
	negl	%eax
	pushl	%eax
	movl	%ebx, %eax
	negl	%eax
	pushl	%eax
	pushl	%esi
	pushl	12(%ebp)
	call	bitmask_overlap_pos
	addl	$32, %esp
	testl	%eax, %eax
	je	.L244
	movl	24(%ebp), %edx
	addl	%ebx, (%edx)
	movl	28(%ebp), %ecx
	addl	%edi, (%ecx)
	jmp	.L277
.Lfe6:
	.size	bitmask_overlap_pos,.Lfe6-bitmask_overlap_pos
	.p2align 2,,3
.globl bitmask_overlap_area
	.type	bitmask_overlap_area,@function
bitmask_overlap_area:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$40, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %esi
	movl	16(%ebp), %ebx
	movl	20(%ebp), %ecx
	.p2align 2,,3
.L342:
	movl	(%edx), %edi
	cmpl	%edi, %ebx
	movl	$0, -36(%ebp)
	jge	.L291
	movl	4(%edx), %eax
	cmpl	%eax, %ecx
	movl	%eax, -40(%ebp)
	jge	.L291
	movl	4(%esi), %eax
	movl	%eax, -44(%ebp)
	negl	%eax
	cmpl	%eax, %ecx
	jle	.L291
	testl	%ebx, %ebx
	jns	.L358
	movl	%edx, %eax
	negl	%ebx
	movl	%esi, %edx
	negl	%ecx
	movl	%eax, %esi
	jmp	.L342
.L358:
	testl	%ecx, %ecx
	js	.L293
	movl	%ebx, %eax
	shrl	$5, %eax
	imull	-40(%ebp), %eax
	addl	%ecx, %eax
	movl	8(%edx), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	movl	-40(%ebp), %eax
	subl	%ecx, %eax
	cmpl	-44(%ebp), %eax
	jle	.L294
	movl	-44(%ebp), %eax
.L294:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %eax
	movl	8(%esi), %ecx
	movl	%eax, -20(%ebp)
.L357:
	movl	%ebx, %eax
	andl	$31, %eax
	movl	%ecx, -24(%ebp)
	movl	%eax, -28(%ebp)
	je	.L297
	movl	$32, -32(%ebp)
	movl	(%esi), %edx
	subl	%eax, -32(%ebp)
	decl	%edx
	leal	-1(%edi), %eax
	shrl	$5, %eax
	shrl	$5, %ebx
	shrl	$5, %edx
	subl	%ebx, %eax
	leal	1(%edx), %ecx
	cmpl	%eax, %ecx
	jle	.L298
	testl	%eax, %eax
	jle	.L344
	movl	-40(%ebp), %edx
	sall	$2, %edx
	movl	%edx, -52(%ebp)
	movl	%eax, %esi
.L309:
	movl	-16(%ebp), %ecx
	movl	-20(%ebp), %edi
	cmpl	%edi, %ecx
	movl	%ecx, -48(%ebp)
	movl	-24(%ebp), %ebx
	jae	.L346
	.p2align 2,,3
.L308:
	movl	-48(%ebp), %eax
	movl	(%eax), %edx
	movb	-28(%ebp), %cl
	movl	-40(%ebp), %edi
	shrl	%cl, %edx
	movl	(%eax,%edi,4), %eax
	movb	-32(%ebp), %cl
	sall	%cl, %eax
	orl	%eax, %edx
	andl	(%ebx), %edx
	movl	%edx, %eax
	shrl	$1, %eax
	andl	$-613566757, %eax
	movl	%edx, %edi
	subl	%eax, %edi
	shrl	$2, %edx
	andl	$1227133513, %edx
	movl	%edi, %eax
	subl	%edx, %eax
	movl	%eax, %edx
	shrl	$3, %edx
	addl	%edx, %eax
	andl	$-954437177, %eax
	movl	%eax, %edx
	shrl	$6, %edx
	addl	%edx, %eax
	movl	%eax, %edx
	shrl	$12, %edx
	leal	(%edx,%eax), %edx
	shrl	$24, %eax
	addl	%eax, %edx
	andl	$63, %edx
	addl	$4, -48(%ebp)
	movl	-20(%ebp), %eax
	addl	%edx, -36(%ebp)
	addl	$4, %ebx
	cmpl	%eax, -48(%ebp)
	jb	.L308
.L346:
	movl	-52(%ebp), %edx
	movl	-24(%ebp), %edi
	movl	-44(%ebp), %ecx
	addl	%edx, -16(%ebp)
	addl	%edx, -20(%ebp)
	leal	(%edi,%ecx,4), %edi
	decl	%esi
	movl	%edi, -24(%ebp)
	jne	.L309
.L344:
	movl	-16(%ebp), %eax
	movl	-20(%ebp), %edx
	cmpl	%edx, %eax
	movl	%eax, -48(%ebp)
	movl	-24(%ebp), %ebx
	jae	.L354
.L315:
	movl	-48(%ebp), %ecx
	movl	(%ecx), %edx
	movb	-28(%ebp), %cl
	shrl	%cl, %edx
	andl	(%ebx), %edx
	movl	%edx, %eax
	shrl	$1, %eax
	andl	$-613566757, %eax
	movl	%edx, %edi
	subl	%eax, %edi
	shrl	$2, %edx
	andl	$1227133513, %edx
	movl	%edi, %eax
	subl	%edx, %eax
	movl	%eax, %edx
	shrl	$3, %edx
	addl	%edx, %eax
	andl	$-954437177, %eax
	movl	%eax, %edx
	shrl	$6, %edx
	addl	%edx, %eax
	movl	%eax, %edx
	shrl	$12, %edx
	leal	(%edx,%eax), %edx
	shrl	$24, %eax
	addl	%eax, %edx
	andl	$63, %edx
	addl	$4, -48(%ebp)
	movl	-20(%ebp), %eax
	addl	$4, %ebx
	addl	%edx, -36(%ebp)
	cmpl	%eax, -48(%ebp)
	jb	.L315
.L354:
	movl	-36(%ebp), %eax
.L289:
	addl	$40, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.L298:
	testl	%ecx, %ecx
	je	.L354
	movl	-40(%ebp), %ecx
	sall	$2, %ecx
	movl	%ecx, -52(%ebp)
	leal	1(%edx), %esi
.L327:
	movl	-16(%ebp), %edi
	movl	-20(%ebp), %eax
	cmpl	%eax, %edi
	movl	%edi, -48(%ebp)
	movl	-24(%ebp), %ebx
	jae	.L352
	.p2align 2,,3
.L326:
	movl	-48(%ebp), %ecx
	movl	(%ecx), %edx
	movb	-28(%ebp), %cl
	shrl	%cl, %edx
	movl	-40(%ebp), %edi
	movl	-48(%ebp), %ecx
	movl	(%ecx,%edi,4), %eax
	movb	-32(%ebp), %cl
	sall	%cl, %eax
	orl	%eax, %edx
	andl	(%ebx), %edx
	movl	%edx, %eax
	shrl	$1, %eax
	andl	$-613566757, %eax
	movl	%edx, %edi
	subl	%eax, %edi
	shrl	$2, %edx
	andl	$1227133513, %edx
	movl	%edi, %eax
	subl	%edx, %eax
	movl	%eax, %edx
	shrl	$3, %edx
	addl	%edx, %eax
	andl	$-954437177, %eax
	movl	%eax, %edx
	shrl	$6, %edx
	addl	%edx, %eax
	movl	%eax, %edx
	shrl	$12, %edx
	leal	(%edx,%eax), %edx
	shrl	$24, %eax
	addl	%eax, %edx
	andl	$63, %edx
	addl	$4, -48(%ebp)
	movl	-20(%ebp), %eax
	addl	%edx, -36(%ebp)
	addl	$4, %ebx
	cmpl	%eax, -48(%ebp)
	jb	.L326
.L352:
	movl	-52(%ebp), %edx
	movl	-24(%ebp), %edi
	movl	-44(%ebp), %ecx
	addl	%edx, -16(%ebp)
	addl	%edx, -20(%ebp)
	leal	(%edi,%ecx,4), %edi
	decl	%esi
	movl	%edi, -24(%ebp)
	jne	.L327
	jmp	.L354
.L297:
	movl	%edi, %eax
	subl	%ebx, %eax
	movl	(%esi), %edx
	cmpl	%edx, %eax
	jle	.L329
	movl	%edx, %eax
.L329:
	decl	%eax
	shrl	$5, %eax
	incl	%eax
	je	.L354
	movl	-40(%ebp), %edi
	sall	$2, %edi
	movl	%eax, %esi
.L340:
	movl	-16(%ebp), %eax
	movl	-20(%ebp), %edx
	cmpl	%edx, %eax
	movl	%eax, -48(%ebp)
	movl	-24(%ebp), %ebx
	jae	.L356
	.p2align 2,,3
.L339:
	movl	-48(%ebp), %ecx
	movl	(%ebx), %edx
	andl	(%ecx), %edx
	movl	%edx, %eax
	addl	$4, %ecx
	shrl	$1, %eax
	andl	$-613566757, %eax
	movl	%ecx, -48(%ebp)
	movl	%edx, %ecx
	subl	%eax, %ecx
	shrl	$2, %edx
	andl	$1227133513, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	movl	%eax, %edx
	shrl	$3, %edx
	addl	%edx, %eax
	andl	$-954437177, %eax
	movl	%eax, %edx
	shrl	$6, %edx
	addl	%edx, %eax
	movl	%eax, %edx
	shrl	$12, %edx
	leal	(%edx,%eax), %edx
	shrl	$24, %eax
	addl	%eax, %edx
	andl	$63, %edx
	movl	-20(%ebp), %eax
	addl	$4, %ebx
	addl	%edx, -36(%ebp)
	cmpl	%eax, -48(%ebp)
	jb	.L339
.L356:
	movl	-24(%ebp), %ecx
	movl	-44(%ebp), %edx
	addl	%edi, -16(%ebp)
	addl	%edi, -20(%ebp)
	leal	(%ecx,%edx,4), %ecx
	decl	%esi
	movl	%ecx, -24(%ebp)
	jne	.L340
	jmp	.L354
.L293:
	movl	%ebx, %eax
	shrl	$5, %eax
	imull	-40(%ebp), %eax
	movl	8(%edx), %edx
	leal	(%edx,%eax,4), %eax
	movl	-44(%ebp), %edx
	movl	%eax, -16(%ebp)
	addl	%ecx, %edx
	movl	-40(%ebp), %eax
	cmpl	%edx, %eax
	jle	.L296
	movl	%edx, %eax
.L296:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	leal	0(,%ecx,4), %eax
	movl	8(%esi), %ecx
	subl	%eax, %ecx
	jmp	.L357
	.p2align 2,,3
.L291:
	xorl	%eax, %eax
	jmp	.L289
.Lfe7:
	.size	bitmask_overlap_area,.Lfe7-bitmask_overlap_area
	.p2align 2,,3
.globl bitpos_setup
	.type	bitpos_setup,@function
bitpos_setup:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	xorl	%edi, %edi
	.p2align 2,,3
.L370:
	xorl	%esi, %esi
	xorl	%ebx, %ebx
	xorl	%ecx, %ecx
	.p2align 2,,3
.L369:
	movl	$1, %eax
	sall	%cl, %eax
	testl	%eax, %edi
	je	.L366
	addl	%ecx, %esi
	incl	%ebx
.L366:
	incl	%ecx
	cmpl	$7, %ecx
	jle	.L369
	movl	%ebx, %edx
	sall	$4, %edx
	leal	(%esi,%ebx,8), %eax
	sall	$8, %eax
	addl	%esi, %edx
	sall	$16, %edx
	leal	(%eax,%esi), %eax
	addl	%edx, %eax
	leal	(%ebx,%ebx,2), %edx
	leal	(%esi,%edx,8), %edx
	sall	$24, %edx
	addl	%edx, %eax
	movl	%eax, bitpos(,%edi,4)
	incl	%edi
	cmpl	$255, %edi
	jbe	.L370
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.Lfe8:
	.size	bitpos_setup,.Lfe8-bitpos_setup
	.p2align 2,,3
.globl bitmask_draw
	.type	bitmask_draw,@function
bitmask_draw:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$72, %esp
	movl	8(%ebp), %esi
	movl	16(%ebp), %ecx
	movl	(%esi), %edi
	cmpl	%edi, %ecx
	jge	.L375
	movl	4(%esi), %eax
	cmpl	%eax, 20(%ebp)
	movl	%eax, -80(%ebp)
	movl	%eax, -40(%ebp)
	jge	.L375
	movl	12(%ebp), %edx
	movl	4(%edx), %edx
	movl	%edx, %eax
	negl	%eax
	cmpl	%eax, 20(%ebp)
	movl	%edx, -44(%ebp)
	jle	.L375
	testl	%ecx, %ecx
	js	.L378
	movl	20(%ebp), %eax
	testl	%eax, %eax
	js	.L379
	movl	%ecx, %eax
	shrl	$5, %eax
	imull	-80(%ebp), %eax
	addl	20(%ebp), %eax
	movl	8(%esi), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	movl	-80(%ebp), %eax
	subl	20(%ebp), %eax
	cmpl	-44(%ebp), %eax
	jle	.L380
	movl	-44(%ebp), %eax
.L380:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %ebx
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, -20(%ebp)
.L381:
	movl	%ecx, %eax
	andl	$31, %eax
	movl	%eax, -28(%ebp)
	je	.L383
	movl	$32, -32(%ebp)
	movl	12(%ebp), %edx
	subl	%eax, -32(%ebp)
	movl	(%edx), %eax
	decl	%edi
	decl	%eax
	shrl	$5, %edi
	shrl	$5, %ecx
	shrl	$5, %eax
	incl	%eax
	subl	%ecx, %edi
	cmpl	%edi, %eax
	movl	%eax, -36(%ebp)
	jle	.L384
	testl	%edi, %edi
	jle	.L489
	movl	-40(%ebp), %ecx
	sall	$2, %ecx
	movl	%ecx, -48(%ebp)
	movl	%edi, %esi
.L399:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	cmpl	%ebx, %edx
	movl	%eax, -84(%ebp)
	jae	.L491
	.p2align 2,,3
.L393:
	movl	-84(%ebp), %ecx
	movl	(%ecx), %eax
	movb	-28(%ebp), %cl
	sall	%cl, %eax
	orl	%eax, (%edx)
	addl	$4, %edx
	addl	$4, -84(%ebp)
	cmpl	%ebx, %edx
	jb	.L393
.L491:
	movl	-48(%ebp), %eax
	addl	%eax, -16(%ebp)
	addl	%eax, %ebx
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jae	.L493
	.p2align 2,,3
.L398:
	movl	-84(%ebp), %ecx
	movl	(%ecx), %eax
	movb	-32(%ebp), %cl
	shrl	%cl, %eax
	orl	%eax, (%edx)
	addl	$4, %edx
	addl	$4, -84(%ebp)
	cmpl	%ebx, %edx
	jb	.L398
.L493:
	movl	-20(%ebp), %edx
	movl	-44(%ebp), %eax
	leal	(%edx,%eax,4), %edx
	decl	%esi
	movl	%edx, -20(%ebp)
	jne	.L399
.L489:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jae	.L375
.L404:
	movl	-84(%ebp), %ecx
	movl	(%ecx), %eax
	movb	-28(%ebp), %cl
	sall	%cl, %eax
	orl	%eax, (%edx)
	addl	$4, %edx
	addl	$4, -84(%ebp)
	cmpl	%ebx, %edx
	jb	.L404
	.p2align 2,,3
.L375:
	addl	$72, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.L384:
	xorl	%esi, %esi
	cmpl	-36(%ebp), %esi
	jge	.L375
	movl	-40(%ebp), %edi
	sall	$2, %edi
.L420:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	cmpl	%ebx, %edx
	movl	%eax, -84(%ebp)
	jae	.L497
	.p2align 2,,3
.L414:
	movl	-84(%ebp), %ecx
	movl	(%ecx), %eax
	movb	-28(%ebp), %cl
	sall	%cl, %eax
	orl	%eax, (%edx)
	addl	$4, %edx
	addl	$4, -84(%ebp)
	cmpl	%ebx, %edx
	jb	.L414
.L497:
	addl	%edi, -16(%ebp)
	addl	%edi, %ebx
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	cmpl	%ebx, %edx
	movl	%eax, -84(%ebp)
	jae	.L499
	.p2align 2,,3
.L419:
	movl	-84(%ebp), %ecx
	movl	(%ecx), %eax
	movb	-32(%ebp), %cl
	shrl	%cl, %eax
	orl	%eax, (%edx)
	addl	$4, %edx
	addl	$4, -84(%ebp)
	cmpl	%ebx, %edx
	jb	.L419
.L499:
	movl	-20(%ebp), %edx
	movl	-44(%ebp), %eax
	incl	%esi
	leal	(%edx,%eax,4), %edx
	cmpl	-36(%ebp), %esi
	movl	%edx, -20(%ebp)
	jl	.L420
	jmp	.L375
.L383:
	movl	%edi, %eax
	subl	%ecx, %eax
	movl	12(%ebp), %ecx
	movl	(%ecx), %edx
	cmpl	%edx, %eax
	jle	.L422
	movl	%edx, %eax
.L422:
	decl	%eax
	shrl	$5, %eax
	leal	1(%eax), %edi
	xorl	%esi, %esi
	cmpl	%edi, %esi
	jge	.L375
	movl	-40(%ebp), %eax
	movl	-44(%ebp), %edx
	sall	$2, %eax
	sall	$2, %edx
	movl	%eax, -52(%ebp)
	movl	%edx, -56(%ebp)
	.p2align 2,,3
.L432:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jae	.L502
	.p2align 2,,3
.L431:
	movl	-84(%ebp), %ecx
	movl	(%ecx), %eax
	orl	%eax, (%edx)
	addl	$4, %edx
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L431
.L502:
	movl	-52(%ebp), %eax
	movl	-56(%ebp), %edx
	incl	%esi
	addl	%eax, -16(%ebp)
	addl	%eax, %ebx
	addl	%edx, -20(%ebp)
	cmpl	%edi, %esi
	jl	.L432
	jmp	.L375
.L379:
	movl	%ecx, %eax
	shrl	$5, %eax
	imull	-80(%ebp), %eax
	movl	8(%esi), %edx
	leal	(%edx,%eax,4), %eax
	movl	-44(%ebp), %edx
	movl	%eax, -16(%ebp)
	addl	20(%ebp), %edx
	movl	-80(%ebp), %eax
	cmpl	%edx, %eax
	jle	.L382
	movl	%edx, %eax
.L382:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %ebx
	movl	20(%ebp), %eax
	movl	12(%ebp), %edx
	sall	$2, %eax
	movl	8(%edx), %edx
	subl	%eax, %edx
	movl	%edx, -20(%ebp)
	jmp	.L381
.L378:
	negl	%ecx
	movl	%ecx, -60(%ebp)
	movl	20(%ebp), %ecx
	negl	%ecx
	testl	%ecx, %ecx
	movl	%esi, -24(%ebp)
	movl	12(%ebp), %esi
	js	.L434
	movl	-60(%ebp), %eax
	movl	4(%esi), %edx
	shrl	$5, %eax
	imull	%edx, %eax
	addl	%ecx, %eax
	movl	%edx, -80(%ebp)
	movl	8(%esi), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	movl	-80(%ebp), %eax
	subl	%ecx, %eax
	movl	-24(%ebp), %ecx
	movl	4(%ecx), %ecx
	cmpl	%ecx, %eax
	movl	%ecx, -76(%ebp)
	jle	.L435
	movl	%ecx, %eax
.L435:
	movl	-16(%ebp), %edx
	movl	-24(%ebp), %ecx
	leal	(%edx,%eax,4), %ebx
	movl	8(%ecx), %ecx
.L518:
	movl	-60(%ebp), %eax
	andl	$31, %eax
	movl	%ecx, -20(%ebp)
	movl	%eax, -28(%ebp)
	je	.L438
	movl	$32, -32(%ebp)
	movl	(%esi), %edi
	subl	%eax, -32(%ebp)
	decl	%edi
	movl	-60(%ebp), %eax
	shrl	$5, %eax
	shrl	$5, %edi
	movl	-24(%ebp), %edx
	subl	%eax, %edi
	movl	(%edx), %eax
	decl	%eax
	shrl	$5, %eax
	incl	%eax
	cmpl	%edi, %eax
	movl	%eax, -36(%ebp)
	jle	.L439
	testl	%edi, %edi
	jle	.L504
	movl	-80(%ebp), %ecx
	sall	$2, %ecx
	movl	%ecx, -64(%ebp)
	movl	%edi, %esi
.L454:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	cmpl	%ebx, %edx
	movl	%eax, -84(%ebp)
	jae	.L506
	.p2align 2,,3
.L448:
	movl	(%edx), %eax
	movb	-28(%ebp), %cl
	shrl	%cl, %eax
	addl	$4, %edx
	movl	-84(%ebp), %ecx
	orl	%eax, (%ecx)
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L448
.L506:
	movl	-64(%ebp), %eax
	addl	%eax, -16(%ebp)
	addl	%eax, %ebx
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jae	.L508
	.p2align 2,,3
.L453:
	movl	(%edx), %eax
	movb	-32(%ebp), %cl
	sall	%cl, %eax
	addl	$4, %edx
	movl	-84(%ebp), %ecx
	orl	%eax, (%ecx)
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L453
.L508:
	movl	-20(%ebp), %edx
	movl	-76(%ebp), %eax
	leal	(%edx,%eax,4), %edx
	decl	%esi
	movl	%edx, -20(%ebp)
	jne	.L454
.L504:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jae	.L375
.L459:
	movl	(%edx), %eax
	movb	-28(%ebp), %cl
	shrl	%cl, %eax
	addl	$4, %edx
	movl	-84(%ebp), %ecx
	orl	%eax, (%ecx)
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L459
	jmp	.L375
.L439:
	xorl	%esi, %esi
	cmpl	-36(%ebp), %esi
	jge	.L375
	movl	-80(%ebp), %edi
	sall	$2, %edi
.L475:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	cmpl	%ebx, %edx
	movl	%eax, -84(%ebp)
	jae	.L512
	.p2align 2,,3
.L469:
	movl	(%edx), %eax
	movb	-28(%ebp), %cl
	shrl	%cl, %eax
	addl	$4, %edx
	movl	-84(%ebp), %ecx
	orl	%eax, (%ecx)
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L469
.L512:
	addl	%edi, -16(%ebp)
	addl	%edi, %ebx
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	cmpl	%ebx, %edx
	movl	%eax, -84(%ebp)
	jae	.L514
	.p2align 2,,3
.L474:
	movl	(%edx), %eax
	movb	-32(%ebp), %cl
	sall	%cl, %eax
	addl	$4, %edx
	movl	-84(%ebp), %ecx
	orl	%eax, (%ecx)
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L474
.L514:
	movl	-20(%ebp), %edx
	movl	-76(%ebp), %eax
	incl	%esi
	leal	(%edx,%eax,4), %edx
	cmpl	-36(%ebp), %esi
	movl	%edx, -20(%ebp)
	jl	.L475
	jmp	.L375
.L438:
	movl	(%esi), %eax
	movl	-24(%ebp), %ecx
	subl	-60(%ebp), %eax
	movl	(%ecx), %edx
	cmpl	%edx, %eax
	jle	.L477
	movl	%edx, %eax
.L477:
	decl	%eax
	shrl	$5, %eax
	movl	%eax, %edi
	incl	%edi
	je	.L375
	movl	-80(%ebp), %eax
	movl	-76(%ebp), %edx
	sall	$2, %eax
	sall	$2, %edx
	movl	%eax, -68(%ebp)
	movl	%edx, -72(%ebp)
	movl	%edi, %esi
.L487:
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jae	.L517
	.p2align 2,,3
.L486:
	movl	-84(%ebp), %ecx
	movl	(%edx), %eax
	addl	$4, %edx
	orl	%eax, (%ecx)
	addl	$4, %ecx
	cmpl	%ebx, %edx
	movl	%ecx, -84(%ebp)
	jb	.L486
.L517:
	movl	-68(%ebp), %eax
	movl	-72(%ebp), %edx
	addl	%eax, -16(%ebp)
	addl	%eax, %ebx
	addl	%edx, -20(%ebp)
	decl	%esi
	jne	.L487
	jmp	.L375
.L434:
	movl	-60(%ebp), %eax
	movl	4(%esi), %edx
	shrl	$5, %eax
	imull	%edx, %eax
	movl	%edx, -80(%ebp)
	movl	8(%esi), %edx
	leal	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	movl	-24(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, %edx
	movl	%eax, -76(%ebp)
	addl	%ecx, %edx
	movl	-80(%ebp), %eax
	cmpl	%edx, %eax
	jle	.L437
	movl	%edx, %eax
.L437:
	movl	-16(%ebp), %edx
	leal	(%edx,%eax,4), %ebx
	leal	0(,%ecx,4), %eax
	movl	-24(%ebp), %ecx
	movl	8(%ecx), %ecx
	subl	%eax, %ecx
	jmp	.L518
.Lfe9:
	.size	bitmask_draw,.Lfe9-bitmask_draw
	.local	bitpos
	.comm	bitpos,1024,32
	.ident	"GCC: (GNU) 3.2.2 20030109 (Debian prerelease)"
