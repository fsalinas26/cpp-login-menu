#pragma once
#include "UserClass.h"

bool showPassword = false;
bool isRegistering = false;
bool beginLogin = false;

int Size_Of_Input_Fields = 200;
int spinner_Radius = 10;
float dropButtonDown = 40.0f;
float fade_in_speed = 0.6f;
void Register()
{

}
void LoginPage()//Draw Login Form Page
{

	ImGui::PushFont(LargeFont);
	ImGui::TextCenter("Your Program Title");
	ImGui::PopFont();
	ImGui::PushFont(Smallfont);
	ImGui::PushItemWidth(Size_Of_Input_Fields);
	ImVec2 Alignment(ImGui::GetWindowSize().x / 2 - (ImGui::CalcItemWidth()/2), 230);//Align at center of window with respect to itemWidth
	ImGui::Indent(Alignment.x);
	ImGui::SetCursorPosY(Alignment.y);
	ImGui::Text("Username");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
	ImGui::InputText("##usernameField", globalUser.username,sizeof(globalUser.username), beginLogin ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_CharsNoBlank);
	ImGui::PopStyleColor();
	ImGui::Text("Password");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
	ImGui::InputText("##passwordField", globalUser.password,sizeof(globalUser.password), beginLogin ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_CharsNoBlank | (showPassword ? ImGuiInputTextFlags_CharsNoBlank : ImGuiInputTextFlags_Password));
	ImGui::PopStyleColor();
	ImGui::SameLine();
	ImGui::Checkbox("##passworvisible", &showPassword);
	if (isRegistering)
	{
		if (dropButtonDown > 0)
			dropButtonDown -= fade_in_speed;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - dropButtonDown);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.1, 0.1, abs((dropButtonDown)-40.0f)/ 40.0f));
		ImGui::Text("License");
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 1.0, abs((dropButtonDown)-40.0f) / 40.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(FrameColor.x, FrameColor.y, FrameColor.z, abs((dropButtonDown)-40.0f) / 40.0f));
		ImGui::InputText("##License", globalUser.variable, sizeof(globalUser.variable));
		ImGui::PopStyleColor(2);
	}
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (80 / 2));//Set button at center of screen
	if (!beginLogin)
	{
		
		if (ImGui::Button(isRegistering ? "Register":"Login", ImVec2(80, 20)))
		{
			beginLogin = true;
			showPassword = true;
		}
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - (80 / 2), ImGui::GetWindowSize().y - 30));
		if (ImGui::Button(isRegistering ? "Login" : "Register", ImVec2(80, 20))) { dropButtonDown = 45; isRegistering = !isRegistering; }
		
		
	}
	else {
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ((spinner_Radius / 2) / 2));
		ImGui::Spinner("##loadingSpin", 10.0, 3, IM_COL32(0, 0, 0, 255));
	}


	ImGui::PopItemWidth();

	
}