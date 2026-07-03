#include "sortAndShuffle.h"

/*
* This method sorts the artists alphabetically. It takes as input:
* - sortedArtists: the array of artists that that should be sorted
* - numOfArtists: the total number of artists
* This function is using the selection sort algorithm
*/
void sortArtists(Song collection[], int totalSongs){
    // create a temp variable for the swapping process
    Song swap;

    for (int i = 0; i < (totalSongs - 1); i++) {
        // Minimum index to track the index with the lowest value
        int minIndex = i;

        // loop through the collection to find the minimum value
        // Starts the loop from one index ahead of the current minimum index
        for (int j = i + 1; j < totalSongs; j++) {
            // Comparing the current minimum value to the next string
            // Update the minimum index if a string has a lower value than the current minimum
            if (strcasecmp(collection[minIndex].artist, collection[j].artist) > 0) {
                minIndex = j;
            }
        }
        swap = collection[i]; // Temp variable to store content of collection at index i
        collection[i] = collection[minIndex]; // update the current value at index i with the minimum value
        collection[minIndex] = swap; // Assign the value of swap to the collection at minIndex
    }
}

/*
* This method sorts the songs of a specific artist alphabetically. It takes as input:
* - songsOfAnArtist: the array of the songs of an artist that was provided from the standard input
* - numOfArtists: the number of artists provided from the standard input
*/
void sortSongs(Song collection[], int totalSongs){
    // Temp varibale to store a song title during swapping process
    char swap[80];

    // Sorted collection by artist name
    // This helps group the songs sequentially per artist
    sortArtists(collection, totalSongs);

    // Outer loop for insertion sort
    for (int i = 0; i < totalSongs; i++) {
        // initialise the starting index to be compared
        int j = i;

        // move the index of current value down until its no longer smaller than the previous index
        while ( j > 0 && (strcmp(collection[j].artist, collection[j-1].artist) == 0) && (strcasecmp(collection[j].songName, collection[j-1].songName) < 0)) {
            // perform the swapping process between the current index and previous index
            strcpy(swap, collection[j].songName);
            strcpy(collection[j].songName, collection[j - 1].songName);
            strcpy(collection[j-1].songName, swap);
            // Decrement the index being compared to continue the check
            j--;
        }
    }
}

/*
* This method shuffles a set of songs. It takes as input:
* - songsToBeShuffled: the array of the songs that should be shuffled
* - numOfSongs: the number of songs to be shuffled
*/
void shuffleSongs(Song collection[], int totalSongs){
    Song swap;
    int doubledSize = totalSongs * 2;
    // Create an array of Song type which contains two instances of every song title
    Song *doubled = malloc(sizeof(Song) * doubledSize);

    // Adjust the gap if the user input less than 6 songs
    // It is impossible to achieve the 5 song gap with 5 or less songs
    int gap = (totalSongs >= 6)? 5: totalSongs - 1;

    // Generates a random seed everytime the program is executed
    // the "time" calculates the second
    srand(time(NULL));

    // Set the elements of the doubled array to hold 2 instances of every song title
    for (int i = 0; i < totalSongs; i++) {
        doubled[i] = collection[i];
        doubled[totalSongs + i] = doubled[i];
    }

    // Fisher Yates algorithm
    for (int i = 0; i < doubledSize; i++) {
        int found_valid = 0;
        int attempt = 0;

        // try to shuffle the playlist if it doesn't meet the required gap and less than 100 attempt
        while (!found_valid && attempt <= 100) {
            // Get a random index to swap with the current index
            int randomIndex = i + rand() % (doubledSize - i);

            // Only perform a swap if the random index is different than the current one
            // This saves time and resources since swapping with itself changes nothing
            if (randomIndex != i) {
                swap = doubled[i];
                doubled[i] = doubled[randomIndex];
                doubled[randomIndex] = swap;
            }

            // Check if the swap is valid and meet the requirement
            // if its valid we change the valid state to 1 indicating successful swapping
            if (validLocation(doubled, i, gap)) {
                found_valid = 1;
            }

            // If the position after the swap is invalid, we swap it back to the original position
            else {
                if (randomIndex != i) {
                    swap = doubled[i];
                    doubled[i] = doubled[randomIndex];
                    doubled[randomIndex] = swap;
                    attempt++;
                }
            }
        }

        // restart the shuffle from the beginning
        // if we accede the 100 attempt and still haven't found a valid shuffle

        if (!found_valid) {
            i = -1;
            for (int j = 0; j < totalSongs; j++) {
                doubled[j] = collection[j];
                doubled[totalSongs + j] = doubled[j];
            }
        }
    }

    // print the shuffled playlist
    for (int i = 0; i < doubledSize; i++) {
        printf("%s - %s\n", doubled[i].artist, doubled[i].songName);
    }

    // Free up the dynamically allocated memory for doubled to be reused
    free(doubled);
}

// this function checks if the current position is valid and meet the gap constraint
bool validLocation(Song collection[], int currentPosition, int gap) {
    // Initialise variable for the gap between two instances of identical song
    int lookback;

    // Determine how far back we need to check based on current position/index
    // If the current position is less than the required gap
    // we need to check from the starting index up to the current index
    // if the position is bigger than the required gap we have to check "gap" elements
    lookback = (currentPosition> gap)? gap : currentPosition;

    // Loop through the array to find if there is an identical song inside the gap
    for (int i = 1; i <= lookback; i++) {
        // If an identical song is found then the position is invalid
        if (strcmp(collection[currentPosition].artist, collection[currentPosition-i].artist) == 0 && strcmp(collection[currentPosition].songName, collection[currentPosition-i].songName) == 0) {
            return false;
        }
    }
    return true;
}

