/*
�ڷᱸ�� �� �˰��� ��������Ʈ: �˻��� - ��������
21611663 ������ 21611591 �賭��
*/
//#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "winmm.lib")   // �ð� ������ ���� ���̺귯�� ����
#include <iostream>

///making hash table
#include <map> 
#include <vector>
#include <string>

///����� ����
#include <iomanip> // cout ���� ����� ����(setw)�� ����

///getting file contents
#include<fstream>

///searching
#include <sstream> //string stream
#include <string>  // ������� ���� string �Է¹ޱ�

///ȭ�� Ŭ���� & �ð�����
#include <windows.h>

using namespace std;

///making hash table - map1
typedef struct info {
	short unsigned int doc;  // document id
	short unsigned int seq;  // �ܾ� ��ġ(����)
	struct info *next;       // ���� struct info�� ����ų (��۴��) ����ü ������
}info;

///searching table - map3
typedef struct doccountinfo {  //map3�� ���� ����ü
	short unsigned int doc;    // document id, scoring ���ϹǷ� �Ⱦ�, semi scoring
	short unsigned int var;    // �л�
	struct doccountinfo *next; // ���� struct info�� ����ų (��۴��) ����ü ������
}doccountinfo;

typedef map<string, info*> _map;		// �ؽ�Ʈ���Ϸ� ���� ������ �ܾ� map(���: doc ��ȣ, seq ��ȣ)
_map dic1;

typedef map<int, vector<int>> _map2;	// �˻��� �ܾ ���� ������ doc ��ȣ map(���: seq ��ȣ)
_map2 dic2;

typedef map< int, doccountinfo*> _map3; // ���������� doc ���� ������ ������ doc ���� map(���: doc ��ȣ, seq �л�)
_map3 dic3;

///searching
string express[100];       // StringEngine()�� ���� ���� �ܾ� 100���� �Է� ���� //�������� �����ؼ� �� ����
string sameexpress[50];	   // ���� �ܾ� �˻��� ���� ���� �迭 ����

int k = 0;                 // �Է��� �ܾ� ������ŭ ���� -> �Է��� �ܾ� ������ŭ ī��Ʈ�� ����
int seq[10];               // �ܾ� 10�� ������� �ʱ�ȭ -> �л� ���ÿ� map2�� ��۴�� ���� �� ���Ϳ��� �л� ��� ���
int f1 = 0, f2 = 0;		   // ���� �ܾ� �и��� ���� flag -> ddengine���� ���

///map1 ���� �Լ�
void insert(string, int, int);    // map1�� �ܾ�(��۴�� ���: doc ��ȣ, seq ��ȣ) ���� �Լ�
void textinput(string);			  // text���Ͽ��� �о�� �ܾ�, DOC, DOC ������ ����(Sequence)�� �����ϴ� �Լ�
void traverse();                  // map1�� ��� ��� ���
int StringEngine();			      // �˻��⿡ �Է��� ���ڿ��� ó���ϴ� �Լ�

///map2 �����Լ�
void SearchEngine();			  // �ܾ� ���� ���θ� ���� map2�� ��۴�� �ޱ�
void semi_scoring();			  // map2(doc ��ȣ�� map, seq�� ��۴�� ����)�� �޸� ���͵��� �����ؼ� �л��� ����ϰ�, 
				    			  // map3�� �Ѱ��ִ� ����(map3�� ����ü(��ũ�帮��Ʈ)�� ����)	  
float var_seq(int[], int);		  // document ���ο� �ִ� �˻��� �ܾ���� �л� ����

///map3 ���� �Լ�
void insert_scoring(int, int, int);		// map3, ����ü�� ����
void dd_engine();						// searching ���� ��� ��� �Լ�
void textprint(string, int, string *);  // text�� ���� doc ��ȣ�� �ܾ �� ��� ��ġ�� ã�� line�� ����ϴ� �Լ�

