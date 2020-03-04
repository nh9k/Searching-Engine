/*
자료구조 및 알고리즘 텀프로젝트: 검색기 - 동동이팀
21611663 이주형 21611591 김난희
*/
//#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "winmm.lib")   // 시간 측정을 위한 라이브러리 포함
#include <iostream>

///making hash table
#include <map> 
#include <vector>
#include <string>

///출력폭 조정
#include <iomanip> // cout 사용시 출력폭 조정(setw)을 위함

///getting file contents
#include<fstream>

///searching
#include <sstream> //string stream
#include <string>  // 띄워쓰기 포함 string 입력받기

///화면 클리어 & 시간측정
#include <windows.h>

using namespace std;

///making hash table - map1
typedef struct info {
	short unsigned int doc;  // document id
	short unsigned int seq;  // 단어 위치(순서)
	struct info *next;       // 다음 struct info를 가르킬 (댕글댕글) 구조체 포인터
}info;

///searching table - map3
typedef struct doccountinfo {  //map3에 넣을 구조체
	short unsigned int doc;    // document id, scoring 안하므로 안씀, semi scoring
	short unsigned int var;    // 분산
	struct doccountinfo *next; // 다음 struct info를 가르킬 (댕글댕글) 구조체 포인터
}doccountinfo;

typedef map<string, info*> _map;		// 텍스트파일로 부터 정리한 단어 map(멤버: doc 번호, seq 번호)
_map dic1;

typedef map<int, vector<int>> _map2;	// 검색한 단어에 대해 정리한 doc 번호 map(멤버: seq 번호)
_map2 dic2;

typedef map< int, doccountinfo*> _map3; // 마지막으로 doc 최종 정렬을 구현한 doc 개수 map(멤버: doc 번호, seq 분산)
_map3 dic3;

///searching
string express[100];       // StringEngine()의 전역 변수 단어 100개만 입력 받음 //전역으로 선언해서 더 빠름
string sameexpress[50];	   // 유사 단어 검색을 막기 위한 배열 선언

int k = 0;                 // 입력한 단어 개수만큼 증가 -> 입력한 단어 개수만큼 카운트한 변수
int seq[10];               // 단어 10개 계산으로 초기화 -> 분산 계산시에 map2의 댕글댕글 나온 한 벡터에서 분산 모아 계산
int f1 = 0, f2 = 0;		   // 유사 단어 분리를 위한 flag -> ddengine에서 사용

///map1 관련 함수
void insert(string, int, int);    // map1에 단어(댕글댕글 멤버: doc 번호, seq 번호) 삽입 함수
void textinput(string);			  // text파일에서 읽어와 단어, DOC, DOC 내부의 순서(Sequence)를 삽입하는 함수
void traverse();                  // map1의 모든 멤버 출력
int StringEngine();			      // 검색기에 입력한 문자열을 처리하는 함수

///map2 관련함수
void SearchEngine();			  // 단어 존재 여부를 따져 map2에 댕글댕글 달기
void semi_scoring();			  // map2(doc 번호가 map, seq가 댕글댕글 벡터)에 달린 벡터들을 참조해서 분산을 계산하고, 
				    			  // map3로 넘겨주는 역할(map3와 구조체(링크드리스트)에 삽입)	  
float var_seq(int[], int);		  // document 내부에 있는 검색한 단어들의 분산 조사

///map3 관련 함수
void insert_scoring(int, int, int);		// map3, 구조체에 삽입
void dd_engine();						// searching 최종 결과 출력 함수
void textprint(string, int, string *);  // text로 부터 doc 번호와 단어를 알 경우 위치를 찾아 line을 출력하는 함수

