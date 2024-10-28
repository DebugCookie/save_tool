#include "Snippet.h"
#include <cassert>
#include <iostream>

void Snippet::Load(std::string aFileName)
{
	if (aFileName.empty())
	{
		std::string defaultFileContents = "<?xml version = \"1.0\" encoding=\"utf-8\" ?>\n\
		<CodeSnippets  xmlns=\"http://schemas.microsoft.com/VisualStudio/2005/CodeSnippet\">\n\
			<CodeSnippet Format=\"1.0.0\">\n\
				<Header>\n\
					<Title>Getter</Title>\n\
					<Shortcut>getter</Shortcut>\n\
					<Description>Code snippet for a Get() function</Description>\n\
					<Author>Coco</Author>\n\
				</Header>\n\
				<Snippet>\n\
					<Declarations>\n\
						<Literal>\n\
							<ID>type</ID>\n\
							<ToolTip>Type of the returning variable</ToolTip>\n\
							<Default>int</Default>\n\
						</Literal>\n\
						<Literal>\n\
							<ID>name</ID>\n\
							<ToolTip>Name of the returning variable</ToolTip>\n\
							<Default>myVariable</Default>\n\
						</Literal>\n\
					</Declarations>\n\
					<Code Language=\"cpp\">\n\
						<![CDATA[[[nodiscard]] inline $type$ Get$name$() const\n\
							{\n\
								return my$name$;\n\
							}]]>\n\
					</Code>\n\
				</Snippet>\n\
			</CodeSnippet>\n\
		</CodeSnippets>";

		pugi::xml_parse_result result = myDocument.load_string(defaultFileContents.c_str());
		assert(result);
	}
	else
	{
		pugi::xml_parse_result result = myDocument.load_file(aFileName.c_str());
		assert(result);
		myFile = aFileName;
	}
	
	

	myTitleChild = myDocument.first_child().first_child().first_child().first_child().first_child();
	myShortcutChild = myTitleChild.parent().next_sibling().first_child();
	myDescriptionChild = myShortcutChild.parent().next_sibling().first_child();
	myAuthorChild = myDescriptionChild.parent().next_sibling().first_child();

	mySnippetTypes = myAuthorChild.parent().next_sibling();
	if (mySnippetTypes.child("SnippetType").empty() == false)
	{
		pugi::xml_node walkingChild = mySnippetTypes.first_child();
		for (size_t i = 0; i < 3; i++)
		{
			if (walkingChild.child_value() == (std::string)"Expansion")
			{
				myImGui.expansion = true;
				myHasExpansion = true;
			}
			else if (walkingChild.child_value() == (std::string)"Refactoring")
			{
				myImGui.refactoring = true;
				myHasRefactoring = true;
			}
			else if (walkingChild.child_value() == (std::string)"SurroundsWith")
			{
				myImGui.surroundsWith = true;
				myHasSurroundsWith = true;
			}
			walkingChild = walkingChild.next_sibling();
		}
	}

	mySnippetNode = myAuthorChild.parent().parent().next_sibling();
}

void Snippet::Save()
{
	//myDocument.save_file("GetterOut.snippet");
	myDocument.save_file(myFile.c_str());
}

bool Snippet::GetSnippetType(std::string aSnippetType) const
{
	if (aSnippetType == (std::string)"Expansion")
	{
		return myHasExpansion;
	}
	else if (aSnippetType == (std::string)"Refactoring")
	{
		return myHasRefactoring;
	}
	else if (aSnippetType == (std::string)"SurroundsWith")
	{
		return myHasSurroundsWith;
	}
	else
	{
		assert(false && "You spelled wrong");
		return false;
	}
}

void Snippet::SetSnippetType(std::string aSnippetType, bool aValue)
{
	if (aSnippetType == (std::string)"Expansion")
	{
		myHasExpansion = aValue;
	}
	else if (aSnippetType == (std::string)"Refactoring")
	{
		myHasRefactoring = aValue;
	}
	else if (aSnippetType == (std::string)"SurroundsWith")
	{
		myHasSurroundsWith = aValue;
	}
	else
	{
		assert(false && "You spelled wrong");
	}
}

void Snippet::AddSnippetType(std::string aSnippetType)
{
	if (HasAnySnippetType() == false)
	{
		mySnippetTypes = myAuthorChild.parent().parent().append_child("SnippetTypes");
	}
	if (GetSnippetType(aSnippetType) == false)
	{
		mySnippetTypes.append_child("SnippetTypeNEW");
	}
	mySnippetTypes.child("SnippetTypeNEW").append_buffer(aSnippetType.c_str(), aSnippetType.size() * sizeof(char));
	mySnippetTypes.child("SnippetTypeNEW").set_name("SnippetType");
	SetSnippetType(aSnippetType, true);
}

