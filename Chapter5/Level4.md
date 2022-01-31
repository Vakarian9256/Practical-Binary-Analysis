# CTF Level 4
The hint for this level is "watch closely while I run".

We'll run the executable in GDB and log every step we make:
```
$ gdb lvl4
set disassembly-flavor intel
set pagination off
set logging on
set logging redirect on
b *0x400550
r
display/i $pc
while 1
si
end
```
*The entry point can be found with _readelf_.

In the log file, a call to _setenv_ which sets a flag can be found:
```
1: x/i $pc
=> 0x7ffff7dee9c8 <_dl_runtime_resolve_avx+344>:	bnd jmp r11
__setenv (name=0x400700 "FLAG", value=0x7fffffffde90 "656cf8aecb76113a4dece1688c61d0e7", replace=1) at setenv.c:253
```
The oracle accepts the flag: 656cf8aecb76113a4dece1688c61d0e7.