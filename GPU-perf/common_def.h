#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include <QMessageBox>
#include <QDebug>

namespace GPU_INFO {
    enum INFO_TABLE{
        PRODUCT=0,
        VENDOR,
        BOIS_VERSION,
        TECHNOLOGY,
        DIE_SIZE,
        RELEASE_DATE,
        TRANSISTORS,
        BUS_WIDTH,
        GFLOPS,
        BANDWIDTH,
        TEXTURE_FILLRATE,
        ROPS,
        PIXEL_FILLRATE,
        TMUS,
        GLES_VERSION,
        SHADERS,
        OGL_VERSION,
        MEMORY_SIZE,
        GLSL_VERSION,
        MEMORY_TYPE,
        ELSL_VERSION,
        MEM_CLOCK,
        VULKAN_VERSION,
        GPU_CLOCK,
        OCL_VERSION,
        MAX_POWER,
        CUDA_VERSION,
        DRIVER_VERSION,
        MAX
    };
}

#endif // COMMON_DEF_H
