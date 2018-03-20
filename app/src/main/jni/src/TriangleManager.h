#pragma once
#include <time.h>
#include <math.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include "Singleton.h"
#include "glm.hpp"
#include "ext.hpp"
#include <thread>
#include "JniHelper.h"
class TriangleManager : public Singleton<TriangleManager> {
public:
    enum class FILTER {
        DEFAULT                 = 0,    // 默认
        GRAY                    = 1,    // 黑白
        COOL                    = 2,    // 冷色调
        WARM                    = 3,    // 暖色调
        BLUR                    = 4,    // 模糊
        MAGNIFIER               = 5     // 放大镜
    };

public:
    TriangleManager();
    ~TriangleManager();
    void                        onCreate();
    void                        onResume();
    void                        onPause();
    void                        onStop();
    void                        onDestroy();
    void                        initGL(int widgetWidth, int widgetHeight);
    void                        drawFrame();
    void                        onChange(int widgetWidth, int widgetHeight);
    void                        setAssetsBmp(AAssetManager* mgr, const char* fileName);
    void                        setFilterMode(FILTER filterMode);
    GLint                       getCameraTextureId();

private:
    int                         _widgetWidth;
    int                         _widgetHeight;
    GLuint                      _sProgramPlay;
    GLuint						_vaoId;
    GLuint						_vboBuffer;
    GLuint                      _textureId;
    GLint						_positionLoc;
    GLint						_textCoordLoc;
    GLint						_mvpMatrixLoc;
    GLint                       _sampler2DLoc;
    GLint                       _filterModeLoc;
    GLint                       _changeColorLoc;
    GLint                       _weightMagnLoc;
    glm::mat4                   _mvpMatrix;
    glm::mat4                   _modelMatrix;
    glm::mat4                   _viewMatrix;
    glm::mat4                   _projectionMatrix;
    uint8_t*                    _bmpBuffer;
    FILTER                      _filterMode;
    const float                 _defaultFilter[3] = { 0.0f, 0.0f, 0.0f };
    const float                 _grayFilter[3] = { 0.299f, 0.587f, 0.114f };
    const float                 _coolFilter[3] = { 0.0f, 0.0f, 0.1f };
    const float                 _warmFilter[3] = { 0.1f, 0.1f, 0.0f };
    const float                 _blurFilter[3] = { 0.006f, 0.004f, 0.002f };
    const float                 _magnFilter[3] = { 0.0f, 0.0f, 0.4f };
    float                       _filterColor[3];
    float                       _mWeightManifier;
};