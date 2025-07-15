#include "queue.h"
#include <fstream>
#include <cstring>

using namespace std;

ifstream fin("input.in");
ofstream fout("output.out");

struct Song {
  int Preference, Duration;
  char Name[100];  // song name
};

// 3 queues based on priority +1 for song history
QueueCirc<Song> q_vip, q_reg, q_low, q_history;
Song current_song, current_song_aux;

// put song into correct queue
void add (Song song_add) {
  if (song_add.Preference == 1) {
    q_vip.enqueue(song_add);
  } else if (song_add.Preference == 2) {
    q_reg.enqueue(song_add);
  } else {
    q_low.enqueue(song_add);
  }
}

// pick the next song to play based on queue priority
void play () {
  if (!q_vip.isEmpty()) {
    fout << "Now  playing: " << q_vip.peek().Name << " (" << q_vip.peek().Duration << "s)" << endl;
    q_history.enqueue(q_vip.peek());
    current_song = q_vip.peek();  // store for skip/replay
    current_song_aux = current_song;
    q_vip.dequeue();
  } else if (!q_reg.isEmpty()) {
    fout << "Now  playing: " << q_reg.peek().Name << " (" << q_reg.peek().Duration << "s)" << endl;
    q_history.enqueue(q_reg.peek());
    current_song = q_reg.peek();
    current_song_aux = current_song;
    q_reg.dequeue();
  } else if (!q_low.isEmpty()) {
    fout << "Now  playing: " << q_low.peek().Name << " (" << q_low.peek().Duration << "s)" << endl;
    q_history.enqueue(q_low.peek());
    current_song = q_low.peek();
    current_song_aux = current_song;
    q_low.dequeue();
  }
}

// skip n seconds from current song
void skip (int n) {
  if (current_song.Duration > n) {
    fout << "Skipped " << n << "s. ";
    current_song.Duration -= n;
    fout << current_song.Duration << "s remaining" << endl;
  } else {
    fout << "Skipped " << n << "s. Song finished" << endl;
  }
}

// replays last played song
void replay () {
  QueueCirc<Song> q_temp;

  // save all history except last
  while (q_history.getSize() > 1) {
    q_temp.enqueue(q_history.peek());
    q_history.dequeue();
  }

  current_song = q_history.peek();  // last played
  q_history.dequeue();
  fout << "Replaying: " << current_song.Name << endl;

  // put back old history
  while (!q_temp.isEmpty()) {
    q_history.enqueue(q_temp.peek());
    q_temp.dequeue();
  }
  q_history.enqueue(current_song);  // also add replayed one
}

// show previously played songs
void history () {
  QueueCirc<Song> q_temp;
  int cnt = 1;
  fout << "History:" << endl;
  while (!q_history.isEmpty()) {
    fout << cnt << ". " << q_history.peek().Name << endl;
    cnt++;
    q_temp.enqueue(q_history.peek());
    q_history.dequeue();
  }

  // restore history queue
  while (!q_temp.isEmpty()) {
    q_history.enqueue(q_temp.peek());
    q_temp.dequeue();
  }
}