int main() {
	//DWORD dwstart, dwend;			// ���� �ð�, ���ð�
	//DWORD dwElapsed;				// �ɸ� �ð�

	//dwstart = timeGetTime();		// ���� �ð� ����
	textinput("document.txt");
	//textinput("test.txt");
	//dwend = timeGetTime();	    // �� �ð� ����
	//dwElapsed = dwend - dwstart;	// �ɸ� �ð� ����
	//cout << "map 1: timeGetTime : " << dwElapsed * 0.001f << endl; // ���

	while (1)
	{
		//dwstart = timeGetTime();		
		int returnValue = StringEngine();			// �˻��⿡ �Է��� ���ڿ��� ó���ϴ� �Լ�
		//dwend = timeGetTime();			
		//dwElapsed = dwend - dwstart;	
		//cout << "�˻�: timeGetTime : " << dwElapsed * 0.001f << endl; 

		if (returnValue)
		{
			if (returnValue == 2)
			{
				system("cls"); //�ܼ�ȭ�� �����
				continue;
			}
			cout << "���α׷��� �����մϴ�.\n";
			return 0;
		}
		/*dwstart = timeGetTime();*/		

		SearchEngine();	 // �ܾ� ���� ���θ� ���� map2�� ��۴�� �ޱ�
		semi_scoring();	 // map2(doc ��ȣ�� map, seq�� ��۴�� ����)�� �޸� ���͵��� �����ؼ� �л��� ����ϰ�, map3�� �Ѱ���

		/*dwend = timeGetTime();			
		dwElapsed = dwend - dwstart;	
		cout << "map2~map3: timeGetTime : " << dwElapsed * 0.001f << endl; */

		//dwstart = timeGetTime();		

		dd_engine();	 // searching ���� ��� ��� �Լ�

		//dwend = timeGetTime();			
		//dwElapsed = dwend - dwstart;	
		//cout << "��� ���: timeGetTime : " << dwElapsed * 0.001f << endl; 

		dic2.clear();	 //map2 clear()
		dic3.clear();	 //map3 clear()
	}
}

void textprint(string textfile, int doc_num, string *word) // text�� ���� doc ��ȣ�� �ܾ �� ��� ��ġ�� ã�� line�� ����ϴ� �Լ�
{
	//��۴��(�л�,doc num ����� ����ü:map3)���� scan��
	string line, out;
	size_t fp;
	int i = 1;

	ifstream in(textfile);
	while (getline(in, line))
	{
		if (i == doc_num)				         //�� ���� doc_num ������
		{
			for (int p = 0; p < k; p++)          //�˻��� �ܾ� ������ŭ for loop
			//int po = line.substr(fp);
			//line.find(express[i]) != string::npos         
			{
				fp = line.find(word[p]);	     //doc �ȿ��� �˻��� �ܾ� ��ġ ã��
				if (fp == string::npos)          //�ش��ϴ� �ܾ ������ skip
				{
					continue;
				}
				out = line.substr(fp, 30);       //fp���� 30��ŭ �б�

				cout << "  ..." << out << endl;  // ���
				//cout << "......." << ch << "......." << endl;
				//���ڿ��� �ִ� ��ġ�� ���
				//�ѱ���� 2byte�� �ѱ��ڸ� �����ϴµ� 1byte�� ����� ��� ?�� ��
			}
			//line.substr(fp)�ϸ� ���ڿ��� ��µ�
			break;
		}
		i++;
	}
	in.close();
}

