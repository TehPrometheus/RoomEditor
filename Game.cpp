#include "pch.h"
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "Game.h"

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	// initialize game resources here
	InitTextures(g_NamedTexturesArr, g_TexturesSize, g_Numbers, g_GridSize);
	InitGrid(g_CellArr, g_NrRows, g_NrCols);
	InitPanel(g_PanelArr, g_NrPanelRows, g_NrPanelCols);

	InitPanelTextures(g_PanelArr, g_NrPanelRows, g_NrPanelCols, g_NamedTexturesArr, g_TexturesSize);
	InitButton(g_ExportButton);
	SetObstacles(g_CellArr, g_NrRows, g_NrCols);

}

void Draw()
{
	ClearBackground(0,0,0);

	// Put the drawing of textures here
	DrawGridTextures(g_CellArr, g_NrRows, g_NrCols);
	DrawPanelTextures(g_PanelArr, g_NrPanelRows, g_NrPanelCols);
	DrawButton(g_ExportButton);


	//Comment this out to disable the debug grid
	//DrawDebugGrid(g_CellArr, g_NrRows, g_NrCols);

	//Comment this out to disable the debug panel
	DrawDebugPanel(g_PanelArr, g_NrPanelRows, g_NrPanelCols);
}

void Update(float elapsedSec)
{
}

void End()
{
	// free game resources here
	DeleteTextures();
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{

}

void OnKeyUpEvent(SDL_Keycode key)
{

}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{


	ProcessGridHold(g_CellArr, g_NrRows, g_NrCols, e);


}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	
	if (e.button == SDL_BUTTON_LEFT)
	{
		ProcessPanelClick(g_PanelArr, g_NrPanelRows, g_NrPanelCols, e);
		ProcessExportButtonClick(g_ExportButton, e);		
		ProcessGridClick(g_CellArr, g_NrRows, g_NrCols, e);
	}
	if (e.button == SDL_BUTTON_RIGHT)
	{
		M2Checker(e);
	}
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		ResetButtonTexture(g_ExportButton);
	}

}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

// Utils
int GetIndex(const int rowIdx, const int colIdx, const int nrCols)
{
	return rowIdx * nrCols + colIdx;
}
bool IsPointInRect(const Rectf& rectangle, const Point2f& point)
{
	return (point.x >= rectangle.left &&
		point.x <= rectangle.left + rectangle.width &&
		point.y >= rectangle.bottom &&
		point.y <= rectangle.bottom + rectangle.height);

	return false;

}

