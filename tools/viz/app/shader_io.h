#include <bgfx/bgfx.h>
#include <string>

bgfx::ShaderHandle LoadShader(const std::string& name);
bgfx::ProgramHandle LoadProgram(const std::string& vs_name, const std::string& fs_name);