void dd_engine() // searching ���� ��� ��� �Լ�
{
	//DWORD dwstart, dwend;			// ���� �ð�, ���ð�
	//DWORD dwElapsed;				// �ɸ� �ð�

	//dwstart = timeGetTime();		// ���� �ð� ����
	
	//system("cls"); //�ܼ�ȭ�� �����
	map<int, doccountinfo*>::reverse_iterator iter; // map�� ��ȸ�ϱ� ���� iterator
	struct doccountinfo *scan;                      // scan���� ������, iterater�� �����ϸ� �ڷ� ������ �ر��� �� ����
	int i;											// �� ���� �� ���� ��������� �����ϴ� ����
	char key = 0, key1 = 0;							// key�� �˻��� ������� ���� ����, key1�� ����ܾ� �˻��� ���� ���� ����

	if (f1 == 1 && f2 == 1)							// �����ϴ� �ܾ�� �������� �ʴ� �ܾ �Բ� �˻��� ���
	{
		cout << "   ���� �ܾ ������ �Բ� ����Ͻðڽ��ϱ�? document �������� �ݿ����� �ʽ��ϴ�.(y/n) ";
		cin >> key1;
	}

	if (f2 == 1)									// �ܾ �����ϴ� ��� ������ ���
	{
		//cout << "   * * * * * * * DDONGDDONG ENGINE * * * * * * *" << "\n" << "   * * * * * * * ----------------- * * * * * * *" << '\n' << endl;
		cout << "  ...";
		for (i = 0; i < k; i++) 
		{
			cout << express[i] << ' ';				 // ��� �ܾ� ���
		}
		cout << "�� �˻���� �Դϴ�." << "\n" << endl;
		cout << "   #    ��Document��   �˻��� �ܾ��� �л� ����" << endl;
	}

	for (iter = dic3.rbegin(), i = 1; iter != dic3.rend(); ++iter) {        // map�� ������ ó������ (index) �ܾ� ���
		cout << "   -------------------------------------------" << endl;
		cout << "  ..." << (*iter).first << "���� �˻�� ���Ե� Document �˻� �� ..." << endl;
		for (scan = (*iter).second; scan != NULL; scan = scan->next, i++) { // �ܾ� �Ʒ� ��� document ���� �����, setw()�� �� ������
			cout << setw(4) << i << setw(11) << " ��" << setw(4) << scan->doc << "��   " << "         " << setw(13) << scan->var << endl;

			if (key1 == 'n')											    // ���� �ܾ ����� �ʿ䰡 ���� ���
				textprint("document.txt", scan->doc, sameexpress);
			//else cout << "����� �ܾ �����ϴ�." << endl;

			else                                                            // ���� �ܾ ����� �ʿ䰡 �ִ� ���
				textprint("document.txt", scan->doc, express);


			if (i % 15 == 0)												//15�� doc�� ���
			{	
				//dwend = timeGetTime();	    // �� �ð� ����
				//dwElapsed = dwend - dwstart;	// �ɸ� �ð� ����
				//cout << "��� ���: timeGetTime : " << dwElapsed * 0.001f << endl; // ���

				cout << "��� �˻��Ͻðڽ��ϱ�?(y/n) ";
				cin >> key;
				cout << endl;
				if (key == 'n') break;
			}
		}
		if (key == 'n') break;
	}
	cout << '\n' << '\n' << '\n' << endl;
}

void semi_scoring() // map2(doc ��ȣ�� map, seq�� ��۴�� ����)�� �޸� ���͵��� �����ؼ� �л��� ����ϰ�, 
{					// map3�� �Ѱ��ִ� ����
	map<int, vector<int>>::iterator iter;	// map�� iterator
	vector<int>::iterator vec;				// ������ iterator

	int i, var_t, doc_t;					//����, �л�, ���� ��ȣ

	for (iter = dic2.begin(); iter != dic2.end(); iter++) {	//map ����� ���� �� ������, index ������
		i = 0;								// ��� ��� �޸� ���� ������ ó������ ���� ����
		var_t = 0;							// ����ϴ� �л� �ʱ�ȭ

		//cout << "DOC " << (*iter).first << " >>> seq  ";

		for (vec = (*iter).second.begin(); vec != (*iter).second.end(); vec++, i++) {
			//cout << *vec << ' '; // ��۴�� seq ���� �� ����
			seq[i] = *vec;		   //�ϳ��� document ��ȣ �ȿ� �޸� ������ �迭�� ��� ���߿� �л� ����ϱ� ����
								   //(map�� index(���� ��ȣ)�� ���� �޸� ������ �ϳ��� ����(vec) 
		}

		//cout << '\n';
		//cout << "doc ���� " << i << endl;

		if (i > 1)						 //������ �ϳ� �̻��̸� �л��� ����Ѵ�
			var_t = var_seq(seq, i);

		doc_t = (*iter).first;	         //map�� index�� ������ȣ
		insert_scoring(i, doc_t, var_t); //����, ���� ��ȣ, �л��� map3�� �����Ѵ�.
	}
}

float var_seq(int data[], int count) // document ���ο� �ִ� �˻��� �ܾ���� �л� ����
{
	int i;							 // ������ ���� ���� ����
	int sum = 0;					 // ���� ����ϴ� ����

	for (i = 0; i < count; ++i)
		sum += data[i];              // ����

	float mean = sum / count;        // ���

	sum = 0.0;				         // ���� ������ �ٸ� �̿��� �ϱ� ���� �ʱ�ȭ

	///�л� ����ϱ�
	for (i = 0; i < count; ++i)
		sum += (data[i] - mean) * (data[i] - mean);

	sum /= (int)(count - 1);         // ���� �л�
	//cout <<"var: "<< sum <<endl;   // �л� ���

	return sum;						 // map2 �ϳ��� index���� ��� ��� �޸� vector���� ��ü �л�
}

