#pragma once

#include "audio/audio_player.h";

//global this is bad
float global_time_temp = 0;

void rhythm_loop(float delta_time, audio_player& audio)
{
    //printf("rhythm %f \n", delta_time);

    global_time_temp += delta_time;

    if (global_time_temp > 3.f)
    {
        global_time_temp = 0;

        //audio.play_oneoff_sound("honk");
    }
};

void hero_loop(float delta_time) 
{
    printf("hero %f \n", delta_time);
};