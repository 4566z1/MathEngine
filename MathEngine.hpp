#pragma once
#include "ImPainter.hpp"

#include <vector>
using std::vector;

namespace ME
{
	namespace MathStruct
	{
		struct Vector3
		{
			float x, y, z;

			Vector3 operator-(const Vector3& src)
			{
				Vector3 buf;
				buf.x = x - src.x;
				buf.y = y - src.y;
				buf.z = z - src.z;
				return buf;
			}
		};

		struct Vector2
		{
			float x, y;
		};

		struct TriangleVector
		{
			Vector3 point0;
			Vector3 point1;
			Vector3 point2;
		};

		struct Transform
		{
			Vector3 position = { 0.0f,0.0f,0.0f };
			Vector3 rotation = {0.0f, 0.0f, 0.0f};
			Vector3 scale = { 1.0f, 1.0f, 1.0f };
		};
	}

	namespace MathCalc
	{
		float dotproduct(const MathStruct::Vector3& vector1, const MathStruct::Vector3& vector2);
		MathStruct::Vector3 multiplica(const MathStruct::Vector3& vector1, const MathStruct::Vector3& vector2);
	}

	class Object
	{
	public:
		virtual ~Object() = default;
		Object() = default;

		 [[nodicard]] virtual vector<MathStruct::TriangleVector>& vertexes() { return m_vertexes; }
		 [[nodicard]] virtual MathStruct::Transform& transform() { return m_transform; }
	protected:
		MathStruct::Transform m_transform;
		vector<MathStruct::TriangleVector> m_vertexes;
	};

	class Sphere : public Object
	{
	public:
		~Sphere() override = default;
		Sphere(const float& radius = 100.0f) : m_radius(radius)
		{
			calc_vertexes();
		}
		
		[[nodiscard]] vector<MathStruct::TriangleVector>& vertexes() override { calc_vertexes(); return m_vertexes; };
		void set_radius(const float& radius) { m_radius = radius; }
		void calc_vertexes();
	private:
		float m_radius;
	};

	class Camera : public Object
	{
	public:
		~Camera() override = default;
		Camera()
		{
			m_transform.position.z = 1000.0f;
		}

		void set_camera(const float& r_distance, const float& d_distance)
		{
			m_r_distance = r_distance;
			m_d_distance = d_distance;
		}

		[[nodiscard]] MathStruct::Vector2 perspective_project(const MathStruct::Vector3& convert_vector) const;
	private:
		float m_r_distance = 1000.0f; // view range
		float m_d_distance = 1000.0f; // view distance
	};

	class MathEngine
	{
	public:
		virtual ~MathEngine() = default;
		MathEngine(wstring_view window_title, const int& width = 1280, const int& height = 800)
			: m_window_width(width),m_window_height(height), m_painter(window_title, width, height)
		{
			init();
		}

		void loop();
		void render(Object& object);
	private:
		int m_window_height;
		int m_window_width;

		ImPainter m_painter;
		Camera m_camera;

		void init();
	};
}
