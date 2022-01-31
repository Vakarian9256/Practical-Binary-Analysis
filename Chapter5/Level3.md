# CTF Level 3
The hint for this level is "fix four broken things".

At first sight we can see the file is an elf but different than the previous 2 files.
```
$ readelf -h lvl3
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 0b 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            Novell - Modesto
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Motorola Coldfire
  Version:                           0x1
  Entry point address:               0x4005d0
  Start of program headers:          4022250974 (bytes into file)
  Start of section headers:          4480 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         29
  Section header string table index: 28
readelf: Error: Reading 0x1f8 bytes extends past end of file for program headers
```
In the output of _readelf_ 3 abnormalities can be found:
1.  The OS/ABI field is 'Novel - Modesto' instead of the usual 'Unix - System V'.
2.  The Machine field is 'Motorola Coldfire' instead of the machine type needed to run the file, 'Advanced Micro Devices X86-64'.
3.  The start of the program headers is bigger than the size of the file.

The file cannot be ran. The hint tells us we must fix the 3 errors we found.

We can patch the ELF file by dumping the contents with the _xxd_ command into a .txt file and changing the wrong values into the correct ones.
Now to create a new ELF file from the .txt file the following command can be used: 'xxd -r -p \<txt-file\> \<new-elf-file\>'.

If we now run the new ELF, a flag is written to the standard output. However, if we try to pass it to the oracle an error is sent. This makes sense since the hint said we have to fix 4 errors, and we only fixed 3.

Lets try to analyse the .text section:
```
$ objdump -M intel -d -s -j .text ptlvl3

ptlvl3:     file format elf64-x86-64


Disassembly of section .text:

0000000000400550 <.text>:
```
The .text section is empty!

When we look at the sections using the _readelf_ command, we can see that the .text section is of type NO_BITS! This must be the 4th & final error that we must fix. In order to patch the type we have to find the offset for the _sh\_type_ field.
The section headers start 4480 bytes into the file, the .text section is at index 14 of the section header table, each section header is 64 bytes long and the _sh\_type is 4 bytes into the section header. Then the offset is 4480+14*64+4 = 5380d = 0x1504.

Now that all 4 errors have been patched, lets try to run the new ELF executable. The new executable writes a new flag, which the oracle does accept: 
```
$ ./plvl3
3a5c381e40d2fffd95ba4452a0fb4a40  ./plvl3
```