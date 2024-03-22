# SSD1306

This library is the driver for SSD1306 based OLED screens. The original implementation is from Neven Boyanov, Tinusaur Team.

This library is tested on PIC18F27/47/57Q84 and Dealikee 0.96" OLED Display Module, 128 x 64 px.

## How to use the code

```
#include "ssd1306.h"

void main(void) {
    ssd1306_init(0x3C);

    while(1) {
        // your code, for example
        ssd1306_fillscreen(0);
        ssd1306_setscale(1);
        ssd1306_string_pos(0, 0, "Main Menu");
        ssd1306_setscale(0);
        ssd1306_string_pos(10, 2, "Diagnostics");
        ssd1306_string_pos(10, 3, "Other");
    }
}
```

## Contribution
You can improve this library by your contribution. If you want to improve the code or have a fix for some issues with the library, please feel free to fork this library and submit a new pull request with your changes and description
