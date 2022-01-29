# Extending the Profiler
I decided to go in the route of changing the log_syscall function to be called from _instrument\_insn_:
```C
if(ProfileSyscalls.Value() && INS_IsSysCall(ins))
{
   INS_InsertCall(
   ins, IPOINT_BEFORE, (AFUNPTR)log_syscall, 
   IARG_INST_PTR, IARG_SYSCALL_NUMBER, 
   IARG_END
   );
}
```
The new _log\_syscall_ function takes an instruction pointer and a number of the system call that has been called.
The instruction is used to get the image which we'll use to check if the system call has been made from the main executable:
```C
static void log_syscall(ADDRINT ins, ADDRINT sysnum)
{
   IMG img = IMG_FindByAddress(INS_Address(ins));
   if (IMG_Valid(img) && IMG_IsMainExecutable(img))
   {
      syscalls[sysnum]++;
      syscall_count++;
   }
}
```