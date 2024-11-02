#pragma once
#include <imgui.h>
#include <Windows.h>

#include <string_view>
using std::wstring_view;

class ImPainter
{
public:
	virtual ~ImPainter();
	ImPainter(wstring_view window_title, 
		const int& width, 
		const int& height, 
		const ImVec4& background_color = { 0.45f, 0.55f, 0.60f, 1.00f });

	[[nodiscard]] const bool& is_done() const { return m_is_done; }
	void begin();
	void end();

	[[nodiscard]] ImDrawList& draw() const { return *m_draw_list; }
private:
	bool m_is_done = false;

	ImDrawList* m_draw_list;
	ImVec4 m_background_color;
	WNDCLASSEXW m_wndclass;
	HWND m_hwnd;
};