int main() {
	//DWORD dwstart, dwend;			// 시작 시간, 끝시간
	//DWORD dwElapsed;				// 걸린 시간

	//dwstart = timeGetTime();		// 시작 시간 측정
	textinput("document.txt");
	//textinput("test.txt");
	//dwend = timeGetTime();	    // 끝 시간 측정
	//dwElapsed = dwend - dwstart;	// 걸린 시간 측정
	//cout << "map 1: timeGetTime : " << dwElapsed * 0.001f << endl; // 출력

	while (1)
	{
		//dwstart = timeGetTime();		
		int returnValue = StringEngine();			// 검색기에 입력한 문자열을 처리하는 함수
		//dwend = timeGetTime();			
		//dwElapsed = dwend - dwstart;	
		//cout << "검색: timeGetTime : " << dwElapsed * 0.001f << endl; 

		if (returnValue)
		{
			if (returnValue == 2)
			{
				system("cls"); //콘솔화면 지우기
				continue;
			}
			cout << "프로그램을 종료합니다.\n";
			return 0;
		}
		/*dwstart = timeGetTime();*/		

		SearchEngine();	 // 단어 존재 여부를 따져 map2에 댕글댕글 달기
		semi_scoring();	 // map2(doc 번호가 map, seq가 댕글댕글 벡터)에 달린 벡터들을 참조해서 분산을 계산하고, map3로 넘겨줌

		/*dwend = timeGetTime();			
		dwElapsed = dwend - dwstart;	
		cout << "map2~map3: timeGetTime : " << dwElapsed * 0.001f << endl; */

		//dwstart = timeGetTime();		

		dd_engine();	 // searching 최종 결과 출력 함수

		//dwend = timeGetTime();			
		//dwElapsed = dwend - dwstart;	
		//cout << "결과 출력: timeGetTime : " << dwElapsed * 0.001f << endl; 

		dic2.clear();	 //map2 clear()
		dic3.clear();	 //map3 clear()
	}
}

void textprint(string textfile, int doc_num, string *word) // text로 부터 doc 번호와 단어를 알 경우 위치를 찾아 line을 출력하는 함수
{
	//댕글댕글(분산,doc num 저장된 구조체:map3)마다 scan함
	string line, out;
	size_t fp;
	int i = 1;

	ifstream in(textfile);
	while (getline(in, line))
	{
		if (i == doc_num)				         //줄 수와 doc_num 같으면
		{
			for (int p = 0; p < k; p++)          //검색한 단어 개수만큼 for loop
			//int po = line.substr(fp);
			//line.find(express[i]) != string::npos         
			{
				fp = line.find(word[p]);	     //doc 안에서 검색한 단어 위치 찾음
				if (fp == string::npos)          //해당하는 단어가 없으면 skip
				{
					continue;
				}
				out = line.substr(fp, 30);       //fp에서 30만큼 읽기

				cout << "  ..." << out << endl;  // 출력
				//cout << "......." << ch << "......." << endl;
				//문자열이 있는 위치를 출력
				//한국어는 2byte에 한글자를 저장하는데 1byte를 출력할 경우 ?로 뜸
			}
			//line.substr(fp)하면 문자열이 출력됨
			break;
		}
		i++;
	}
	in.close();
}

