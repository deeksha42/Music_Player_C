# Music Player using C

## Overview

This project is a simple command-line music player application implemented in C for the course 'Data Structures and its Applicatons'. The application allows users to create and manage playlists, add songs to playlists, and play songs in sequence. The songs are read from a `songs.csv` file and stored in a pool from which users can select songs to add to their playlists. Playlists are managed using a doubly linked list structure to allow easy navigation between songs.

## Abstract Data Types (ADT)

### `SONG`

The `SONG` ADT represents a song and includes the following attributes:

- **`title`**: A string representing the title of the song.
- **`album`**: A string representing the album the song belongs to.
- **`id`**: An integer representing the unique identifier of the song.
- **`year`**: An integer representing the year of release.
- **`duration`**: A double representing the duration of the song in minutes.

### `PlaylistNode`

The `PlaylistNode` ADT represents a node in a playlist's linked list structure:

- **`song`**: A pointer to a `SONG` structure.
- **`next`**: A pointer to the next node in the playlist.
- **`prev`**: A pointer to the previous node in the playlist.

### `Playlist`

The `Playlist` ADT represents a playlist, which is a collection of songs:

- **`header_node`**: A pointer to the first `PlaylistNode` in the playlist.
- **`playlist_name`**: A string representing the name of the playlist.
- **`size`**: An integer representing the number of songs in the playlist.
- **`pid`**: An integer representing the unique identifier of the playlist.
- **`now_playing`**: A pointer to the currently playing `PlaylistNode`.

### Global Variables

- **`song_pool`**: An array of pointers to `SONG` structures, representing all songs loaded from `songs.csv`.
- **`pool_insert_index`**: An integer that keeps track of the number of songs in the `song_pool`.
- **`Playlists`**: An array of `Playlist` structures representing all user-created playlists.
- **`playlist_max_index`**: An integer tracking the number of playlists created.
- **`active_playlist`**: An integer representing the index of the currently active playlist.

## Files

- **`music_player.h`**: The header file containing function prototypes, data type definitions, and global variable declarations.
- **`music_player.c`**: The main implementation file where the functions are defined.
- **`songs.csv`**: A CSV file containing song data. Each row corresponds to a song and includes the following columns:
  - `title`: The title of the song.
  - `album`: The album name.
  - `year`: The year of release.
  - `duration`: The duration of the song in minutes.

## Functions

### Initialization and Setup

- `void init_playlists(Playlist Playlists[10]);`
  - Initializes the array of playlists.
- `SONG* createSong(const char* title, const char* album, const int year, const double duration);`

  - Creates a new `SONG` structure and returns a pointer to it.

- `void readFromCSV();`
  - Reads song data from `songs.csv` and populates the `song_pool`.

### Playlist Management

- `int is_pool_empty();`

  - Checks if the `song_pool` is empty.

- `int does_playlist_exist(Playlist p);`

  - Checks if a given playlist exists.

- `void create_playlist();`

  - Creates a new playlist and adds it to the `Playlists` array.

- `void add_to_playlist_circular(Playlist p);`

  - Adds a song to the given playlist in a circular manner.

- `int delete_playlist(Playlist* p);`
  - Deletes a playlist and frees associated memory.

### User Interaction and Display

- `void show_all_songs_raw();`

  - Displays all songs available in the `song_pool`.

- `int select_song_from_list();`

  - Allows the user to select a song from the list of all songs.

- `void user_song_input();`

  - Allows the user to input a song manually.

- `void show_playlist(Playlist p);`

  - Displays all songs in a given playlist.

- `void show_song_details(Playlist p);`

  - Displays details of the currently playing song.

- `void show_all_playlists();`

  - Displays all existing playlists.

- `void switch_playlist();`

  - Switches the active playlist.

- `void main_menu();`
  - Displays the main menu and handles user input.

### Playback Control

- `void play_next_song(Playlist* p);`

  - Plays the next song in the playlist.

- `void play_prev_song(Playlist* p);`
  - Plays the previous song in the playlist.

### Memory Management

- `void free_all_memory();`
  - Frees all dynamically allocated memory.

## Usage

1. **Compile the Program:**
   Use a C compiler to compile the `music_player.c` file. For example:

   ```bash
   gcc -o music_player main.c music_player.c
   ```

2. **Run the Program:**
   Execute the compiled program:

   ```bash
   ./music_player
   ```

3. **Main Menu:**
   Once the program is running, you will see a main menu with options to create playlists, add songs, play songs, and more.

4. **Creating and Managing Playlists:**
   Follow the on-screen instructions to create and manage your playlists.

5. **Playing Songs:**
   Navigate through the playlist using the options provided in the menu.

## CSV Format

Ensure that the `songs.csv` file is formatted correctly. Each line should contain the following comma-separated values:

```
track__album__name,track__album__release_date,track__duration_ms,track__name
```

For example:

```
Don't Start Now,31-10-2019,183290,Don't Start Now
```

## Memory Management

The program dynamically allocates memory for songs and playlists. It is important to ensure that all memory is freed when the program exits to prevent memory leaks. The `free_all_memory()` function is called to handle this.

---
