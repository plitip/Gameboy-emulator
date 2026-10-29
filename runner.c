#include "gb.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
if (argc<2){
printf("Rom couldn't open\n");
return 1;
} 

FILE *file_pointer = fopen(argv[1], "rb");

if (file_pointer == NULL) {
printf("Rom couldn't open\n");
return 1;
}

fseek(file_pointer, 0, SEEK_END);

long romsize = ftell(file_pointer);
rewind(file_pointer);


if (romsize > 8388608){
printf("rom too big");
fclose(file_pointer);
return 1;
}


uint8_t *rom = malloc(romsize);
if (rom == NULL){
    printf("Rom couldn't open\n");
    fclose(file_pointer);
    return 1;
}

fread(rom,1,romsize,file_pointer);
fclose(file_pointer);
gb *g = gb_create(rom, romsize);

gb_run_frame(g);
gb_destroy(g);
free(rom);

return 0;
}