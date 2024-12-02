#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stack>

using namespace std;

struct Point {
    int x, y;
};

// Массивы для движения в 4 направлениях: вверх, вправо, вниз, влево
int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

// Функция для отображения приветственного сообщения
void showWelcomeMessage() {
    cout << "--------------------------------------------------" << endl;
    cout << "Курсовая работа: Генерация лабиринта и поиск пути" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Добро пожаловать! Эта программа генерирует лабиринт," << endl;
    cout << "ищет кратчайший путь и выводит координаты пути." << endl;
    cout << "--------------------------------------------------" << endl;
}

// Проверка корректности входного размера лабиринта
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
            if (i == 0 || j == 0 || i == N - 1 || j == N - 1) {
                maze[i][j] = '#'; 
            }
            else {
                maze[i][j] = '.';  
            }
        }
    }
    srand(static_cast<unsigned>(time(0))); 
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            if (rand() % 3 == 0) {
                maze[i][j] = '#';  
            }
        }
    }
    maze[1][1] = '.';           
    maze[N - 2][N - 2] = '.'; 
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

// Сохранение лабиринта в блокнот
void saveMazeToFile(const vector<vector<char>>& maze, const string& filename) {
    ofstream outFile(filename);
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            outFile << cell << " ";
        }
        outFile << endl;
    }
    outFile.close();
}

// Поиск пути методом BFS (поиск в ширину)
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

// Вывод координат найденного пути
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

int main() {
    setlocale(LC_ALL, "Russian");
   
    showWelcomeMessage();

    int N = getLabyrinthSize();
    vector<vector<char>> maze(N, vector<char>(N));

    generateMaze(maze, N);
    cout << "Сгенерированный лабиринт:" << endl;
    printMaze(maze);
    saveMazeToFile(maze, "labyrinth.txt");

    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<Point>> parent(N, vector<Point>(N));

    if (BFS(maze, N, dist, parent)) {
        markPath(maze, parent, N);
        cout << "Лабиринт с отмеченным путем:" << endl;
        printMaze(maze);
        printPath(parent, N);
    }
    else {
        cout << "Путь не найден." << endl;
    }
    return 0;
}
