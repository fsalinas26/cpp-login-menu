#pragma once
#include "CallbackThread.h"

bool showPassword = false;
bool isRegistering = false;
bool resetHWID = false;
bool forgotPassword = false;
float Size_Of_Input_Fields = 200.0;
int spinner_Radius = 10;
float fadeLoginPage = 1.0;

Call Command = LOGIN;

std::vector<std::string> CommandStrings = {
	"Login",
	"Register",
	"Reset Password",
	"Reset HWID"
};

void LoginPage()//Draw Login Form Page
{
	ImGui::PushFont(LargeFont);
	ImGui::TextCenter("Your Program Title");
	if (g_response == "Login Success")
	{
		fadeLoginPage = ImGui::LoginPageFade({ ImGuiCol_Text,ImGuiCol_FrameBg,ImGuiCol_Button },0.4,1.0f);
		ImGui::SetCursorPosY((ImGui::GetWindowSize().y /2 - 30.0) - (30.0 * fadeLoginPage));
		ImGui::TextCenterCol(("Welcome, " + std::string(globalUser.username)).c_str(), ImVec4(TextColor.x, TextColor.y, TextColor.z, 1.0 - fadeLoginPage));
		if (fadeLoginPage == 0.0)return;
	}
	ImGui::PopFont();
	ImGui::PushFont(Smallfont);
	ImGui::PushItemWidth(Size_Of_Input_Fields*fadeLoginPage);
	ImVec2 Alignment(ImGui::GetWindowSize().x / 2 - (ImGui::CalcItemWidth() / 2), 230);	//Align at center of window with respect to itemWidth
	ImGui::Indent(Alignment.x);
	ImGui::SetCursorPosY(Alignment.y);
	ImGui::Text("Username");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1.0* fadeLoginPage));
	ImGui::InputText("##usernameField", globalUser.username, sizeof(globalUser.username), fetchingData ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_CharsNoBlank);
	ImGui::PopStyleColor();
	ImGui::Text(Command == FORGOT_PASSWORD ? "New Password" : "Password");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1 * fadeLoginPage));
	ImGui::InputText("##passwordField", globalUser.password, sizeof(globalUser.password), showPassword ? ImGuiInputTextFlags_CharsNoBlank : ImGuiInputTextFlags_Password);
	ImGui::PopStyleColor();
	ImGui::SameLine();
	ImGui::Checkbox("##passworvisible", &showPassword);
	showPassword = ImGui::IsItemHovered();
	double inputOpacity = ImGui::DropDownAnimation("license", 0.6f, 40.0f, (Command == FORGOT_PASSWORD || Command == REGISTER) ? DOWN : UP, { ImGuiCol_Text,ImGuiCol_FrameBg }, false);
	ImGui::Text("License");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, inputOpacity*fadeLoginPage));
	ImGui::InputText("##License", globalUser.variable, sizeof(globalUser.variable));
	ImGui::PopStyleColor();
	ImGui::DropDownAnimation("license", 0.6f, 40.0f, DOWN, { ImGuiCol_Text,ImGuiCol_FrameBg }, true);



	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (140 / 2)+ImGui::GetStyle().WindowPadding.x/2);	//Set button at center of screen
	if (!fetchingData)
	{

		if (ImGui::Button(CommandStrings[Command].c_str(), ImVec2(140, 20)) || GetAsyncKeyState(VK_RETURN) & WM_KEYUP)
		{
			if (std::string(globalUser.username).size() < 1 || std::string(globalUser.password).size() < 1 || (Command == REGISTER || Command == FORGOT_PASSWORD)? std::string(globalUser.variable).size()<1:false)
			{
				g_response = "Missing Field";
			}
			else
			{
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ServerCall, (LPVOID)Command, 0, 0);
			}
		}
		if (g_response.size() > 1)
		{
			ImGui::TextCenterCol(g_response.substr(0, 100).c_str(), (g_response.rfind("Success") != -1) ? ImVec4(0, 1, 0, 1.0*fadeLoginPage) : ImVec4(1, 0, 0, 1.0*fadeLoginPage));
		}

	}
	else {
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ((spinner_Radius / 2) / 2));
		ImGui::Spinner("##loadingSpin", 10.0, 3, IM_COL32(0, 0, 0, 255));
	}
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - (80 / 2), ImGui::GetWindowSize().y - 60));
	if (ImGui::Button(Command == LOGIN ? "Register" : "Login", ImVec2(80, 20))) {Command = (Command == LOGIN) ? REGISTER : LOGIN;}
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2) - (160 * 2) / 2);
	if (ImGui::Button("Forgot Password?", ImVec2(160, 20))) { Command = FORGOT_PASSWORD; }
	ImGui::SameLine();
	if (ImGui::Button("Reset HWID", ImVec2(160, 20))) { Command = RESET_HWID; }

	ImGui::PopItemWidth();

	
}