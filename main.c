#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, const char *argv[]){
    if(argc < 2) {fprintf(stderr, "Fail: No input file specified.\n"); return 1;}
    else if(argc > 4) {fprintf(stderr, "Fail: Too much arguments.\n"); return 1;}

    bool decompile = false;
    const char *input_filename = NULL;
    const char *output_filename = "output";

    if(*argv[1] == '-') { 
        decompile = true;
        input_filename = argv[2];
        if(argc > 3) output_filename = argv[3];
    } else {
        input_filename = argv[1];
        if(argc > 2) output_filename = argv[2];
    }

    FILE *input = fopen(input_filename, decompile?"rb":"r");
    if(input == NULL) {fprintf(stderr, "Fail: Error reading input file '%s'.\n", input_filename); return 2;}
    FILE *output = fopen(output_filename, decompile?"w":"wb");
    if(output == NULL) {fprintf(stderr, "Fail: Error opening output file '%s'.\n", output_filename); return 2;}

    char a_byte[8];
    short bit_index = 0;
    short base_2[] = {1, 2, 4, 8, 16, 32, 64, 128};

    if(!decompile) while(true){
        char c = fgetc(input);
        if(c == EOF) {
            if (bit_index > 0) fprintf(stderr, "Warning: The last %d binary ascii character in the input file is ignored.\n", bit_index);
            break;
        } else if (!(c == '0' || c == '1')) continue; // skip character that is not 0 and 1.


        a_byte[bit_index] = c;

        if(bit_index >= 7){ // a_byte is filled
            int b_byte = 0;
            for(int i = 0; i < 8; i++){
                int exp = 7 - i;
                b_byte += (a_byte[i] == '0'? 0 : 1) * base_2[exp];
            }

            fputc(b_byte, output);
            bit_index = 0;
            continue;
        }


        bit_index++;
    }
    else while(true){
        int c = fgetc(input);
        if(c == EOF) break;

        for(int i = 7; i >= 0; i--){
            int d = base_2[i];
            if(c >= d){
                fputc('1', output);
                c -= d;
            } else fputc('0', output);
        }
        fputc(' ', output);
    }

    fclose(input);
    fclose(output);

    printf("%s output written to '%s'.\n", decompile?"Ascii":"Binary", output_filename);

    return 0;
}