void insert_scoring(int doc_count, int doc_t, int var_t) // map3, ����ü�� ����
{
	struct doccountinfo *present = new doccountinfo;
	struct doccountinfo *scan;

	if (dic3.count(doc_count) == 0) // ó�� ���� �ܾ��� ���(map indexing)
	{
		present->doc = doc_t;   // ����ü, ���� 1: document
		present->var = var_t;   // ����ü, ���� 2: sequence
		present->next = NULL;   // ����ü, ������
		dic3.insert(pair<int, doccountinfo*>(doc_count, present)); // map�� index�� �ܾ, ����ü�� doc, seq ������ ����
	}
	else //ó�� ���� �ܾ �ƴ� ��
	{
		map<int, doccountinfo*>::iterator iter;          // map�� index�� Ȯ���� iterator
		iter = dic3.find(doc_count);                     // ������ �ܾ��� map indexing ��ġ�� �޾ƿ�

		if ((*iter).second->var > var_t)
		{
			present->next = (*iter).second->next;   // ���� ������ ����ü�� ����Ű�� ���� ���� ����Ű�� ��
			(*iter).second->next = present;         // map ����ü �ڿ� ���� ������ ����ü�� ������

			/// sequence ������ ���� ���� �ڷ� ������
			present->doc = (*iter).second->doc;     // ���� ������ ����ü���� map ����ü�� document�� ����
			present->var = (*iter).second->var;     // ���� ������ ����ü���� map ����ü�� �л��� ����

			(*iter).second->doc = doc_t;            // map ����ü�� document�� ���� ������ ����ü�� ����
			(*iter).second->var = var_t;            // map ����ü�� �л��� ���� ������ ����ü�� ����
		}
		else // ���ο� document�� map ����ü ���� ��ũ�帮��Ʈ(map �ƴ� ����ü)�� �޸� ���(swap�� �ʿ� ���� ����)
		{
			for (scan = (*iter).second; scan != NULL; scan = scan->next) //map ����ü���� ���� scan�� ������
			{
				if (scan->next == NULL) //��ȸ�� �� �ϰ�, sequence�� ���� Ŀ�� �� �ڿ� ���Ե� ���
				{
					///scan�� ������ present�� �����ϴ� ����
					present->next = scan->next;   //���� scan�� ����Ű�� ���� present�� ����Ű��
					scan->next = present;         //scan�� present�� ����Ŵ

					present->doc = doc_t;         // doc ���� 
					present->var = var_t;         // �л� ����
					break;                        // �����ϰ� �ٽ� for�� �˻� ���ϰ� break �ϵ��� ��
				}
				else if (scan->next->var > var_t) // scan->next ==NULL�ε�, seq�� ��ȸ�� ��� �������Ƿ�,
				{                                 // scan->next ���ǹ� ���� �˻� �Ŀ� �˻���
					///scan�� ������ present�� �����ϴ� ����
					present->next = scan->next;   //���� scan�� ����Ű�� ���� present�� ����Ű��
					scan->next = present;         //scan�� present�� ����Ŵ

					present->doc = doc_t;         // doc ���� 
					present->var = var_t;         // �л� ����
					break;                        // �����ϰ� �ٽ� for�� �˻� ���ϰ� break �ϵ��� ��
				}

			}
		}
	}
}

void SearchEngine() // �ܾ� ���� ���θ� ���� map2�� ��۴�� �ޱ�
{
	map<string, info*>::iterator iter;     // map�� index�� Ȯ���� iterator
	struct info *scan;                     // scan���� ������, iterater�� �����ϸ� �ڷ� ������ �ر��� �� ����

	int si = 0;					           // sameexpress �迭�� ���� ����, sameexpress�� ���� �ܾ� �˻��� �ƴ� �� ���
	f1 = 0, f2 = 0;						   // ���� �ܾ� �˻��� �� �� �ʿ��� flag
	cout << '\n';

	for (int i = 0; i < k; i++)
	{
		iter = dic1.find(express[i]);    // ������ �ܾ��� map indexing ��ġ�� �޾ƿ�
		if (iter == dic1.end())          // �Է��� �ܾ �������� ���� ��
		{
			f1 = 1;						 // dd_engine���� ���� �ܾ �־� ����� �� ���� ��, f1==1, f2==2���� �˻�
			//cout << "[" << i << "] " << express[i] << endl;
			cout << " ��" << express[i] << "��" << " �ܾ �������� �ʽ��ϴ�." << endl;
			continue;
		}
		else {						     // �Է��� �ܾ ������ ��
			f2 = 1;						 // �ܾ ������ �� �ܾ ���� ������ ����ؾ��ϹǷ�, ����ϴ� flag ����
			sameexpress[si] = express[i];
			si++;
			//cout << "[" << i << "] " << express[i] << endl;
			cout << " ��" << (*iter).first << "��" << " �ܾ �����մϴ�." << endl;

			/// �ܾ� �Ʒ� ��� document ���� ����� //setw()�� �� ������
			for (scan = (*iter).second; scan != NULL; scan = scan->next) { 
			   //cout << "       Seq " << setw(4) << scan->seq << "  /  DOC " << setw(4) << scan->doc << endl;

				dic2[scan->doc].push_back(scan->seq); // _map2�� ��۴�� �ޱ�
			}
		}
	}
}

