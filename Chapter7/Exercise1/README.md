# Changing the Date Format
First, lets find the default date format.
If we run _date_ we can see that the default format is: 
Fri 28 Jan 16:59:23 CET 2022.
Now lets search through the strings of the file:

```
$strings date-modified
...
%s online help: <%s>
GNU coreutils
%a %b %e %H:%M:%S %Z %Y
...
```
Seems like %a %b %e %H:%M:%S %Z %Y fits the output, this must be the default format!

We can find the string using hexeditor and change it. In this write-up I'll try to only print the time & the timezone: %H:%M:%S %Z.

However, we're still receiving the same output! This might suggest this string is either only for display or _date_ is keeping it as a safety measure.

Lets start off with looking at library function calls the file is doing:
```
$ltrace -i -C ./date-modified
...
[0x402231] nl_langinfo(0x2006c, 0x40c060, 0, 0) = 0x7f489c083965
...
```
One of the library functions used is _nl_langinfo_. By looking at the [manual](https://man7.org/linux/man-pages/man3/nl_langinfo.3.html) we can see that this function returns a string according to the first parameter, _item_. Some of _items_ values include the date & time format!
We can change the _item_ to whatever value in the enum specified in the _langinfo.h_ file.
In our case, we'll want to change it into T_FMT.
Now when we run the modified date file, it prints the wanted format:
```
$ /bin/date
Fri 28 Jan 17:16:31 CET 2022
$./date-modified
17:17:24 CET
```

However, if we want to specify our own format, we could do so! According to the [documentation](https://man7.org/linux/man-pages/man3/nl_langinfo.3.html#RETURN_VALUE), if _item_ is invalid, _nl\_langinfo_ will return a pointer to an empty string. How does that help us? In the disassembly of _date_, one can see the following instructions:
```assembly
402223:	bf 6c 00 02 00       	mov    edi,0x2006c
402228:	89 4c 24 1c          	mov    DWORD PTR [rsp+0x1c],ecx
40222c:	e8 af f7 ff ff       	call   4019e0 <nl_langinfo@plt>
402231:	80 38 00             	cmp    BYTE PTR [rax],0x0
402234:	49 89 c4             	mov    r12,rax
402237:	b8 44 a9 40 00       	mov    eax,0x40a944
40223c:	8b 4c 24 1c          	mov    ecx,DWORD PTR [rsp+0x1c]
402240:	4c 0f 44 e0          	cmove  r12,rax
402244:	e9 81 fc ff ff       	jmp    401eca <__sprintf_chk@plt+0x37a>
...
```
In the instructions following the call to _nl\_langinfo_, the program checks if _nl\_langinfo_ returns an empty string, and if it does, then it replaces rax with an address. We can find the address' contents by running it in gdb and breaking in the instruction specifying it. 
```
(gdb) x/11c 0x40a944
0x40a944:	37 '%'	72 'H'	58 ':'	37 '%'	77 'M'	58 ':'	37 '%'	83 'S'
0x40a94c:	32 ' '	37 '%'	90 'Z'
```
The address points to the string we found at the start!

This means that _date_ will only use this format if _nl\_langinfo_ returns an empty string. Thus if we patch the instruction "mov    edi,0x2006c" to "mov    edi,0xffffffff", the program will use the default format at 0x40a944, which we can change to whatever we want.
