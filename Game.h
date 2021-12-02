#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "RoomEditor - Aerts,Tanguy - 1DAE10" };

// Change the window dimensions here
float g_WindowWidth{ 1280 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation



#pragma region ownDeclarations
// Declare your own global variables here
const int g_NrPanelRows{ 10 };
const int g_NrPanelCols{ 5 };
const int g_PanelSize{ g_NrPanelRows * g_NrPanelCols };
const int g_NrRows{ 9 };
const int g_NrCols{ 13 };
const int g_GridSize{ g_NrRows * g_NrCols };
const int g_TexturesSize{ 100 }; // Careful, we do not know yet how many textures we'll need. ADDED WALL
const int g_NrRoomsPerLevel{ 5 }; 
const int g_RoomArrSize{ 100 }; 
const int g_LevelArrSize{ 1 }; // lets try to make one level first, each level always has g_NrRoomsPerLevel rooms

bool g_IsM1Down{ false };

struct NamedTexture
{
	std::string name;
	Texture texture;
	bool isObstacle{ false };
};
struct Cell
{
	Rectf dstRect;
	Texture texture;
	bool isObstacle{false};
};
struct Room
{
	Cell cells[g_GridSize]{};
	std::string roomType;
};
struct Level
{
	Room Rooms[g_NrRoomsPerLevel]{};
	std::string levelName;
};
struct Button
{
	Rectf dstRect;
	NamedTexture namedTexture;
};

Room g_Rooms[g_RoomArrSize]{};
Level g_Levels[g_LevelArrSize]{};
Cell g_CellArr[g_GridSize]{};
Cell g_PanelArr[g_PanelSize]{};
Texture g_Numbers[g_GridSize]{};
NamedTexture g_NamedTexturesArr[g_TexturesSize]{};
Texture g_SelectedTexture{};
Button g_ExportButton{};
std::string g_RoomName{ "test" };

const Color4f   g_Green{ 0 / 255.f, 236 / 255.f, 0 / 255.f, 255 / 255.f },
				g_GreenTransparent{ 0 / 255.f, 236 / 255.f, 0 / 255.f, 100 / 255.f },
				g_Blue{ 0 / 255.f, 0 / 255.f, 241 / 255.f , 255 / 255.f },
				g_Yellow{ 253 / 255.f, 253 / 255.f, 152 / 255.f , 255 / 255.f },
				g_Red{ 255 / 255.f, 0 / 255.f, 0 / 255.f , 255 / 255.f },
				g_DarkRed{ 155 / 255.f, 0 / 255.f, 0 / 255.f , 255 / 255.f },
				g_RedTransparent{ 255 / 255.f, 0 / 255.f, 0 / 255.f , 100 / 255.f },
				g_Gold{ 176 / 255.f, 149 / 255.f, 58 / 255.f , 255 / 255.f },
				g_Pink{ 252 / 255.f, 128 / 255.f, 125 / 255.f , 255 / 255.f },
				g_Cyan{ 0 / 255.f, 255 / 255.f, 255 / 255.f , 255 / 255.f },
				g_Magenta{ 255 / 255.f, 0 / 255.f, 255 / 255.f , 255 / 255.f },
				g_Grey{ 204 / 255.f, 204 / 255.f, 204 / 255.f, 255 / 255.f },
				g_Black{ 0,0,0,1 },
				g_White{ 1,1,1,1 };

// Declare your own functions here

// Utils
int GetIndex(const int rowIdx, const int colIdx, const int nrCols); 
bool IsPointInRect(const Rectf& rectangle, const Point2f& point);

// Texture Handling
void InitTextures(NamedTexture namedTextureArr[], const int arrSize, Texture textureNumbersArr[], const int numbersArrSize);
void LoadTexturesFromFolder(std::string folderPath, NamedTexture namedTextureArr[], const int arrSize);
void LoadTexture(const std::string texturePath, NamedTexture& namedTexture, std::string name, bool isObstacle);
void DeleteTextures();
Texture FetchTexture(std::string textureName);
std::string FetchTextureName(Texture texture);

// Button Handling
void InitButton(Button& button);
void DrawButton(Button& button);
void ResetButtonTexture(Button& button);


// Grid Handling
void InitGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawDebugGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawGridTextures(Cell cellArr[], int nrRows, int nrCols);
void SetObstacles(Cell cellArr[], int nrRows, int nrCols); //sets all appropriate cells to 'isObstacle = true' for the appropriate textures

// Panel Handling
void InitPanel(Cell cellArr[], int nrRows, int nrCols);
void DrawPanelTextures(Cell cellArr[], int nrRows, int nrCols);
void DrawDebugPanel(Cell cellArr[], int nrRows, int nrCols);
void InitPanelTextures(Cell cellArr[], int nrRows, int nrCols, NamedTexture namedTextureArr[], const int arrSize);

// Input Handling
void ProcessPanelClick(Cell cellArr[], int nrRows, int nrCols, const SDL_MouseButtonEvent& e);
void ProcessGridClick(Cell cellArr[], int nrRows, int nrCols, const SDL_MouseButtonEvent& e);
void ProcessExportButtonClick(Button& button, const SDL_MouseButtonEvent& e);
void ProcessGridHold(Cell cellArr[], int nrRows, int nrCols, const SDL_MouseMotionEvent& e);
void M2Checker(const SDL_MouseButtonEvent& e);

// Level writing/reading handling
void SaveRoomLayout(Cell cellArr[], int cellArrSize, const std::string& saveFileName);
void LoadRoomLayout(Cell targetCellArr[], std::string fileName);
#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
