//---------------------------------------------------------------------------
//
// Shader.cpp
// Author: Marshall Hahn
//
//---------------------------------------------------------------------------

#include "shader.hpp"
#include <iostream>

shaderProgram g_shader;



void shaderProgram::init(shaderT c, char* vertShader, char* fragShader)
{
  //char* vertShader="SHADERS/first.vert";
  //char* fragShader="SHADERS/first.frag";

  current = c;

	std::cout<<"shader init: "<<vertShader<<" : "<<fragShader<<std::endl;

  char* vertSrc = readShaderToString(vertShader);
  char* fragSrc = readShaderToString(fragShader);
  
  if( vertSrc == NULL || fragSrc == NULL )
  {
    std::cout<<"Failed to reader shaders."<<std::endl;
    programs[current] = 0;
  }
  else
  {
    if( installShaders( vertSrc, fragSrc ) == -1 ) programs[current] = 0;
  }
  
  current = NONE;

  delete[] vertSrc;
  delete[] fragSrc;
}

shaderProgram::shaderProgram()
{
	for( unsigned i=0; i<NUM_OF_SHADERS; i++ ) programs[i] = 0;

	current = NONE;
}

shaderProgram::~shaderProgram()
{

}

int shaderProgram::printOglError(char *file, int line)
{
  //
  // Returns 1 if an OpenGL error occurred, 0 otherwise.
  //
  GLenum glErr;
  int    retCode = 0;

  glErr = glGetError();
  while (glErr != GL_NO_ERROR)
  {
      printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
      retCode = 1;
      glErr = glGetError();
  }
  return retCode;
}

GLint shaderProgram::getUniLoc(const GLchar *name)
{
  GLint loc;

  loc = glGetUniformLocation(programs[current], name);

  if( programs[current] == 0 ) return -1;

  if (loc == -1)
      printf("No such uniform named \"%s\"\n", name);

  printOpenGLError();  // Check for OpenGL errors
  return loc;
}

GLint shaderProgram::getAttrLoc(const GLchar *name)
{
  GLint loc;

  loc = glGetAttribLocation(programs[current], name);

  if ( programs[current] == 0 ) return -1;

  if (loc == -1)
      printf("No such attribute named \"%s\"\n", name);

  printOpenGLError();  // Check for OpenGL errors
  return loc;
}

void shaderProgram::printShaderInfoLog(GLuint shader)
{
  int infologLength = 0;
  int charsWritten  = 0;
  GLchar *infoLog;

  printOpenGLError();  // Check for OpenGL errors

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

  printOpenGLError();  // Check for OpenGL errors

  if (infologLength > 0)
  {
      infoLog = (GLchar *)malloc(infologLength);
      if (infoLog == NULL)
      {
          printf("ERROR: Could not allocate InfoLog buffer\n");
          exit(1);
      }
      glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
      printf("Shader InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
  }
  printOpenGLError();  // Check for OpenGL errors
}

void shaderProgram::printProgramInfoLog()
{
  int infologLength = 0;
  int charsWritten  = 0;
  GLchar *infoLog;

  printOpenGLError();  // Check for OpenGL errors

  glGetProgramiv(programs[current], GL_INFO_LOG_LENGTH, &infologLength);

  printOpenGLError();  // Check for OpenGL errors

  if (infologLength > 0)
  {
      infoLog = (GLchar *)malloc(infologLength);
      if (infoLog == NULL)
      {
          printf("ERROR: Could not allocate InfoLog buffer\n");
          exit(1);
      }
      glGetProgramInfoLog(programs[current], infologLength, &charsWritten, infoLog);
      printf("Program InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
  }
  printOpenGLError();  // Check for OpenGL errors
}

char* shaderProgram::readShaderToString(char *fileName)
{
  
  FILE *fp;
  long len;
  char *buf;
  
  fp = fopen(fileName,"rb");
  if ( fp == NULL )
  {
    printf("Cannot read the file %s\n", fileName);
    return NULL;
  }
     
  fseek(fp, 0, SEEK_END); //go to end of file
  
  len = ftell(fp); //get position at end (length)
  
  if ( len == -1 )
  {
      printf("Cannot determine size of the shader %s\n", fileName);
      return NULL;
  }
  
  fseek(fp, 0, SEEK_SET); //go to beginning
  
  buf = new char[len+1];
    
  fread(buf, len, 1, fp); 
  
  buf[len] = '\0';
  
 // if( ferror(fp))
  //{
  //  delete[] buf;
  //  return NULL;
  //}
  
  fclose(fp);
  
 // for(unsigned i=0; i<len; i++) printf("%c",buf[i]);
 // printf("\n");
  
  return buf;
}

int shaderProgram::installShaders(const GLchar *vertexShader, const GLchar *fragmentShader)
{
  GLuint vs, fs;   // handles to objects
  GLint  vertCompiled, fragCompiled;    // status values
  GLint  linked;

  // Create a vertex shader object and a fragment shader object

  vs = glCreateShader(GL_VERTEX_SHADER);
  if(vs == 0 )
  {
	std::cout<<"glCreateShader(GL_VERTEX_SHADER) call failed."<<std::endl;
    return -1;
  }
  fs = glCreateShader(GL_FRAGMENT_SHADER);
  if(fs == 0 )
  {
    std::cout<<"glCreateShader(GL_FRAGMENT_SHADER) call failed."<<std::endl;
    return -1;
  }

  // Load source code strings into shaders

  glShaderSource(vs, 1, &vertexShader, NULL);
  glShaderSource(fs, 1, &fragmentShader, NULL);

  glCompileShader(vs);
  printOpenGLError();  // Check for OpenGL errors
  glGetShaderiv(vs, GL_COMPILE_STATUS, &vertCompiled);
  printShaderInfoLog(vs);

  glCompileShader(fs);
  printOpenGLError();  // Check for OpenGL errors
  glGetShaderiv(fs, GL_COMPILE_STATUS, &fragCompiled);
  printShaderInfoLog(fs);

  if (!vertCompiled || !fragCompiled)
  {
    std::cout<<"Error occured when compiling shaders."<<std::endl;
    return -1;
  }

  // Create a program object and attach the two compiled shaders

  programs[current] = glCreateProgram();
  glAttachShader(programs[current], vs);
  glAttachShader(programs[current], fs);

  // Link the program object and print out the info log

  glLinkProgram(programs[current]);
  printOpenGLError();  // Check for OpenGL errors
  glGetProgramiv(programs[current], GL_LINK_STATUS, &linked);
  printProgramInfoLog();

  if (!linked) 
  {
    std::cout<<"Program occured when linking shaders."<<std::endl;
    return -1;
  }
  


  // Install program object as part of current state

  //glUseProgram(programs[current]);

  return 0;
}
