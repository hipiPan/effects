#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <math.h>

#define PI 3.14159265359f
#define TWO_PI 6.28318530718f
#define INV_PI 0.31830988618f
#define INV_TWO_PI 0.15915494309f

inline glm::vec3 getAxisX(const glm::mat4& mat)
{
	return glm::vec3(mat[0][0], mat[0][1], mat[0][2]);
}

inline glm::vec3 getAxisY(const glm::mat4& mat)
{
	return glm::vec3(mat[1][0], mat[1][1], mat[1][2]);
}

inline glm::vec3 getAxisZ(const glm::mat4& mat)
{
	return glm::vec3(mat[2][0], mat[2][1], mat[2][2]);
}

inline glm::vec3 getTranslate(const glm::mat4& mat)
{
	return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
}

// screen to world space
inline glm::vec3 unProject(const glm::vec4& viewPort, const glm::vec2& point, float depth, const glm::mat4& viewProjectionMatrix)
{
	glm::mat4 inverseViewProjectionMatrix = glm::inverse(viewProjectionMatrix);
	glm::vec4 screen = glm::vec4((point.x - viewPort.x) / viewPort.z, ((viewPort.w - point.y) - viewPort.y) / viewPort.w, depth, 1.0f);

	// to[-1 , 1]
	screen.x = screen.x * 2.0f - 1.0f;
	screen.y = screen.y * 2.0f - 1.0f;
	screen.z = screen.z * 2.0f - 1.0f;

	glm::vec4 worldPoint = inverseViewProjectionMatrix * screen;
	worldPoint.x = worldPoint.x / worldPoint.w;
	worldPoint.y = worldPoint.y / worldPoint.w;
	worldPoint.z = worldPoint.z / worldPoint.w;

	glm::vec3 out = glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
	return out;
}

inline void printMatrixInfo(const glm::mat4& mat)
{
	printf("%f   %f   %f   %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
	printf("%f   %f   %f   %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
	printf("%f   %f   %f   %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
	printf("%f   %f   %f   %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

inline glm::mat4 createPerspective(float fov, float aspect, float near, float far)
{
	float thetaY = (fov)*3.1415926f/180.0f*0.5f;
	float tanThetaY = std::tan(thetaY);

	glm::mat4 ret = glm::mat4(0.0f);
	ret[0][0] = 1.0f / (aspect * tanThetaY);
	ret[1][1] = 1.0f / (tanThetaY);
	ret[2][2] = -(far + near)/(far - near);
	ret[2][3] = -2.0f*far*near/(far - near);
	ret[3][2] = -1.0f;
	ret = glm::transpose(ret);
	return ret;
}

inline glm::mat4 createOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	glm::mat4 ret(1);
	ret[0][0] = 2.0f / (right - left);
	ret[1][1] = 2.0f / (top - bottom);
	ret[2][2] = -2.0f / (far - near);
	ret[3][0] = -(right + left) / (right - left);
	ret[3][1] = -(top + bottom) / (top - bottom);
	ret[3][2] = -(far + near) / (far - near);
	return ret;
}

inline glm::mat4 createOrthographic(float width, float height, float near, float far)
{
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	return createOrthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
}

inline glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	glm::vec3 f(glm::normalize(center - eye));
	glm::vec3 s(normalize(cross(f, up)));
	glm::vec3 u(cross(s, f));

	glm::mat4 ret(1);
	ret[0][0] = s.x;
	ret[1][0] = s.y;
	ret[2][0] = s.z;
	ret[0][1] = u.x;
	ret[1][1] = u.y;
	ret[2][1] = u.z;
	ret[0][2] = -f.x;
	ret[1][2] = -f.y;
	ret[2][2] = -f.z;
	ret[3][0] = -dot(s, eye);
	ret[3][1] = -dot(u, eye);
	ret[3][2] = dot(f, eye);
	return ret;
}

inline float getRandom(unsigned int *seed0, unsigned int *seed1)
{
    *seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);
    *seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

    unsigned int ires = ((*seed0) << 16) + (*seed1);

    union {
        float f;
        unsigned int ui;
    } res;

    res.ui = (ires & 0x007fffff) | 0x40000000;
    return (res.f - 2.0f) / 2.0f;
}
