# CTF Level 5
The hint for this level is:

"Secrets hidden in code unused

The method of redirection is the key

Static rather than dynamically"

The new file is a normal 64-bit ELF executable.

Running the executable simply prints to output "nothing to see here" and exist.
```
$ ./lvl5
nothing to see here
```
As usual, we'll first check system calls and library function calls:
```
$$ ltrace -i -C ./lvl5
[0x400549] __libc_start_main(0x400500, 1, 0x7ffe4b960108, 0x4006f0 <unfinished ...>
[0x40050e] puts("nothing to see here"nothing to see here
)                       = 20
[0xffffffffffffffff] +++ exited (status 1) +++
```
```
$ strace -i ./lvl5
[00007f43bec1f777] execve("./lvl5", ["./lvl5"], [/* 59 vars */]) = 0
[00007f24b816a4b9] brk(NULL)            = 0x22ed000
[00007f24b816b3c7] access("/etc/ld.so.nohwcap", F_OK) = -1 ENOENT (No such file or directory)
[00007f24b816b3c7] access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directory)
[00007f24b816b367] open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
[00007f24b816b2f4] fstat(3, {st_mode=S_IFREG|0644, st_size=104399, ...}) = 0
[00007f24b816b4ba] mmap(NULL, 104399, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f24b835b000
[00007f24b816b467] close(3)             = 0
[00007f24b816b3c7] access("/etc/ld.so.nohwcap", F_OK) = -1 ENOENT (No such file or directory)
[00007f24b816b367] open("/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
[00007f24b816b387] read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\t\2\0\0\0\0\0"..., 832) = 832
[00007f24b816b2f4] fstat(3, {st_mode=S_IFREG|0755, st_size=1868984, ...}) = 0
[00007f24b816b4ba] mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f24b835a000
[00007f24b816b4ba] mmap(NULL, 3971488, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f24b7d86000
[00007f24b816b557] mprotect(0x7f24b7f46000, 2097152, PROT_NONE) = 0
[00007f24b816b4ba] mmap(0x7f24b8146000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1c0000) = 0x7f24b8146000
[00007f24b816b4ba] mmap(0x7f24b814c000, 14752, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f24b814c000
[00007f24b816b467] close(3)             = 0
[00007f24b816b4ba] mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f24b8359000
[00007f24b816b4ba] mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f24b8358000
[00007f24b8150bd5] arch_prctl(ARCH_SET_FS, 0x7f24b8359700) = 0
[00007f24b816b557] mprotect(0x7f24b8146000, 16384, PROT_READ) = 0
[00007f24b816b557] mprotect(0x600000, 4096, PROT_READ) = 0
[00007f24b816b557] mprotect(0x7f24b8375000, 4096, PROT_READ) = 0
[00007f24b816b537] munmap(0x7f24b835b000, 104399) = 0
[00007f24b7e7cc34] fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 0), ...}) = 0
[00007f24b7e82e19] brk(NULL)            = 0x22ed000
[00007f24b7e82e19] brk(0x230e000)       = 0x230e000
[00007f24b7e7d2c0] write(1, "nothing to see here\n", 20nothing to see here
) = 20
[00007f24b7e52748] exit_group(1)        = ?
[????????????????] +++ exited with 1 +++
```
Neither proves fruitful. However, strings provides some useful strings:
```
$ strings lvl5
...
key = 0x%08x
decrypted flag = %s
nothing to see here
...
```
The hint mentions that there are "secrets" in unused code, so we'll look at the disassembled .text section of the executable:
```
400500:	48 83 ec 08          	sub    rsp,0x8
400504:	bf 97 07 40 00       	mov    edi,0x400797
400509:	e8 a2 ff ff ff       	call   4004b0 <puts@plt>
40050e:	b8 01 00 00 00       	mov    eax,0x1
400513:	48 83 c4 08          	add    rsp,0x8
400517:	c3                   	ret    
```
There is nothing special in the main section, so lets look further:
```
4005fb:	eb 93                	jmp    400590 <__printf_chk@plt+0xb0>
4005fd:	0f 1f 00             	nop    DWORD PTR [rax]
400600:	b8 00 00 00 00       	mov    eax,0x0
400605:	48 85 c0             	test   rax,rax
400608:	74 f1                	je     4005fb <__printf_chk@plt+0x11b>
40060a:	55                   	push   rbp
40060b:	48 89 e5             	mov    rbp,rsp
40060e:	ff d0                	call   rax
400610:	5d                   	pop    rbp
400611:	e9 7a ff ff ff       	jmp    400590 <__printf_chk@plt+0xb0>
400616:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
40061d:	00 00 00 
400620:	53                   	push   rbx
400621:	be 74 07 40 00       	mov    esi,0x400774
400626:	bf 01 00 00 00       	mov    edi,0x1
40062b:	48 83 ec 30          	sub    rsp,0x30
40062f:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
400636:	00 00 
400638:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
40063d:	31 c0                	xor    eax,eax
40063f:	48 b8 10 60 21 33 15 	movabs rax,0x6223331533216010
400646:	33 23 62 
400649:	c6 44 24 20 00       	mov    BYTE PTR [rsp+0x20],0x0
40064e:	48 89 04 24          	mov    QWORD PTR [rsp],rax
400652:	48 b8 45 65 76 34 41 	movabs rax,0x6675364134766545
400659:	36 75 66 
40065c:	48 89 44 24 08       	mov    QWORD PTR [rsp+0x8],rax
400661:	48 b8 17 67 75 64 10 	movabs rax,0x6570331064756717
400668:	33 70 65 
40066b:	48 89 44 24 10       	mov    QWORD PTR [rsp+0x10],rax
400670:	48 b8 18 35 76 62 11 	movabs rax,0x6671671162763518
400677:	67 71 66 
40067a:	48 89 44 24 18       	mov    QWORD PTR [rsp+0x18],rax
40067f:	8b 1c 25 40 05 40 00 	mov    ebx,DWORD PTR ds:0x400540
400686:	31 c0                	xor    eax,eax
400688:	89 da                	mov    edx,ebx
40068a:	e8 51 fe ff ff       	call   4004e0 <__printf_chk@plt>
40068f:	48 8d 54 24 20       	lea    rdx,[rsp+0x20]
400694:	48 89 e0             	mov    rax,rsp
400697:	66 0f 1f 84 00 00 00 	nop    WORD PTR [rax+rax*1+0x0]
40069e:	00 00 
4006a0:	31 18                	xor    DWORD PTR [rax],ebx
4006a2:	48 83 c0 04          	add    rax,0x4
4006a6:	48 39 d0             	cmp    rax,rdx
4006a9:	75 f5                	jne    4006a0 <__printf_chk@plt+0x1c0>
4006ab:	31 c0                	xor    eax,eax
4006ad:	48 89 e2             	mov    rdx,rsp
4006b0:	be 82 07 40 00       	mov    esi,0x400782
4006b5:	bf 01 00 00 00       	mov    edi,0x1
4006ba:	e8 21 fe ff ff       	call   4004e0 <__printf_chk@plt>
4006bf:	31 c0                	xor    eax,eax
4006c1:	48 8b 4c 24 28       	mov    rcx,QWORD PTR [rsp+0x28]
4006c6:	64 48 33 0c 25 28 00 	xor    rcx,QWORD PTR fs:0x28
4006cd:	00 00 
4006cf:	75 06                	jne    4006d7 <__printf_chk@plt+0x1f7>
4006d1:	48 83 c4 30          	add    rsp,0x30
4006d5:	5b                   	pop    rbx
4006d6:	c3                   	ret    
4006d7:	e8 e4 fd ff ff       	call   4004c0 <__stack_chk_fail@plt>
4006dc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
4006e0:	bf 97 07 40 00       	mov    edi,0x400797
4006e5:	e9 c6 fd ff ff       	jmp    4004b0 <puts@plt>
4006ea:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
```
This section of code can be read as:
```c
rsp = rsp-48
rsp[0-8]=0x6223331533216010
rsp[8-16]=0x6675364134766545
rsp[16-24]=0x6675364134766545
rsp[24-32]=0x6671671162763518
rsp[32]=0
ebx = *0x400540
printf("key = %08x\n", ebx)
for (i=0; i<32; i = i+8):
    rsp[i] = rsp[i] ^ ebx
printf("decrypted flag = %s\n", rsp)
```
Our objective is to redirect control to the instruction at the address 0x400620.
The final part of the hint is that redirection should be done **manually** rather than **dynamically**. We can do so by redirecting control through _\_\_libc\_start\_main_ through patching its parameter specifying the address of _main_:
```
40052d:	50                   	push   rax
40052e:	54                   	push   rsp
40052f:	49 c7 c0 60 07 40 00 	mov    r8,0x400760
400536:	48 c7 c1 f0 06 40 00 	mov    rcx,0x4006f0
40053d:	48 c7 c7 00 05 40 00 	mov    rdi,0x400500
400544:	e8 87 ff ff ff       	call   4004d0 <__libc_start_main@plt>
```
We'll patch the instruction _mov    rdi,0x400500_ into _mov    rdi,0x400620_ using hexedit. Now if we run the executable we get the desired flag!
```
$ ./lvl5
key = 0x00400620
decrypted flag = 0fa355cbec64a05f7a5d050e836b1a1f
```