void dd_engine() // searching 최종 결과 출력 함수
{
	//DWORD dwstart, dwend;			// 시작 시간, 끝시간
	//DWORD dwElapsed;				// 걸린 시간

	//dwstart = timeGetTime();		// 시작 시간 측정
	
	//system("cls"); //콘솔화면 지우기
	map<int, doccountinfo*>::reverse_iterator iter; // map을 순회하기 위한 iterator
	struct doccountinfo *scan;                      // scan으로 참조함, iterater를 수정하면 자료 구조가 붕괴될 수 있음
	int i;											// 한 번에 몇 개씩 출력할지를 결정하는 변수
	char key = 0, key1 = 0;							// key는 검색을 계속할지 묻는 변수, key1은 유사단어 검색을 할지 묻는 변수

	if (f1 == 1 && f2 == 1)							// 존재하는 단어와 존재하지 않는 단어를 함께 검색한 경우
	{
		cout << "   유사 단어가 있으면 함께 출력하시겠습니까? document 순서에는 반영되지 않습니다.(y/n) ";
		cin >> key1;
	}

	if (f2 == 1)									// 단어가 존재하는 경우 무조건 출력
	{
		//cout << "   * * * * * * * DDONGDDONG ENGINE * * * * * * *" << "\n" << "   * * * * * * * ----------------- * * * * * * *" << '\n' << endl;
		cout << "  ...";
		for (i = 0; i < k; i++) 
		{
			cout << express[i] << ' ';				 // 모든 단어 출력
		}
		cout << "의 검색결과 입니다." << "\n" << endl;
		cout << "   #    『Document』   검색한 단어의 분산 정도" << endl;
	}

	for (iter = dic3.rbegin(), i = 1; iter != dic3.rend(); ++iter) {        // map의 끝부터 처음까지 (index) 단어 목록
		cout << "   -------------------------------------------" << endl;
		cout << "  ..." << (*iter).first << "개의 검색어가 포함된 Document 검색 중 ..." << endl;
		for (scan = (*iter).second; scan != NULL; scan = scan->next, i++) { // 단어 아래 모든 document 들을 출력함, setw()는 폭 맞춰줌
			cout << setw(4) << i << setw(11) << " 『" << setw(4) << scan->doc << "』   " << "         " << setw(13) << scan->var << endl;

			if (key1 == 'n')											    // 유사 단어를 출력할 필요가 없는 경우
				textprint("document.txt", scan->doc, sameexpress);
			//else cout << "출력할 단어가 없습니다." << endl;

			else                                                            // 유사 단어를 출력할 필요가 있는 경우
				textprint("document.txt", scan->doc, express);


			if (i % 15 == 0)												//15개 doc씩 출력
			{	
				//dwend = timeGetTime();	    // 끝 시간 측정
				//dwElapsed = dwend - dwstart;	// 걸린 시간 측정
				//cout << "결과 출력: timeGetTime : " << dwElapsed * 0.001f << endl; // 출력

				cout << "계속 검색하시겠습니까?(y/n) ";
				cin >> key;
				cout << endl;
				if (key == 'n') break;
			}
		}
		if (key == 'n') break;
	}
	cout << '\n' << '\n' << '\n' << endl;
}

void semi_scoring() // map2(doc 번호가 map, seq가 댕글댕글 벡터)에 달린 벡터들을 참조해서 분산을 계산하고, 
{					// map3로 넘겨주는 역할
	map<int, vector<int>>::iterator iter;	// map의 iterator
	vector<int>::iterator vec;				// 벡터의 iterator

	int i, var_t, doc_t;					//개수, 분산, 서류 번호

	for (iter = dic2.begin(); iter != dic2.end(); iter++) {	//map 멤버가 끝이 날 때까지, index 끝까지
		i = 0;								// 댕글 댕글 달린 벡터 개수를 처음부터 세기 위함
		var_t = 0;							// 계산하는 분산 초기화

		//cout << "DOC " << (*iter).first << " >>> seq  ";

		for (vec = (*iter).second.begin(); vec != (*iter).second.end(); vec++, i++) {
			//cout << *vec << ' '; // 댕글댕글 seq 벡터 값 참조
			seq[i] = *vec;		   //하나의 document 번호 안에 달린 순서를 배열로 모아 나중에 분산 계산하기 위함
								   //(map의 index(서류 번호)에 따라 달린 순서를 하나씩 읽음(vec) 
		}

		//cout << '\n';
		//cout << "doc 개수 " << i << endl;

		if (i > 1)						 //개수가 하나 이상이면 분산을 계산한다
			var_t = var_seq(seq, i);

		doc_t = (*iter).first;	         //map의 index는 서류번호
		insert_scoring(i, doc_t, var_t); //개수, 서류 번호, 분산을 map3에 삽입한다.
	}
}

float var_seq(int data[], int count) // document 내부에 있는 검색한 단어들의 분산 조사
{
	int i;							 // 벡터의 수를 세는 변수
	int sum = 0;					 // 총합 계산하는 변수

	for (i = 0; i < count; ++i)
		sum += data[i];              // 총합

	float mean = sum / count;        // 평균

	sum = 0.0;				         // 총합 변수를 다른 이용을 하기 위해 초기화

	///분산 계산하기
	for (i = 0; i < count; ++i)
		sum += (data[i] - mean) * (data[i] - mean);

	sum /= (int)(count - 1);         // 최종 분산
	//cout <<"var: "<< sum <<endl;   // 분산 출력

	return sum;						 // map2 하나의 index에서 댕글 댕글 달린 vector들의 전체 분산
}

