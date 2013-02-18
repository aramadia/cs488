 /* ***************************************************************
 *
 * File : SoundManager.h
 *
 * Author : Tiberiu Popa
 *          J. Alexander Clarke
 * Date   : June 18th, 2002
 *
 * Modified: 
 *  21 Nov 2004 - Fryan Pan
 *    * Changed to OpenAL demo
 *
 * Purpose: Header file for the sound manager
 *
 * ****************************************************************/
#ifndef _SOUND_MANAGER__H
#define _SOUND_MANAGER__H
#pragma interface

// include OpenAL header files
#include <AL/al.h> 
#include <AL/alut.h> 

// Controls max number of OpenAL primitives
// buffers - store sound data
// sources - store attributes of a sound emitter within 3D space 
#define MAX_BUFFERS 100 
#define MAX_SOURCES 100

// wrapper calss around SDL and SDL_mixer libraries
class SoundManager {
  public:
    SoundManager();
    ~SoundManager();

    // Load a WAV file into a buffer, and returns an index to the buffer 
    int LoadWav(char* file);

    // Adds a buffer to the queue of the given source 

    // This isn't the epitomy of perfect object-oriented design,
    // It's probably cleaner to manage sources, listeners, and buffers
    // with separate classes. (or use libopenalpp)
    
    // Manipulate sources 
    // You can queue buffers to play on any src_index, 0 <= src_index < MAX_SOURCES.
    //
    // Sources have several states: {INITIAL, PLAYING, STOPPED, PAUSED}
    // INITIAL = At start of queue and not playing 
    // PLAYING = Playing
    // STOPPED = Not playing.  Will return to INITIAL on next Play command
    // PAUSED = Not playing.  Will continue from current state on next Play.
    //
    // Certain source attributes cannot be changed in certain states. 
    
    // Returns an index to a new source
    int MakeSource();
    void QueueBuffer(int src_index, int buffer_index);
    
    // Change source state
    void Play(int src_index); 
    void Stop(int src_index); 
    void Pause(int src_index);
    void Rewind(int src_index);

    // Setting source attributes 
    // @param loop indicates whether to loop after reaching the last buffer
    // in the queue.  
    void SetLoop(int src_index, bool loop);
    void SetPosition(int src_index, float x, float y, float z);  

    // Change listener state
    void SetListenerPosition(float x, float y, float z);
    void SetListenerOrientation(float at_x, float at_y, float at_z,
            float up_x, float up_y, float up_z); 

  private:
    bool checkError(const char* description); 

    // internal data
    ALuint buffers[MAX_BUFFERS]; 
    ALuint sources[MAX_SOURCES];

    // number of sound clips loaded
    int nBuffers;
    int nSources;
};


// the global instance of the sound manager
extern SoundManager SM;


#endif // _SOUND_MANAGER__H
