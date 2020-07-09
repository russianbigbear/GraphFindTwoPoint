#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // кол-во вершин

/*ввод графа из файла, если он задан матрицей*/
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/*ввод графа из файла, если он задан векторно*/
void input_vector(ifstream &in, int **graph)
{
	int i, j;
	string trash;
	while (!in.eof())
	{
		in >> i;
		in >> trash;
		in >> j;
		graph[i - 1][j - 1] = 1;
	}
}

/* печать графа (вывод в graphic.png) */
void print_graph(int **graph) {
	ofstream out;
	out.open("graphic.dot");
	out << "digraph G{" << "\n";
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0)
				out << i + 1 << " -> " << j + 1 << ";\n";
	out << "}";
	out.close();
	system("dot -Tpng graphic.dot -o graphic.png ");
	system("graphic.png");
}

/*перевод из массива в векторную форму*/
void matrix_to_vector(int **graph, vector<vector<int>>& VGraph) {

	for (int i = 0; i < N; i++)
	{
		VGraph.push_back(vector<int>());
		for (int j = 0; j < N; j++)
		{
			VGraph[i].push_back(0);
			VGraph[i][j] = graph[i][j];
		}
	}
}

/* BFS - Обход в ширину
starting_top - стартовая вершина обхода;
owned_points - вектор множества вершин, родительской вершины;
VGraph - граф в векторной форме;
*/

vector<vector<int>> BFS(int starting_top, vector<vector<int>>& VGraph) {

	queue<int> q; //очередь для вершин обхода
	vector<bool> used(N); //просмотр, выполнен обход вершины или нет
	vector<vector<int>> owned_points;

	for (int i = 0; i < N; i++) owned_points.push_back(vector<int>()); 

	q.push(starting_top); used[starting_top] = true;  //занесли первую вершину
	while (!q.empty())
	{
		int v = q.front(); q.pop();
		for (int i = 0; i < N; i++) {
			if (VGraph[v][i]) {
				owned_points[v].push_back(i + 1); // заносим принадлежащие вершины, родительской
				if (!used[i] && VGraph[v][i]) {
					used[i] = true;
					q.push(i);
				}
			}
		}
	}

	return owned_points;
}

/*доступные вершине точки*/
void available_points_block(bool *s_owned, vector<vector<int>> available_points) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < available_points[i].size(); j++) {
			int point_k = available_points[i][j] - 1;
			if (s_owned[point_k] == false) s_owned[point_k] = true;
		}
}

/* Основная функция
ifs - входной поток;
ofs - выходной поток;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "Редактировать файл ввода? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);

		int mode; // матрица - 0, вектор  - 1

		ifs.open("input.txt");
		ifs >> N;
		ifs >> mode;

		int **graph = new int*[N]; //зануляем матрицу
		for (int i = 0; i < N; i++)
		{
			graph[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph[i][j] = 0;
		}

		if (mode == 0)
		{ // если граф задан матрицей
			input_matrix(ifs, graph);
		}
		else
		{ // если граф задан вектором
			input_vector(ifs, graph);
		}
		ifs.close();

		do {
			cout << "Вывести граф? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph);
		} while (ans != 1 && ans != 0);

		vector <vector<int>> VGraph; 
		matrix_to_vector(graph, VGraph); 

		bool *s_first_available = new bool[N]; for (int i = 0; i < N; i++) s_first_available[i] = false;
		bool *s_second_available = new bool[N]; for (int i = 0; i < N; i++) s_second_available[i] = false;

		int s_first, s_second;
		do {
			cout << "Задайте 2 вершины." << endl;
			cin >> s_first >> s_second;
		} while ((s_first < 1 || s_first > N) || (s_second < 1 || s_second > N));
		cout << endl;

		available_points_block(s_first_available, BFS(s_first - 1, VGraph));
		available_points_block(s_second_available, BFS(s_second - 1, VGraph));

		/*вывод результата в файл*/
		ofs.open("output.txt");
		ofs << "Точки одновременно доступные из " << s_first << " и " << s_second << ": ";
		for (int i = 0; i < N; i++)
			if(s_first_available[i] && s_second_available[i])
				ofs << i + 1 <<  " ";
		ofs.close();

		do {
			cout << "Вывести результат? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt"); //открытие результата
		} while (ans != 1 && ans != 0);
	}

	catch (...) {
		cout << "Ошибка файла!";
	}

	cout << "Программа выполнена." << endl;
	return 0;
}