void insert_scoring(int doc_count, int doc_t, int var_t) // map3, 구조체에 삽입
{
	struct doccountinfo *present = new doccountinfo;
	struct doccountinfo *scan;

	if (dic3.count(doc_count) == 0) // 처음 나온 단어일 경우(map indexing)
	{
		present->doc = doc_t;   // 구조체, 정보 1: document
		present->var = var_t;   // 구조체, 정보 2: sequence
		present->next = NULL;   // 구조체, 포인터
		dic3.insert(pair<int, doccountinfo*>(doc_count, present)); // map의 index에 단어를, 구조체에 doc, seq 정보를 삽입
	}
	else //처음 나온 단어가 아닐 때
	{
		map<int, doccountinfo*>::iterator iter;          // map의 index를 확인할 iterator
		iter = dic3.find(doc_count);                     // 삽입할 단어의 map indexing 위치를 받아옴

		if ((*iter).second->var > var_t)
		{
			present->next = (*iter).second->next;   // 현재 삽입할 구조체가 가리키는 곳은 원래 가리키던 곳
			(*iter).second->next = present;         // map 구조체 뒤에 현재 삽입할 구조체를 연결함

			/// sequence 순서가 뒤인 것을 뒤로 보내기
			present->doc = (*iter).second->doc;     // 현재 삽입한 구조체에는 map 구조체의 document를 넣음
			present->var = (*iter).second->var;     // 현재 삽입한 구조체에는 map 구조체의 분산을 넣음

			(*iter).second->doc = doc_t;            // map 구조체의 document를 현재 삽입한 구조체에 넣음
			(*iter).second->var = var_t;            // map 구조체의 분산을 현재 삽입한 구조체에 넣음
		}
		else // 새로운 document가 map 구조체 뒤의 링크드리스트(map 아닌 구조체)에 달릴 경우(swap이 필요 없는 삽입)
		{
			for (scan = (*iter).second; scan != NULL; scan = scan->next) //map 구조체에서 부터 scan이 시작함
			{
				if (scan->next == NULL) //순회를 다 하고, sequence가 가장 커서 맨 뒤에 삽입될 경우
				{
					///scan의 다음에 present를 삽입하는 과정
					present->next = scan->next;   //원래 scan이 가리키던 곳을 present가 가리키고
					scan->next = present;         //scan은 present를 가리킴

					present->doc = doc_t;         // doc 정보 
					present->var = var_t;         // 분산 정보
					break;                        // 삽입하고 다시 for문 검사 안하고 break 하도록 함
				}
				else if (scan->next->var > var_t) // scan->next ==NULL인데, seq를 조회할 경우 오류나므로,
				{                                 // scan->next 조건문 먼저 검사 후에 검사함
					///scan의 다음에 present를 삽입하는 과정
					present->next = scan->next;   //원래 scan이 가리키던 곳을 present가 가리키고
					scan->next = present;         //scan은 present를 가리킴

					present->doc = doc_t;         // doc 정보 
					present->var = var_t;         // 분산 정보
					break;                        // 삽입하고 다시 for문 검사 안하고 break 하도록 함
				}

			}
		}
	}
}

void SearchEngine() // 단어 존재 여부를 따져 map2에 댕글댕글 달기
{
	map<string, info*>::iterator iter;     // map의 index를 확인할 iterator
	struct info *scan;                     // scan으로 참조함, iterater를 수정하면 자료 구조가 붕괴될 수 있음

	int si = 0;					           // sameexpress 배열을 위한 인자, sameexpress는 유사 단어 검색이 아닐 때 사용
	f1 = 0, f2 = 0;						   // 유사 단어 검색을 할 때 필요한 flag
	cout << '\n';

	for (int i = 0; i < k; i++)
	{
		iter = dic1.find(express[i]);    // 삽입할 단어의 map indexing 위치를 받아옴
		if (iter == dic1.end())          // 입력한 단어가 존재하지 않을 때
		{
			f1 = 1;						 // dd_engine에서 유사 단어가 있어 출력할 지 물을 때, f1==1, f2==2인지 검사
			//cout << "[" << i << "] " << express[i] << endl;
			cout << " 『" << express[i] << "』" << " 단어가 존재하지 않습니다." << endl;
			continue;
		}
		else {						     // 입력한 단어가 존재할 때
			f2 = 1;						 // 단어가 존재할 때 단어에 대한 정보를 출력해야하므로, 사용하는 flag 변수
			sameexpress[si] = express[i];
			si++;
			//cout << "[" << i << "] " << express[i] << endl;
			cout << " 『" << (*iter).first << "』" << " 단어가 존재합니다." << endl;

			/// 단어 아래 모든 document 들을 출력함 //setw()는 폭 맞춰줌
			for (scan = (*iter).second; scan != NULL; scan = scan->next) { 
			   //cout << "       Seq " << setw(4) << scan->seq << "  /  DOC " << setw(4) << scan->doc << endl;

				dic2[scan->doc].push_back(scan->seq); // _map2에 댕글댕글 달기
			}
		}
	}
}

