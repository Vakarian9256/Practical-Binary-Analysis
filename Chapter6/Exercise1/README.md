 # Confusing objdump

```assembly
gcc -masm=intel -o conf.out conf.c
objdump -M intel -d --section .text conf.out
400526:	48                   	rex.W
400527:	b8                   	.byte 0xb8

0000000000400528 <main>:
400528:	55                   	push   rbp
400529:	48 89 e5             	mov    rbp,rsp
40052c:	48 83 ec 10          	sub    rsp,0x10
400530:	89 7d fc             	mov    DWORD PTR[rbp-0x4],edi
400533:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
400537:	bf d4 05 40 00       	mov    edi,0x4005d4
40053c:	e8 bf fe ff ff       	call   400400 <puts@plt>
400541:	b8 00 00 00 00       	mov    eax,0x0
400546:	c9                   	leave  
400547:	c3                   	ret    
400548:	0f 1f 84 00 00 00 00 	nop    DWORD PTR [rax+rax*1+0x0]
40054f:	00 
```

```assembly
gcc -masm=intel -o conf.out conf.c
strip conf.out
objdump -M intel -d --section .text conf.out

...
400526:	48 b8 55 48 89 e5 48 	movabs rax,0x10ec8348e5894855
40052d:	83 ec 10 
400530:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
400533:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
400537:	bf d4 05 40 00       	mov    edi,0x4005d4
40053c:	e8 bf fe ff ff       	call   400400 <puts@plt>
400541:	b8 00 00 00 00       	mov    eax,0x0
400546:	c9                   	leave  
400547:	c3                   	ret    
```

As can be seen, the first 3 instructions of the non-stripped version of the ELF are now interpreted as rax and a 64-bit constant in the stripped ELF.