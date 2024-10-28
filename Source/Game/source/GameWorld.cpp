#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <fstream>
#include <sstream>
#include <filesystem>

void GameWorld::Init()
{
	if (!std::filesystem::exists("imgui.ini"))
	{
		ImGui::LoadIniSettingsFromDisk("DefaultPanelLayout.ini");
	}

	auto& engine = *Tga::Engine::GetInstance();

	Tga::Vector2ui intResolution = engine.GetRenderSize();
	Tga::Vector2f resolution = { (float)intResolution.x, (float)intResolution.y };


	//center window to screen
	{
		RECT rect;
		GetWindowRect(*engine.GetHWND(), &rect);

		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		int x = (screenWidth - windowWidth) / 2;
		int y = (screenHeight - windowHeight) / 2;

		SetWindowPos(*engine.GetHWND(), HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}


	//pugi::xml_parse_result result = myDocument.load_file("Getter.snippet");

	//assert(result == true);

	//myDocument.save_file("GetterOut.snippet");
	Load();




	auto lang = TextEditor::LanguageDefinition::CPlusPlus();

	// set your own known preprocessor symbols...
	static const char* ppnames[] = { "NULL", "PM_REMOVE",
		"ZeroMemory", "DXGI_SWAP_EFFECT_DISCARD", "D3D_FEATURE_LEVEL", "D3D_DRIVER_TYPE_HARDWARE", "WINAPI","D3D11_SDK_VERSION", "assert" };
	// ... and their corresponding values
	static const char* ppvalues[] = {
		"#define NULL ((void*)0)",
		"#define PM_REMOVE (0x0001)",
		"Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ",
		"enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0",
		"enum D3D_FEATURE_LEVEL",
		"enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
		"#define WINAPI __stdcall",
		"#define D3D11_SDK_VERSION (7)",
		" #define assert(expression) (void)(                                                  \n"
		"    (!!(expression)) ||                                                              \n"
		"    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
		" )"
	};

	for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = ppvalues[i];
		lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
	}

	// set your own identifiers
	static const char* identifiers[] = {
		"HWND", "HRESULT", "LPRESULT","D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC","MSG","LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		"ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		"ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		"IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor" };
	static const char* idecls[] =
	{
		"typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
		"typedef tagMSG MSG\n * Message structure","typedef LONG_PTR LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		"ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		"ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		"IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor" };
	for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = std::string(idecls[i]);
		lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
	}
	myEditor.SetLanguageDefinition(lang);
	//ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("../Source/External/imgui/misc/fonts/Cousine-Regular.ttf", 1.0f);
	//myEditor.SetPalette(TextEditor::GetLightPalette());

	// error markers
	//TextEditor::ErrorMarkers markers;
	//markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
	//markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
	//myEditor.SetErrorMarkers(markers);

	// "breakpoint" markers
	//TextEditor::Breakpoints bpts;
	//bpts.insert(24);
	//bpts.insert(47);
	//myEditor.SetBreakpoints(bpts);

	//static const char* myIncorrectFile = "ImGuiColorTextEdit/TextEditor.cpp";
	//	static const char* myIncorrectFile = "test.cpp";


	std::ifstream t(myIncorrectFile);
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		myEditor.SetText(str);
	}

	myEditor.SetShowWhitespaces(false);


}
void GameWorld::Update(float /*aTimeDelta*/)
{
	//ImGui::ShowDemoWindow();
	//ImGui::ShowStyleEditor();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			//if (ImGui::MenuItem("Create")) 
			//{

			//}
			//if (ImGui::MenuItem("Open", "Ctrl+O")) 
			//{

			//}
			if (ImGui::MenuItem(("Save to " + mySnippet.GetFile()).c_str()/*, "Ctrl+S"*/))
			{
				mySnippet.SetCode(myEditor.GetText());
				mySnippet.Save();
			}
			//ImGui::Text("Load");
			//ImGui::SameLine();
			ImGui::PushID(112);
			//if (ImGui::BeginCombo("", "", ImGuiComboFlags_NoPreview))
			if (ImGui::BeginMenu("Load"))
			{
				std::filesystem::path directory = std::filesystem::current_path();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.4f, 0.8f, 1.f));
				ImGui::Text(("Folder: " + directory.string()).c_str());
				ImGui::PopStyleColor();
				for (const auto& entry : std::filesystem::directory_iterator(directory))
				{
					if (entry.is_regular_file())
					{
						std::string extension = entry.path().extension().string();
						if (extension == ".snippet")
						{
							//std::cout << "File found: " << entry.path() << std::endl;
							//ImGui::Text(entry.path().string().c_str());
							if (ImGui::MenuItem(entry.path().filename().string().c_str()))
							{
								Load(entry.path().string());
							}
						}
					}
				}
				ImGui::EndMenu();
				//ImGui::EndCombo();
			}
			ImGui::PopID();

			//if (ImGui::MenuItem("Save as..")) 
			//{
			//	mySnippet.Save();
			//}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (ImGui::Begin("Propperties"))
	{
		ImGuiInputText(mySnippet.GetTitleChild(), "Title", 0);


		ImGuiInputText(mySnippet.GetShortcutChild(), "Shortcut", 1);


		ImGuiInputText(mySnippet.GetDescriptionChild(), "Description", 2);


		ImGuiInputText(mySnippet.GetAuthorChild(), "Author", 3);




		//static bool wantExpansion = false;
		//static bool wantRefactoring = false;
		//static bool wantSurroundsWith = false;
		ImGuiSnippetTypeCheckBox("Expansion", mySnippet.myImGui.expansion);
		ImGuiSnippetTypeCheckBox("Refactoring", mySnippet.myImGui.refactoring);
		ImGuiSnippetTypeCheckBox("SurroundsWith", mySnippet.myImGui.surroundsWith);



		if (ImGui::Button("Add Variable"))
		{
			mySnippet.AddLiteral();
			//mySnippet.SetLiteral("testID", "gfdsfg", "fgdsg", "tggrr");
		}

		//static int currentItem = 0;
		//static const char* vars[] = { "gfds", "gfdfgd" };

		//ImGui::Combo("Variables", &currentItem, vars, 2);

		if (ImGui::BeginCombo("Variables", "", ImGuiComboFlags_HeightLarge))
		{
			std::vector<pugi::xml_node> literals = mySnippet.GetAllLiterals();

			for (size_t i = 0; i < literals.size(); i++)
			{
				ImGui::PushID(1000 + (int)i);
				if (ImGui::Button((std::string("") + "Delete " + literals[i].child("ID").first_child().value()).c_str()))
				{
					mySnippet.RemoveLiteral(literals[i].child("ID").first_child().value());
				}
				ImGui::PopID();

				ImGuiInputText(literals[i].child("ID").first_child(), "ID", (int)i + 4);


				ImGuiInputText(literals[i].child("ToolTip").first_child(), "ClassName", (int)i + 5);


				ImGuiInputText(literals[i].child("Default").first_child(), "Default Value", (int)i + 6);

				ImGui::Text("");
			}

			ImGui::EndCombo();
		}



	}
	ImGui::End();

	TextEditorWindow();
	ImGui::PushID(5555555);
	if (ImGui::Begin("File", false, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text((mySnippet.GetFile() + " - This updates when you save").c_str());
			ImGui::EndMenuBar();
		}
		std::ifstream file(mySnippet.GetFile());
		std::stringstream buffer;
		buffer << file.rdbuf();
		ImGui::Text(buffer.str().c_str());
		file.close();
	}
	ImGui::End();
	ImGui::PopID();

	if (ImGui::Begin("ErrorWindow"))
	{
		//ImGui::Text("Errors and warnings goes here");
		//ImGui::Text(myEditor.GetText().c_str());
	}
	ImGui::End();


}

