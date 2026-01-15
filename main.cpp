#include "h.h"
#include <iostream>
#include <fstream>
#include <limits>
#ifdef _WIN64
#include <windows.h>
#endif

using namespace std;

// Функции пользовательского интерфейса
// правила
void showRules() {
    system("cls");
    cout << "================================" << endl;
    cout << "        ПРАВИЛА ИГРЫ" << endl;
    cout << "================================" << endl;
    cout << endl;
    cout << "ЦЕЛЬ ИГРЫ:" << endl;
    cout << "Дойти от точки старта (S) до точки финиша (F)" << endl;
    cout << "через случайно сгенерированный лабиринт." << endl;
    cout << endl;
    cout << "СИМВОЛЫ НА КАРТЕ:" << endl;
    cout << "  # - стена (непроходима)" << endl;
    cout << "  . - непройденный путь (можно ходить)" << endl;
    cout << "    (пробел) - пройденный путь" << endl;
    cout << "  S - стартовая позиция" << endl;
    cout << "  F - финиш" << endl;
    cout << "  P - игрок (ваше текущее положение)" << endl;
    cout << endl;
    cout << "УПРАВЛЕНИЕ В ИГРЕ:" << endl;
    cout << "  W или w - движение вверх" << endl;
    cout << "  S или s - движение вниз" << endl;
    cout << "  A или a - движение влево" << endl;
    cout << "  D или d - движение вправо" << endl;
    cout << "  E или e - сохранить игру" << endl;
    cout << "  Q или q - выход в главное меню" << endl;
    cout << endl;
    cout << "СОХРАНЕНИЕ ИГРЫ:" << endl;
    cout << "  - Нажмите E в любой момент для сохранения" << endl;
    cout << "  - Игра сохраняется в файл save.txt" << endl;
    cout << "  - Можно загрузить сохранение из главного меню" << endl;
    cout << "  - После победы сохранение автоматически удаляется" << endl;
    cout << endl;
    cout << "ОСОБЕННОСТИ:" << endl;
    cout << "  - Непройденный путь отображается точками (.)" << endl;
    cout << "  - Пройденный путь отображается пробелами" << endl;
    cout << "  - Минимальный путь рассчитывается волновым алгоритмом" << endl;
    cout << "  - Вы можете видеть свой пройденный путь" << endl;
    cout << endl;
    cout << "================================" << endl;
    cout << "Нажмите любую клавишу для возврата в меню..." << endl;
    cin.ignore();
    cin.get();
}
// ввод размера
void getMazeDimensions(int& width, int& height) {
    system("cls");
    cout << "================================" << endl;
    cout << "  ВЫБОР РАЗМЕРОВ ЛАБИРИНТА" << endl;
    cout << "================================" << endl;
    cout << endl;
    
    cout << "Рекомендации:" << endl;
    cout << "- Маленький лабиринт: 10-15" << endl;
    cout << "- Средний лабиринт: 16-25" << endl;
    cout << "- Большой лабиринт: 26-40" << endl;
    cout << "- Максимальный размер: 50" << endl;
    cout << endl;
    
    do {
        cout << "Введите ширину лабиринта (10-50): ";
        cin >> width;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Введите число от 10 до 50." << endl;
            continue;
        }
        
        if (width < 10) {
            cout << "Ширина должна быть не менее 10!" << endl;
        } else if (width > 50) {
            cout << "Ширина не должна превышать 50!" << endl;
        }
    } while (width < 10 || width > 50);
    
    do {
        cout << "Введите высоту лабиринта (10-30): ";
        cin >> height;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Введите число от 10 до 30." << endl;
            continue;
        }
        
        if (height < 10) {
            cout << "Высота должна быть не менее 10!" << endl;
        } else if (height > 30) {
            cout << "Высота не должна превышать 30!" << endl;
        }
    } while (height < 10 || height > 30);
    
    cout << endl;
    cout << "Выбран размер: " << width << "x" << height << endl;
    cout << "Нажмите любую клавишу для продолжения..." << endl;
    cin.ignore();
    cin.get();
}
// сохранения в файле
bool loadSavedGame() {
    system("cls");
    cout << "================================" << endl;
    cout << "      ЗАГРУЗКА СОХРАНЕНИЯ" << endl;
    cout << "================================" << endl;
    cout << endl;
    
    ifstream testFile("save.txt");
    if (!testFile.good()) {
        cout << "Сохраненная игра не найдена!" << endl;
        cout << "Сначала начните новую игру и сохраните ее." << endl;
        cout << endl;
        cout << "Нажмите любую клавишу для возврата в меню..." << endl;
        cin.ignore();
        cin.get();
        return false;
    }
    testFile.close();
    
    cout << "Найден файл сохранения: save.txt" << endl;
    cout << "Загружаем игру..." << endl;
    cout << endl;
    
    Point player;
    int moves;
    MazeData* loadedMaze = loadMazeFromFile("save.txt", player, moves);
    
    if (loadedMaze == nullptr) {
        cout << "Ошибка при загрузке сохранения!" << endl;
        cout << "Файл сохранения поврежден." << endl;
        cout << endl;
        cout << "Нажмите любую клавишу для возврата в меню..." << endl;
        cin.ignore();
        cin.get();
        return false;
    }
    
    cout << "Игра успешно загружена!" << endl;
    cout << "Размер лабиринта: " << loadedMaze->width << "x" << loadedMaze->height << endl;
    cout << "Текущее положение игрока: (" << player.x << ", " << player.y << ")" << endl;
    cout << "Сделано ходов: " << moves << endl;
    cout << endl;
    cout << "Нажмите любую клавишу для продолжения игры..." << endl;
    cin.ignore();
    cin.get();
    
    GameState* game = loadGameFromMaze(loadedMaze, player, moves);
    playGame(game);
    freeGame(game);
    
    return true;
}

