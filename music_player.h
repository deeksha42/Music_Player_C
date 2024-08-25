// music_player.h

#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#define MAX_SONG_LENGTH 100
#define MAX_ALBUM_LENGTH 100
#define MAX_SONG_POOL 120
#define MAX_STRING_SIZE 100

typedef struct Song {
    char title[MAX_SONG_LENGTH];
    char album[MAX_ALBUM_LENGTH];
    int id;
    int year;
    double duration;
} SONG;

typedef struct PlaylistNode {
    SONG* song;
    struct PlaylistNode* next;
    struct PlaylistNode* prev;
} PlaylistNode;

typedef struct Playlist {
    PlaylistNode* header_node;
    char playlist_name[MAX_STRING_SIZE];
    int size;
    int pid;
    PlaylistNode* now_playing;
} Playlist;

extern SONG* song_pool[MAX_SONG_POOL];
extern int pool_insert_index ;

extern Playlist Playlists[10];
extern int playlist_max_index;
extern int active_playlist;

void init_playlists(Playlist Playlists[10]);

SONG* createSong(const char* title, const char* album, const int year, const double duration);

void readFromCSV();

int is_pool_empty();

int does_playlist_exist(Playlist p);

void show_all_songs_raw();

int select_song_from_list();

void create_playlist();

void add_to_playlist_circular(Playlist p);

void user_song_input();

void show_playlist(Playlist p);

void show_song_details(Playlist p);

void play_next_song(Playlist* p);

void play_prev_song(Playlist* p);

int delete_playlist(Playlist* p);

void free_all_memory();

void show_all_playlists();

void switch_playlist();

void main_menu();

#endif // MUSIC_PLAYER_H
