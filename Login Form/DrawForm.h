#pragma once

bool showPassword = false;
bool isRegistering = false;
bool resetHWID = false;
bool forgotPassword = false;
int Size_Of_Input_Fields = 200;
int spinner_Radius = 10;
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
	ImGui::PopFont();
	ImGui::PushFont(Smallfont);
	ImGui::PushItemWidth(Size_Of_Input_Fields);
	ImVec2 Alignment(ImGui::GetWindowSize().x / 2 - (ImGui::CalcItemWidth() / 2), 230);	//Align at center of window with respect to itemWidth
	ImGui::Indent(Alignment.x);
	ImGui::SetCursorPosY(Alignment.y);
	ImGui::Text("Username");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
	ImGui::InputText("##usernameField", globalUser.username, sizeof(globalUser.username), fetchingData ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_CharsNoBlank);
	ImGui::PopStyleColor();
	ImGui::Text(Command == FORGOT ? "New Password" : "Password");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
	ImGui::InputText("##passwordField", globalUser.password, sizeof(globalUser.password), showPassword ? ImGuiInputTextFlags_CharsNoBlank : ImGuiInputTextFlags_Password);
	ImGui::PopStyleColor();
	ImGui::SameLine();
	ImGui::Checkbox("##passworvisible", &showPassword);
	showPassword = ImGui::IsItemHovered();

	double inputOpacity = ImGui::DropDownAnimation("license", 0.6f, 40.0f, (Command == FORGOT || Command == REGISTER) ? DOWN : UP, { ImGuiCol_Text,ImGuiCol_FrameBg }, false);
	ImGui::Text("License");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, inputOpacity));
	ImGui::InputText("##License", globalUser.variable, sizeof(globalUser.variable));
	ImGui::PopStyleColor();
	ImGui::DropDownAnimation("license", 0.6f, 40.0f, DOWN, { ImGuiCol_Text,ImGuiCol_FrameBg }, true);



	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (140 / 2)+ImGui::GetStyle().WindowPadding.x/2);	//Set button at center of screen
	if (!fetchingData)
	{

		if (ImGui::Button(CommandStrings[Command].c_str(), ImVec2(140, 20)))
		{
			if (std::string(globalUser.username).size() < 1 || std::string(globalUser.password).size() < 1 || (Command == REGISTER || Command == FORGOT)? std::string(globalUser.variable).size()<1:false)
			{
				g_response = "Missing Field";
			}
			else
			{
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ServerFetch, (LPVOID)Command, 0, 0);
			}
		}
		if (g_response.size() > 1)
		{
			ImGui::TextCenterCol(g_response.substr(0, 100).c_str(), (g_response.rfind("Success") != -1) ? ImVec4(0, 1, 0, 1.0) : ImVec4(1, 0, 0, 1));
		}

	}
	else {
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ((spinner_Radius / 2) / 2));
		ImGui::Spinner("##loadingSpin", 10.0, 3, IM_COL32(0, 0, 0, 255));
	}
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - (80 / 2), ImGui::GetWindowSize().y - 60));
	if (ImGui::Button(Command == LOGIN ? "Register" : "Login", ImVec2(80, 20))) {Command = (Command == LOGIN) ? REGISTER : LOGIN;}
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2) - (160 * 2) / 2);
	if (ImGui::Button("Forgot Password?", ImVec2(160, 20))) { Command = FORGOT; }
	ImGui::SameLine();
	if (ImGui::Button("Reset HWID", ImVec2(160, 20))) { Command = RESET; }

	ImGui::PopItemWidth();

	
}