void GameWorld::Render()
{
}

void GameWorld::TextEditorWindow()
{

	auto cpos = myEditor.GetCursorPosition();
	ImGui::Begin("Code ", nullptr, ImGuiWindowFlags_HorizontalScrollbar /*| ImGuiWindowFlags_MenuBar*/);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	//if (ImGui::BeginMenuBar())
	//{
	//	/*if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Save"))
	//		{
	//			auto textToSave = myEditor.GetText();
	//			/// save text....
	//		}
	//		ImGui::EndMenu();
	//	}*/
	//	if (ImGui::BeginMenu("Edit"))
	//	{
	//		bool ro = myEditor.IsReadOnly();
	//		if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
	//			myEditor.SetReadOnly(ro);
	//		ImGui::Separator();

	//		if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && myEditor.CanUndo()))
	//			myEditor.Undo();
	//		if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && myEditor.CanRedo()))
	//			myEditor.Redo();

	//		ImGui::Separator();

	//		if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, myEditor.HasSelection()))
	//			myEditor.Copy();
	//		if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && myEditor.HasSelection()))
	//			myEditor.Cut();
	//		if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && myEditor.HasSelection()))
	//			myEditor.Delete();
	//		if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
	//			myEditor.Paste();

	//		ImGui::Separator();

	//		if (ImGui::MenuItem("Select all", nullptr, nullptr))
	//			myEditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(myEditor.GetTotalLines(), 0));

	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu("View"))
	//	{
	//		if (ImGui::MenuItem("Dark palette"))
	//			myEditor.SetPalette(TextEditor::GetDarkPalette());
	//		if (ImGui::MenuItem("Light palette"))
	//			myEditor.SetPalette(TextEditor::GetLightPalette());
	//		if (ImGui::MenuItem("Retro blue palette"))
	//			myEditor.SetPalette(TextEditor::GetRetroBluePalette());
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, myEditor.GetTotalLines(),
		myEditor.IsOverwrite() ? "Ovr" : "Ins",
		myEditor.CanUndo() ? "*" : " ",
		myEditor.GetLanguageDefinition().mName.c_str(), myIncorrectFile);
	//myEditor.IsColorizerEnabled();
	ErrorHandling();
	ChangeLiteralNameFromEditor();


	myLiteralIndecies.clear();
	myEditor.Render("TextEditor");
	ImGui::End();

}

