#include "music_player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // For sleep (cross-platform)
#include <windows.h>


SONG* song_pool[MAX_SONG_POOL];
int pool_insert_index = 0;

Playlist Playlists[10];
int playlist_max_index=0;
int active_playlist=0;

void init_playlists(Playlist Playlists[10])
{
  for(int i=0;i<10;i++)
  {
    Playlists[i].header_node = NULL;
    Playlists[i].now_playing = NULL;
    Playlists[i].size=0;
    Playlists[i].pid=i;
    
  }
}



//create song node
SONG *createSong(const char *title, const char *album, const int year, const double duration)
{
    SONG *temp = malloc(sizeof(SONG));
    strcpy(temp->title, title);
    strcpy(temp->album, album);
    temp->year = year;
    temp->duration = duration;
    pool_insert_index++;
    return temp;
}


//read csv file containing song information and add songs to song_pool array
void readFromCSV()
{
    FILE *file = fopen("songs.csv", "r");
    char line[1000];
    int i = 0;

    if (file == NULL)
    {
        perror("******\nUnable to load songs from data source");
        printf("Please check if rawdata.csv exists at the root directory of the application.\n");
        printf("You can still enter songs manually.\n******\n");
        printf("Error Message");
    }
    else
    {
        char songtitle[MAX_SONG_LENGTH];
        char releaseDate[MAX_SONG_LENGTH];
        int durationms;
        double duration = 0.0;
        char album[MAX_STRING_SIZE];
        int year;
        while (fgets(line, sizeof(line), file) && pool_insert_index < MAX_SONG_POOL)
        {
            char *token;
            token = strtok(line, ","); //tokenizes current line with delimiter ',' and returns the first substring
            if (token == NULL)
                continue;
            strcpy(songtitle, token);
            token = strtok(NULL, ","); // get the next token
            strncpy(releaseDate, token + 1, 4);
            sscanf(releaseDate, "%d", &year);
            token = strtok(NULL, ",");
            char temp[MAX_STRING_SIZE];
            strncpy(temp, token + 1, 6);
            sscanf(temp, "%d", &durationms);
            duration = durationms / 60000.0;
            token = strtok(NULL, ","); // get the next token
            strcpy(album, token);
            token = strtok(NULL, ","); // get the next token
            //printf("Song read : %s  %s\n",songname,durationm);
            //skip header row
            if (i == 0)
            {
                i++;
                continue;
            }
            song_pool[i - 1] = createSong(songtitle, album, year, duration);
            i++;
        }
    }

    //printf("%d Songs added.\n\n", i);
    fclose(file);
}

//function to check if songpool is empty
int is_pool_empty()
{
    return song_pool[0] == NULL;
}

//Function to check if playlist exists
int does_playlist_exist(Playlist p)
{
    return !(p.header_node == NULL);
}

//display all songs from csv
void show_all_songs_raw()
{
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
        printf("%-1d %-3s %-3s %-3d %0.2lfmin\n", (i + 1), song_pool[i]->title, song_pool[i]->album, song_pool[i]->year, song_pool[i]->duration);
}

