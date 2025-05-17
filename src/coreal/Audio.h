/** 
 * Audio.h 
 * flgl
 * created 05/15/25 by frank collebrusco
 */
#ifndef AUDIO_H
#define AUDIO_H

struct Audio {
    static void init();
    static void destroy();
    static bool is_init();
};

#endif /* AUDIO_H */
