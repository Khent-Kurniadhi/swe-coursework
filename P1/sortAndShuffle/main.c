#include <stdio.h>
#include <string.h>
#include "sortAndShuffle.h"

// Prototype functions
void insertArtistAndSongs(Song collection[], int *totalSongs);
void printSongs(Song collection[], int totalSongs);

int main(){
    Song collection[MAX_SONGS];
    int totalSongs = 0;

    // Function call for user input in the terminal
    insertArtistAndSongs(collection, &totalSongs);

    // Outputs the list of sorted artist and their sorted songs
    printf("\nSorted list of songs:\n");

    sortArtists(collection, totalSongs); // Calls function to sort artist name alphabetically
    sortSongs(collection, totalSongs); // Calls function to sort song title alphabetically

    printSongs(collection, totalSongs);

    // Shuffle the playlist and give show the output in terminal
    printf("\nShuffled Playlist:\n");
    shuffleSongs(collection, totalSongs);
    return 0;
}

void insertArtistAndSongs(Song collection[], int *totalSongs) {
    // Initialise variables
    int length, i, j, songIndex = 0;
    char currentArtist [MAX_LENGTH];    // Tracks the current artist

    // Ask for user input until the maximum amount of artist allowed
    for (i = 0; i < MAX_ARTISTS; i++) {
        printf("Insert an artist/group name: ");
        fgets(currentArtist, MAX_LENGTH, stdin);
        printf("\n");

        // Breaks the loop early if user enters an empty or blank artist name
        if (currentArtist[0] == '\n') {
            break;
        }

        // Gets the length of artist name
        // insert the null character at the index after the last character input (length-1)
        length = strlen(currentArtist);
        currentArtist [length-1] = '\0';

        // Ask for the song title of the current artist until the max permitted amount of song per artist
        for (j = 0; j < MAX_SONG_PER_ARTIST; j++) {

            // Get user input for the ith song
            printf("Enter song %d for %s\n", j+1, currentArtist);
            fgets(collection[songIndex].songName, MAX_LENGTH, stdin);

            // Exits the loop when user press enter without any other input
            if (collection [songIndex].songName[0] == '\n') {
                // Ensures there is atleast 1 song for the current artist(artist removed if no song)
                if (j < 1) {
                    printf("Artist have no songs! %s has been removed\n", currentArtist);
                    i--;
                    break;
                }
                break;
            }
            // get the length of the current song title
            // Add a NULL character to the end of the song title
            length = strlen(collection [songIndex].songName);
            collection [songIndex].songName[length-1] = '\0';

            // Add the corresponding artist name to the current collection index
            strcpy(collection [songIndex].artist, currentArtist);
            songIndex++;
        }
        // add space between song and insert artist
        printf("\n");
    }

    *totalSongs = songIndex;
}

void printSongs(Song collection[], int totalSongs) {
    // Initialise previous artist for the first iteration
    char lastArtist [MAX_LENGTH] = "";

    for (int i = 0; i < totalSongs; i++) {
        // Compare current artist name with previous artist
        // Only print the artist name if the current song has a different artist from the previous song
        if (strcasecmp(collection[i].artist, lastArtist) != 0) {
            printf("%s\n", collection[i].artist);   // print artist name if there is a change of artist
            strcpy(lastArtist, collection[i].artist); // change previous artist to the current artist for next iteration check
        }
        // print out the song title
        printf("\t-  %s\n", collection[i].songName);
    }
    //printf("\n");
}