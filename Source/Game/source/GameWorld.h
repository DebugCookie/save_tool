#pragma once

#include "Snippet.h"
#include "imguiColorTextEdit/TextEditor.h"
#include "tge/math/vector2.h"
#include <string>
#include <unordered_map>

class GameWorld
{
public:
	GameWorld() = default; 
	~GameWorld() = default;

	void Init();
	void Update(float aTimeDelta); 
	void Render();

private:

	void TextEditorWindow();
	void ErrorHandling();
	void ChangeLiteralNameFromEditor();

	//pugi::xml_document myDocument;

	TextEditor myEditor;
	const char* myIncorrectFile = "TextEditorTest.txt";
	//std::string myCurrentFile = "Getter.snippet";
	//index, line
	std::unordered_map<int, std::vector<int>> myLiteralIndecies;

	Snippet mySnippet;

	const size_t myMaxString = 128;
	void Tooltip(std::string aToolTip);
	void ImGuiInputText(std::string* aTextVar, std::string aLabel);
	void ImGuiInputText(pugi::xml_node aNode, std::string aLabel, int anImGuiID);
	bool FindChildByStringValue(pugi::xml_node aNode, std::string aValue, pugi::xml_node& aOutNode);
	void ImGuiSnippetTypeCheckBox(std::string aSnippetType, bool& aBoolRef);
	void Load(std::string aFile = "");
};