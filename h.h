#ifndef H_H
#define H_H

#include <vector>
#include <string>

// структура для хранения координат
struct Point {
    int x, y;
};
// фулл структура лабиринта (размеры, символы, пройденный путь, волновой алгоритм для поиска пути, старт и финиш)
struct MazeData {
    int width, height; 
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<int>> dist;
    Point start;
    Point finish;
};
// структура интерфейса(лабиринт, игрок, передвежения, мин. путь)
struct GameState {
    MazeData* maze;
    Point player;
    int moves;
    int shortestPath;
};

// функции для работы с лабиринтом
MazeData* createMaze(int width, int height);
void generateMaze(MazeData* maze);
void setStartAndFinish(MazeData* maze);
int findShortestPath(MazeData* maze);
void displayMaze(const MazeData* maze, Point player);
void markVisited(MazeData* maze, Point p);
bool isValidMove(const MazeData* maze, Point p);
bool saveMazeToFile(const MazeData* maze, const std::string& filename, Point player, int moves);
MazeData* loadMazeFromFile(const std::string& filename, Point& player, int& moves);
void freeMaze(MazeData* maze);

// функции для игры
GameState* createGame(int width, int height);
GameState* loadGameFromMaze(MazeData* maze, Point player, int moves);
bool saveGame(const GameState* game, const std::string& filename = "save.txt");
void playGame(GameState* game);
void freeGame(GameState* game);

#endif