// ConsoleApplication7.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include <boost/intrusive/treap_set.hpp>
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
using namespace boost::intrusive;
//������������ ����� ������ � �������
enum NewspaperROW
{
	INDEX,
	NAME,
	FIOOFRED,
	COST,
	SURNAME
};
//����
enum Menu
{
	ADD,
	FIND,
	DELETE,
	SAVE,
	READASFILE,
	EXET,
};
//��������� ����� ������ � �������
struct newspapers {
	newspapers(){
		index = 0;
		name = "name";
		fioofred = "fioofred";
		cost = 0;
		surname = "surname";
	}
	newspapers(int index__, std::string name__) {
		index = index__;
		name = name__;
	}
	newspapers(int index__, std::string name__, std::string fioofred__, double_t cost__, std::string surname__) {
		index = index__;
		name = name__;
		fioofred = fioofred__;
		cost = cost__;
		surname = surname__;
	}
	int index =0;//������ �������
	std::string name ="";//�������� ������
	std::string fioofred = "";//��� ���������
	double_t cost = 0;//���� ������
	std::string surname = "";//������� ����������
	std::string to_string() {
		return (std::to_string(index) + " | " + name +" | "+ fioofred + " | " + std::to_string(cost) + " | " + surname);
	}
};
//�����-���� ������
class MyClass : public bs_set_base_hook<>
{
	int int_;//������ �������
	unsigned int prio_;//���
	newspapers newspaper_;//��������� POJO
public:
	bs_set_member_hook<> member_hook_;
	//MyClass(int i) : int_(i)
	//{}
	MyClass(int i) :int_(i)
	{}
	MyClass(int i, unsigned int prio) :int_(i), prio_(prio)
	{}
	MyClass(int i, unsigned int prio, newspapers newspaper) : int_(i), prio_(prio), newspaper_(newspaper)
	{}
	newspapers get_newspaper() const
	{
		return this->newspaper_;
	}
	unsigned int get_priority() const
	{
		return this->prio_;
	}
	friend bool operator< (const MyClass &a, const MyClass &b)
	{
		return a.int_ < b.int_;
	}
	friend bool operator> (const MyClass &a, const MyClass &b)
	{
		return a.int_ > b.int_;
	}
	friend bool priority_order(const MyClass &a, const MyClass &b)
	{
		return a.prio_ < b.prio_;
	}  //Lower value means higher priority
	friend bool priority_inverse_order(const MyClass &a, const MyClass &b)
	{
		return a.prio_ > b.prio_;
	}  //Higher value means higher priority
};
struct inverse_priority
{
	bool operator()(const MyClass &a, const MyClass &b) const
	{
		return priority_inverse_order(a, b);
	}
};
//��������� treap_set ��������� ������� ���, ������� ����� ������� �������� � �������� �������
typedef treap_set< MyClass, compare<std::greater<MyClass> > >     BaseSet;
//��������� ��������������� � ������� ���-��������, ������� ����� �������
typedef member_hook<MyClass, bs_set_member_hook<>, &MyClass::member_hook_> MemberOption;
typedef treap_multiset
< MyClass, MemberOption, priority<inverse_priority> > MemberMultiset;
//������ ������ � ������ ���������� ��������
static auto split(std::string s, std::string delimiter = " ") {
	size_t pos = 0;
	std::string* arr = new std::string[100];
	int i=0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		arr[i++] = s.substr(0, pos);
		s.erase(0, pos + delimiter.length());
	}
	arr[i] = s;
	return arr;
}
class MainManager
{
public:
	MainManager();
	~MainManager();
	//������� ���� ���������
	void Start();
	//������ �� ����� ���������
	void ReadAsFile();
	//�������� �� ������
	void Delete();
	//����� ��������
	void Find();
	//���������� ��������
	void Add();
	void Check();
private:
	MemberMultiset baseset;
	std::vector<MyClass> values;
};