// Texture Handling
void InitTextures(NamedTexture namedTextureArr[], const int arrSize, Texture textureNumbersArr[], const int numbersArrSize)
{
	LoadTexturesFromFolder("resources", namedTextureArr, arrSize);

	// Load numbers 0 through 95 for debug grid
	/*for (int i = 0; i < numbersArrSize; i++)
	{
		bool success = TextureFromString(std::to_string(i), "resources/Font.otf", 13, g_White, textureNumbersArr[i]);
		if (!success)
		{
			std::cout << "Loading number " + std::to_string(i) + " from Font.otf failed" << std::endl;
		}
	}*/
}
void LoadTexturesFromFolder(std::string folderPath, NamedTexture namedTextureArr[], const int arrSize)
{
	std::fstream file;
	file.open(folderPath + "/textures.txt", std::ios::in);
	if (!file)
	{
		std::cout << "CAN NOT FIND TEXTURES FILE\n";
	}

	std::string textureFileName{};
	std::string textureInternalName{};
	std::string boolBuffer{};
	bool isAddingToBuffer{ false };
	bool isPastFirstComma{ false };
	bool isPastSecondComma{ false };
	char currentChar;
	int index{};

	while (file)
	{
		currentChar = file.get();
		if (currentChar == '}')
		{
			if (boolBuffer == "true")
			{
				LoadTexture(folderPath + "/" + textureFileName, namedTextureArr[index], textureInternalName, true);
			}
			else
			{
				LoadTexture(folderPath + "/" + textureFileName, namedTextureArr[index], textureInternalName, false);
			}
			isAddingToBuffer = false;
			textureFileName = "";
			textureInternalName = "";
			boolBuffer = "";
			isPastFirstComma = false;
			isPastSecondComma = false;
			++index;
		}
		else if (currentChar == ',')
		{
			if (isPastFirstComma == true) isPastSecondComma = true;
			else isPastFirstComma = true;
		}
		else if (isAddingToBuffer == true)
		{
			if (isPastSecondComma)
			{
				boolBuffer += currentChar;
			}
			else if (isPastFirstComma)
			{
				textureInternalName += currentChar;
			}
			else
			{
				textureFileName += currentChar;
			}
		}
		else if (currentChar == '{')
		{
			isAddingToBuffer = true;
		}
	}

	file.close();
}
void LoadTexture(const std::string texturePath, NamedTexture& namedTexture, std::string name, bool isObstacle) {
	bool success = TextureFromFile(texturePath, namedTexture.texture);
	namedTexture.name = name;
	namedTexture.isObstacle = isObstacle;
	if (!success)
	{
		std::cout << "Loading of " << name << " texture failed" << std::endl;
	}
}
void DeleteTextures()
{
	// Delete named textures array
	for (int i = 0; i < g_TexturesSize; i++)
	{
		DeleteTexture(g_NamedTexturesArr[i].texture);
	}

	// Delete textures stored in the grid array
	for (int i = 0; i < g_GridSize; i++)
	{
		DeleteTexture(g_CellArr[i].texture);
	}

	// Delete textures stored in the panel array
	for (int i = 0; i < g_PanelSize; i++)
	{
		DeleteTexture(g_PanelArr[i].texture);
	}


	// Delete number textures array
	for (int i = 0; i < g_GridSize; i++)
	{
		DeleteTexture(g_Numbers[i]);
	}
}
Texture FetchTexture(std::string textureName)
{
	for (int index{}; index < g_TexturesSize; ++index)
	{
		if (textureName == g_NamedTexturesArr[index].name)
			return g_NamedTexturesArr[index].texture;
	}
	return g_NamedTexturesArr[0].texture;
}
std::string FetchTextureName(Texture texture)
{
	for (int index{}; index < g_TexturesSize; ++index)
	{
		if (g_NamedTexturesArr[index].texture.id == texture.id)
			return g_NamedTexturesArr[index].name;
	}
	return g_NamedTexturesArr[0].name;
}

// Button Handling
void InitButton(Button& button)
{
	Point2f pos{ 0,0 };
	Rectf dstRect{};


	dstRect.width = 200.f;
	dstRect.height = 53.f;
	dstRect.left = pos.x;
	dstRect.bottom = pos.y;
	button.dstRect = dstRect;
	
	button.namedTexture.texture = FetchTexture("Accept_Button_Normal");
}
void DrawButton(Button& button)
{
	DrawTexture(button.namedTexture.texture, button.dstRect);
}
void ResetButtonTexture(Button& button)
{
	button.namedTexture.texture = FetchTexture("Accept_Button_Normal");
}

