#pragma once
#include "VideoDriverGL.h"
#include "../ShaderProgram.h"
#include "../file.h"

namespace oxygine
{
	class ShaderProgramGL: public ShaderProgram
	{
	public:
		ShaderProgramGL():_program(0){}
		~ShaderProgramGL(){}

		void init(GLuint p)
		{
			_program = p;
		}

		unsigned int getID() const 
		{
			return _program;
		}

		void bind()
		{
			glUseProgram(_program);
		}

		void setUniform(const char *id, const Vector4 *v, int num)
		{
			int p = glGetUniformLocation(_program, id);
			glUniform4fv(p, num, v->m);
		}

		void setUniform(const char *id, const Matrix *mat)
		{
			int p = glGetUniformLocation(_program, id);
			glUniformMatrix4fv(p, 1, GL_FALSE, mat->ml);
		}

		void setUniform(const char *id, float val)
		{
			int p = glGetUniformLocation(_program, id);
			glUniform1f(p, val);
		}

	private:
		GLuint _program;
	};

	class UberShaderProgram
	{
	public:
		struct shader
		{
			shader():program(0), flags(0){}
			ShaderProgramGL *program;
			int flags;
		};

		enum
		{
			ALPHA_PREMULTIPLY = 1,
			SEPARATE_ALPHA = 1 << 1,
			MASK = 1 << 2,
			SIZE = 1 << 3
		};
		typedef Closure<void (ShaderProgramGL *)> ShaderUniformsCallback;

		UberShaderProgram(){}
		UberShaderProgram(const file::buffer &baseShader, const char *prepend="", const char *append = "");
		~UberShaderProgram();

		void setShaderUniformsCallback(ShaderUniformsCallback cb){_cb = cb;}

		ShaderUniformsCallback	getShaderUniformsCallback() const {return _cb;}
		shader*					getShaderProgram(int flags);

	protected:
		shader _shaders[SIZE];
		file::buffer _data;
		ShaderUniformsCallback _cb;		

		unsigned int createShader(unsigned int type, const char* data, const char *prepend, const char *append);
		unsigned int createProgram(int vs, int fs, const VertexDeclarationGL *decl);
	};


	class VideoDriverGLES20: public VideoDriverGL
	{
	public:
		VideoDriverGLES20();
		~VideoDriverGLES20();

		spNativeTexture createTexture();
		const file::buffer& getShaderBody() const {return _shaderBody;}

		void begin(const Matrix &proj, const Matrix &view, const Rect &viewport, const Color *clearColor);

		void drawBatch(const batch &b);
		void setDefaultSettings();		

	protected:
		void updateConstants();
		void setProgram(ShaderProgramGL *p);
		void bindTexture(int sampler, const char *id, void* handle);
		file::buffer _shaderBody;
		UberShaderProgram _us;

		ShaderProgramGL *_currentProgram;

		Matrix _matrixVP;

		blend_mode _blend;
	};
}