//function to display all songs in song pool pagewise
int select_song_from_list() {
    int step = 0;
    int songs_per_page = 10;
    int max_pages = (pool_insert_index + songs_per_page - 1) / songs_per_page;
    
    while (1) {
        system("cls");
        printf("\n----------------------------------------------------------------------\n");
        printf("                             SONGS LIST\n");
        printf("----------------------------------------------------------------------\n");
        printf("   #   |  Title                                            | Duration\n");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = step * songs_per_page; i < (step + 1) * songs_per_page && i < pool_insert_index; i++) {
            printf("  %2d   |  %s | %2.2lf min\n", i + 1, song_pool[i]->title, song_pool[i]->duration);
        }
        
        printf("----------------------------------------------------------------------\n");
        printf("[Enter -2 to go to prev page] | Page %d of %d | [Enter -1 to go to next page]\n", step + 1, max_pages);
        printf("----------------------------------------------------------------------\n");
        printf(" <<<< Enter 0 to go back to main menu.\n");
        printf("----------------------------------------------------------------------\n");
        printf("Enter your choice : ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            choice = -3;  // Invalid choice
        }
        
        if (choice == -1) {
            if (step < max_pages - 1) {
                step++;
            } else {
                printf("This is the last page.\n");
                sleep(1);
            }
        } else if (choice == -2) {
            if (step > 0) {
                step--;
            } else {
                printf("This itself is the first page.\n");
                sleep(1);
            }
        } else if (choice == 0) {
            return 0;  // Go back to the main menu
        } else if (choice < -2 || choice > pool_insert_index) {
            printf("Please enter a valid choice.\n");
            sleep(1);
        } else {
            return choice;  // Song selected
        }
    }
}

//Allocates memory for a new playlist and prompts user to add a song to it
void create_playlist()
{
    int song_number = 0, i = 0;
    system("cls");

    if (playlist_max_index >= 10)
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("\nMaximum number of playlists reached.\n");
        printf("\n----------------------------------------------------------------------\n");
        return;
    }

    if(playlist_max_index<10 && (Playlists[playlist_max_index].header_node==NULL))
    {
      
      char name[MAX_STRING_SIZE];
      printf("\n----------------------------------------------------------------------\n");
      printf("Enter the name for the new playlist: ");
      scanf("%s", name);

      PlaylistNode* newPlaylist = (PlaylistNode*)malloc(sizeof(PlaylistNode));
      Playlists[playlist_max_index].header_node = newPlaylist;
      strcpy(Playlists[playlist_max_index].playlist_name, name);

      printf("\n-----New playlist created: %s -----\n", name);
      


      printf("\nPick the song you want to add to your new playlist\n");

      song_number = select_song_from_list();
    if (song_number > 0 && song_number <= 100)
    {
        newPlaylist->song = song_pool[song_number - 1];
        newPlaylist->prev = newPlaylist;
        newPlaylist->next = newPlaylist;
        Playlists[playlist_max_index].now_playing = newPlaylist;
        playlist_max_index++;
        system("cls");
        printf("---\n%s has been added to your playlist.\n---", song_pool[song_number - 1]->title);
    }
    else if (song_number == 0)
    {
        system("cls");
    }
    else
    {
        printf("\nThere was a problem while handling your request, try again.\n");
    }
    }

    else{
      printf("\nThere was a problem while handling your request, try again.\n");
    }

}

void add_to_playlist_circular(Playlist p) {
    int song_number = 0;

    if (!does_playlist_exist(p)) {
        printf("\n----------------------------------------------------------------------\n");
        printf("\nYou haven't created a playlist yet. Create a new playlist first.\n");
        printf("\n----------------------------------------------------------------------\n");
        sleep(1);
        return;
    }

    system("cls");
    printf("\nPick the song you want to add\n");
    song_number = select_song_from_list();

    if (song_number > 0) {
        PlaylistNode* new_node = (PlaylistNode*)malloc(sizeof(PlaylistNode));
        new_node->song = song_pool[song_number - 1];

        if (p.header_node == NULL) {
            // The playlist is empty, set the new node as the head and make it circular.
            new_node->prev= new_node;
            new_node->next= new_node;
            p.header_node = new_node;
            p.now_playing = new_node;
        } else {
            // Add the new song to the end of the playlist.
            new_node->next = p.header_node;
            new_node->prev= p.header_node->prev;
            p.header_node->prev->next = new_node;
            p.header_node->prev = new_node;
        }
        printf("\n----------------------------------------------------------------------\n");
        printf("\n%s has been added to your playlist.\n", song_pool[song_number - 1]->title);
    } else {
        system("cls");
    }
}