void GameWorld::ErrorHandling()
{
	TextEditor::ErrorMarkers err;
	std::vector<std::string> lines = myEditor.GetTextLines();
	std::vector<pugi::xml_node> literals = mySnippet.GetAllLiterals();
	int currentLine = myEditor.GetCursorPosition().mLine;
	int endCount = 0;

	for (size_t i = 0; i < lines.size(); i++)
	{
		if (i == currentLine)
		{
			continue;
		}
		int key = (int)i + 1;
		int count$ = 0;

		for (size_t pos = 0; (pos = lines[i].find('$', pos)) != std::string::npos; pos++)
		{
			if (myLiteralIndecies.find((int)i) == myLiteralIndecies.end())
			{
				myLiteralIndecies[(int)i] = {};
			}
			myLiteralIndecies.at((int)i).push_back((int)pos);
			count$++;
		}

		if (count$ % 2 != 0)
		{
			err.insert({ key, "Error: Odd number of $" });
		}
		else if (myLiteralIndecies.find((int)i) != myLiteralIndecies.end())
		{
			for (size_t k = 0; k < myLiteralIndecies.at((int)i).size(); k += 2)
			{
				std::string found(lines[(int)i].begin() + myLiteralIndecies.at((int)i)[(int)k] + 1,
					lines[(int)i].begin() + myLiteralIndecies.at((int)i)[(int)k + 1]);

				if (mySnippet.HasLiteral(found))
				{
					if (err.find(key) != err.end())
					{
						err.insert({ key, "" });
					}
					if (found == "end")
					{
						if (endCount > 0)
						{
							err[key].append("Error: To many $end$ found\n");
						}
						endCount++;
					}
					else
					{
						err[key].append("Error: " + found + " is not a variable\n");
					}
				}
			}
		}
	}

	if (ImGui::Begin("ErrorWindow"))
	{
		if (endCount == 0)
		{
			ImGui::Text("Warning: $end$ is needed in the code");
		}
		else if (endCount > 1)
		{
			ImGui::Text("Warning: To many $end$ is found");
		}
	}
	ImGui::End();

	myEditor.SetErrorMarkers(err);
}