int StringEngine() // �˻��⿡ �Է��� ���ڿ� ó���ϴ� �Լ�
{
	string sentence;

	cout << "   * * * * * * * DDONGDDONG ENGINE * * * * * * *" << "\n" << "   * * * * * * * ----------------- * * * * * * *" << "      (���α׷� ����: Q, ȭ�� Ŭ����: C)" << endl;
	cout << "\n" << "  �˻�:  ";

	getline(cin, sentence);                 // ��� ���� ���� ���� �Է� �ޱ�
	//cout << sentence<<endl;
	//DWORD dwstart1, dwend1;			// ���� �ð�, ���ð�
	//DWORD dwElapsed1;				// �ɸ� �ð�

	//dwstart1 = timeGetTime();		// ���� �ð� ����
	
	stringstream stringEngine(sentence);    // ���ڿ� ó���ϴ� ��Ʈ��
	k = 0;

	while (stringEngine >> express[k])      // ���� ������ string �迭�� �Է�
	{
		if (express[k] == "Q")              // Q�� ������ ���α׷� ����
			return 1;
		if (express[k] == "C")				// C�� ������ ȭ�� Ŭ����
			return 2;
		//cout << express[k] << endl;       // �и��� �ܾ� ���
		k++;                                // �Է��� �ܾ� ������ŭ ����
	}
	//for (int i = 0; i < k; i++)           // ���ڿ� �и� ���ߴ��� �׽�Ʈ �ڵ�
	//   cout << "["<< i <<"] "<< express[i] << endl;
	
	stringEngine.clear();  // stringstream�� ����� ���ڿ��� clear������ ����
	// stringstream�� ����� ���ڿ��� �����ϰ� ������, stringstream ss; ss.str("")�� ����ϸ� �ȴ�.
    // clear�� �ǽ��� ��쿡��, ���ο� ���ڿ��� �޾��� �� ù ��ġ���� ������ �����ϴ�.

	//dwend1 = timeGetTime();	    // �� �ð� ����
	//dwElapsed1 = dwend1 - dwstart1;	// �ɸ� �ð� ����
	//cout << "�˻� ���ڿ� ó��: timeGetTime : " << dwElapsed1 * 0.001f << endl; // ���

	return 0;
}

void textinput(string textfile) // text���Ͽ��� �о�� �ܾ�, DOC, DOC ������ ����(Sequence)�� �����ϴ� �Լ� 
{
	string word;

	int count_seq = 1;  // ���� �ʱ�ȭ
	int count_doc = 1;  // doc ��ȣ �ʱ�ȭ

	ifstream in(textfile);

	while (in.is_open()) {
		in >> word;

		//cout << "�ܾ�: " << word << endl;
		//cout << "seq: " << count_seq << endl;
		//cout << "doc: " << count_doc << endl;

		insert(word, count_doc, count_seq); // �Է¹��� ���ڿ��� insert �Լ�(insert("�ܾ�", DOC, SEQ))�� �־ hasing table �����
		count_seq++;                        // �ܾ ������ ������ ����+1

		if (count_seq == 1001)              // document�� ������ �ܾ���� �� ������
		{
			count_seq = 1;                  // �ܾ ������ ������ ������ 1�� �ʱ�ȭ
			count_doc++;                    // ������ȣ +1
		}
		if (in.eof())                       // ���� ���̸� input ����
		{
			break;
		}
	}
	in.close();                             // ��Ʈ�� �ݱ�

	/*
	else
	   std::cout << "������ ã�� �� �����ϴ�!" << endl;
	*/
}

