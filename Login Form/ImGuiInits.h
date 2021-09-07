#include <deque>
#include <string>
#include <map>



ImVec4 TextColor = ImVec4(0.1, 0.1, 0.1, 1.0);
ImVec4 FrameColor = ImVec4(0.1, 0.1, 0.1, 1.0);

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void InitStyle();

enum direction {
	UP = 1,
	DOWN = 0
};


std::map<char*, float> animMap;
std::map<DWORD, ImVec4> fadeMap;
float fadeMult = 1.0;
double time_start = -1.0;

void InitStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGui::GetStyle().WindowPadding = ImVec2(10.0f, 10.0f);
	ImGui::GetStyle().FrameRounding = 5.0f;
	ImGui::GetStyle().WindowRounding = 5.0f;
	ImGui::GetStyle().ChildRounding = 0;
	ImGui::GetStyle().ItemInnerSpacing = ImVec2(8, 6);
	ImGui::GetStyle().IndentSpacing = 25.0f;
	ImGui::GetStyle().ScrollbarSize = 20.0f;
	ImGui::GetStyle().ScrollbarRounding = 5.0f;
	ImGui::GetStyle().GrabMinSize = 20.0f;
	ImGui::GetStyle().GrabRounding = 3.0f;
	ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0, 5);;


	colors[ImGuiCol_Text] = TextColor;
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(1.0, 1.0, 1.0, 1.0);
	colors[ImGuiCol_ChildBg] = FrameColor;
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.0, 0.0, 0.0, 0.5f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = FrameColor;
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(1, 1, 1, 1);
	colors[ImGuiCol_TitleBgActive] = ImVec4(1, 1, 1, 0.6f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1, 1, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(1, 1, 1, 0.5);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(1, 1, 1, 1.0f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1, 1, 1, 1.0f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1, 1, 1, 1.0f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.00f, 0.8f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(1.0, 1.0, 1.0, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.8, 0.8, 0.8, 1.0f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.0, 1.0, 0.1, 0.5f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}


bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

#define PI 3.1415926535897932f
void rotatePointAng(ImVec2& pointToRotate, float angle)
{

	angle = (angle * (PI / 180.f));
	float cosTheta = (cos(angle));
	float sinTheta = (sin(angle));
	float returnVecy = cosTheta * pointToRotate.y + sinTheta * pointToRotate.x;
	float returnVecx = cosTheta * pointToRotate.x - sinTheta * pointToRotate.y;


	pointToRotate = ImVec2((returnVecx), (returnVecy));
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImGui {
	void TextCenter(std::string text) {
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((ImGui::GetStyle().WindowPadding.x/2) +(windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}
	int CenterComponentX(int WidthOfComponent)
	{
		ImVec2 WindowSize = ImGui::GetWindowSize();
		return((WindowSize.x / 2) - (WidthOfComponent / 2));
	}
	//Colored Text
	//Takes string to center with respect to font size
	//Takes ImVec4 Color
	void TextCenterCol(std::string text, ImVec4 col) {
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((ImGui::GetStyle().WindowPadding.x / 2) + (windowWidth - textWidth) * 0.5f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::Text(text.c_str());
		ImGui::PopStyleColor();
	}
	bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		// Render
		const float circleStart = size.x * 0.7f;
		const float circleEnd = size.x;
		const float circleWidth = circleEnd - circleStart;

		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);


		const float t = g.Time;
		const float r = size.y / 2;
		const float speed = 1.5f;

		const float a = speed * 0;
		const float b = speed * 0.333f;
		const float c = speed * 0.666f;

		const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
		const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
		const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
	}
	//Drop Down Animation function to move a components y position while adjusting its ALPHA proportionally
	//*name is the unique label for the item in the map
	//*speed is the number of pixels to move per function call
	//*startingOffset is the components starting position. ALPHA is 0.0 at startingOffset
	//*direction use true for UP and false for DOWN
	//*ColorsToFade takes a vector of DWORD ImGui Color Styles whose alpha should be changed during the animation
	//Set ClearCols to true to pop the style vars after the items have been drawn
	//Return value is the Alpha
	double DropDownAnimation(char* name, float speed, float startingOffset, bool direction, std::vector<DWORD> ColorsToFade, bool ClearCols)
	{
		if (ClearCols)
		{
			ImGui::PopStyleColor(ColorsToFade.size());
			return 0.0f;
		}
		std::map<char*, float>::iterator it;
		it = animMap.find(name);
		//animMap[name] == Y OFFSET 
		if (it == animMap.end())
		{
			animMap[name] = startingOffset;
		}
		if (direction == DOWN)
		{
			if (animMap[name] > 0)
			{
				animMap[name] -= speed;
			}
		}
		else if(direction == UP)
		{
			if (animMap[name] < startingOffset)
			{
				animMap[name] += speed;
			}
		}
		for (auto& DWORD : ColorsToFade)
		{
			ImVec4 original = ImGui::GetStyleColorVec4(DWORD);
			ImGui::PushStyleColor(DWORD, ImVec4(original.x, original.y, original.z, abs(animMap[name]-startingOffset) / startingOffset));
		}
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - animMap[name]);
		return abs(animMap[name] - startingOffset) / startingOffset;
	}
	double LoginPageFade(std::vector<DWORD> StylesToFade, double animationDelay,double animationTime)
	{
		if (fadeMult <= 0.0)return 0.0;
		ImGuiContext& g = *GImGui;
		if (time_start == -1.0)
			time_start = g.Time;
		if (g.Time - time_start < animationDelay)
			return 1.0;
		fadeMult = 1.0 - ((g.Time - (time_start + animationDelay)) / animationTime);
		ImVec4* colors = ImGui::GetStyle().Colors;
		for (auto& style : StylesToFade)
		{
			std::map<DWORD,ImVec4>::iterator it;
			it = fadeMap.find(style);
			if (it == fadeMap.end())
			{
				fadeMap[style] = colors[style];
			}
			ImVec4 temp = fadeMap[style];
			temp.w *= fadeMult;
			ImGui::PushStyleColor(style, temp);
		}
		return fadeMult;
		
	}

	bool UpdateNotes(const char* label, const ImVec2& size_arg, ImVec4 color) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		//(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;
		const ImU32 yellow = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0, 1.0, 0.0, 1.0));
		int sizfe = 0;
		for (int i = 0; i < 3; i++) {
			window->DrawList->AddLine(ImVec2(2 + pos.x, (3 + (i * 5)) + pos.y), ImVec2(13 + pos.x, (3 + (i * 5)) + pos.y), ImGui::GetColorU32(color), 2.5f);
		}

	}
	
	bool DrawLine(const char* label, ImVec2 p1, ImVec2 p2, const ImVec2& size_arg, float thickness, ImVec4 color) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		if (!ItemAdd(bb, id))
			return false;
		window->DrawList->AddLine(ImVec2(pos.x + p1.x, pos.y + p1.y), ImVec2(pos.x + p2.x, pos.y + p2.y), ImGui::ColorConvertFloat4ToU32(color), thickness);

	}

	bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		// Render
		window->DrawList->PathClear();

		int num_segments = 60;
		int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

		const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

		const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

		for (int i = 0; i < num_segments; i++) {
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
				centre.y + ImSin(a + g.Time * 8) * radius));
		}

		window->DrawList->PathStroke(color, false, thickness);
	}

}