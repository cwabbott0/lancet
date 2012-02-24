#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        if(argc != 3) {
                printf("Usage: %s [input (malisc format)] [output]\n", argv[0]);
                return 0;
        }

        FILE *in_f = fopen(argv[1], "rb");
        if(!in_f) {
                printf("Could not open file %s\n", argv[1]);
                return 1;
        }

        int tag, length;
        do {
                fread(&tag, sizeof(tag), 1, in_f);
        } while (tag != 0x4e494244);

        fread(&length, sizeof(length), 1, in_f);

        unsigned int *binary = malloc(length);
        fread(binary, 1, length, in_f);
        fclose(in_f);

        FILE *out_f = fopen(argv[2], "wb");
        if(!out_f) {
                printf("Could not open file %s\n", argv[2]);
                return 1;
        }

        fwrite(binary, 1, length, out_f);
        return 0;
}
