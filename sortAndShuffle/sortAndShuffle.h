#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SONG_PER_ARTIST 3
#define MAX_ARTISTS 4
#define MAX_SONGS (MAX_SONG_PER_ARTIST * MAX_ARTISTS)
#define MAX_LENGTH 80

// Create a structure which holds the name of song and its corresponding artist
typedef struct {
    char artist [MAX_LENGTH];
    char songName [MAX_LENGTH];
}Song;

void sortArtists(Song collection[], int totalSongs);

void sortSongs(Song collection[], int totalSongs);

void shuffleSongs(Song collection[], int totalSongs);

bool validLocation(Song collection[], int currentPosition, int gap);