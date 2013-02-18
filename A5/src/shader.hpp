//---------------------------------------------------------------------------
// Shader.hpp
// Author: Marshall Hahn
//
// Include this header anywhere you want to access the global shaderProgram instance.
// Notice the extern statement in shader.hpp:	extern shaderProgram g_shader;
// And this statement in shader.cpp:			shaderProgram g_shader;
//
// This object can be used to load all available shaders, intialized them, 
// control which one is currently active, etc.
//
//---------------------------------------------------------------------------

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

class shaderProgram
{
public:

    /////////////////Supported Shaders BEGIN ///////////////////////
	//To add a new shader,	give it an entry in enum shaderT
	//						increase NUM_OF_SHADERS by one
	//						update initShaders

	enum shaderT
	{
		PERVERTEX,
		NONE
	};

	static const unsigned NUM_OF_SHADERS = 2;

	void initShaders()
	{
		std::cout<<"Loading "<<NUM_OF_SHADERS-1<<" shaders..."<<std::endl;
		int maxa;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxa);
		std::cout<<"Max User-defined attributes: "<<maxa<<std::endl;

		init(PERVERTEX, "shaders/pvlight.vert", "shaders/pvlight.frag");
	}

	/////////////////Supported Shaders END ///////////////////////

	shaderT current;
	shaderT currentp;
	int programs[NUM_OF_SHADERS];

	inline void setActive(shaderT p)
	{
		if( current != p )
		{
			current = p;
			//if( programs[current] != 0)
			//{	
				glUseProgram( programs[current] );
			//}
		}
	}

	inline shaderT getActive()
	{
		return current;
	}

	void init(shaderT c, char* vertShader, char* fragShader);

	shaderProgram();
	~shaderProgram();
  
	GLint getUniLoc(const GLchar *name);

	GLint getAttrLoc(const GLchar *name);

	inline void uniform1i(char *var, GLint val)
	{ 
		if( current != NONE )
		{
			int loc = getUniLoc(var);
			if(loc != -1) glUniform1i( loc, val ); 
		}
	}
  
	inline void uniform1f(char *var, GLfloat val)
	{ 
		if( current != NONE )
		{
			int loc = getUniLoc(var);
			if( loc != -1 ) glUniform1f(loc , val); 
		}
	}

	inline void uniform3f(char *var, GLfloat v0, GLfloat v1, GLfloat v2)
	{ 
		if( current != NONE )
		{
			int loc = getUniLoc(var);
			if( loc != -1 ) glUniform3f(loc, v0, v1, v2);
		}
	}

	inline void uniform4f(char *var, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{ 
		if( current != NONE )
		{
			int loc = getUniLoc(var);
			if( loc != -1 ) glUniform4f(loc, v0, v1, v2, v3);
		}
	}

	inline void uniform4f(char *var, GLfloat* d)
	{ 
		if( current != NONE )
		{
			int loc = getUniLoc(var);
			if( loc != -1 ) glUniform4f(loc, d[0], d[1], d[2], d[3]);
		}
	}

	inline void uniformMatrix4fv(char* var, const GLfloat*  val, bool transpose)
	{ 
		if( current != NONE )
		{
		  int loc = getUniLoc(var);
		  if( loc != -1 ) glUniformMatrix4fv( loc, 1, transpose, val); 
		}
	}
  
	inline void vertexAttrib3f(char* var, GLfloat v1, GLfloat v2, GLfloat v3)
	{ 
		if( current != NONE )
		{
		  int loc = getUniLoc(var);
		  if(loc != -1 ) glVertexAttrib3f( loc, v1, v2, v3); 
		}
	}
  
private:
  
  int printOglError(char *file, int line);
  void printShaderInfoLog(GLuint shader);
  void printProgramInfoLog();
  char* readShaderToString(char *fileName);
  int installShaders(const GLchar *brickVertex, const GLchar *brickFragment);
  inline void printOpenGLError() { printOglError(__FILE__, __LINE__); }
};

extern shaderProgram g_shader;

#endif /*SHADER_HPP_*/

