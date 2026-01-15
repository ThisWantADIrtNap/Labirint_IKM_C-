#include "h.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// направления
const int dx[4] = {-1, 0, 1, 0};
const int dy[4] = {0, 1, 0, -1};
// функции для работы с лабиринтом
MazeData* createMaze(int width, int height) {
    MazeData* maze = new MazeData;
    // нечетные размеры для правильной генерации
    maze->width = width;
    maze->height = height;
    if (maze->width % 2 == 0) maze->width++;
    if (maze->height % 2 == 0) maze->height++;
    // инициализация
    maze->grid.resize(maze->height, vector<char>(maze->width, '#'));
    maze->visited.resize(maze->height, vector<bool>(maze->width, false));
    maze->dist.resize(maze->height, vector<int>(maze->width, -1));
    return maze;
}
// создание лабиринта
void generateMaze(MazeData* maze) {
    srand(time(nullptr));
    // начальная точка для генерации
    int startX = 1, startY = 1;
    stack<Point> stack;
    Point startPoint = {startX, startY};
    stack.push(startPoint);
    maze->grid[startX][startY] = '.';
    vector<int> directions = {0, 1, 2, 3};
    while (!stack.empty()) {
        Point current = stack.top();
        int x = current.x;
        int y = current.y;
        // направления
        for (int i = 0; i < 4; i++) {
            int swapWith = rand() % 4;
            int temp = directions[i];
            directions[i] = directions[swapWith];
            directions[swapWith] = temp;
        }
        bool found = false;
        for (int dir : directions) {
            int nx = x + dx[dir] * 2;
            int ny = y + dy[dir] * 2;
            if (nx >= 1 && nx < maze->height-1 && ny >= 1 && ny < maze->width-1 && 
                maze->grid[nx][ny] == '#') {
                // убираем стену между текущей и новой точкой
                maze->grid[x + dx[dir]][y + dy[dir]] = '.';
                maze->grid[nx][ny] = '.';
                Point newPoint = {nx, ny};
                stack.push(newPoint);
                found = true;
                break;
            }
        }
        if (!found) {
            stack.pop();
        }
    }
    setStartAndFinish(maze);
}

void setStartAndFinish(MazeData* maze) {
    // старт
    maze->start = {0, 1};
    maze->grid[maze->start.x][maze->start.y] = 'S';
    maze->visited[maze->start.x][maze->start.y] = true;
    
    // финиш
    maze->finish = {maze->height - 1, maze->width - 2};
    maze->grid[maze->finish.x][maze->finish.y] = 'F';
}

int findShortestPath(MazeData* maze) {
    // матрица расстояния
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->dist[i][j] = -1;
        }
    }