void insert(string word_t, int doc_t, int seq_t) // map 1, ����ü�� ����
{
	struct info *present = new info;
	struct info *scan; 

	if (dic1.count(word_t) == 0) // ó�� ���� �ܾ��� ���(map indexing)
	{
		string word = word_t;   // �ܾ�
		present->doc = doc_t;   // ����ü, ���� 1: document
		present->seq = seq_t;   // ����ü, ���� 2: sequence

		present->next = NULL;   // ����ü,  ������
		dic1.insert(pair<string, info*>(word, present)); // map�� index�� �ܾ, ����ü�� doc, seq ������ ����
	}
	else //ó�� ���� �ܾ �ƴ� ��
	{
		map<string, info*>::iterator iter;          // map�� index�� Ȯ���� iterator
		iter = dic1.find(word_t);                   // ������ �ܾ��� map indexing ��ġ�� �޾ƿ�
		if ((*iter).second->doc > doc_t)            // map�� �ִ� ����ü�� �� ->map ����ü�� ������ ���(swap�� �ʿ��� ���)
		{
			present->next = (*iter).second->next;   // ���� ������ ����ü�� ����Ű�� ���� ���� ����Ű�� ��
			(*iter).second->next = present;         // map ����ü �ڿ� ���� ������ ����ü�� ������

			/// sequence ������ ���� ���� �ڷ� ������
			present->doc = (*iter).second->doc;     // ���� ������ ����ü���� map ����ü�� document�� ����
			present->seq = (*iter).second->seq;     // ���� ������ ����ü���� map ����ü�� sequence�� ����

			(*iter).second->doc = doc_t;            // map ����ü�� document�� ���� ������ ����ü�� ����
			(*iter).second->seq = seq_t;            // map ����ü�� sequence�� ���� ������ ����ü�� ����
		}
		else // ���ο� document�� map ����ü ���� ��ũ�帮��Ʈ(map �ƴ� ����ü)�� �޸� ���(swap�� �ʿ� ���� ����)
		{
			for (scan = (*iter).second; scan != NULL; scan = scan->next) //map ����ü���� ���� scan�� ������
			{
				/*cout << "in" << endl;
				cout << scan->next << endl;
				cout << (*iter).second->next << endl;
				cout << scan->doc << endl;
				cout << scan->seq << endl;*/

				if (scan->next == NULL) //��ȸ�� �� �ϰ�, sequence�� ���� Ŀ�� �� �ڿ� ���Ե� ���
				{
					///scan�� ������ present�� �����ϴ� ����
					present->next = scan->next;   //���� scan�� ����Ű�� ���� present�� ����Ű��
					scan->next = present;         //scan�� present�� ����Ŵ

					present->doc = doc_t;         // doc ���� 
					present->seq = seq_t;         // seq ����
					break;                        // �����ϰ� �ٽ� for�� �˻� ���ϰ� break �ϵ��� ��
				}
				else if (scan->next->doc > doc_t) // scan->next ==NULL�ε�, seq�� ��ȸ�� ��� �������Ƿ�,
				{                                 // scan->next ���ǹ� ���� �˻� �Ŀ� �˻���
					///scan�� ������ present�� �����ϴ� ����
					present->next = scan->next;   //���� scan�� ����Ű�� ���� present�� ����Ű��
					scan->next = present;         //scan�� present�� ����Ŵ

					present->doc = doc_t;         // doc ���� 
					present->seq = seq_t;         // seq ����
					break;                        // �����ϰ� �ٽ� for�� �˻� ���ϰ� break �ϵ��� ��
				}

			}
		}
	}
}

void traverse() //map1�� ��� ��� ���
{
	map<string, info*>::iterator iter;        // map�� ��ȸ�ϱ� ���� iterator
	struct info *scan;                        // scan���� ������, iterater�� �����ϸ� �ڷ� ������ �ر��� �� ����

	int k = 1;                                // �ܾ��� ������ ����ϱ� ����

	cout << "<Traverse> ��� �ܾ� ����� ����մϴ�." << "\n" << endl;
	cout << "    ��Word������   #  /  DOC    #" << endl;

	for (iter = dic1.begin(); iter != dic1.end(); iter++, k++) {           // map�� ó������ ������ (index) �ܾ� ���

		cout << "--------------------------------------------" << endl;
		cout << "[" << k << "] " << "��" << (*iter).first << "��" << endl; // map1�� index ���

		for (scan = (*iter).second; scan != NULL; scan = scan->next) {     // �ܾ� �Ʒ� ��� document, sequence���� �����, setw()�� �� ������
			cout << "       Seq " << setw(4) << scan->seq << "  /  DOC " << setw(4) << scan->doc << endl;
			//cout << "next: " << (*iter).second->next << endl;
		}
	}
	cout << endl;
}