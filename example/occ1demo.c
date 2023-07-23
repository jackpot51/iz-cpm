#include "common.c"

#define LINERATE 15600
#define LOOPS_PER_LINE (LOOPS_PER_S / LINERATE)

#define CHRA 1
#define CHRB 4

void main(void) {
    clear_screen();

    // Disable interrupts
    __asm__("di");

    uint8_t * vram = (uint8_t *)0xF000;

    for (uint16_t i = 0; i < 0x80; i++) {
        uint16_t y = ((i / 16) + 1) * 2;
        uint16_t x = (i % 16) * 2;
        vram[y * 128 + x] = (uint8_t)i;
    }

    // Clocks per instruction: http://www.z80.info/z80time.txt
    __asm
        push hl
        push de
        push af

    00001$:
        ld hl, #0xF000    // 10 clocks, row 0 in vram

        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        nop               // 4 clocks

        nop               // 4 clocks
        nop               // 4 clocks
        nop               // 4 clocks

        jp 00002$         // 10 clocks, just for alignment
        // Finish this line at 256 clocks
    00002$:
        ld hl, #0xF000    // 10 clocks, row 0 in vram

        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRB    // 10 clocks
        inc l             // 4 clocks
        ld (hl), #CHRA    // 10 clocks
        nop               // 4 clocks

        nop               // 4 clocks
        nop               // 4 clocks
        nop               // 4 clocks

        jp 00001$         // 10 clocks
        // Finish this line at 256 clocks

        pop af
        pop de
        pop hl
    __endasm;

    // Enable interrupts
    __asm__("ei");

    // Wait for key
    while (!getchar()) {}

    clear_screen();
    exit();
}
