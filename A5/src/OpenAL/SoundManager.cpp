/* ***************************************************************
 *
 * File : SoundManager.cpp
 *
 * Author : Tiberiu Popa
 * 	    J. Alexander Clarke
 * Date   : June 18th, 2002
 *
 * Modified:
 *  21 Nov 2004 - Fryan Pan
 *    * Changed to basic OpenAL
 *
 * Purpose: Implementation of the sound manager
 *
 * ****************************************************************/
#pragma implementation
#include <cstdlib>
#include <iostream>
#include "SoundManager.h"


SoundManager SM;

// This is just to check for errors.  Normally, there's no need
// to check errors as frequently as this class does...
#define AL_CHECK_ERR(F)\
  {\
    F;\
    checkError(__FILE__ #F);\
  }

SoundManager::SoundManager()
  : nBuffers(0), nSources(0)
{
  // Initialize OpenAL.
  AL_CHECK_ERR(alutInit(0, NULL));

  // sound setup

  // create buffers
  AL_CHECK_ERR(alGenBuffers(MAX_BUFFERS, buffers)); 
  AL_CHECK_ERR(alGenSources(MAX_SOURCES, sources)); 
}

SoundManager::~SoundManager(){
  // Stop all sources before deleting them
  for(int i = 0; i < nSources; ++i) {
    Stop(i);
  }
  AL_CHECK_ERR(alDeleteSources(MAX_SOURCES, sources));
  AL_CHECK_ERR(alDeleteBuffers(MAX_BUFFERS, buffers));
  AL_CHECK_ERR(alutExit());
}

/* *************** Load WAV file into Buffer *****************/
int SoundManager::LoadWav(char* file) {
  if(nBuffers >= MAX_BUFFERS - 1) std::cerr << "Out of sources" << std::endl;

  ALenum     format;
  ALsizei    size;
  ALsizei    frequency;
  ALboolean  loop;
  ALvoid*    data;

  // Read in WAV data to buffer and delete temporary array used to
  // load the data. 
  AL_CHECK_ERR(alutLoadWAVFile(reinterpret_cast<ALbyte*>(file), &format, &data, &size, 
        &frequency, &loop));
  AL_CHECK_ERR(alBufferData(buffers[nBuffers], format, data, size, frequency));
  AL_CHECK_ERR(alutUnloadWAV(format, data, size, frequency));
  return nBuffers++;
}

/* *************** Manipulate sounde sources *****************/
int SoundManager::MakeSource() {
  if(nSources >= MAX_SOURCES - 1) std::cerr << "Out of sources" << std::endl;
  return nSources++;
}

void SoundManager::QueueBuffer(int src_index, int buffer_index) {
  AL_CHECK_ERR(alSourceQueueBuffers(sources[src_index], 1, &buffers[buffer_index]));
}

void SoundManager::Play(int src_index) {
  AL_CHECK_ERR(alSourcePlay(sources[src_index]));
}

void SoundManager::Stop(int src_index) {
  AL_CHECK_ERR(alSourceStop(sources[src_index]));
}

void SoundManager::Pause(int src_index) {
  AL_CHECK_ERR(alSourcePause(sources[src_index]));
}

void SoundManager::Rewind(int src_index) {
  AL_CHECK_ERR(alSourceRewind(sources[src_index]));
}

void SoundManager::SetLoop(int src_index, bool loop) {
  AL_CHECK_ERR(alSourcei(sources[src_index], AL_LOOPING, loop));
}

void SoundManager::SetPosition(int src_index, float x, float y, float z) {
  AL_CHECK_ERR(alSource3f(sources[src_index], AL_POSITION, x, y, z));
}

/* *************** Manipulate Listener ***********************/

void SoundManager::SetListenerPosition(float x, float y, float z) {
  AL_CHECK_ERR(alListener3f(AL_POSITION, x, y, z));
}

void SoundManager::SetListenerOrientation(float at_x, float at_y, float at_z,
    float up_x, float up_y, float up_z) {
  static ALfloat orientation[6]; 
  orientation[0] = at_x;
  orientation[1] = at_y;
  orientation[2] = at_z;

  orientation[3] = up_x;
  orientation[4] = up_y;
  orientation[5] = up_z;

  AL_CHECK_ERR(alListenerfv(AL_ORIENTATION, orientation));
}

/* *************** Check for Errors **************************/
bool SoundManager::checkError(const char* description) {
  ALenum error = alGetError();
  const char* msg = "";
  switch(error) {
    case AL_NO_ERROR: return false; 
    case AL_INVALID_NAME: msg = "Invalid Name"; break; 
#ifdef AL_ILLEGAL_ENUM
    case AL_ILLEGAL_ENUM: 
#else
    case AL_INVALID_ENUM: 
#endif
        msg = "Illegal Enum Parameter"; break; 
    case AL_INVALID_VALUE: msg = "Invalid Enum Parameter Value"; break; 
#ifdef AL_ILLEGAL_COMMAND
    case AL_ILLEGAL_COMMAND: 
#else
    case AL_INVALID_OPERATION: 
#endif
        msg = "Illegal Call"; break; 
    case AL_OUT_OF_MEMORY: msg = "Out of Memory"; break; 
  }
  std::cerr << "OpenAL Error: " << description << std::endl;
  std::cerr << "    " << msg << std::endl; 
  return true;
}
