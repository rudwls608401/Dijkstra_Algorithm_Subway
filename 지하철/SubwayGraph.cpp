#include "SubwayGraph.h"
using namespace std;

void SubwayGraph::Subway(const char* name, int N)
{
	fstream file;
	file.open(name); // file open

	int start = -1, end = -1; // 시작역과 도착역 초기화
	int min; // 가장 시간이 짧은 도착역 저장
	Station* temp; // file에서 읽어온 역정보를 임시저장
	Station* virtual_temp; // 아무 정보도 없는 class.
	n = N;

	// class field init
	map = new Station[n]; // 지하철 망
	station_name = new string[n];
	check = new color[n];

	int n1, n2, time;
	string s1, s2;

	// 인접리스트를 이용해 지하철망을 만드는 과정
	while (!file.eof())
	{
		file >> n1;
		file >> n2;
		file >> time;
		file >> s1;
		file >> s2;

		// 배열이 0부터 시작하기 떄문에 -1씩 빼준다.
		n1 -= 1;
		n2 -= 1;

		// temp의 n2인덱스에 저장될 역정보
		temp = new Station();
		temp->num = n2;
		temp->time = time;
		if (s1 == s2){
			temp->transfer = 1;
		}
		else {
			temp->transfer = 0;
		}

		// station끼리 연결, 인접리스트를 만드는 과정
		virtual_temp = &map[n1];
		while (virtual_temp->next != NULL)
		{
			virtual_temp = virtual_temp->next;
		}
		virtual_temp->next = temp;

		// temp의 n1인덱스에 저장될 역정보
		temp = new Station();
		temp->num = n1;
		temp->time = time;
		if (s1 == s2){
			temp->transfer = 1;
		}
		else {
			temp->transfer = 0;
		}

		// station끼리 연결, 인접리스트를 만드는 과정
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

	string input_station; // 역 입력

	// 지하철 시작, 끝부분 입력과 예외처리
	while (start == end)
	{

		while (start == -1)
		{
			cout << "출발 : ";
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
				cout << "존재하는 역이 아닙니다." << endl;
			}
		}

		while (end == -1)
		{

			cout << "도착 : ";
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
				cout << "존재하는 역이 아닙니다." << endl;
			}

		}

		if (start == end)
		{
			cout << "출발역과 도착역이 같습니다." << endl;
			start = end = -1;
		}
	}

	// check배열, map init
	for (int i = 0; i < n; i++)
	{
		check[i] = white;
		map[i].num = -1;
		map[i].time = 99999999;
		map[i].transfer = 99999999;
	}

	// 시작점과 연결된 인접리스트의 시간과 환승횟수를 map에 넣는다.
	for (Station* p = map[start].next; p != NULL; p = p->next)
	{
		if (station_name[start] == station_name[p->num]) // 시작점이 곧 환승역일때 
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
	int choice; // 최단시간과 최소환승중에 택
	cout << "최단시간 -> input 0 / 최소환승 -> input 1 : ";
	cin >> choice;
	cout << endl;

	if (choice == 0){
		cout << "*** 최단시간 경로 ***" << endl;
		Dijkstra(start, true);

		// 도착역 중에서 소요시간이 가장 작은 역의 index선택
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
		cout << "*** 최소환승 경로 ***" << endl;
		this->Dijkstra(start, false);

		// 도착역 중에서 환승횟수가 가장 작은 역의 index선택
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

void SubwayGraph::Dijkstra(int start, bool choose) // 다익스트라 알고리즘 사용
{
	int minvalue_pos;
	check[start] = gray;

	// 시작점 초기화
	map[start].time = 0;
	map[start].transfer = 0;

	for (int i = 0; i < n - 2; i++)
	{
		minvalue_pos = ChooseSubwayPath(choose);
		check[minvalue_pos] = gray;

		for (Station* p = map[minvalue_pos].next; p != NULL; p = p->next)
		{
			if (choose) // 최단시간 경로일때
			{
				if (map[minvalue_pos].time + p->time < map[p->num].time) // 시간을 비교하여 업데이트
				{
					map[p->num].time = map[minvalue_pos].time + p->time;
					map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
				}
				else if (map[minvalue_pos].time + p->time == map[p->num].time) // 시간이 같을 경우
				{
					if (map[minvalue_pos].transfer + p->transfer < map[p->num].transfer) // 환승횟수 비교
					{
						map[p->num].time = map[minvalue_pos].time + p->time;
						map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
					}
				}
			}
			else // 최소환승 경로일때
			{
				if (map[minvalue_pos].transfer + p->transfer < map[p->num].transfer) // 환승횟수를 비교하여 업데이트
				{
					map[p->num].time = map[minvalue_pos].time + p->time;
					map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
				}
				else if (map[minvalue_pos].transfer + p->transfer == map[p->num].transfer) // 환승횟수가 같을 경우
				{
					if (map[minvalue_pos].time + p->time < map[p->num].time) // 시간 비교
					{
						map[p->num].time = map[minvalue_pos].time + p->time;
						map[p->num].transfer = map[minvalue_pos].transfer + p->transfer;
					}
				}
			}

		}
	}

}

int SubwayGraph::ChooseSubwayPath(bool choose) // choose = 1이면 최단시간을 가지는 역의 index반환, choose = 0이면 최소환승을 가지는 역의 index반환
{
	int min_time = 99999999;
	int min_transfer = 99999999;
	int pos = -1;

	if (choose) // 최단시간
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
	else // 최소환승
	{

		for (int i = 0; i < n; i++)
		{
			if (check[i] == white)
			{
				if (map[i].transfer < min_transfer) // 환승 횟수가 작은 인접리스트 선택
				{
					min_time = map[i].time;
					min_transfer = map[i].transfer;
					pos = i;
				}
				else if (map[i].transfer == min_transfer) // 환승 횟수가 같으면
				{
					if (map[i].time < min_time) // 시간이 덜드는 인접리스트 선택
					{
						min_time = map[i].time;
						min_transfer = map[i].transfer;
						pos = i;
					}
				}
			}
		}

	}
	return pos; // 인덱스 반환
}

bool SubwayGraph::SubwayRoute(stack<int> &s, int start, int end) // stack에 지나간 지하철을 넣어주는 재귀함수
{
	s.push(start);

	if (start == end) // 종료조건
		return true;
	else{ // recursive
		for (Station* p = map[start].next; p != NULL; p = p->next) // 시작점에 연결된 엣지를 모두 조사한다
		{
			if (map[start].time + p->time == map[p->num].time && map[start].transfer + p->transfer == map[p->num].transfer)
			{
				if (this->SubwayRoute(s, p->num, end) == false)	s.pop();// 다음 경로가 존재하지 않을 경우
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

	// 경로 출력
	for (i = 0, pre = -1; !reverse_stack.empty(); i++)
	{

		if (pre != -1 && station_name[pre] == station_name[reverse_stack.top()])
		{
			if (station_name[reverse_stack.top()] != station_name[start])
				cout << "(환승)";

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
	cout << endl << "@ 지나는 역의 수 @ : " << i - 1 << "개 " << endl;
	cout << "@ 소요시간 @ : ";
	if (map[end].time / 60 == 0){
		cout << m << "분" << endl;
	}
	else{
		cout << h << "시간 " << m << "분" << endl;
	}
	cout << "@ 환승횟수 @ : " << map[end].transfer << "번" << endl;
}