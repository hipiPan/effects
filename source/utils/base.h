#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#define LOGE(...)                     \
    do                                \
    {                                 \
        fprintf(stderr, "[ERROR]: " __VA_ARGS__); \
        fflush(stderr); \
    } while (false)

#define LOGW(...)                     \
    do                                \
    {                                 \
        fprintf(stderr, "[WARN]: " __VA_ARGS__); \
        fflush(stderr); \
    } while (false)

#define LOGI(...)                     \
    do                                \
    {                                 \
        fprintf(stderr, "[INFO]: " __VA_ARGS__); \
        fflush(stderr); \
    } while (false)