MainManager::MainManager()
{
	Start();
}
void MainManager::Start() {
	bool isRun = true;
	while (isRun)
	{
		//std::cout.clear();
		//std::cin.clear();
		int switcher = -1;
		std::cout
			<< " 0 - ADD\n"
			<< " 1 - FIND\n"
			<< " 2 - DELETE\n"
			<< " 3 - SAVE\n"
			<< " 4 - READASFILE\n"
			<< " 5 - EXET\n";
		std::cin >> switcher;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		switch (switcher)
		{
		case ADD: {Add(); break; }
		case FIND: {Find(); break; }
		case DELETE: {Delete(); break; }
		case SAVE: {Check(); std::cout << " �� ��������, ��� ����������\n"; }break;
		case READASFILE: {ReadAsFile(); }break;
		case EXET: {isRun = false; }break;
		default:
			break;
		}
	}
}
void MainManager::ReadAsFile() {
	std::string fpath;
	//std::cin.clear();
	std::cout << "Print file path" << '\n';
	std::getline(std::cin, fpath);
	std::string line;
	std::ifstream file(fpath);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::string* sArr = split(line, ";");
			std::cout << (sArr[INDEX].c_str()) << '\n';
			try
			{
				values.push_back(
					MyClass(
						std::atoi(sArr[INDEX].c_str()),
						rand(),
						newspapers(
							std::atoi(sArr[INDEX].c_str()),
							sArr[NAME],
							sArr[FIOOFRED],
							std::atof(sArr[COST].c_str()),
							sArr[SURNAME])
					)
				);
				baseset.insert(values.back());
				std::cout << values.back().get_newspaper().to_string() << '\n';
			}
			catch (const std::exception&)
			{
				std::cout << "ERROR" << '\n';
			}
			
		}
		file.close();
	}
	else std::cout << "Unable to open file" << '\n';
}
void MainManager::Delete()
{
	int index;
	std::cout << "FindByIndex:" << '\n';
	std::cin >> index;
	try
	{
		baseset.erase(MyClass(index));
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR" << '\n';
	}
	
}
void MainManager::Find()
{
	int index=0;
	std::cout << "FindByIndex:" << '\n';
	std::cin >> index;
	auto str = baseset.find(index);

	try
	{
		std::cout
			<< "Find: "
			<< (*str).get_newspaper().to_string()
			<< '\n';
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR" << '\n';
	}
}
void MainManager::Check() {	
}
void MainManager::Add() {
	std::string line;
	std::cout << "Add:" << '\n';
	std::getline(std::cin, line);
	std::string* sArr = split(line, ";");
	try
	{
		values.push_back(MyClass(
			std::atoi(sArr[INDEX].c_str()),
			rand(),
			newspapers(
				std::atoi(sArr[INDEX].c_str()),
				sArr[NAME],
				sArr[FIOOFRED],
				std::atof(sArr[COST].c_str()),
				sArr[SURNAME])
		));
		baseset.insert(values.back());
		std::cout << (sArr[INDEX].c_str()) << "ADD_NORNAL\n "<< values.back().get_newspaper().to_string() << '\n';
	}
	catch (const std::exception&)
	{
		std::cout << (sArr[INDEX].c_str()) << "ADD_ERROR"<< '\n';
	}
	
}
MainManager::~MainManager()
{
}

int main()
{	
	MainManager* mainManager = new MainManager();
	return 0; 
	//��� ���������� ����� �����, ����� �� ��������.
	/*
	typedef std::vector<MyClass>::iterator VectIt;
	std::vector<MyClass> values;

	MemberMultiset membermultiset;

	for (int i = 0; i < 100; ++i) {
	newspapers newspaper(i, (std::string)("112345"+ std::to_string(i)));
	values.push_back(MyClass(i, (i % 10), newspaper));
	}
	//Now insert them in the sets
	for (VectIt it(values.begin()), itend(values.end()); it != itend; ++it) {
		baseset.insert(*it);
		membermultiset.insert(*it);
		//newspapers newspaper(1, (std::string)("x11234567"));
		//auto qw = new MyClass(2, (2 % 10), newspaper);
		
		//std::printf("%s", baseset.find(*it)->newspaper_.name.c_str());
		//std::printf("%s", it->newspaper_.name.c_str());
	}
	for (int i = 0; i < 100; ++i) {
		std::printf("%s \n", (*baseset.find(MyClass(i))).get_newspaper().to_string());
	}
	//Now test treap_sets
	{
		BaseSet::reverse_iterator rbit(baseset.rbegin()), rbitend(baseset.rend());
		MemberMultiset::iterator mit(membermultiset.begin()), mitend(membermultiset.end());
		VectIt it(values.begin()), itend(values.end());

		//Test the objects inserted in the base hook treap_set
		for (; it != itend; ++it, ++rbit)
			if (&*rbit != &*it)   return 1;

		//Test the objects inserted in the member hook treap_set
		for (it = values.begin(); it != itend; ++it, ++mit)
			if (&*mit != &*it) return 1;

		//Test priority order
		for (int i = 0; i < 100; ++i) {
			if (baseset.top()->get_priority() != static_cast<unsigned int>(i / 10))
				return 1;
			if (membermultiset.top()->get_priority() != 9u - static_cast<unsigned int>(i / 10))
				return 1;
			baseset.erase(baseset.top());
			membermultiset.erase(membermultiset.top());
		}
	}*/
}

