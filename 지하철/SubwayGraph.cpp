#include "SubwayGraph.h"
using namespace std;

void SubwayGraph::Subway(const char* name, int N)
{
	fstream file;
	file.open(name); // file open

	int start = -1, end = -1; // ���ۿ��� ������ �ʱ�ȭ
	int min; // ���� �ð��� ª�� ������ ����
	Station* temp; // file���� �о�� �������� �ӽ�����
	Station* virtual_temp; // �ƹ� ������ ���� class.
	n = N;

	// class field init
	map = new Station[n]; // ����ö ��
	station_name = new string[n];
	check = new color[n];

	int n1, n2, time;
	string s1, s2;

	// ��������Ʈ�� �̿��� ����ö���� ����� ����
	while (!file.eof())
	{
		file >> n1;
		file >> n2;
		file >> time;
		file >> s1;
		file >> s2;

		// �迭�� 0���� �����ϱ� ������ -1�� ���ش�.
		n1 -= 1;
		n2 -= 1;

		// temp�� n2�ε����� ����� ������
		temp = new Station();
		temp->num = n2;
		temp->time = time;
		if (s1 == s2){
			temp->transfer = 1;
		}
		else {
			temp->transfer = 0;
		}

		// station���� ����, ��������Ʈ�� ����� ����
		virtual_temp = &map[n1];
		while (virtual_temp->next != NULL)
		{
			virtual_temp = virtual_temp->next;
		}
		virtual_temp->next = temp;

		// temp�� n1�ε����� ����� ������
		temp = new Station();
		temp->num = n1;
		temp->time = time;
		if (s1 == s2){
			temp->transfer = 1;
		}
		else {
			temp->transfer = 0;
		}

		// station���� ����, ��������Ʈ�� ����� ����
		virtual_temp = &map[n2];
		while (virtual_temp->next != NULL)
		{
			virtual_temp = virtual_temp->next;
		}
		virtual_temp->next = temp;

		station_name[n1] = s1;
		station_name[n2] = s2;

	}
	file.close();

	string input_station; // �� �Է�

	// ����ö ����, ���κ� �Է°� ����ó��
	while (start == end)
	{

		while (start == -1)
		{
			cout << "��� : ";
			cin >> input_station;

			for (int i = 0; i < n; i++)
			{
				if (station_name[i] == input_station)
				{
					start = i;
					break;
				}
			}

			if (start == -1)
			{
				cout << "�����ϴ� ���� �ƴմϴ�." << endl;
			}
		}

		while (end == -1)
		{

			cout << "���� : ";
			cin >> input_station;

			for (int i = 0; i < n; i++)
			{
				if (station_name[i] == input_station)
				{
					end = i;
					cout << endl;
					break;
				}
			}

			if (end == -1)
			{
				cout << "�����ϴ� ���� �ƴմϴ�." << endl;
			}

		}

		if (start == end)
		{
			cout << "��߿��� �������� �����ϴ�." << endl;
			start = end = -1;
		}
	}

	// check�迭, map init
	for (int i = 0; i < n; i++)
	{
		check[i] = white;
		map[i].num = -1;
		map[i].time = 99999999;
		map[i].transfer = 99999999;
	}

	// �������� ����� ��������Ʈ�� �ð��� ȯ��Ƚ���� map�� �ִ´�.
	for (Station* p = map[start].next; p != NULL; p = p->next)
	{
		if (station_name[start] == station_name[p->num]) // �������� �� ȯ�¿��϶� 
		{
			map[p->num].time = 0;
			map[p->num].transfer = 0;
		}
		else
		{
			map[p->num].time = p->time;
			map[p->num].transfer = p->transfer;
		}
	}

	stack<int> s;
	int choice; // �ִܽð��� �ּ�ȯ���߿� ��
	cout << "�ִܽð� -> input 0 / �ּ�ȯ�� -> input 1 : ";
	cin >> choice;
	cout << endl;

	if (choice == 0){
		cout << "*** �ִܽð� ��� ***" << endl;
		Dijkstra(start, true);

		// ������ �߿��� �ҿ�ð��� ���� ���� ���� index����
		min = map[end].time;
		for (int i = 0; i < n; i++)
		{
			if (station_name[end] == station_name[i])
			{
				if (map[i].time < min)
				{
					min = map[i].time;
					end = i;
				}
			}
		}
	}
	else{
		cout << endl;
		cout << "*** �ּ�ȯ�� ��� ***" << endl;
		this->Dijkstra(start, false);

		// ������ �߿��� ȯ��Ƚ���� ���� ���� ���� index����
		min = map[end].transfer;
		for (int i = 0; i < n; i++)
		{
			if (station_name[end] == station_name[i])
			{
				if (map[i].transfer < min)
				{
					min = map[i].transfer;
					end = i;
				}
			}
		}
	}
	SubwayRoute(s, start, end);
	PrintRoute(s, start, end);
}

