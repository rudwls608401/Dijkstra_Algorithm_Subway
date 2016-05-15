#ifndef SUBWAYGRAPH_H
#define SUBWAYGRAPH_H

#include <iostream>
#include <fstream>
#include <stack> // ����ö ��θ� ����Ҷ� �ʿ�
#include <string> 
using namespace std;

enum color { white, gray }; // ���� �������� ���������� �Ǵ�

class Station // ���� ���� ����
{
public:
	int num; // ���� �Ű��� ����
	int time; // �̵��ð�
	int transfer; // ȯ�� Ƚ��	
	Station* next; // ���� ���� ����Ű�� ������
	bool check; // ȯ�¿����� �ƴ��� check
	Station() : next(NULL) {}
};

class SubwayGraph // ����ö��
{
private:
	string* station_name; // ���̸�
	Station* map; // ���ͽ�Ʈ�� �˰����� ����� ���ο� ����ö ��
	color* check; // white�� ���� ������ ���� ��, gray�� ���� ��
	int n; // ����ö �� ����
public:
	void Subway(const char* name, int N);
	void Dijkstra(int start, bool t); // Dijkstra Algorithm �����κ�
	int ChooseSubwayPath(bool choose); // �ִܽð�, �ּ�ȯ���� ������� �κ� ��
	bool SubwayRoute(stack<int>& s, int start, int end); // Dijkstra Algorithm�� ������ ���� �׷������� ��θ� ���ÿ� �Ű��ִ� ����Լ�
	void PrintRoute(stack<int>& s, int start, int end); // ã�� ��δ� ������ִ� �Լ�.
};

#endif
