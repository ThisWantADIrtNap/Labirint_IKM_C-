#include "h.h"
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Направления движения
const int dx[4] = {-1, 0, 1, 0};
const int dy[4] = {0, 1, 0, -1};

// Функции для работы с лабиринтом

MazeData* createMaze(int width, int height) {
    MazeData* maze = new MazeData;
    
    // Делаем размеры нечетными для правильной генерации
    maze->width = width;
    maze->height = height;
    if (maze->width % 2 == 0) maze->width++;
    if (maze->height % 2 == 0) maze->height++;
    
    // Инициализация матриц
    maze->grid.resize(maze->height, vector<char>(maze->width, '#'));
    maze->visited.resize(maze->height, vector<bool>(maze->width, false));
    maze->dist.resize(maze->height, vector<int>(maze->width, -1));
    
    return maze;
}

void generateMaze(MazeData* maze) {
    srand(time(nullptr));
    
    // 1. Сначала все стены
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->grid[i][j] = '#';
        }
    }
    
    // 2. Создаем сетку проходов (каждая вторая клетка)
    for (int i = 1; i < maze->height - 1; i += 2) {
        for (int j = 1; j < maze->width - 1; j += 2) {
            maze->grid[i][j] = '.';
        }
    }
    
    // 3. Соединяем проходы (пробиваем стены между ними)
    for (int i = 2; i < maze->height - 1; i += 2) {
        for (int j = 2; j < maze->width - 1; j += 2) {
            // Вертикальные соединения (между клетками сверху и снизу)
            if (i < maze->height - 2 && rand() % 2 == 0) {
                maze->grid[i][j-1] = '.';
            }
            // Горизонтальные соединения (между клетками слева и справа)
            if (j < maze->width - 2 && rand() % 2 == 0) {
                maze->grid[i-1][j] = '.';
            }
        }
    }
    
    // 4. Гарантируем проходимость от старта к финишу
    // Создаем прямой путь по периметру
    for (int j = 1; j < maze->width - 1; j++) {
        maze->grid[1][j] = '.';                     // Вторая строка
        maze->grid[maze->height-2][j] = '.';        // Предпоследняя строка
    }
    
    // Соединяем вертикально
    for (int i = 1; i < maze->height - 1; i++) {
        maze->grid[i][1] = '.';                     // Второй столбец
        maze->grid[i][maze->width-2] = '.';         // Предпоследний столбец
    }
    
    // 5. Добавляем случайные проходы
    int extraPassages = (maze->width * maze->height) / 20; // 5% от всех клеток
    for (int i = 0; i < extraPassages; i++) {
        int x = rand() % (maze->height - 2) + 1;
        int y = rand() % (maze->width - 2) + 1;
        maze->grid[x][y] = '.';
    }
    
    setStartAndFinish(maze);
}

void setStartAndFinish(MazeData* maze) {
    // Старт в левом верхнем углу
    maze->start = {0, 1};
    maze->grid[maze->start.x][maze->start.y] = 'S';
    maze->visited[maze->start.x][maze->start.y] = true;
    
    // Финиш в правом нижнем углу
    maze->finish = {maze->height - 1, maze->width - 2};
    maze->grid[maze->finish.x][maze->finish.y] = 'F';
}

int findShortestPath(MazeData* maze) {
    // 1. Создаем матрицу расстояний
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->dist[i][j] = -1;  // -1 = не посещено
        }
    }
    
    // 2. Два массива для текущей и следующей волны
    vector<Point> currentWave;
    vector<Point> nextWave;
    
    // 3. Начинаем со старта
    currentWave.push_back(maze->start);
    maze->dist[maze->start.x][maze->start.y] = 0;
    
    int distance = 0;
    
    // 4. Пока есть клетки в текущей волне
    while (!currentWave.empty()) {
        distance++;
        
        // 5. Обрабатываем ВСЕ клетки текущей волны
        for (Point current : currentWave) {
            // Если это финиш - возвращаем расстояние
            if (current.x == maze->finish.x && current.y == maze->finish.y) {
                return maze->dist[current.x][current.y];
            }
            
            // 6. Проверяем 4 направления
            for (int dir = 0; dir < 4; dir++) {
                int nx = current.x + dx[dir];
                int ny = current.y + dy[dir];
                
                // Проверяем условия
                if (nx >= 0 && nx < maze->height && 
                    ny >= 0 && ny < maze->width &&
                    maze->grid[nx][ny] != '#' && 
                    maze->dist[nx][ny] == -1) {
                    
                    // Нашли новую клетку!
                    maze->dist[nx][ny] = maze->dist[current.x][current.y] + 1;
                    nextWave.push_back({nx, ny});
                }
            }
        }
        
        // 7. Переходим к следующей волне
        currentWave = nextWave;
        nextWave.clear();  // Очищаем для следующей итерации
    }
    
    return -1;  // Путь не найден
}

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

void markVisited(MazeData* maze, Point p) {
    if (p.x >= 0 && p.x < maze->height && p.y >= 0 && p.y < maze->width) {
        maze->visited[p.x][p.y] = true;
    }
}

bool isValidMove(const MazeData* maze, Point p) {
    return p.x >= 0 && p.x < maze->height && p.y >= 0 && p.y < maze->width && 
           maze->grid[p.x][p.y] != '#';
}

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
    
    // Восстанавливаем символы
    maze->grid[maze->start.x][maze->start.y] = 'S';
    maze->grid[maze->finish.x][maze->finish.y] = 'F';
    
    return maze;
}

void freeMaze(MazeData* maze) {
    delete maze;
}

// Функции для игры

GameState* createGame(int width, int height) {
    GameState* game = new GameState;
    game->maze = createMaze(width, height);
    generateMaze(game->maze);
    game->player = game->maze->start;
    game->moves = 0;
    game->shortestPath = findShortestPath(game->maze);
    return game;
}

GameState* loadGameFromMaze(MazeData* maze, Point player, int moves) {
    GameState* game = new GameState;
    game->maze = maze;
    game->player = player;
    game->moves = moves;
    game->shortestPath = findShortestPath(maze);
    return game;
}

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

void freeGame(GameState* game) {
    if (game) {
        freeMaze(game->maze);
        delete game;
    }
}
