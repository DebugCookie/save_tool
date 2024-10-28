#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "pugixml/pugixml.hpp"

class Snippet
{
public:
	Snippet() = default;
	virtual ~Snippet() = default;



	void Load(std::string aFileName = "");
	void Save();
	[[nodiscard]] inline pugi::xml_node GetTitleChild() const { return myTitleChild; }
	[[nodiscard]] inline pugi::xml_node GetShortcutChild() const { return myShortcutChild; }
	[[nodiscard]] inline pugi::xml_node GetDescriptionChild() const { return myDescriptionChild; }
	[[nodiscard]] inline pugi::xml_node GetAuthorChild() const { return myAuthorChild; }

	[[nodiscard]] bool GetSnippetType(std::string aSnippetType) const;
	[[nodiscard]] inline bool HasAnySnippetType() const { return (myHasExpansion || myHasRefactoring || myHasSurroundsWith); }
	[[nodiscard]] inline std::string GetFile() const { return myFile; }
	void SetSnippetType(std::string aSnippetType, bool aValue);

	void AddSnippetType(std::string aSnippetType);
	void RemoveSnippetType(std::string aSnippetType);

	void SetLiteral(std::string aOldID, std::string aID, std::string aToolTip, std::string aDefault);

	void AddLiteral(std::string aID = "", std::string aToolTip = "ToolTip", std::string aDefault = "defaultValue");

	void RemoveLiteral(std::string aID);
	std::vector<pugi::xml_node> GetAllLiterals();
	bool HasLiteral(std::string aID);

	void SetCode(std::string aPieceOfCode);
	std::string GetCode();

	struct ImGuiVars
	{
		bool expansion = false;
		bool refactoring = false;
		bool surroundsWith = false;
	};

	ImGuiVars myImGui;

private:

	pugi::xml_node FindLiteralNodeByID(std::string aID);


	std::string myFile = "default.snippet";
	pugi::xml_document myDocument;

	pugi::xml_node myTitleChild;
	pugi::xml_node myShortcutChild;
	pugi::xml_node myDescriptionChild;
	pugi::xml_node myAuthorChild;

	pugi::xml_node mySnippetTypes;
	bool myHasExpansion = false;
	bool myHasRefactoring = false;
	bool myHasSurroundsWith = false;

	pugi::xml_node mySnippetNode;

	struct Literal
	{
		pugi::xml_node id;
		pugi::xml_node toolTip;
		pugi::xml_node default;
	};

	std::vector<Literal> myLiterals;

};