int main() {
    #ifdef _WIN64
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    int choice = 0;
    int width = 15, height = 15;
    bool exitGame = false;
    
    while (!exitGame) {
        system("cls");
        cout << "================================" << endl;
        cout << "   ДОБРО ПОЖАЛОВАТЬ В ИГРУ" << endl;
        cout << "           ЛАБИРИНТ" << endl;
        cout << "================================" << endl;
        cout << endl;
        cout << "        ГЛАВНОЕ МЕНЮ" << endl;
        cout << "================================" << endl;
        cout << "1. Правила игры" << endl;
        cout << "2. Выбрать размер лабиринта" << endl;
        cout << "3. Начать новую игру" << endl;
        cout << "4. Загрузить сохраненную игру" << endl;
        cout << "0. Выход" << endl;
        cout << "================================" << endl;
        cout << endl;
        
        ifstream saveFile("save.txt");
        if (saveFile.good()) {
            cout << "Доступно сохранение: save.txt" << endl; // есть сохранение
        } else {
            cout << "Сохраненная игра отсутствует" << endl; // нет сохранения
        }
        saveFile.close();
        
        cout << "Текущий размер лабиринта: " << width << "x" << height << endl;
        cout << "================================" << endl;
        cout << "Выберите пункт меню (0-4): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод! Нажмите любую клавишу..." << endl;
            cin.get();
            continue;
        }
        
        switch (choice) {
            case 1:
                showRules();
                break;
                
            case 2:
                getMazeDimensions(width, height);
                break;
                
            case 3:
                {
                    system("cls");
                    cout << "================================" << endl;
                    cout << "      НАЧАЛО НОВОЙ ИГРЫ" << endl;
                    cout << "================================" << endl;
                    cout << "Генерация лабиринта " << width << "x" << height << "..." << endl;
                    cout << "Нажмите любую клавишу для начала..." << endl;
                    cin.ignore();
                    cin.get();
                    
                    GameState* game = createGame(width, height);
                    playGame(game);
                    freeGame(game);
                }
                break;
                
            case 4:
                cin.ignore();
                loadSavedGame();
                break;
                
            case 0:
                system("cls");
                cout << "================================" << endl;
                cout << "        ВЫХОД ИЗ ИГРЫ" << endl;
                cout << "================================" << endl;
                cout << endl;
                cout << "Спасибо за игру! До свидания!" << endl;
                cout << "================================" << endl;
                exitGame = true;
                break;
                
            default:
                cout << "Неверный выбор! Пожалуйста, выберите пункт от 0 до 4." << endl;
                cout << "Нажмите любую клавишу для продолжения..." << endl;
                cin.ignore();
                cin.get();
                break;
        }
    }
    
    if (choice != 0) {
        cout << endl << "Нажмите любую клавишу для выхода..." << endl;
        cin.ignore();
        cin.get();
    }
    
    return 0;
}