// волновой алгоритм
    queue<Point> q;
    q.push(maze->start);
    maze->dist[maze->start.x][maze->start.y] = 0;
    while (!q.empty()) {
        Point current = q.front();
        q.pop();
        // финиш
        if (current.x == maze->finish.x && current.y == maze->finish.y) {
            return maze->dist[current.x][current.y];
        }
        
        //проверяем все направления
        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            
            // проверяем, что точка в пределах лабиринта 
            if (nx >= 0 && nx < maze->height && ny >= 0 && ny < maze->width && 
                maze->grid[nx][ny] != '#' && maze->dist[nx][ny] == -1) {
                maze->dist[nx][ny] = maze->dist[current.x][current.y] + 1;
                Point nextPoint = {nx, ny};
                q.push(nextPoint);
            }
        }
    }
    return -1;
}
// интерфейс при игре
void displayMaze(const MazeData* maze, Point player) {
    system("cls");
    cout << "================================" << endl;
    cout << "          ЛАБИРИНТ" << endl;
    cout << "================================" << endl << endl;
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (i == player.x && j == player.y) {
                cout << 'P';
            } else {
                char cell = maze->grid[i][j];
                if (maze->visited[i][j] && cell == '.') {
                    cout << ' ';
                } else {
                    cout << cell;
                }
            }
        }
        cout << endl;
    }
    cout << "\n==========================================" << endl;
    cout << "Управление: W - вверх, S - вниз, A - влево, D - вправо" << endl;
    cout << "Сохранение: E - сохранить игру" << endl;
    cout << "Выход: Q - выход в меню" << endl;
    cout << "Цель: дойти от S (старт) до F (финиш)" << endl;
    cout << "==========================================" << endl;
}
//
void markVisited(MazeData* maze, Point p) {
    if (p.x >= 0 && p.x < maze->height && p.y >= 0 && p.y < maze->width) {
        maze->visited[p.x][p.y] = true;
    }
}
//
bool isValidMove(const MazeData* maze, Point p) {
    return p.x >= 0 && p.x < maze->height && p.y >= 0 && p.y < maze->width && 
           maze->grid[p.x][p.y] != '#';
}
//
bool saveMazeToFile(const MazeData* maze, const string& filename, Point player, int moves) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << maze->width << " " << maze->height << endl;
    file << player.x << " " << player.y << " " << moves << endl;
    file << maze->start.x << " " << maze->start.y << endl;
    file << maze->finish.x << " " << maze->finish.y << endl;
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            file << maze->grid[i][j];
        }
        file << endl;
    }
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            file << (maze->visited[i][j] ? '1' : '0');
        }
        file << endl;
    }
    file.close();
    return true;
}
//
MazeData* loadMazeFromFile(const string& filename, Point& player, int& moves) {
    ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }
    
    int width, height;
    file >> width >> height;
    
    MazeData* maze = createMaze(width, height);
    maze->width = width;
    maze->height = height;
    
    file >> player.x >> player.y >> moves;
    file >> maze->start.x >> maze->start.y;
    file >> maze->finish.x >> maze->finish.y;
    
    string line;
    getline(file, line);
    
    maze->grid.resize(height, vector<char>(width));
    for (int i = 0; i < height; i++) {
        getline(file, line);
        for (int j = 0; j < width && j < line.length(); j++) {
            maze->grid[i][j] = line[j];
        }
    }
    
    maze->visited.resize(height, vector<bool>(width, false));
    for (int i = 0; i < height; i++) {
        getline(file, line);
        for (int j = 0; j < width && j < line.length(); j++) {
            maze->visited[i][j] = (line[j] == '1');
        }
    }
    
    file.close();
    
    // восстанавливаем символы
    maze->grid[maze->start.x][maze->start.y] = 'S';
    maze->grid[maze->finish.x][maze->finish.y] = 'F';
    
    return maze;
}
// отчиска лабиринта 
void freeMaze(MazeData* maze) {
    delete maze;
}
// функции для игры и всякие проверки
GameState* createGame(int width, int height) {
    GameState* game = new GameState;
    game->maze = createMaze(width, height);
    generateMaze(game->maze);
    game->player = game->maze->start;
    game->moves = 0;
    game->shortestPath = findShortestPath(game->maze);
    return game;
}
//
GameState* loadGameFromMaze(MazeData* maze, Point player, int moves) {
    GameState* game = new GameState;
    game->maze = maze;
    game->player = player;
    game->moves = moves;
    game->shortestPath = findShortestPath(maze);
    return game;
}
//
bool saveGame(const GameState* game, const string& filename) {
    if (saveMazeToFile(game->maze, filename, game->player, game->moves)) {
        cout << "Игра успешно сохранена в файл: " << filename << endl;
        cout << "Нажмите любую клавишу для продолжения..." << endl;
        cin.get();
        return true;
    } else {
        cout << "Ошибка при сохранении игры!" << endl;
        cout << "Нажмите любую клавишу для продолжения..." << endl;
        cin.get();
        return false;
    }
}
// игровая логика передвижения
void playGame(GameState* game) {
    char input;
    bool gameOver = false;
    while (!gameOver) {
        displayMaze(game->maze, game->player);
        cout << "\nХодов сделано: " << game->moves << endl;
        cout << "Минимальное число ходов (волновой алгоритм): " << game->shortestPath << endl;
        cout << "==========================================" << endl;
        
        cout << "Введите команду: ";
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (input == 'e' || input == 'E') {
            saveGame(game);
            continue;
        }
        Point newPos = game->player;
        switch (input) {
            case 'w':
            case 'W':
                newPos.x--;
                break;
            case 's':
            case 'S':
                newPos.x++;
                break;
            case 'a':
            case 'A':
                newPos.y--;
                break;
            case 'd':
            case 'D':
                newPos.y++;
                break;
            case 'q':
            case 'Q':
                cout << "Возврат в главное меню..." << endl;
                cout << "Нажмите любую клавишу для продолжения..." << endl;
                cin.get();
                return;
            default:
                cout << "Неверная команда! Используйте W, A, S, D для движения." << endl;
                cout << "E - сохранить игру, Q - выход в меню" << endl;
                cout << "Нажмите любую клавишу для продолжения..." << endl;
                cin.get();
                continue;
        }
        if (isValidMove(game->maze, newPos)) {
            markVisited(game->maze, game->player);
            game->player = newPos;
            game->moves++;
            markVisited(game->maze, game->player);
            if (game->player.x == game->maze->finish.x && game->player.y == game->maze->finish.y) {
                gameOver = true;
            }
        } else {
            cout << "Здесь стена! Ход невозможен." << endl;
            cout << "Нажмите любую клавишу для продолжения..." << endl;
            cin.get();
        }
    }
    displayMaze(game->maze, game->player);
    cout << "\n================================" << endl;
    cout << "ПОЗДРАВЛЯЕМ! Вы прошли лабиринт!" << endl;
    cout << "================================" << endl;
    cout << "Ваше количество ходов: " << game->moves << endl;
    cout << "Минимальное возможное количество ходов: " << game->shortestPath << endl;
    cout << "================================" << endl;
    
    if (game->moves == game->shortestPath) {
        cout << "Вы прошли лабиринт оптимальным путем!" << endl;
    } else if (game->moves < game->shortestPath * 1.5) {
        cout << "Хороший результат!" << endl;
    } else {
        cout << "Попробуйте пройти лабиринт за меньшее количество ходов!" << endl;
    }
    cout << "================================" << endl;
    
    remove("save.txt");
    cout << "Сохранение игры удалено." << endl;
    cout << "Нажмите любую клавишу для продолжения..." << endl;
    cin.get();
}
// отчистка игры
void freeGame(GameState* game) {
    if (game) {
        freeMaze(game->maze);
        delete game;
    }
}