void Snippet::RemoveSnippetType(std::string aSnippetType)
{
	pugi::xml_node walkingChild = mySnippetTypes.first_child();
	for (size_t i = 0; i < 3; i++)
	{
		if (aSnippetType == walkingChild.child_value())
		{
			if (aSnippetType == (std::string)"Expansion")
			{
				myHasExpansion = false;
				walkingChild.parent().remove_child(walkingChild);
			}
			else if (aSnippetType == (std::string)"Refactoring")
			{
				myHasRefactoring = false;
				walkingChild.parent().remove_child(walkingChild);
			}
			else if (aSnippetType == (std::string)"SurroundsWith")
			{
				myHasSurroundsWith = false;
				walkingChild.parent().remove_child(walkingChild);
			}
		}
		walkingChild = walkingChild.next_sibling();
	}
	if (HasAnySnippetType() == false)
	{
		mySnippetTypes.parent().remove_child(mySnippetTypes);
	}
}

void Snippet::SetLiteral(std::string aOldID, std::string aID, std::string aToolTip, std::string aDefault)
{
	pugi::xml_node literalToChange = FindLiteralNodeByID(aOldID);

	literalToChange.child("ID").first_child().set_value(aID.c_str());
	literalToChange.child("ToolTip").first_child().set_value(aToolTip.c_str());
	literalToChange.child("Default").first_child().set_value(aDefault.c_str());
}

void Snippet::AddLiteral(std::string aID, std::string aToolTip, std::string aDefault)
{
	if (aID == "")
	{
		static int id;
		aID = "ID " + std::to_string(id++);
	}
	pugi::xml_node literalNewNode = mySnippetNode.child("Declarations").append_child("LiteralNEW");
	myLiterals.emplace_back();

	myLiterals.back().id = literalNewNode.append_child("ID");
	myLiterals.back().id.append_buffer(aID.c_str(), aID.size() * sizeof(char));

	myLiterals.back().toolTip = literalNewNode.append_child("ToolTip");
	myLiterals.back().toolTip.append_buffer(aToolTip.c_str(), aToolTip.size() * sizeof(char));
	
	myLiterals.back().default = literalNewNode.append_child("Default");
	myLiterals.back().default.append_buffer(aDefault.c_str(), aDefault.size() * sizeof(char));
	
	literalNewNode.set_name("Literal");
}

void Snippet::RemoveLiteral(std::string aID)
{
	pugi::xml_node nodeToRemove = FindLiteralNodeByID(aID);
	nodeToRemove.parent().remove_child(nodeToRemove);
}

std::vector<pugi::xml_node> Snippet::GetAllLiterals()
{
	std::vector<pugi::xml_node> result;

	size_t childCount = std::distance(mySnippetNode.child("Declarations").begin(), mySnippetNode.child("Declarations").end());
	if (childCount > 0)
	{
		pugi::xml_node walkingChild = mySnippetNode.child("Declarations").first_child();

		for (size_t i = 0; i < childCount; i++)
		{
			result.push_back(walkingChild);
			walkingChild = walkingChild.next_sibling();
		}
	}
	return result;
}

bool Snippet::HasLiteral(std::string aID)
{
	std::vector<pugi::xml_node> allLiterals = GetAllLiterals();
	bool result = 
		(std::find_if(allLiterals.begin(), allLiterals.end(),
		[&aID](const pugi::xml_node& aNode)
		{
			bool result = aNode.child("ID").child_value() == aID;
			return result;
		}) == allLiterals.end());
	return result;
}

void Snippet::SetCode(std::string aPieceOfCode)
{
	mySnippetNode.child("Code").first_child().set_value(aPieceOfCode.c_str());
}

std::string Snippet::GetCode()
{
	return std::string(mySnippetNode.child("Code").child_value());
}

pugi::xml_node Snippet::FindLiteralNodeByID(std::string aID)
{
	pugi::xml_node result;

	size_t childCount = std::distance(mySnippetNode.child("Declarations").begin(), mySnippetNode.child("Declarations").end());
	if (childCount > 0)
	{
		pugi::xml_node walkingChild = mySnippetNode.child("Declarations").first_child();

		for (size_t i = 0; i < childCount; i++)
		{
			if (walkingChild.child("ID").child_value() == aID)
			{
				result = walkingChild;
				break;
			}
			walkingChild = walkingChild.next_sibling();
		}
	}

	return result;
}