//Function that allows the user to create a song of their own
void user_song_input()
{
    int add_another = 1;

    while (add_another && pool_insert_index < MAX_SONG_POOL)
    {
        system("cls");
        printf("\nEnter the details of the song you want to create.\n[ Don't give spaces between each word, use _ instead\n");

        char songname[MAX_STRING_SIZE];
        char album[MAX_STRING_SIZE];
        int year;
        double duration = 0.0;

        printf("Title: ");
        scanf("%s", songname);
        printf("Album Name: ");
        scanf("%s", album);
        printf("Year of release: ");
        if (!(scanf("%d", &year)))
        {
            printf("INVALID INPUT ENTERED\n");
            sleep(1);
            break;
        }
        printf("Duration(in s): ");
        int duration_s;
        if (!(scanf("%d", &duration_s)))
        {
            printf("INVALID INPUT ENTERED\n");
            sleep(1);
            break;
        }
        duration = duration_s / 60;

        song_pool[pool_insert_index] = createSong(songname, album, year, duration);
        printf("\nThe song %s has been created.\n\n", songname);
        printf("\nDo you want to create another song?\n(Enter 1 for yes and 0 for no) : ");
        scanf("%d", &add_another);
    }
    system("cls");
}

//show songs in playlist
void show_playlist(Playlist p)
{
  int i=1;
    if (does_playlist_exist(p))
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("                          YOUR PLAYLIST:  %s\n",p.playlist_name);
        printf("----------------------------------------------------------------------\n");
        printf(" Song Title                                            | Duration\n");
        printf("----------------------------------------------------------------------\n");

        PlaylistNode* current = p.header_node;

        if (current != NULL)
        {
            do
            {
                printf("  %d    %s | %2.2lf min\n",i, current->song->title, current->song->duration);
                current = current->next;
                i++;
            } while (current != p.header_node);  // Continue until we reach the beginning of the circular list
        }
        printf("----------------------------------------------------------------------");
    }
    else
    {
        printf("\nYou haven't created a playlist yet.\n");
    }
}


//show song details
void show_song_details(Playlist p)
{
    if (p.now_playing == NULL || p.now_playing->song == NULL)
    {
        printf("                       CREATE A PLAYLIST FIRST\n");
    }
    else
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("                          DETAILED OVERVIEW\n");
        printf("----------------------------------------------------------------------\n");
        printf("\t   TITLE    |   %s\n", p.now_playing->song->title);
        printf("\t   ALBUM    |   %s\n", p.now_playing->song->album);
        printf("\t   YEAR     |   %d\n", p.now_playing->song->year);
        printf("\t  DURATION  |   %0.2lf minutes\n", p.now_playing->song->duration);
        printf("----------------------------------------------------------------------\n");
    }
    char leave[MAX_STRING_SIZE];
    printf("<<<<< Enter any input to return to MAIN MENU\n");
    if (scanf("%s", leave))
    {
        system("cls");
        return;
    }
}

void play_next_song(Playlist* p)
{
    if (does_playlist_exist(*p) && p->now_playing != NULL)
    {
        p->now_playing = p->now_playing->next;
    }
    else
    {
        printf("NO SONG ADDED TO PLAYLIST\n");
    }
}

void play_prev_song(Playlist* p)
{
    if (does_playlist_exist(*p) && p->now_playing != NULL)
    {
        p->now_playing = p->now_playing->prev;
    }
    else
    {
        printf("NO SONG ADDED TO PLAYLIST\n");
    }
}