int StringEngine() // 검색기에 입력한 문자열 처리하는 함수
{
	string sentence;

	cout << "   * * * * * * * DDONGDDONG ENGINE * * * * * * *" << "\n" << "   * * * * * * * ----------------- * * * * * * *" << "      (프로그램 종료: Q, 화면 클리어: C)" << endl;
	cout << "\n" << "  검색:  ";

	getline(cin, sentence);                 // 띄워 쓰기 포함 문장 입력 받기
	//cout << sentence<<endl;
	//DWORD dwstart1, dwend1;			// 시작 시간, 끝시간
	//DWORD dwElapsed1;				// 걸린 시간

	//dwstart1 = timeGetTime();		// 시작 시간 측정
	
	stringstream stringEngine(sentence);    // 문자열 처리하는 스트림
	k = 0;

	while (stringEngine >> express[k])      // 받은 문장을 string 배열에 입력
	{
		if (express[k] == "Q")              // Q를 누르면 프로그램 종료
			return 1;
		if (express[k] == "C")				// C를 누르면 화면 클리어
			return 2;
		//cout << express[k] << endl;       // 분리한 단어 출력
		k++;                                // 입력한 단어 개수만큼 증가
	}
	//for (int i = 0; i < k; i++)           // 문자열 분리 잘했는지 테스트 코드
	//   cout << "["<< i <<"] "<< express[i] << endl;
	
	stringEngine.clear();  // stringstream에 저장된 문자열을 clear하지는 않음
	// stringstream에 저장된 문자열을 삭제하고 싶으면, stringstream ss; ss.str("")를 사용하면 된다.
    // clear를 실시한 경우에만, 새로운 문자열을 받았을 때 첫 위치부터 추출이 가능하다.

	//dwend1 = timeGetTime();	    // 끝 시간 측정
	//dwElapsed1 = dwend1 - dwstart1;	// 걸린 시간 측정
	//cout << "검색 문자열 처리: timeGetTime : " << dwElapsed1 * 0.001f << endl; // 출력

	return 0;
}

void textinput(string textfile) // text파일에서 읽어와 단어, DOC, DOC 내부의 순서(Sequence)를 삽입하는 함수 
{
	string word;

	int count_seq = 1;  // 순서 초기화
	int count_doc = 1;  // doc 번호 초기화

	ifstream in(textfile);

	while (in.is_open()) {
		in >> word;

		//cout << "단어: " << word << endl;
		//cout << "seq: " << count_seq << endl;
		//cout << "doc: " << count_doc << endl;

		insert(word, count_doc, count_seq); // 입력받은 문자열을 insert 함수(insert("단어", DOC, SEQ))에 넣어서 hasing table 만들기
		count_seq++;                        // 단어가 문서에 나오는 순서+1

		if (count_seq == 1001)              // document의 마지막 단어까지 다 읽으면
		{
			count_seq = 1;                  // 단어가 문서에 나오는 순서를 1로 초기화
			count_doc++;                    // 문서번호 +1
		}
		if (in.eof())                       // 파일 끝이면 input 종료
		{
			break;
		}
	}
	in.close();                             // 스트림 닫기

	/*
	else
	   std::cout << "파일을 찾을 수 없습니다!" << endl;
	*/
}

