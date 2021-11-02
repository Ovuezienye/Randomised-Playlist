/* Written by Ovuezienye Carissa Okoro
   Date: 04.03.2021
   Student Number: 20316933
   Email: ovuezienye.okoro@ucdconnect.ie
   The following is a software engineering project coded using c language.
   The program generates a random playlist of songs given as input.
   Fisher Yates algorithm sourced from https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle was used to shuffle the playlist. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_LENGTH 500
#define MAX_ARTIST_TITLES 500
#define MAX_SONGS 1000

typedef struct
{
    char artist[MAX_LENGTH];
    char **titles;
    int totalSongs;
    int minutes[MAX_ARTIST_TITLES];
    int seconds[MAX_ARTIST_TITLES];
}
Songs;

typedef struct
{
    Songs** allSongs;
    int allArtists;
    int allArtistsSongs;
}
SongsList;

void sortArtists(SongsList* songs)//to sort artist songs alphabetically
{
    for (int n = 0; n<songs->allArtists-1; n++)
    {
        for (int i = 0; i<songs->allArtists-n-1; i++)
        {
            if (strcmp(songs->allSongs[i]->artist, songs->allSongs[i+1]->artist)>0)//to check that the current list is going from a-z
            {
                Songs*tmp = songs->allSongs[i];
                songs->allSongs[i] = songs->allSongs[i+1];
                songs->allSongs[i+1] = tmp;
            }
        }
    }
}

void sortSongs(SongsList* songs)//to sort songs alphabetically
{
    for (int n = 0; n<songs->allArtists; n++)
    {
        for (int i = 0; i<songs->allSongs[n]->totalSongs-1; i++)
        {
            for (int j = 0; j<songs->allSongs[n]->totalSongs-i-1; j++)
            {
                if (strcmp(songs->allSongs[n]->titles[j], songs->allSongs[n]->titles[j+1])>0)//to check that the current list is going from a-z
                {
                    char*tmp = songs->allSongs[n]->titles[j];
                    songs->allSongs[n]->titles[j] = songs->allSongs[n]->titles[j+1];
                    songs->allSongs[n]->titles[j+1] = tmp;
                }
            }
        }
    }
}

void randomize(SongsList* list, int a)//to shuffle songs using fisher yates algorithm
{
    srand(time(NULL));

    for (int n = a-1; n>0; n--)//starts from the last element and swaps them one by one
    {
        int i = rand()%(n+1);//pick a random index from 0 to n

        Songs*tmp = list->allSongs[i];//swaps array[n] with the element at a random index
        list->allSongs[i] = list->allSongs[n];
        list->allSongs[n] = tmp;
    }
    for (int n = 0; n<a; n++)//shuffling songs
    {
        for (int i = list->allSongs[n]->totalSongs-1; i>0; i--)
        {
            int j = rand()%(i+1);

            char*tmp = list->allSongs[n]->titles[j];
            list->allSongs[n]->titles[j] = list->allSongs[n]->titles[i];
            list->allSongs[n]->titles[i] = tmp;
        }
    }
}

void printPlaylist(SongsList* songs)//prints the list of songs
{
    for (int n = 0; n<songs->allArtists; n++)
    {
        printf("%s\n", songs->allSongs[n]->artist);

        for (int i = 0; i<songs->allSongs[n]->totalSongs; i++)//goes through all the songs in the playlist
        {
            printf("\t%s***%d:%d\n", songs->allSongs[n]->titles[i], songs->allSongs[n]->minutes[i], songs->allSongs[n]->seconds[i]);
        }
        printf("\n");
    }
}

int searchForSong(char** playlist, char* title, int totalSongs)//searches if song already exists in the playlist
{
    for (int n = 0; n<totalSongs; n++)
    {
        if (strcmp(playlist[n], title) == 0)
        {
            return 1;//returns true if song exists already
        }
    }
    return 0;//returns false if song doesn't exist already
}

int checkSongs(Songs* song, char* title, int currentSongs)//checks to make sure the last song is not the same as current one
{
    if (currentSongs == 0)
    {
        return 0;
    }
    for (int n = 0; n<song->totalSongs; n++)
    {
        if (strcmp(song->titles[n], title) == 0)

            return 1;
    }
    return 0;

}
void playlist(SongsList* list)//creates playlist
{

    srand(time(NULL));
    char** playList = calloc(list->allArtistsSongs, sizeof(char*));

    int* array = calloc(list->allArtists, sizeof(int));
    int* check = calloc(list->allArtists, sizeof(int));

    int duration = 0;
    int currentSecond = 0;
    int count = list->allArtistsSongs;
    int currentSongs = 0;
    int totalArtistsAvailable = list->allArtists;

    while (duration<60 && count>0 && 0<totalArtistsAvailable)
    {
        int n = rand()%list->allArtists;

        if (array[n]<3) //makes sure the artists don't have more than 3 songs in the playlist
        {
            if (!checkSongs(list->allSongs[n], playList[currentSongs - 1], currentSongs))//making sure the last song isn't the same as current
            {
                int i = rand()%list->allSongs[n]->totalSongs;

                if (!searchForSong(playList, list->allSongs[n]->titles[i], currentSongs))
                {
                    array[n]++;

                    playList[currentSongs] = list->allSongs[n]->titles[i];
                    currentSongs++;
                    count--;

                    printf("%s: '%s' (%d:%d)\n", list->allSongs[n]->artist, list->allSongs[n]->titles[i], list->allSongs[n]->minutes[i], list->allSongs[n]->seconds[i]);
                    duration += list->allSongs[n]->minutes[i];
                    currentSecond += list->allSongs[n]->seconds[i];
                    if (currentSecond >= 60)
                    {
                        duration++;
                        currentSecond -= 60;
                    }
                }

            }
        }
        else
        {
            if (check[n] == 0)
            {
                totalArtistsAvailable--;
                check[n] = 1;
            }
        }
    }
    printf("\nTotal Duration: %d:%d", duration, currentSecond);
}

int main(int argc, char*argv[])
{
    SongsList songs;
    songs.allSongs = calloc(MAX_SONGS, sizeof(Songs*));//initialize to MAX_SONGS
    songs.allArtists = 0;
    songs.allArtistsSongs = 0;

    FILE* fptr;

    if (argc > 1)
    {
        fptr = fopen(argv[1], "r");
    }
    else
        fptr = fopen("/Users/ovuezi/CLionProjects/Assignment_1/songs+artistes.txt", "r");

    if (fptr == NULL)
    {
        printf("No file found\n");

        exit(1);
    }

    else
    {
        songs.allSongs[songs.allArtists] = calloc(1, sizeof(Songs));

        while (fscanf(fptr, "%[^\n\n]", &songs.allSongs[songs.allArtists]->artist) != EOF)
        {
            songs.allSongs[songs.allArtists]->totalSongs = 0;
            songs.allSongs[songs.allArtists]->titles = calloc(MAX_ARTIST_TITLES, sizeof(char*));//allocates memory
            songs.allSongs[songs.allArtists]->titles[songs.allSongs[songs.allArtists]->totalSongs] = calloc(MAX_LENGTH, sizeof(char));//allocates memory

            while (1)
            {
                fscanf(fptr, "%*c");
                fscanf(fptr, "%[^*\n]", songs.allSongs[songs.allArtists]->titles[songs.allSongs[songs.allArtists]->totalSongs]);
                if (strcmp(songs.allSongs[songs.allArtists]->titles[songs.allSongs[songs.allArtists]->totalSongs], "") == 0)
                {
                    break;
                }
                fscanf(fptr, "%*c");
                fscanf(fptr, "%*c");
                fscanf(fptr, "%*c");
                fscanf(fptr, "%d", &songs.allSongs[songs.allArtists]->minutes[songs.allSongs[songs.allArtists]->totalSongs]);
                fscanf(fptr, "%*c");
                fscanf(fptr, "%d", &songs.allSongs[songs.allArtists]->seconds[songs.allSongs[songs.allArtists]->totalSongs]);


                songs.allArtistsSongs++;
                songs.allSongs[songs.allArtists]->totalSongs++;
                songs.allSongs[songs.allArtists]->titles[songs.allSongs[songs.allArtists]->totalSongs] = calloc(MAX_LENGTH, sizeof(char));
            }

            fscanf(fptr, "%*c");
            songs.allArtists++;
            songs.allSongs[songs.allArtists] = calloc(1, sizeof(Songs));

        }
        printf("Unsorted playlist:\n\n");
        printPlaylist(&songs);
        sortArtists(&songs);//prints unsorted list

        printf("Sorted list of artistes and songs:\n\n");
        sortSongs(&songs);
        printPlaylist(&songs);
        randomize(&songs, songs.allArtists);//prints sorted list of artistes and songs

        srand(time(NULL));
        printf("Randomised playlist:\n\n");
        playlist(&songs);//prints playlist
    }
}
