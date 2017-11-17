#include "music.h"
#include "open_interface.h"
#include "timer.h"
#include "lcd.h"
int main(void) {
  //  oi_uartInit();
    oi_t* sensor = oi_alloc();
    oi_init(sensor);
    load_songs();
    //oi_play_song(1);
    while(1);
}