void SubwayGraph::Dijkstra(int start, bool choose) // ���ͽ�Ʈ�� �˰��� ���
{
	int minvalue_pos;
	check[start] = gray;

	// ������ �ʱ�ȭ
	map[start].time = 0;
	map[start].transfer = 0;

	for (int i = 0; i < n - 2; i++)
	{
		minvalue_pos = ChooseSubwayPath(choose);
		check[minvalue_pos] = gray;

		for (Station* p = map[minvalue_pos].next; p != NULL; p = p->next)
		{
			if (choose) // �ִܽð� ����϶�
			{
				if (map[minvalue_pos].time + p->time < map[p->num].time) // �ð��� ���Ͽ� ������Ʈ
				{
					map[p->num].time = map[minvalue_pos].time + p->time;
					map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
				}
				else if (map[minvalue_pos].time + p->time == map[p->num].time) // �ð��� ���� ���
				{
					if (map[minvalue_pos].transfer + p->transfer < map[p->num].transfer) // ȯ��Ƚ�� ��
					{
						map[p->num].time = map[minvalue_pos].time + p->time;
						map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
					}
				}
			}
			else // �ּ�ȯ�� ����϶�
			{
				if (map[minvalue_pos].transfer + p->transfer < map[p->num].transfer) // ȯ��Ƚ���� ���Ͽ� ������Ʈ
				{
					map[p->num].time = map[minvalue_pos].time + p->time;
					map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
				}
				else if (map[minvalue_pos].transfer + p->transfer == map[p->num].transfer) // ȯ��Ƚ���� ���� ���
				{
					if (map[minvalue_pos].time + p->time < map[p->num].time) // �ð� ��
					{
						map[p->num].time = map[minvalue_pos].time + p->time;
						map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
					}
				}
			}

		}
	}

}

int SubwayGraph::ChooseSubwayPath(bool choose) // choose = 1�̸� �ִܽð��� ������ ���� index��ȯ, choose = 0�̸� �ּ�ȯ���� ������ ���� index��ȯ
{
	int min_time = 99999999;
	int min_transfer = 99999999;
	int pos = -1;

	if (choose) // �ִܽð�
	{
		for (int i = 0; i < n; i++)
		{
			if (check[i] == white)
			{
				if (map[i].time < min_time)
				{
					min_time = map[i].time;
					min_transfer = map[i].transfer;
					pos = i;
				}
				else if (map[i].time == min_time)
				{
					if (map[i].transfer < min_transfer)
					{
						min_time = map[i].time;
						min_transfer = map[i].transfer;
						pos = i;
					}
				}
			}
		}

	}
	else // �ּ�ȯ��
	{

		for (int i = 0; i < n; i++)
		{
			if (check[i] == white)
			{
				if (map[i].transfer < min_transfer) // ȯ�� Ƚ���� ���� ��������Ʈ ����
				{
					min_time = map[i].time;
					min_transfer = map[i].transfer;
					pos = i;
				}
				else if (map[i].transfer == min_transfer) // ȯ�� Ƚ���� ������
				{
					if (map[i].time < min_time) // �ð��� ����� ��������Ʈ ����
					{
						min_time = map[i].time;
						min_transfer = map[i].transfer;
						pos = i;
					}
				}
			}
		}

	}
	return pos; // �ε��� ��ȯ
}

bool SubwayGraph::SubwayRoute(stack<int> &s, int start, int end) // stack�� ������ ����ö�� �־��ִ� ����Լ�
{
	s.push(start);

	if (start == end) // ��������
		return true;
	else{ // recursive
		for (Station* p = map[start].next; p != NULL; p = p->next) // �������� ����� ������ ��� �����Ѵ�
		{
			if (map[start].time + p->time == map[p->num].time && map[start].transfer + p->transfer == map[p->num].transfer)
			{
				if (this->SubwayRoute(s, p->num, end) == false)	s.pop();// ���� ��ΰ� �������� ���� ���
				else return true;
			}
		}
		return false;
	}
}

void SubwayGraph::PrintRoute(stack<int>& s, int start, int end)
{
	stack<int> reverse_stack;
	int i, pre;

	while (!s.empty())
	{
		reverse_stack.push(s.top());
		s.pop();
	}

	// ��� ���
	for (i = 0, pre = -1; !reverse_stack.empty(); i++)
	{

		if (pre != -1 && station_name[pre] == station_name[reverse_stack.top()])
		{
			if (station_name[reverse_stack.top()] != station_name[start])
				cout << "(ȯ��)";

			i--;
			pre = reverse_stack.top();
			reverse_stack.pop();
		}
		else
		{
			if (i != 0)
				cout << " -> ";

			cout << station_name[reverse_stack.top()];
			pre = reverse_stack.top();
			reverse_stack.pop();

		}
	}

	int h = map[end].time / 60;
	int m = map[end].time % 60;
	cout << endl << "@ ������ ���� �� @ : " << i - 1 << "�� " << endl;
	cout << "@ �ҿ�ð� @ : ";
	if (map[end].time / 60 == 0){
		cout << m << "��" << endl;
	}
	else{
		cout << h << "�ð� " << m << "��" << endl;
	}
	cout << "@ ȯ��Ƚ�� @ : " << map[end].transfer << "��" << endl;
}