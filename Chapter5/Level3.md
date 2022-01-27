# CTF Level 3
The new level can be unlocked by running oracle with the flag from the previous challenge as a command-line argument. Additionally, a flag _-h_ can be used to show a hint for this level, which is "Fix four broken things".

At first sight we can see the file is an elf but different than the previous 2 files.

In the output of _readelf_ 3 abnormalities can be found:

1.  The OS/ABI field is 'Novel - Modesto' instead of the usual 'Unix - System V'.
2.  The Machine field is 'Motorola Coldfire' instead of the machine type needed to run the file, 'Advanced Micro Devices X86-64'.
3.  The start of the program headers is bigger than the size of the file.

The file cannot be ran. The aforementioned hint tells us we must fix the 3 errors we found.

We can patch the ELF file by dumping the contents with the _xxd_ command into a .txt file and changing the wrong values into the correct ones.
Now to create a new ELF file from the .txt file we can use the following command 'xxd -r -p \<txt-file \>new-elf-file'.

If we now run the new ELF, a flag is written to the standard output. However, if we try to pass it to the oracle an error is sent. This makes sense since the hint said we have to fix 4 errors, and we only fixed 3.

Lets try to analyse the .text section. If we try to analyse it, an error is printed saying that the .text section isn't present in the file.

When we look at the sections using the _readelf_ command, we can see that the .text section is of type NO_BITS! This must be the 4th & final error that we must fix. In order to patch the type we have to find the offset for the _sh\_type_ field.
The section headers start 4480 bytes into the file, the .text section is at index 14 of the section header table, each section header is 64 bytes long and the _sh\_type is 4 bytes into the section header. Then the offset will be 4480+14*64+4 = 5380d = 0x1504.

Now that all 4 errors have been patched, lets try to run the new ELF executable. The new executable writes a new flag, which the oracle does accept!