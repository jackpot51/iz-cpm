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

    //TODO: restore system after some number of interations
    __asm
        // high part comes from i register
        ld a, i
        ld h, a
        // IRQ1 sends offset 0xF8
        ld l, #0xF8

        // HACK
        ld hl, #0xEFF8

        // replace IRQ1 handler
        ld de, #irq1
        ld (hl), e
        inc hl
        ld (hl), d
    idleloop:
        ei
        halt
        jp idleloop

    // 1.28 msec prior to first scan line, this value is magically selected
    // to make things happen. The VSYNC interrupt should be used instead
    // At 4 MHz, this is 5120 cycles
    irq1:               // 19 clocks to get to interrupt handler
        di              // 4 clocks
        push af         // 11 clocks
        push de         // 11 clocks
        push hl         // 11 clocks

        ld de, #211     // 10 clocks
    irq1loop:           // 5040, 24 clocks per iteration
        dec de          // 6 clocks
        ld a, d         // 4 clocks
        or e            // 4 clocks
        jp nz, irq1loop // 10 clocks
        
        ld de, #0       // 10 clocks (nop)
        nop             // 4 clocks

    // Program for beam racing image generation
    #include "occ1/program.asm"
        
        pop hl          // 10 clocks
        pop de          // 10 clocks
        pop af          // 10 clocks
        ei // Enable interrupts
        ret // Return from interrupt
    __endasm;

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
