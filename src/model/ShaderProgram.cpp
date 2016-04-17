#include "model/ShaderProgram.h"

namespace UniLib {
	namespace model {

		Shader::Shader(HASH id/* = 0*/)
		: mID(id)
		{
    
		}

		Shader::~Shader()
		{
		}

		

		unsigned char* Shader::readShaderFile(const char *filename) 
		{
			DRFile file(filename, "rt");
			if(!file.isOpen())
				LOG_ERROR("cannot open file", NULL);
    
			int bytesinfile = file.getSize();
			unsigned char *buffer = (unsigned char*)malloc(bytesinfile+1);
			long unsigned int bytesread = 0;
			file.read(buffer, 1, bytesinfile, &bytesread);
			buffer[bytesread] = 0; // Terminate the string with 0
    
			return buffer;
		}

		// ********************************************************************

		ShaderProgram::ShaderProgram(HASH id/* = 0*/)
		: mId(id), mVertexShader(), mFragmentShader()
		{

		}


		ShaderProgram::~ShaderProgram()
		{

		}


	}
}