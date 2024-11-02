#include "MathEngine.hpp"
#include <numbers>
using namespace ME::MathStruct;

static int window_width;
static int window_height;

Vector2 ME::Camera::perspective_project(const Vector3& convert_vector) const
{
	Vector2 screen_point = { 0 };
	Vector3 view_point = { 0 };
	view_point.x = m_transform.position.x - convert_vector.x;
	view_point.y = m_transform.position.y - convert_vector.y;
	view_point.z = m_transform.position.z - convert_vector.z;
	screen_point.x = m_d_distance * view_point.x / view_point.z;
	screen_point.y = m_d_distance * view_point.y / view_point.z;

	return screen_point;
}

void ME::MathEngine::init()
{
	window_width = m_window_width;
	window_height = m_window_height;
}

void ME::MathEngine::loop()
{
	Sphere sphere;

	while(!m_painter.is_done())
	{
		m_painter.begin();
		render(sphere);

		// Window
		{
			ImGui::Begin("ToolBar", (bool*)(true), ImGuiWindowFlags_MenuBar);
			ImGui::SliderFloat("CameraX", &m_camera.transform().position.x, -500.0f, 500.0f);
			ImGui::SliderFloat("CameraY", &m_camera.transform().position.y,-500.0f, 500.0f);
			ImGui::SliderFloat("CameraZ", &m_camera.transform().position.z, -500.0f, 500.0f);

			ImGui::SliderFloat("SphereX", &sphere.transform().position.x, -500.0f, 500.0f);
			ImGui::SliderFloat("SphereY", &sphere.transform().position.y, -500.0f, 500.0f);
			ImGui::SliderFloat("SphereZ", &sphere.transform().position.z, -500.0f, 500.0f);
			ImGui::End();
		}

		m_painter.end();
	}
}

void ME::MathEngine::render(Object& object)
{
	auto& triangles = object.vertexes();
	for (auto triangle = triangles.begin();triangle < triangles.end() - 1;++triangle)
	{
		// Needs adding light system

		// Black-out
		Vector3 normal_vector = MathCalc::multiplica(triangle->point1 - triangle->point0, triangle->point2 - triangle->point0);
		Vector3 direct_vector = m_camera.transform().position - triangle->point0;
		float view_angle = MathCalc::dotproduct(direct_vector, normal_vector);
		if (view_angle < 0) continue;

		Vector2 screen_vector_0 = m_camera.perspective_project(triangle->point0);
		Vector2 screen_vector_1 = m_camera.perspective_project(triangle->point1);
		Vector2 screen_vector_2 = m_camera.perspective_project(triangle->point2);

		m_painter.draw().AddTriangle(
			{ screen_vector_0.x, screen_vector_0.y },
			{ screen_vector_1.x, screen_vector_1.y },
			{ screen_vector_2.x, screen_vector_2.y },
			ImGui::GetColorU32({ 1.0f,1.0f,1.0f,1.0f })
		);
	}
}

void ME::Sphere::calc_vertexes()
{
	m_vertexes.clear();
	for (float i = 0.0f;i < std::numbers::pi;i += 0.1f)
	{
		for (float j = 0.0f;j < 2 * std::numbers::pi;j += 0.1f)
		{
			TriangleVector triangle_vector;
			triangle_vector.point0.x = m_transform.position.x + m_radius * sin(j) * cos(i);
			triangle_vector.point0.y = m_transform.position.y + m_radius * sin(j) * sin(i);
			triangle_vector.point0.z = m_transform.position.z + m_radius * cos(j);

			triangle_vector.point1.x = m_transform.position.x + m_radius * sin(j - 0.1f) * cos(i);
			triangle_vector.point1.y = m_transform.position.y + m_radius * sin(j - 0.1f) * sin(i);
			triangle_vector.point1.z = m_transform.position.z + m_radius * cos(j - 0.1f);

			triangle_vector.point2.x = m_transform.position.x + m_radius * sin(j) * cos(i + 0.1f);
			triangle_vector.point2.y = m_transform.position.y + m_radius * sin(j) * sin(i + 0.1f);
			triangle_vector.point2.z = m_transform.position.z + m_radius * cos(j);
			m_vertexes.emplace_back(triangle_vector);
		}
	}
}

float ME::MathCalc::dotproduct(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

Vector3 ME::MathCalc::multiplica(const Vector3& vector1, const Vector3& vector2)
{
	Vector3 out_vector;
	out_vector.x = vector1.y * vector2.z - vector1.z * vector2.y;
	out_vector.y = vector1.z * vector2.x - vector1.x * vector2.z;
	out_vector.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return out_vector;
}