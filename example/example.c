#include <stdint.h>

void main(void);

void start(void) __naked {
    __asm
    ld sp, #0x4000  // Set stack pointer to 16 KiB
    jp _main        // Jump to main function
    __endasm;
}

uint16_t bdos(uint8_t func, uint16_t param) __naked {
    // Use parameters
    func;
    param;

    __asm
        push iy
        ld iy, #4
        add iy, sp
        push ix
        // HL not saved as it is return value
        push de
        push bc
        push af

        ld c, (iy)   // func
        ld e,1(iy)  // param (low)
        ld d,2(iy)  // param (high)
        call 5        // Call BDOS

        pop af
        pop bc
        pop de
        // HL not restored as it is return value
        pop ix
        pop iy
        ret
    __endasm;
}

void putchar(char c) {
    bdos(6, c);
}

void puts(const char * s) {
    char c;
    while(c = *s++) {
        bdos(6, c);
    }
}

void clear_screen(void){
    putchar(0x1A);
}

void cursor_position(uint8_t x, uint8_t y) {
    // Position cursor
    putchar(0x1B);
    putchar('=');
    //TODO: ensure y <= 80
    putchar(y + 32);
    //TODO: ensure x <= 80
    putchar(x + 32);
}

// OSBORNE 1: 4 MHz, runs delay loop in 24 clocks
// Run emulator at 4 Mhz and 6 clocks per instruction to match!
#define CLOCKS_PER_S 4000000
#define CLOCKS_PER_LOOP 24
#define LOOPS_PER_S (CLOCKS_PER_S / CLOCKS_PER_LOOP)
#define FRAMERATE 30
#define LOOPS_PER_FRAME (LOOPS_PER_S / FRAMERATE)

void delay_frame(void) __naked {
    __asm
        push de
        push af

        ld de, #LOOPS_PER_FRAME
    loop:           // Clocks per instruction: http://www.z80.info/z80time.txt
        dec de      // 6 clocks
        ld a, d     // 4 clocks
        or e        // 4 clocks
        jp nz, loop // 10 clocks
                    // 24 clocks total

        pop af
        pop de
        ret
    __endasm;
}

#define BOX_LEFT 0
#define BOX_RIGHT 51
#define BOX_TOP 2
#define BOX_BOTTOM 23

struct Ball {
    char c;
    int16_t x;
    int16_t y;
    int16_t vx;
    int16_t vy;
    int16_t ax;
    int16_t ay;
};

#define BALLS 3

void main(void) {
    //delay_ms(1000);
    //return;

    clear_screen();

    puts("CP/M Physics");

    struct Ball balls[BALLS];

    // Initialize balls
    for (int i = 0; i < BALLS; i++) {
        struct Ball * ball = &balls[i];
        ball->c = 'o';
        ball->x = 0;
        ball->y = 0;
        ball->vx = 0;
        ball->vy = 0;
        ball->ax = 0;
        ball->ay = (1 << 8) / 8;
    }

    balls[0].c = 'o';
    balls[0].x = (4 << 8);
    balls[0].y = (4 << 8);
    balls[0].vx = (1 << 8);

    balls[1].c = 'O';
    balls[1].x = (8 << 8);
    balls[1].y = (8 << 8);
    balls[1].vx = -(1 << 8);

    balls[2].c = '0';
    balls[2].x = (12 << 8);
    balls[2].y = (12 << 8);
    balls[2].vx = (1 << 8);

    for(int frame = 0; frame < FRAMERATE * 10; frame++) {
        // Clear old balls
        for (int i = 0; i < BALLS; i++) {
            struct Ball * ball = &balls[i];

            cursor_position((ball->x >> 8), (ball->y >> 8));
            putchar(' ');
        }

        // Update and draw new balls
        for (int i = 0; i < BALLS; i++) {
            struct Ball * ball = &balls[i];

            // Calculate new ball position
            ball->x += ball->vx;
            ball->y += ball->vy;

            // Bounce ball off walls
            if (ball->x <= (BOX_LEFT << 8) || ball->x >= (BOX_RIGHT << 8)) {
                ball->vx = -ball->vx;
                ball->x += ball->vx;
            }
            if (ball->y <= (BOX_TOP << 8) || ball->y >= (BOX_BOTTOM << 8)) {
                ball->vy = -ball->vy;
                ball->y += ball->vy;
            }

            // Calculate new ball velocity
            ball->vx += ball->ax;
            ball->vy += ball->ay;

            // Print new ball
            cursor_position((ball->x >> 8), (ball->y >> 8));
            putchar(ball->c);
        }

        cursor_position(13, 0);

        char c = bdos(6, 0xFF);
        if (c == 'q') {
            break;
        }

        delay_frame();
    }

    clear_screen();

    // Exit program
    bdos(0, 0);
}
