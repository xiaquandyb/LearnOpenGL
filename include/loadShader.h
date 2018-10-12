#ifndef LOADSHADERS_H
#define LOADSHADERS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>

// ShaderInfo结构体
struct ShaderInfo{
    GLenum type;
    const char *fileName;
    GLuint shader;
};

// 从文件中读数据
static const GLchar* readShader(const char* filename){
    FILE* infile;
    fopen_s(&infile, filename, "rb");
    if(!infile){
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
        return NULL;
    }
    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    GLchar* source = new GLchar[len + 1];
    fread(source, 1, len, infile);
    fclose(infile);
    source[len] = 0;
    return const_cast<const GLchar*>(source);
}

// 导入着色器
GLuint loadShaders(ShaderInfo *shaders){
    if(shaders == nullptr){
        return 0;
    }
    GLuint program = glCreateProgram(); // 创建着色器程序
    ShaderInfo *entry = shaders;
    while(entry->type != GL_NONE){
        entry->shader = glCreateShader(entry->type); // 创建着色器对象
        const GLchar *source = readShader(entry->fileName);
        if(source == nullptr){
            for (entry = shaders; entry->type != GL_NONE; ++entry) {
                glDeleteShader(entry->shader); // 删除着色器对象
                entry->shader = 0;
            }
            delete[] source;
            return 0;
        }
        glShaderSource(entry->shader, 1, &source, NULL); // 将着色器源码关联到对象上
        delete[] source;
        glCompileShader(entry->shader); // 编译着色器源码
        GLint compiled;
        glGetShaderiv(entry->shader, GL_COMPILE_STATUS, &compiled); // 查询编译结果
        if(!compiled){
            GLsizei len;
            glGetShaderiv(entry->shader, GL_INFO_LOG_LENGTH, &len); // 获取错误信息长度
            GLchar *log = new GLchar[len + 1];
            glGetShaderInfoLog(entry->shader, len, &len, log); // 错误信息
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete[] log;
            return 0;
        }
        glAttachShader(program, entry->shader); // 将着色器对象关联到着色器程序
        ++entry;
    }
    glLinkProgram(program); // 链接着色器程序
    for(entry = shaders; entry->type != GL_NONE; ++entry){
        glDeleteShader(entry->shader);
        entry->shader = 0;
    }
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked); // 查询链接结果
    if(!linked){
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len); // 获取错误信息长度
        GLchar *log = new GLchar[len + 1];
        glGetProgramInfoLog(program, len, &len, log); // 错误信息
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete[] log;
        return 0;
    }
    return program;
}

#endif // LOADSHADERS_H