//main menu
void main_menu()
{
 
    printf("\n----------------------------------------------------------------------\n");
    printf("                              MAIN MENU\n");
    printf("----------------------------------------------------------------------\n");

    if (Playlists[active_playlist].now_playing == NULL || Playlists[active_playlist].now_playing->song == NULL)
    {
        printf("                          NO TRACK SELECTED\n");
    }
    else
    {
        printf("  PLAYLIST |   %s\n",Playlists[active_playlist].playlist_name);
        printf("    NOW    |   %s\n", Playlists[active_playlist].now_playing->song->title);
        printf("  PLAYING  |   %0.2lf minutes\n", Playlists[active_playlist].now_playing->song->duration);
    }
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Action   \n");
    printf("----------------------------------------------------------------------\n");
    printf("   1   |  Create a new song\n");
    printf("   2   |  Display all available songs\n"); 
    printf("   3   |  Create a new playlist\n");
    printf("   4   |  Switch to another playlist\n");
    printf("   5   |  Add a song to the current playlist\n");
    printf("   6   |  Show current playlist\n");
    printf("   7   |  Play previous track\n");
    printf("   8   |  Play next track\n");
    printf("   9   |  Show available playlists\n");
    if (Playlists[active_playlist].now_playing != NULL && Playlists[active_playlist].now_playing->song != NULL)
        printf("   10  |  Show more information about the song playing\n");
    if (does_playlist_exist(Playlists[active_playlist]))
    {
        printf("   11  |  Delete current playlist\n");
    }  
    printf("  -1   |  Exit music player\n");
    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}


//Helper function to delete the playlist created and free all resources taken up by PlaylistNodes
int delete_playlist(Playlist* p) {
    if (!does_playlist_exist(*p)) {
        printf("Playlist not found\n");
        return 0;
    }

    int deletedIndex = p->pid;

    PlaylistNode* current = p->header_node;
    PlaylistNode* temp;

    do {
        temp = current;
        current = current->next;
        free(temp);
    } while (current != p->header_node);

    p->header_node = NULL;
    p->now_playing = NULL;

    // Update playlist_max_index
    if (playlist_max_index == deletedIndex) {
        playlist_max_index--;
    }

    // Update active_playlist if the deleted playlist is currently active
    if (active_playlist == deletedIndex) {
        active_playlist = (playlist_max_index >= 0) ? 0 : -1;
    }

    // If there are no playlists left, reset both variables
    if (playlist_max_index == -1) {
        active_playlist = -1;
    }

    return 1;
}



//Frees resources that had been dynamically allocated
void free_all_memory()
{
  for(int i=0;i<playlist_max_index;i++)
  {
    delete_playlist(&Playlists[i]);
  }
 
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
        free(song_pool[i]);
    printf("\n---END---");

    //Generate a 2 second delay
    sleep(2);
}


//show all available playlists
void show_all_playlists() {
  system("cls");
    printf("\n----------------------------------------------------------------------\n");
    printf("\n\t\tAvailable Playlists:\n");
    printf("\n----------------------------------------------------------------------\n");
    int count=0;
    for (int i = 0; i < 10; i++) {
      if(Playlists[i].header_node!= NULL)
      {
        count++;
        printf("%d.   %s\n",count, Playlists[i].playlist_name);
       
      }    
    }
    if(count==0)
    {
      printf("\nYour Library is empty . Create a playlist.\n");
      return;
    }
    
}


//switch playist
void switch_playlist() {
    char name[MAX_STRING_SIZE];
    printf("----------------------------------------------------------------------\n");
    if( Playlists[active_playlist].header_node != NULL)
    {
      printf("Currently Playing from playlist: %s\n", Playlists[active_playlist].playlist_name);
      printf("----------------------------------------------------------------------\n");

    }

    else
    {
      printf("No Playlist selected\n ");
      printf("----------------------------------------------------------------------\n");

    }

    printf("Enter the name of the playlist you want to switch to: ");
    scanf("%s", name);

    for (int i = 0; i < playlist_max_index; i++) {
        if (strcmp(Playlists[i].playlist_name, name) == 0) {
            active_playlist = i;
            printf("Switched to playlist: %s\n", Playlists[active_playlist].playlist_name);
            return;
        }
    }

    printf("Playlist not found: %s\n", name);
}