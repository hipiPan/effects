#pragma once

#include "Core/Base.h"
#include "Renderer/Effects/BaseEffect.h"
#include "Math/Math.h"
#include <string>
#include <unordered_map>

EFFECTS_NAMESPACE_BEGIN

class Material
{
public:
    Material();
    ~Material();
    void setType(const EffectType& type) {
        mType = type;
    }
    EffectType getType() { return mType; }
    std::unordered_map<std::string, int>& getIntParams(){return mIntParams;}
    std::unordered_map<std::string, float>& getFloatParams(){return mFloatParams;}
    std::unordered_map<std::string, glm::vec3>& getFloat3Params(){return mFloat3Params;}
    std::unordered_map<std::string, glm::vec4>& getFloat4Params(){return mFloat4Params;}
    std::unordered_map<std::string, std::string>& getStringParams(){return mStringParams;}
protected:
    EffectType mType;
    std::unordered_map<std::string, int> mIntParams;
    std::unordered_map<std::string, float> mFloatParams;
    std::unordered_map<std::string, glm::vec3> mFloat3Params;
    std::unordered_map<std::string, glm::vec4> mFloat4Params;
    std::unordered_map<std::string, std::string> mStringParams;
};

EFFECTS_NAMESPACE_END