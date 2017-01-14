#include "shader_io.h"
#include "entry/entry.h"
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <iostream>

static const bgfx::Memory* loadMem(bx::FileReaderI* reader, const std::string& file_path)
{
    if (bx::open(reader, file_path.data())) {
        uint32_t size = (uint32_t)bx::getSize(reader);
        const bgfx::Memory* mem = bgfx::alloc(size + 1);
        bx::read(reader, mem->data, size);
        bx::close(reader);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    DBG("Failed to load %s.", file_path.data());
    return NULL;
}

std::string ShaderPath(bgfx::RendererType::Enum rendered_type)
{
    switch (rendered_type) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:
            return "shaders/dx9/";

        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:
            return "shaders/dx11/";

        case bgfx::RendererType::Gnm:
            return "shaders/pssl/";

        case bgfx::RendererType::Metal:
            return "shaders/metal/";

        case bgfx::RendererType::OpenGL:
            return "../tools/viz/shaders/glsl/";

        case bgfx::RendererType::OpenGLES:
            return "shaders/essl/";

        case bgfx::RendererType::Vulkan:
            return "shaders/spirv/";

        // TODO: assert
        case bgfx::RendererType::Count:
            BX_CHECK(false, "You should not be here!");
            break;
    }
    return "<unknown renderer type>";
}

static bgfx::ShaderHandle LoadShader(bx::FileReaderI* reader, const std::string& name)
{
    auto file_path = ShaderPath(bgfx::getRendererType()) + name + ".bin";
    return bgfx::createShader(loadMem(reader, file_path));
}

bgfx::ShaderHandle LoadShader(const std::string& name)
{
    return LoadShader(entry::getFileReader(), name);
}

bgfx::ProgramHandle LoadProgram(bx::FileReaderI* reader,
                                const std::string& vs_name,
                                const std::string& fs_name)
{
    bgfx::ShaderHandle vsh = LoadShader(reader, vs_name);
    bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
    if (!fs_name.empty()) {
        fsh = LoadShader(reader, fs_name);
    }
    return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

bgfx::ProgramHandle LoadProgram(const std::string& vs_name, const std::string& fs_name)
{
    return LoadProgram(entry::getFileReader(), vs_name, fs_name);
}
