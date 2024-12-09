#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <algorithm>
#include <random>
#include <string>
#include <functional>

using namespace std;

struct Point {
    int x, y;
};

// Направления для движения
int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

void showWelcomeMessage() {
    cout << "--------------------------------------------------" << endl;
    cout << "Курсовая работа" << endl;
    cout << "Выполнил студент группы 23ВВВ4: Соснин Глеб" << endl;
    cout << "Принял: доцент Юрова О.В." << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Добро пожаловать! Эта программа генерирует лабиринт," << endl;
    cout << "ищет кратчайший путь, загружает и сохраняет лабиринт." << endl;
    cout << "--------------------------------------------------" << endl;
}

int getLabyrinthSize() {
    int N;
    while (true) {
        cout << "Введите размер лабиринта (нечетное число >= 5): ";
        cin >> N;
        if (cin.fail() || N < 5 || N % 2 == 0) {
            cout << "Ошибка: размер должен быть нечетным числом >= 5." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    return N;
}

// Генерация лабиринта 
void generateMaze(vector<vector<char>>& maze, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            maze[i][j] = '#'; 
        }
    }

    maze[1][1] = '.'; 
    maze[N - 2][N - 2] = '.'; 

    random_device rd;
    mt19937 gen(rd());

    function<void(int, int)> carvePath = [&](int x, int y) {
        vector<int> directions = { 0, 1, 2, 3 };
        shuffle(directions.begin(), directions.end(), gen);

        for (int i : directions) {
            int nx = x + dx[i] * 2;
            int ny = y + dy[i] * 2;
            if (nx > 0 && ny > 0 && nx < N - 1 && ny < N - 1 && maze[nx][ny] == '#') {
                maze[x + dx[i]][y + dy[i]] = '.';
                maze[nx][ny] = '.';
                carvePath(nx, ny);
            }
        }
        };

    carvePath(1, 1);

    int x = 1, y = 1;
    while (x != N - 2 || y != N - 2) {
        if (x < N - 2) maze[++x][y] = '.';
        if (y < N - 2) maze[x][++y] = '.';
    }
}

// Вывод лабиринта на экран
void printMaze(const vector<vector<char>>& maze) {
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Сохранение лабиринта в файл
void saveMazeToFile(const vector<vector<char>>& maze, const string& filename) {
    ofstream outFile(filename);
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            outFile << cell << " ";
        }
        outFile << endl;
    }
    outFile.close();
    cout << "Лабиринт сохранён в файл: " << filename << endl;
}

// Загрузка лабиринта из файла
bool loadMazeFromFile(vector<vector<char>>& maze, const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Ошибка: не удалось открыть файл: " << filename << endl;
        return false;
    }

    vector<vector<char>> tempMaze;
    string line;
    while (getline(inFile, line)) {
        vector<char> row;
        for (char c : line) {
            if (c != ' ' && c != '\n') row.push_back(c);
        }
        if (!row.empty()) tempMaze.push_back(row);
    }
    inFile.close();

    if (tempMaze.size() != tempMaze[0].size()) {
        cout << "Ошибка: лабиринт должен быть квадратным." << endl;
        return false;
    }

    maze = tempMaze;
    return true;
}

// Поиск пути 
bool BFS(vector<vector<char>>& maze, int N, vector<vector<int>>& dist, vector<vector<Point>>& parent) {
    queue<Point> q;
    q.push({ 1, 1 });
    dist[1][1] = 0;

    while (!q.empty()) {
        Point p = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < N && ny < N && maze[nx][ny] == '.' && dist[nx][ny] == -1) {
                dist[nx][ny] = dist[p.x][p.y] + 1;
                parent[nx][ny] = p;
                q.push({ nx, ny });

                if (nx == N - 2 && ny == N - 2) {
                    return true;
                }
            }
        }
    }
    return false;
}

void markPath(vector<vector<char>>& maze, vector<vector<Point>>& parent, int N) {
    Point p = { N - 2, N - 2 };
    while (!(p.x == 1 && p.y == 1)) {
        maze[p.x][p.y] = '*';
        p = parent[p.x][p.y];
    }
    maze[1][1] = '*';
}

// Координаты пути
void printPath(const vector<vector<Point>>& parent, int N) {
    vector<Point> path;
    Point p = { N - 2, N - 2 };
    while (!(p.x == 1 && p.y == 1)) {
        path.push_back(p);
        p = parent[p.x][p.y];
    }
    path.push_back({ 1, 1 });
    cout << "Найденный путь: ";
    for (int i = path.size() - 1; i >= 0; --i) {
        cout << "(" << path[i].x << ", " << path[i].y << ")";
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}

// Главное меню
void mainMenu() {
    vector<vector<char>> maze;
    while (true) {
        cout << "\nМеню:\n1. Генерация нового лабиринта\n2. Сохранить лабиринт в файл\n3. Загрузить лабиринт из файла\n4. Найти путь\n5. Выход\nВаш выбор: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int N = getLabyrinthSize();
            maze.resize(N, vector<char>(N));
            generateMaze(maze, N);
            cout << "Сгенерированный лабиринт:" << endl;
            printMaze(maze);
        }
        else if (choice == 2) {
            if (maze.empty()) {
                cout << "Сначала сгенерируйте лабиринт." << endl;
                continue;
            }
            string filename;
            cout << "Введите имя файла для сохранения: ";
            cin >> filename;
            saveMazeToFile(maze, filename);
        }
        else if (choice == 3) {
            string filename;
            cout << "Введите имя файла для загрузки: ";
            cin >> filename;
            if (loadMazeFromFile(maze, filename)) {
                cout << "Лабиринт успешно загружен:" << endl;
                printMaze(maze);
            }
        }
        else if (choice == 4) {
            if (maze.empty()) {
                cout << "Лабиринт не загружен!" << endl;
                continue;
            }
            int N = maze.size();
            vector<vector<int>> dist(N, vector<int>(N, -1));
            vector<vector<Point>> parent(N, vector<Point>(N));

            if (BFS(maze, N, dist, parent)) {
                markPath(maze, parent, N);
                cout << "Лабиринт с отмеченным путём:" << endl;
                printMaze(maze);
                printPath(parent, N);
            }
            else {
                cout << "Путь не найден." << endl;
            }
        }
        else if (choice == 5) {
            cout << "Выход из программы." << endl;
            break;
        }
        else {
            cout << "Неверный выбор, попробуйте снова." << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    showWelcomeMessage();
    mainMenu();
    return 0;
}