void insert(string word_t, int doc_t, int seq_t) // map 1, 구조체에 삽입
{
	struct info *present = new info;
	struct info *scan; 

	if (dic1.count(word_t) == 0) // 처음 나온 단어일 경우(map indexing)
	{
		string word = word_t;   // 단어
		present->doc = doc_t;   // 구조체, 정보 1: document
		present->seq = seq_t;   // 구조체, 정보 2: sequence

		present->next = NULL;   // 구조체,  포인터
		dic1.insert(pair<string, info*>(word, present)); // map의 index에 단어를, 구조체에 doc, seq 정보를 삽입
	}
	else //처음 나온 단어가 아닐 때
	{
		map<string, info*>::iterator iter;          // map의 index를 확인할 iterator
		iter = dic1.find(word_t);                   // 삽입할 단어의 map indexing 위치를 받아옴
		if ((*iter).second->doc > doc_t)            // map에 있는 구조체와 비교 ->map 구조체에 삽입할 경우(swap이 필요한 경우)
		{
			present->next = (*iter).second->next;   // 현재 삽입할 구조체가 가리키는 곳은 원래 가리키던 곳
			(*iter).second->next = present;         // map 구조체 뒤에 현재 삽입할 구조체를 연결함

			/// sequence 순서가 뒤인 것을 뒤로 보내기
			present->doc = (*iter).second->doc;     // 현재 삽입한 구조체에는 map 구조체의 document를 넣음
			present->seq = (*iter).second->seq;     // 현재 삽입한 구조체에는 map 구조체의 sequence를 넣음

			(*iter).second->doc = doc_t;            // map 구조체의 document를 현재 삽입한 구조체에 넣음
			(*iter).second->seq = seq_t;            // map 구조체의 sequence를 현재 삽입한 구조체에 넣음
		}
		else // 새로운 document가 map 구조체 뒤의 링크드리스트(map 아닌 구조체)에 달릴 경우(swap이 필요 없는 삽입)
		{
			for (scan = (*iter).second; scan != NULL; scan = scan->next) //map 구조체에서 부터 scan이 시작함
			{
				/*cout << "in" << endl;
				cout << scan->next << endl;
				cout << (*iter).second->next << endl;
				cout << scan->doc << endl;
				cout << scan->seq << endl;*/

				if (scan->next == NULL) //순회를 다 하고, sequence가 가장 커서 맨 뒤에 삽입될 경우
				{
					///scan의 다음에 present를 삽입하는 과정
					present->next = scan->next;   //원래 scan이 가리키던 곳을 present가 가리키고
					scan->next = present;         //scan은 present를 가리킴

					present->doc = doc_t;         // doc 정보 
					present->seq = seq_t;         // seq 정보
					break;                        // 삽입하고 다시 for문 검사 안하고 break 하도록 함
				}
				else if (scan->next->doc > doc_t) // scan->next ==NULL인데, seq를 조회할 경우 오류나므로,
				{                                 // scan->next 조건문 먼저 검사 후에 검사함
					///scan의 다음에 present를 삽입하는 과정
					present->next = scan->next;   //원래 scan이 가리키던 곳을 present가 가리키고
					scan->next = present;         //scan은 present를 가리킴

					present->doc = doc_t;         // doc 정보 
					present->seq = seq_t;         // seq 정보
					break;                        // 삽입하고 다시 for문 검사 안하고 break 하도록 함
				}

			}
		}
	}
}

void traverse() //map1의 모든 멤버 출력
{
	map<string, info*>::iterator iter;        // map을 순회하기 위한 iterator
	struct info *scan;                        // scan으로 참조함, iterater를 수정하면 자료 구조가 붕괴될 수 있음

	int k = 1;                                // 단어의 순번을 출력하기 위함

	cout << "<Traverse> 모든 단어 목록을 출력합니다." << "\n" << endl;
	cout << "    『Word』순서   #  /  DOC    #" << endl;

	for (iter = dic1.begin(); iter != dic1.end(); iter++, k++) {           // map의 처음부터 끝까지 (index) 단어 목록

		cout << "--------------------------------------------" << endl;
		cout << "[" << k << "] " << "『" << (*iter).first << "』" << endl; // map1의 index 출력

		for (scan = (*iter).second; scan != NULL; scan = scan->next) {     // 단어 아래 모든 document, sequence들을 출력함, setw()는 폭 맞춰줌
			cout << "       Seq " << setw(4) << scan->seq << "  /  DOC " << setw(4) << scan->doc << endl;
			//cout << "next: " << (*iter).second->next << endl;
		}
	}
	cout << endl;
}