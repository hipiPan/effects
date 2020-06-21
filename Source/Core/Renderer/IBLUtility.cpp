#include "IBLUtility.h"
#include "../Gfx/GfxResources.h"
#include "../RenderResources/Mesh.h"
#include "../Utility/FileUtility.h"
#include "../../Math/Math.h"
#include "../../Importers/TextureImporter.h"
EFFECTS_NAMESPACE_BEGIN

    static int gStandardWidth = 512;
    static int gStandardHeight = 512;
    static glm::mat4 gCaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    static glm::mat4 gCaptureViews[] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    IBLUtility::IBLUtility()
    {
        mCubeMesh = genCubeMesh();

        GfxRenderbufferDesc captureRenderbufferDesc;
        captureRenderbufferDesc.width = gStandardWidth;
        captureRenderbufferDesc.height = gStandardHeight;
        captureRenderbufferDesc.internalformat = GL_DEPTH_COMPONENT24;
        mCaptureRenderbuffer = createGfxRenderbuffer(captureRenderbufferDesc);

        mCaptureFramebuffer = createGfxFramebuffer();
        attachGfxFramebufferRenderBuffer(mCaptureFramebuffer, mCaptureRenderbuffer);

        std::string equirectangularToCubemapVertSource;
        std::string equirectangularToCubemapFragSource;
        readFileData("./BuiltinResources/Shaders/ibl/equirectangularToCubemap.vs", equirectangularToCubemapVertSource);
        readFileData("./BuiltinResources/Shaders/ibl/equirectangularToCubemap.fs", equirectangularToCubemapFragSource);
        GfxProgramDesc equirectangularToCubemapProgramDesc;
        equirectangularToCubemapProgramDesc.vertSource = equirectangularToCubemapVertSource;
        equirectangularToCubemapProgramDesc.fragSource = equirectangularToCubemapFragSource;
        equirectangularToCubemapProgramDesc.define = "";
        mEquirectangularToCubemapProgram = createGfxProgram(equirectangularToCubemapProgramDesc);

        GfxSamplerDesc samplerDesc;
        mSampler = createGfxSampler(samplerDesc);
    }

    IBLUtility::~IBLUtility()
    {
        if(mCubeMesh)
            delete mCubeMesh;
        destroyGfxTexture(mHDRMap);
        destroyGfxTexture(mEnvCubeMap);
        destroyGfxSampler(mSampler);
        destroyGfxFramebuffer(mCaptureFramebuffer);
        destroyGfxRenderbuffer(mCaptureRenderbuffer);
        destroyGfxProgram(mEquirectangularToCubemapProgram);
    }

    void IBLUtility::loadHdrEnvMap(const std::string &path)
    {
        // load hdr texture
        int width, height, channels;
        float* pixels = loadFloatImage(path.c_str(), width, height, channels, true);
        GfxTextureDesc hdrTextureDesc;
        hdrTextureDesc.width = width;
        hdrTextureDesc.height = height;
        hdrTextureDesc.componentType = GL_FLOAT;
        hdrTextureDesc.internalFormat = GL_RGB16F;
        hdrTextureDesc.format = GL_RGB;

        mHDRMap = createGfxTexture(hdrTextureDesc);
        writeGfxTextureData(mHDRMap, pixels);
        setGfxTextureSampler(mHDRMap, mSampler);
        delete[] pixels;

        // create env cubemap
        GfxTextureDesc envTextureDesc;
        envTextureDesc.width = gStandardWidth;
        envTextureDesc.height = gStandardHeight;
        envTextureDesc.componentType = GL_FLOAT;
        envTextureDesc.format = GL_RGB;
        envTextureDesc.internalFormat = GL_RGB16F;
        envTextureDesc.arraySize = 6;
        envTextureDesc.depth = 1;
        mEnvCubeMap = createGfxTexture(envTextureDesc);

//        std::vector<std::string> faces {
//                "./BuiltinResources/Textures/skybox/right.jpg",
//                "./BuiltinResources/Textures/skybox/left.jpg",
//                "./BuiltinResources/Textures/skybox/top.jpg",
//                "./BuiltinResources/Textures/skybox//bottom.jpg",
//                "./BuiltinResources/Textures/skybox//front.jpg",
//                "./BuiltinResources/Textures/skybox/back.jpg",
//        };
//
//        for (unsigned int i = 0; i < faces.size(); i++)
//        {
//            unsigned char *data = loadImage(faces[i].c_str(), width, height, channels, false);
//            if (data)
//            {
//                writeGfxTextureData(mEnvCubeMap, data, width, height, i);
//                delete[] data;
//            }
//        }

        writeGfxTextureData(mEnvCubeMap, nullptr, 0);
        writeGfxTextureData(mEnvCubeMap, nullptr, 1);
        writeGfxTextureData(mEnvCubeMap, nullptr, 2);
        writeGfxTextureData(mEnvCubeMap, nullptr, 3);
        writeGfxTextureData(mEnvCubeMap, nullptr, 4);
        writeGfxTextureData(mEnvCubeMap, nullptr, 5);
        setGfxTextureSampler(mEnvCubeMap, mSampler);

        glViewport(0, 0, gStandardWidth, gStandardHeight);
        for (int i = 0; i < 6; ++i)
        {
            attachGfxFramebufferCubeMap(mCaptureFramebuffer, 0, i, mEnvCubeMap);
            bindGfxFramebuffer(mCaptureFramebuffer);
            glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            setGfxProgramMat4(mEquirectangularToCubemapProgram, "u_projection", &gCaptureProjection[0][0]);
            setGfxProgramMat4(mEquirectangularToCubemapProgram, "u_view", &gCaptureViews[i][0][0]);
            setGfxProgramSampler(mEquirectangularToCubemapProgram, "u_equirectangularMap", mHDRMap);
            bindGfxProgram(mEquirectangularToCubemapProgram);
            mCubeMesh->draw(GL_TRIANGLES);
            unbindGfxProgram(mEquirectangularToCubemapProgram);
            unbindGfxFramebuffer(mCaptureFramebuffer);
        }

    }
EFFECTS_NAMESPACE_END