// show current playlist by preference level
void show () {
  QueueCirc<Song> q_temp;
  fout << "Playlist: " << endl;
  fout << "Preference 1: ";
  if (!q_vip.isEmpty()) {
    while (!q_vip.isEmpty()) {
      if (q_vip.getSize() > 1) {
        fout << q_vip.peek().Name << "," << endl;
      } else {
        fout << q_vip.peek().Name << endl;
      }
      q_temp.enqueue(q_vip.peek());
      q_vip.dequeue();
    }

    // put back VIP songs
    while (!q_temp.isEmpty()) {
      q_vip.enqueue(q_temp.peek());
      q_temp.dequeue();
    }
  } else {
    fout << endl;
  }
  fout << "Preference 2: ";
  if (!q_reg.isEmpty()) {
    while (!q_reg.isEmpty()) {
      if (q_reg.getSize() > 1) {
        fout << q_reg.peek().Name << "," << endl;
      } else {
        fout << q_reg.peek().Name << endl;
      }
      q_temp.enqueue(q_reg.peek());
      q_reg.dequeue();
    }
    // put back regular songs
    while (!q_temp.isEmpty()) {
      q_reg.enqueue(q_temp.peek());
      q_temp.dequeue();
    }
  } else {
    fout << endl;
  }
  fout << "Preference 3: ";
  if (!q_low.isEmpty()) {
    while (!q_low.isEmpty()) {
      if (q_low.getSize() > 1) {
        fout << q_low.peek().Name << "," << endl;
      } else {
        fout << q_low.peek().Name << endl;
      }
      q_temp.enqueue(q_low.peek());
      q_low.dequeue();
    }
    // and put back low songs too
    while (!q_temp.isEmpty()) {
      q_low.enqueue(q_temp.peek());
      q_temp.dequeue();
    }
  }
  fout << endl;
}

// try to remove a song from any queue
void remove (Song song) {
  QueueCirc<Song> q_temp;
  bool ok = false;
  // try removing from each queue
  if (!q_vip.isEmpty()) {
    while (!q_vip.isEmpty()) {
      if (strcmp(q_vip.peek().Name, song.Name) != 0) {
        q_temp.enqueue(q_vip.peek());
        q_vip.dequeue();
      } else {
        ok = true;
        q_vip.dequeue();
      }
    }
    while (!q_temp.isEmpty()) {
      q_vip.enqueue(q_temp.peek());
      q_temp.dequeue();
    }
  }
  if (!q_reg.isEmpty()) {
    while (!q_reg.isEmpty()) {
      if (strcmp(q_reg.peek().Name, song.Name) != 0) {
        q_temp.enqueue(q_reg.peek());
        q_reg.dequeue();
      } else {
        ok = true;
        q_reg.dequeue();
      }
    }
    while (!q_temp.isEmpty()) {
      q_reg.enqueue(q_temp.peek());
      q_temp.dequeue();
    }
  }
  if (!q_low.isEmpty()) {
    while (!q_low.isEmpty()) {
      if (strcmp(q_low.peek().Name, song.Name) != 0) {
        q_temp.enqueue(q_low.peek());
        q_low.dequeue();
      } else {
        ok = true;
        q_low.dequeue();
      }
    }
    while (!q_temp.isEmpty()) {
      q_low.enqueue(q_temp.peek());
      q_temp.dequeue();
    }
  }
  // log result
  if (ok == true) {
    fout << "Removed: " << song.Name << endl;
  } else {
    fout << "The song " << song.Name << " does not exist."<< endl;
  }
}
// entry point - reads commands and dispatches them
int main () {
  char input_var[1001];
  Song songs[1001];
  int cnt = 0;
  while (fin>>input_var) {
    if (strcmp(input_var, "ADD") == 0) {
      fin >> songs[cnt].Preference >> songs[cnt].Duration >> songs[cnt].Name;
      add(songs[cnt]);
      cnt++;
    }
    if (strcmp(input_var, "PLAY") == 0) {
      play();
    }
    if (strcmp(input_var, "SKIP") == 0) {
      int minutes;
      fin >> minutes;
      if (minutes < 0) {
        cout << "The skipping value is invalid (the value is less than 0 or is of double / float type)" << endl;
      } else {
        skip(minutes);
      }
    }
    if (strcmp(input_var, "REPLAY") == 0) {
      replay();
    }
    if (strcmp(input_var, "HISTORY") == 0) {
      history();
    }
    if (strcmp(input_var, "SHOW") == 0) {
      show();
    }
    if (strcmp(input_var, "REMOVE") == 0) {
      Song removed_song;
      fin >> removed_song.Name;
      remove(removed_song);
    }
  }
  return 0;
}
