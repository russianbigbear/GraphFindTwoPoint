#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // ���-�� ������

/*���� ����� �� �����, ���� �� ����� ��������*/
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/*���� ����� �� �����, ���� �� ����� ��������*/
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

/* ������ ����� (����� � graphic.png) */
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

/*������� �� ������� � ��������� �����*/
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

/* BFS - ����� � ������
starting_top - ��������� ������� ������;
owned_points - ������ ��������� ������, ������������ �������;
VGraph - ���� � ��������� �����;
*/

vector<vector<int>> BFS(int starting_top, vector<vector<int>>& VGraph) {

	queue<int> q; //������� ��� ������ ������
	vector<bool> used(N); //��������, �������� ����� ������� ��� ���
	vector<vector<int>> owned_points;

	for (int i = 0; i < N; i++) owned_points.push_back(vector<int>()); 

	q.push(starting_top); used[starting_top] = true;  //������� ������ �������
	while (!q.empty())
	{
		int v = q.front(); q.pop();
		for (int i = 0; i < N; i++) {
			if (VGraph[v][i]) {
				owned_points[v].push_back(i + 1); // ������� ������������� �������, ������������
				if (!used[i] && VGraph[v][i]) {
					used[i] = true;
					q.push(i);
				}
			}
		}
	}

	return owned_points;
}

/*��������� ������� �����*/
void available_points_block(bool *s_owned, vector<vector<int>> available_points) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < available_points[i].size(); j++) {
			int point_k = available_points[i][j] - 1;
			if (s_owned[point_k] == false) s_owned[point_k] = true;
		}
}

/* �������� �������
ifs - ������� �����;
ofs - �������� �����;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "������������� ���� �����? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);

		int mode; // ������� - 0, ������  - 1

		ifs.open("input.txt");
		ifs >> N;
		ifs >> mode;

		int **graph = new int*[N]; //�������� �������
		for (int i = 0; i < N; i++)
		{
			graph[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph[i][j] = 0;
		}

		if (mode == 0)
		{ // ���� ���� ����� ��������
			input_matrix(ifs, graph);
		}
		else
		{ // ���� ���� ����� ��������
			input_vector(ifs, graph);
		}
		ifs.close();

		do {
			cout << "������� ����? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph);
		} while (ans != 1 && ans != 0);

		vector <vector<int>> VGraph; 
		matrix_to_vector(graph, VGraph); 

		bool *s_first_available = new bool[N]; for (int i = 0; i < N; i++) s_first_available[i] = false;
		bool *s_second_available = new bool[N]; for (int i = 0; i < N; i++) s_second_available[i] = false;

		int s_first, s_second;
		do {
			cout << "������� 2 �������." << endl;
			cin >> s_first >> s_second;
		} while ((s_first < 1 || s_first > N) || (s_second < 1 || s_second > N));
		cout << endl;

		available_points_block(s_first_available, BFS(s_first - 1, VGraph));
		available_points_block(s_second_available, BFS(s_second - 1, VGraph));

		/*����� ���������� � ����*/
		ofs.open("output.txt");
		ofs << "����� ������������ ��������� �� " << s_first << " � " << s_second << ": ";
		for (int i = 0; i < N; i++)
			if(s_first_available[i] && s_second_available[i])
				ofs << i + 1 <<  " ";
		ofs.close();

		do {
			cout << "������� ���������? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt"); //�������� ����������
		} while (ans != 1 && ans != 0);
	}

	catch (...) {
		cout << "������ �����!";
	}

	cout << "��������� ���������." << endl;
	return 0;
}
