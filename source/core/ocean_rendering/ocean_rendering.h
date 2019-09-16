#pragma once
#include "../../utils/base.h"
#include "../../utils/gl_inc.h"
#include "../../utils/gl_utils.h"
#include "../../utils/utils.h"

class OceanRenderer
{
public:
	OceanRenderer();
	~OceanRenderer();
	void set_camera(Camera* camera);
	void prepare();
	void update();
	void render();
private:
	float phillips_spectrum(float a, float l, glm::vec2 k, glm::vec2 wind_direction);
private:
	Camera* m_camera;
	glm::vec2 m_wind_direction;
	uint32_t m_number;
};