// Grid Handling
void InitGrid(Cell cellArr[], int nrRows, int nrCols)
{
	const float cellSize{ 60.f };
	const float offset{ 300.f };
	const int cellAmount{ nrRows * nrCols };

	Point2f pos{ 0.5f * (g_WindowWidth - (nrCols * cellSize) - offset),
				 0.5f * (g_WindowHeight + ((nrRows - 2) * cellSize)) }; // ensures grid is always centered

	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			cellArr[GetIndex(row, col, nrCols)].dstRect.bottom = pos.y;
			cellArr[GetIndex(row, col, nrCols)].dstRect.left = pos.x;
			cellArr[GetIndex(row, col, nrCols)].dstRect.width = cellSize;
			cellArr[GetIndex(row, col, nrCols)].dstRect.height = cellSize;

			pos.x += cellSize;
		}
		pos.x -= nrCols * cellSize;
		pos.y -= cellSize;

	}


	for (int i = 0; i < cellAmount; i++)
	{
		if (cellArr[i].texture.id == 0)
		{
			cellArr[i].texture = FetchTexture("empty");

		}

	}

	cellArr[GetIndex(0,0,nrCols)].texture = FetchTexture("floor0");
	cellArr[GetIndex(0, nrCols - 1, nrCols)].texture = FetchTexture("floor0");
	cellArr[GetIndex(nrRows-1, nrCols - 1, nrCols)].texture = FetchTexture("floor0");
	cellArr[GetIndex(nrRows - 1, 0, nrCols)].texture = FetchTexture("floor0");

}
void DrawDebugGrid(Cell cellArr[], int nrRows, int nrCols)
{
	SetColor(g_White);
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			Point2f pos{ cellArr[GetIndex(row, col, nrCols)].dstRect.left, 
						 cellArr[GetIndex(row, col, nrCols)].dstRect.bottom };

			DrawTexture(g_Numbers[GetIndex(row, col, nrCols)], pos);
			DrawRect(cellArr[GetIndex(row, col, nrCols)].dstRect);
		}
	}
}
void DrawGridTextures(Cell cellArr[], int nrRows, int nrCols)
{
	int cellAmount{ nrRows * nrCols };
	for (int index{}; index < cellAmount; ++index)
	{	
		DrawTexture(cellArr[index].texture, cellArr[index].dstRect);
	}
}
void SetObstacles(Cell cellArr[], int nrRows, int nrCols)
{
	int cellAmount{ nrRows * nrCols };
	for (int index{}; index < cellAmount; ++index)
	{
		// this if statement sets all appropriate cells to 'isObstacle = true' for the appropriate textures
		// Might need global array to store obstacles in the future to shorten this if statement
		if (cellArr[index].texture.id == FetchTexture("wall_horizontal_top").id ||
			cellArr[index].texture.id == FetchTexture("wall_horizontal_bottom").id ||
			cellArr[index].texture.id == FetchTexture("wall_vertical_left").id ||
			cellArr[index].texture.id == FetchTexture("wall_vertical_right").id)
		{
			cellArr[index].isObstacle = true;
		}

	}


}

// Panel Handling
void InitPanel(Cell cellArr[], int nrRows, int nrCols)
{
	const float cellSize{ 60.f };
	const float offset{ 950.f };

	Point2f pos{ 0.5f * (g_WindowWidth - (nrCols * cellSize) + offset),
				 0.5f * (g_WindowHeight + ((nrRows - 2) * cellSize)) }; // ensures grid is always centered

	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			cellArr[GetIndex(row, col, nrCols)].dstRect.bottom = pos.y;
			cellArr[GetIndex(row, col, nrCols)].dstRect.left = pos.x;
			cellArr[GetIndex(row, col, nrCols)].dstRect.width = cellSize;
			cellArr[GetIndex(row, col, nrCols)].dstRect.height = cellSize;

			pos.x += cellSize;
		}
		pos.x -= nrCols * cellSize;
		pos.y -= cellSize;

	}

}
void InitPanelTextures(Cell cellArr[], int nrRows, int nrCols, NamedTexture namedTextureArr[], const int arrSize)
{
	const int cellAmount{ nrRows * nrCols };

	for (int index = 0; index < cellAmount; index++)
	{
		cellArr[index].texture = namedTextureArr[index].texture;

	}

	for (int i = 0; i < cellAmount; i++)
	{
		if (cellArr[i].texture.id == 0)
		{
			cellArr[i].texture = FetchTexture("empty");

		}

	}



}
void DrawPanelTextures(Cell cellArr[], int nrRows, int nrCols)
{
	int cellAmount{ nrRows * nrCols };
	for (int index{}; index < cellAmount; ++index)
	{
		DrawTexture(cellArr[index].texture, cellArr[index].dstRect);
	}
}
void DrawDebugPanel(Cell cellArr[], int nrRows, int nrCols)
{
	SetColor(g_White);
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			Point2f pos{ cellArr[GetIndex(row, col, nrCols)].dstRect.left,
						 cellArr[GetIndex(row, col, nrCols)].dstRect.bottom };

			DrawTexture(g_Numbers[GetIndex(row, col, nrCols)], pos);
			DrawRect(cellArr[GetIndex(row, col, nrCols)].dstRect);
		}
	}
}

