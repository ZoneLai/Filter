#include "Common.h"
#include "TriangleManager.h"
#include "GLUtil.h"
#include "shader/camera.frag"
#include "shader/camera.vert"
/* GL_OES_EGL_image_external */
#ifndef GL_OES_EGL_image_external
#define GL_TEXTURE_EXTERNAL_OES                                 0x8D65
#endif
template<> TriangleManager* Singleton<TriangleManager>::msSingleton = nullptr;
TriangleManager::TriangleManager()
    : _widgetWidth(0)
    , _widgetHeight(0)
    , _sProgramPlay(0)
    , _vaoId(0)
    , _vboBuffer(0)
    , _textureId(0)
    , _positionLoc(-1)
    , _textCoordLoc(-1)
    , _mvpMatrixLoc(-1)
    , _sampler2DLoc(-1)
    , _filterModeLoc(-1)
    , _changeColorLoc(-1)
    , _weightMagnLoc(-1)
    , _mvpMatrix(glm::mat4(1.0f))
    , _modelMatrix(glm::mat4(1.0f))
    , _viewMatrix(glm::mat4(1.0f))
    , _projectionMatrix(glm::mat4(1.0f))
    , _bmpBuffer(nullptr)
    , _filterMode(FILTER::DEFAULT)
    , _filterColor{0.0f}
    , _mWeightManifier(0.0f)
{
}

TriangleManager::~TriangleManager() {
}

void TriangleManager::onCreate() {
}

void TriangleManager::onResume() {
}

void TriangleManager::onPause() {
}

void TriangleManager::onStop() {
}

void TriangleManager::onDestroy() {
    glDeleteVertexArrays(1, &_vaoId);
    glDeleteBuffers(1, &_vboBuffer);
    glDeleteTextures(1, &_textureId);
    glDeleteProgram(_sProgramPlay);
    if (_bmpBuffer != nullptr) {
        delete[] _bmpBuffer;
        _bmpBuffer = nullptr;
    }
}

void TriangleManager::initGL(int widgetWidth, int widgetHeight) {
    _widgetWidth    = widgetWidth;
    _widgetHeight   = widgetHeight;
    if (CompileShaderProgram(camera_play_vert, camera_play_frag, &_sProgramPlay)) {
        _positionLoc	    = glGetAttribLocation(_sProgramPlay,    "a_Position");
        _textCoordLoc		= glGetAttribLocation(_sProgramPlay,    "a_TextCoord");
        _mvpMatrixLoc       = glGetUniformLocation(_sProgramPlay,   "u_MvpMatrix");
        _sampler2DLoc       = glGetUniformLocation(_sProgramPlay,   "u_Texture");
        _filterModeLoc      = glGetUniformLocation(_sProgramPlay,   "u_FilterMode");
        _changeColorLoc     = glGetUniformLocation(_sProgramPlay,   "u_ChangeColor");
        _weightMagnLoc      = glGetUniformLocation(_sProgramPlay,   "u_Weight");
        GLfloat vertices[]  = {
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 1
             0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // 3
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // 3
             0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // 2
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f  // 4
        };
        glGenVertexArrays(1, &_vaoId);
        glBindVertexArray(_vaoId);
        glGenBuffers(1, &_vboBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vboBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 顶点坐标
        glVertexAttribPointer(
            _positionLoc,
            3,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GL_FLOAT),
            (GLvoid*)0
        );
        glEnableVertexAttribArray(_positionLoc);
        // 纹理坐标
        glVertexAttribPointer(
            _textCoordLoc,
            2,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GL_FLOAT),
            (GLvoid*)(3 * sizeof(GL_FLOAT))
        );
        glEnableVertexAttribArray(_textCoordLoc);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        // 创建纹理
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
       // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _imageWidth, _imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _bmpBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        LOGE("CompileShaderProgram===================");
    }
}

void TriangleManager::drawFrame() {
    glViewport(0, 0, _widgetWidth, _widgetHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(_vaoId);
    glUseProgram(_sProgramPlay);
    glUniformMatrix4fv(_mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));

    glUniform1i(_filterModeLoc, (int)_filterMode);
    glUniform3fv(_changeColorLoc, 1, _filterColor);
    glUniform1i(_weightMagnLoc, _mWeightManifier);

    // 启用纹理单元 绑定纹理对象
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glUniform1i(_sampler2DLoc, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

void TriangleManager::onChange(int widgetWidth, int widgetHeight) {
    _widgetWidth        = widgetWidth;
    _widgetHeight       = widgetHeight;
    _mWeightManifier    = (float)_widgetWidth / _widgetHeight;
    _modelMatrix	    = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    _viewMatrix         = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    _projectionMatrix   = glm::perspective(glm::radians(45.0f), static_cast<float>(_widgetWidth) / static_cast<float>(_widgetHeight), 0.1f, 100.0f);
    _mvpMatrix		    = _projectionMatrix * _viewMatrix * _modelMatrix;
}

void TriangleManager::setFilterMode(FILTER filterMode) {
    switch (filterMode) {
        case FILTER::DEFAULT:
        {
            _filterMode = FILTER::DEFAULT;
            memcpy(_filterColor, _defaultFilter, 3 * sizeof(float));
        }
        break;
        case FILTER::GRAY:
        {
            _filterMode = FILTER::GRAY;
            memcpy(_filterColor, _grayFilter, 3 * sizeof(float));
        }
        break;
        case FILTER::COOL:
        {
            _filterMode = FILTER::COOL;
            memcpy(_filterColor, _coolFilter, 3 * sizeof(float));
        }
        break;
        case FILTER::WARM:
        {
            _filterMode = FILTER::WARM;
            memcpy(_filterColor, _warmFilter, 3 * sizeof(float));
        }
        break;
        case FILTER::BLUR:
        {
            _filterMode = FILTER::BLUR;
            memcpy(_filterColor, _blurFilter, 3 * sizeof(float));
        }
        break;
        case FILTER::MAGNIFIER:
        {
            _filterMode = FILTER::MAGNIFIER;
            memcpy(_filterColor, _magnFilter, 3 * sizeof(float));
        }
        break;
    }
}

void TriangleManager::setAssetsBmp(AAssetManager* mgr,  const char* fileName) {
    AAsset* asset       = AAssetManager_open(mgr, fileName, AASSET_MODE_UNKNOWN);
    off_t fileSize      = AAsset_getLength(asset);
    uint8_t* buffer     = new uint8_t[fileSize];
    int numBytesRead    = AAsset_read(asset, buffer, fileSize);
    LOGE("numBytesRead = %d", numBytesRead);
    if (_bmpBuffer != nullptr) {
        delete[] _bmpBuffer;
        _bmpBuffer = nullptr;
    }
    _bmpBuffer = new uint8_t[fileSize];
    memcpy(_bmpBuffer, buffer, fileSize);
    delete[] buffer;
    buffer = nullptr;
    AAsset_close(asset);
}

GLint TriangleManager::getCameraTextureId() {
    return _textureId;
}