void GameWorld::ChangeLiteralNameFromEditor()
{
//	if (myEditor.HasSelection())
//	{
//		if (mySnippet.HasLiteral(myEditor.GetSelectedText()))
//		{
//			std::vector<std::string> text = myEditor.GetTextLines();
//
//			for each (auto obj in myLiteralIndecies)
//			{
//				for (int i = 0; i < obj.second.size(); i++)
//				{
//#pragma warning (push)
//#pragma warning (disable:4244)
//					for (int pos = 0; (pos = (int)text[(int)obj.first].find((int)obj.second[i], pos)) != std::string::npos; pos++)
//#pragma warning (pop)
//					{
//						ImGui::BeginTooltip();
//						ImGui::Text(std::string("X: " + std::to_string(obj.first) + " i: " + std::to_string(i) + " Y: " + std::to_string(obj.second[i])).c_str());
//						ImGui::Text(std::string(text.begin() + obj.second[i], text.begin() + obj.second[i+1]).c_str());
//						ImGui::EndTooltip();
//
//					}
//				}
//
//			}
//
//			//Change all instances in the text and the ID of the literal
//		}
//	}
}

void GameWorld::Tooltip(std::string aToolTip)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text(aToolTip.c_str());
		ImGui::EndTooltip();
	}
}

void GameWorld::ImGuiInputText(std::string* aTextVar, std::string aLabel)
{
	std::string tempString;
	tempString.reserve(myMaxString);
	tempString = *aTextVar;
	char* testText = tempString.data();
	ImGui::InputText(aLabel.c_str(), testText, myMaxString);
	*aTextVar = testText;
}

void GameWorld::ImGuiInputText(pugi::xml_node aNode, std::string aLabel, int anImGuiID)
{
	std::string tempString;
	tempString.reserve(myMaxString);
	tempString = aNode.value();
	ImGui::PushID(anImGuiID);
	ImGuiInputText(&tempString, aLabel);
	ImGui::PopID();
	aNode.set_value(tempString.c_str());
}

bool GameWorld::FindChildByStringValue(pugi::xml_node aNode, std::string aValue, pugi::xml_node& aOutNode)
{
	bool result = false;
	size_t childCount = std::distance(aNode.begin(), aNode.end());
	if (childCount > 0)
	{
		pugi::xml_node walkingChild = aNode.first_child();

		for (size_t i = 0; i < childCount; i++)
		{
			if (walkingChild.child_value() == aValue)
			{
				result = true;
				aOutNode = walkingChild;
				break;
			}
			walkingChild = walkingChild.next_sibling();
		}
	}

	return result;
}

void GameWorld::ImGuiSnippetTypeCheckBox(std::string aSnippetType, bool& aBoolRef)
{
	if (ImGui::Checkbox(aSnippetType.c_str(), &aBoolRef))
	{

		bool hasTheThing = mySnippet.GetSnippetType(aSnippetType);
		if (aBoolRef == true && hasTheThing == false)
		{
			mySnippet.AddSnippetType(aSnippetType);
		}
		else if (aBoolRef == false && hasTheThing == true)
		{
			mySnippet.RemoveSnippetType(aSnippetType);
		}
	}
}

void GameWorld::Load(std::string aFile)
{
	mySnippet.Load(aFile);
	myEditor.SetText(mySnippet.GetCode());
}
