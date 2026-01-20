#ifndef H_H
#define H_H

#include <iostream>
#include <vector>
#include <string>

// Структуры
struct Point {
    int x, y;
};

struct MazeData {
    int width;
    int height;
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<int>> dist;
    Point start;
    Point finish;
};

struct GameState {
    MazeData* maze;
    Point player;
    int moves;
    int shortestPath;
};

// Функции для работы с лабиринтом
MazeData* createMaze(int width, int height);
void generateMaze(MazeData* maze);  // Простая генерация без стека
void setStartAndFinish(MazeData* maze);
int findShortestPath(MazeData* maze);
void displayMaze(const MazeData* maze, Point player);
void markVisited(MazeData* maze, Point p);
bool isValidMove(const MazeData* maze, Point p);
bool saveMazeToFile(const MazeData* maze, const std::string& filename, Point player, int moves);
MazeData* loadMazeFromFile(const std::string& filename, Point& player, int& moves);
void freeMaze(MazeData* maze);

// Функции для игры
GameState* createGame(int width, int height);
GameState* loadGameFromMaze(MazeData* maze, Point player, int moves);
bool saveGame(const GameState* game, const std::string& filename = "save.txt");
void playGame(GameState* game);
void freeGame(GameState* game);

#endif 
