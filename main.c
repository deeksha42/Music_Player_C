#include "music_player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep (cross-platform)


int main()
{
    int userChoice = 0;
    readFromCSV(); // Load songs from a CSV file
    init_playlists(Playlists); // Initialize the playlist array
    system("cls");

  
    while (userChoice != -1)
    {
        main_menu(); // Display the main menu
        printf("Enter your choice: ");
        if (scanf("%d", &userChoice) != 1)
        {
            system("cls");
            printf("Invalid input. Please enter a valid option.\n");
            continue;
        }

        switch (userChoice)
        {
            case 1:
                user_song_input(); // Allow the user to create a new song
                break;

            case 2:
                select_song_from_list(); // Display and select songs from the available songs list
                system("cls");
                break;

            case 3:
                create_playlist(); // Create a new playlist 
                break;
            case 4: switch_playlist();
                break;
            case 5:
                add_to_playlist_circular(Playlists[active_playlist]); // Add a song to the current playlist
                break;

            case 6:
                show_playlist(Playlists[active_playlist]); // Show the current playlist
                break;

            case 7:
                play_prev_song(&Playlists[active_playlist]); // Play the previous song in the playlist
                break;

            case 8:
                play_next_song(&Playlists[active_playlist]); // Play the next song in the playlist
                break;

            case 9:
                show_all_playlists(); // Show available playlists
                break;
            case 10:
                show_song_details(Playlists[active_playlist]); // Show details of the song being played
                break;
            case 11:
                if (does_playlist_exist(Playlists[active_playlist]))
                {
                    // If a playlist exists, delete it
                    printf("---");
                    if (delete_playlist(&Playlists[active_playlist]))
                    {
                       printf("\nThe playlist was successfully deleted.\n");
                    }
                       
                    else
                        printf("\nYou haven't created a playlist yet. Nothing to delete\n");
                    printf("---");
                }
            

            case -1:
                break;

            default:
                system("cls");
                printf("Invalid input. Please enter a valid option.\n");
                break;
        }
    }

    free_all_memory();
    return 0;
}
