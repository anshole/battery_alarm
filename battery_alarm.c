#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

Mix_Music *play_sound = NULL;

void get_battery();
void cleanUp();
void check_battery(char *output);
void sound_alarm();

int main(int argc, char *argv[])
{
	while(1) {
		get_battery();
		sleep(200);
	}
}

void get_battery()
{
	FILE *fp;
	char path[2000];

	/* Open the command for reading. */
	fp = popen("upower -i $(upower -e | grep BAT) | grep --color=never -E 'state|to\ full|to\ empty|percentage'", "r");

	if (fp == NULL) {
		printf("Failed to run command.\n" );
		exit(1);
	}

	int k;
	k  = 0;
	char *battery_perc;
	char *word = "%";

	/* Read the output a line at a time, print it. */
	while (fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", path);
		if (strstr(path, word)) {
			battery_perc = path;
		} else {
			printf("Cannot find battery level.");
		}
	}

	/* close */
	pclose(fp);

	check_battery(battery_perc);

	return 0;
}

void check_battery(char *output)
{	
	char *temp = output;
	while (*temp) {
		if (isdigit(*temp)) {
			long val = strtol(temp, &temp, 10);
			printf("%ld\n", val);
			/* Sound alarm if battery goes below 40%. */
			if (val < 40) {
				sound_alarm();
			}
		} else {
			temp++;
		}
	}

	temp = "";
}

/* This is from StackOverflow. I haven't looked at SDL lib deeply but this works for now. */
void sound_alarm()
{
    int channel;
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int audio_buffers;

    if(SDL_Init(SDL_INIT_AUDIO)<0)
        printf("Error In Init");

    audio_rate = 44100;
    audio_format = AUDIO_S16;
    audio_channels = 2;
    audio_buffers = 4096;


    if(Mix_OpenAudio(audio_rate, audio_format, 2, 4096)<0) {
        printf("An error occurred.");

    } else {
        Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
        printf("Opened audio at %d Hz %d bit %s (%s), %d bytes audio buffer\n", audio_rate,
                (audio_format&0xFF),
                (audio_channels > 2) ? "surround" :                         (audio_channels > 1) ? "stereo" : "mono", 
                (audio_format&0x1000) ? "BE" : "LE",
                audio_buffers );
    }

    play_sound = Mix_LoadMUS("alarm.mp3");

    if ( play_sound == NULL ) {
        fprintf(stderr, "Couldn't load alarm.mp3: %s\n",
            SDL_GetError());
        cleanUp();
        return;
    }

    Mix_PlayMusic(play_sound, -1);


    while (Mix_PlayingMusic() || Mix_PausedMusic()) {
            SDL_Delay(100);
    }

    cleanUp();
}

void cleanUp()
{
	Mix_FreeMusic(play_sound);
	Mix_CloseAudio();
	SDL_Quit();
}




