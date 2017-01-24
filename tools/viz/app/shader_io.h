#ifndef SPACE_VIZ_APP_SHADER_IO_H
#define SPACE_VIZ_APP_SHADER_IO_H

#include <bgfx/bgfx.h>
#include <string>

bgfx::ShaderHandle LoadShader(const std::string& name);
bgfx::ProgramHandle LoadProgram(const std::string& vs_name, const std::string& fs_name);

#endif