// Input Handling
void ProcessPanelClick(Cell cellArr[], int nrRows, int nrCols, const SDL_MouseButtonEvent& e)
{
	Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			if (IsPointInRect(cellArr[GetIndex(row,col,nrCols)].dstRect, mousePos))
			{
				g_SelectedTexture = cellArr[GetIndex(row, col, nrCols)].texture;

			}

		}

	}


}
void ProcessGridClick(Cell cellArr[], int nrRows, int nrCols, const SDL_MouseButtonEvent& e)
{
	Point2f mousePos{ float(e.x), float(g_WindowHeight - e.y) };
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			if (IsPointInRect(cellArr[GetIndex(row, col, nrCols)].dstRect, mousePos))
			{
				cellArr[GetIndex(row, col, nrCols)].texture = g_SelectedTexture;

			}

		}

	}


}
void ProcessGridHold(Cell cellArr[], int nrRows, int nrCols, const SDL_MouseMotionEvent& e)
{
	Point2f mousePos{ float(e.x), float(g_WindowHeight - e.y) };
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			if (IsPointInRect(cellArr[GetIndex(row, col, nrCols)].dstRect, mousePos) && g_IsM1Down)
			{
				cellArr[GetIndex(row, col, nrCols)].texture = g_SelectedTexture;

			}

		}

	}

}
void M2Checker(const SDL_MouseButtonEvent& e)
{

	g_IsM1Down = !g_IsM1Down;
	/*if (e.button == SDL_BUTTON_LEFT)
	{
		g_IsM1Down = true;
	}
	else
	{
		g_IsM1Down = false;
	}*/
}
void ProcessExportButtonClick(Button& button, const SDL_MouseButtonEvent& e)
{
	Point2f mousePos{ float(e.x), float(g_WindowHeight - e.y) };

	if (IsPointInRect(button.dstRect, mousePos))
	{
		button.namedTexture.texture = FetchTexture("Accept_Button_Dark");
		SaveRoomLayout(g_CellArr, g_GridSize, g_RoomName);
	}


}

// Level writing/reading handling
void SaveRoomLayout(Cell cellArr[], int cellArrSize, const std::string& saveFileName)
{
	std::fstream file;
	file.open("RoomData/" + saveFileName + ".room", std::ios::out);
	if (!file)
	{
		std::cout << "CAN NOT SAVE ROOM FILE\n";
	}

	for (int index{}; index < cellArrSize; ++index)
	{
		file << "{"
			<< FetchTextureName(cellArr[index].texture)
			<< "}\n";
	}

	file.close();
}
void LoadRoomLayout(Cell targetCellArr[], std::string fileName)
{
	std::fstream file;
	file.open("RoomData/" + fileName, std::ios::in);
	if (!file)
	{
		std::cout << "CAN NOT LOAD ROOM FILE\n";
	}

	std::string textureNameBuffer{};
	bool isAddingToBuffer{ false };
	char currentChar;
	int index{};

	while (file)
	{
		currentChar = file.get();
		if (currentChar == '}')
		{
			targetCellArr[index].texture = FetchTexture(textureNameBuffer);
			std::cout << textureNameBuffer << '\n';
			isAddingToBuffer = false;
			textureNameBuffer = "";
			++index;
		}
		else if (isAddingToBuffer == true)
		{
			textureNameBuffer = textureNameBuffer + currentChar;
		}
		else if (currentChar == '{')
		{
			isAddingToBuffer = true;
		}
	}

	InitGrid(targetCellArr, g_NrRows, g_NrCols);

	file.close();
}
#pragma endregion ownDefinitions