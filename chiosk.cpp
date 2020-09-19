#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <io.h>
#include <Windows.h>
#include <limits.h>
#include <sstream>

using namespace std;

// menu이름에 따른 가격과 만드는데 필요한 재료를 저장한 구조체
struct menu
{
	int price = 0;
	string ingre = "No Ingre";

	menu()
		: price(0), ingre("No Ingre") {}
	menu(int price, string ingre)
		: price(price), ingre(ingre) {}

};
int num_50000 = 200;
int num_10000 = 200;
int num_5000 = 200;
int num_1000 = 200;
long long int sum = 0; // 필요 없을거같음.

vector<string> ingre_name; // 재료 이름 저장. 
map<string, int> ingre_info; // 재료 정보 저장.
vector<string> menu_name; //메뉴이름 저장
map<string, menu> menu_info; // 메뉴 정보 저장

void load_data()
{
	// ingredients.txt 파일 읽기
	ifstream in("ingredients.txt");

	if (!in.is_open()) // 안열리면
	{
		return; // 종료
	}

	string temp_ingre_name; // 재료 이름 저장
	int ingre_count; // 재료 개수 저장
	int count = 0;
	while (1)
	{
		in >> temp_ingre_name;
		in >> ingre_count;
		//cin이 아니라 in(file 입출력 스트림임)

		if (in.fail()) //실패했다는건 제일 마지막이라는거까지 완료했다는것
		{
			break;
		}
		// 간장 30 이후에는 아무것도 없자나 그걸 실패로 간주
		ingre_name.push_back(temp_ingre_name);
		ingre_info.insert(pair<string, int>(temp_ingre_name, ingre_count)); //재료이름을 넣으면 재료개수가 보이는거임!
		// map 삽입 이유: 재료 이름을 삽입하면 재고 갯수 나오게

		/*
		ingredients.txt//전체선택 무엇
		닭 40
		양념 20
		간장 30

		닭 40 이렇게 먼저 들어오고
		각각 temp_ingre_name/ingre_count에 입력
		이걸 무한 반복
		*/
	}

	in.close(); // in에다가 다른 path를 넣기 위해 close()

	// chicken_menu 디렉토리에 있는 txt파일 내용 읽어서 menu에다가 저장.
	const string menu_dir_path = "./chicken_menu/*.txt";
	_finddata_t fd;//
	long handle = _findfirst(menu_dir_path.c_str(), &fd); // 처음 txt파일 읽음.
	//_findfirst라는 함수의 return값이 fd라는 파일 구조체에 저장 => 이걸 통해서 파일접근
	//handle: 오류값
	if (handle == -1)
	{
		return;
	}

	int result = 0;
	do
	{
		// fd.name ==> 파일 이름(ex : 간장치킨.txt)
		string menu_path = "./chicken_menu/"; //기본경로이고
		menu_path += fd.name; //이걸통해 뒤에 더 붙임

		in.open(menu_path);

		if (!in.is_open())
		{
			return;
		}

		string temp_menu_name = fd.name;
		// .txt부분 지우기(ex : 간장치킨.txt ==> 간장치킨)
		temp_menu_name.erase(temp_menu_name.length() - 4, temp_menu_name.length() - 1);

		int menu_price; // 각 메뉴 가격 저장
		string menu_ingre; // 각 메뉴를 만드는데 소모되는 재료 저장.
		while (1)
		{
			in >> menu_price;
			in >> menu_ingre;

			if (in.fail())
			{
				break;
			}

			menu_name.push_back(temp_menu_name);
			menu temp_menu = { menu_price, menu_ingre };
			menu_info.insert(pair<string, menu>(temp_menu_name, temp_menu));
		}
		result = _findnext(handle, &fd);

		in.close();
	} while (result != -1);

	_findclose(handle);
}

void create_menu_txt(const string& add_menu_name) {
	// 추가한 메뉴의 txt파일 생성
	ofstream out;
	string menu_path = "./chicken_menu/";

	menu_path += add_menu_name + ".txt";

	out.open(menu_path);
	out << menu_info[add_menu_name].price << " " << menu_info[add_menu_name].ingre << endl;
	menu_path = "./chicken_menu/";
	out.close();
}

void delete_menu_txt(const string& del_menu_name) {
	// 삭제한 메뉴의 txt파일 삭제하기
	string remove_path = "./chicken_menu/" + del_menu_name + ".txt";

	if (remove(remove_path.c_str()) != 0) {
		perror("Error deleting file!!");
	}
	else {
		cout << del_menu_name << "(이)가 삭제되었습니다." << endl;
	}
}

void update_ingre() { // 종료할 때 txt파일 갱신하기
   // ingredients.txt 파일 갱신
	ofstream out("ingredients.txt");

	for (int i = 0; i < ingre_name.size(); i++) {
		out << ingre_name[i] << " " << ingre_info[ingre_name[i]] << endl;
	}
	out.close();
}

int get_min(int a, int b) {
	if (a >= b) {
		return b;
	}
	else {
		return a;
	}
}

vector<string> tokenizer(const string& data, const char delimiter = ' ') {
	vector<string> result;
	string token;
	stringstream ss(data);

	while (getline(ss, token, delimiter)) {
		result.push_back(token);
	}

	return result;
}

void start_menu(string& temp_select)
{
	cout << "지금까지는 이런 키오스크가 없었다.(정말로)" << endl;
	cout << "이것은 치킨키오스크인가 치오스크키친인가" << endl;
	cout << "안녕하세요 CHIOSK입니다." << endl << endl;
	cout << "한번 시작해볼까요?" << endl;
	cout << "1. 관리자 \n2. 사용자 \n3. 종료" << endl << endl;
	cout << endl;


	while (1) {
		if (cin.fail()) {
			cin.ignore(256, '\n');
			continue;
		}
		cout << "어느 모드로 접근하시겠습니까? : ";
		getline(cin, temp_select);

		// 처음 시작 메뉴 입력에 대한 예외처리
		if (temp_select.length() == 1) {
			int temp_num = atoi(temp_select.c_str());
			if (temp_num >= 1 && temp_num <= 3) {
				break;
			}
			else {
				cout << "< 1 ~ 3사이의 정수를 입력하세요. >" << endl;
			}
		}
		else {
			cout << "< 1 ~ 3사이의 정수를 입력하세요. >" << endl;
		}
	}

	system("cls");
};

void menu_add()
{
	string add_menu_name; // 추가할 메뉴 이름을 저장하는 변수 ==> 20글자 이하

	cout << "========== 메뉴 추가 ==========" << endl;
	while (1) {
		cout << "추가할 메뉴 이름을 입력하세요 : ";
		getline(cin, add_menu_name);

		// 메뉴 추가할 때 메뉴 이름 입력에 대한 예외처리
		if (add_menu_name.length() > 40) {
			cout << "< 메뉴 이름은 최대 40바이트 입니다. >" << endl;
		}
		else if (add_menu_name.length() == 0) {  // 아무 문자 입력 안하고 엔터 누름.
			cout << "< 적어도 1개 이상의 문자를 입력하세요. >" << endl;
		}
		else if (add_menu_name == " ") {
			cout << "< 메뉴 이름에는 공백을 포함할 수 없습니다. >" << endl;
		}
		else if (add_menu_name[add_menu_name.length() - 1] == ' ') {
			cout << "< 재료 이름에는 공백을 포함할 수 없습니다. >" << endl;
		}
		else if (tokenizer(add_menu_name).size() > 1) {
			cout << "< 메뉴 이름에는 공백을 포함할 수 없습니다. >" << endl;
		}
		else { // 추가할 메뉴 이름이 20글자 이하이고, 공백이 없음.
			int menu_name_index =
				find(menu_name.begin(), menu_name.end(), add_menu_name) - menu_name.begin();
			// 메뉴가 존재하면 menu_name에서의 index 저장, 존재하지 않으면 menu_name.size()저장.

			if (menu_name_index >= 0 && menu_name_index < menu_name.size()) { // 이미 존재하는 메뉴
				cout << "< 이미 존재하는 메뉴입니다. >" << endl;
			}
			else { // 존재하지 않는 메뉴임 ==> 추가하기!
				break;
			}
		}
	}

	string add_menu_ingre;
	while (1) {
		cout << "메뉴제작에 필요한 재료를 입력하세요 : ";
		getline(cin, add_menu_ingre);

		// 메뉴 추가시 새로운 메뉴를 만드는데 필요한 재료 입력에 대한 예외처리
		if (add_menu_ingre.length() == 0) { // 아무 입력 없이 엔터키를 눌렀을 때
			cout << "< 적어도 1개 이상의 문자를 입력하세요. >" << endl;
		}
		else if (add_menu_ingre == " ") {
			cout << "< 재료 이름에는 공백을 포함할 수 없습니다. >" << endl;
		}
		else if (add_menu_ingre[add_menu_ingre.length() - 1] == ' ') {
			cout << "< 재료 이름에는 공백을 포함할 수 없습니다. >" << endl;
		}
		else if (tokenizer(add_menu_ingre).size() > 1) {
			cout << "< 재료 이름에는 공백을 포함할 수 없습니다. >" << endl;
		}
		else {
			int ingre_name_index =
				find(ingre_name.begin(), ingre_name.end(), add_menu_ingre) - ingre_name.begin();

			if (ingre_name_index >= 0 && ingre_name_index < ingre_name.size()) {
				cout << "< 이미 존재하는 재료입니다. >" << endl;
			}
			else {
				break;
			}
		}
	}

	// 추가할 메뉴 가격 입력에 대한 예외처리
	int add_price = -1;
	while (1) {
		cout << "추가할 메뉴의 가격을 입력하세요 : ";
		string temp_price;
		getline(cin, temp_price);

		if (tokenizer(temp_price).size() > 1) {
			cout << "< 가격에는 공백을 포함할 수 없습니다. >" << endl;
			continue;
		}
		else if (temp_price.length() == 0) {
			cout << "< 적어도 1000원 이상의 가격을 입력하세요. >" << endl;
		}
		else if (temp_price[temp_price.length() - 1] == ' ') {
			cout << "< 가격에는 공백을 포함해서는 안됩니다. >" << endl;
			continue;
		}
		else {
			bool is_only_num = true;
			for (int i = 0; i < temp_price.length(); i++) {
				if (temp_price[i] < '0' || temp_price[i] > '9') {
					if (temp_price[i] != '.' && temp_price[i] != '-') {
						cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			add_price = atoi(temp_price.c_str());

			// 2147483647보다 큰 수를 입력하면 add_price에 2147483647이 저장됨.
			// -2147483648보다 큰 수를 입력하면 add_price에 -2147483648이 저장됨.
			if (add_price < 0) {
				cout << "< 가격은 음수가 될 수 없습니다. >" << endl;
				add_price = 0;
			}
			else if (add_price == 0) {
				cout << "< 가격이 0원은 될 수 없습니다. >" << endl;
				add_price = 0;
			}
			else if (add_price >= INT_MAX) {
				cout << "< 2,147,483,647이상의 수는 입력할 수 없습니다. >" << endl;
				add_price = 0;
			}
			else if (add_price % 1000 != 0) {
				cout << "< 가격은 1000원 이상의 단위로 입력해야 합니다. >" << endl;
			}
			else {
				break;
			}
		}
	}

	ingre_name.push_back(add_menu_ingre);
	ingre_info.insert(pair<string, int>(add_menu_ingre, 0));
	menu_name.push_back(add_menu_name);
	menu add_temp_menu = { add_price, add_menu_ingre };
	menu_info.insert(pair<string, menu>(add_menu_name, add_temp_menu));

	create_menu_txt(add_menu_name);
	update_ingre();

	cout << "< 메뉴 추가가 완료되었습니다. >" << endl << endl;
	system("pause");
	system("cls");
};

void menu_delete()
{
	int choice_del = -1;

	cout << "========== 메뉴 삭제 ==========" << endl;
	cout << "< 현재 판매중인 메뉴는 다음과 같습니다.>" << endl;

	cout << "0. 이전화면" << endl;
	for (int i = 0; i < menu_name.size(); i++)
	{
		cout << i + 1 << ". " << menu_name[i] << endl;
	}
	cout << endl << endl;

	while (1) {
		cout << "삭제할 메뉴를 선택하세요 : ";
		string temp_del;
		int ace = -1;

		getline(cin, temp_del);
		if (tokenizer(temp_del).size() > 1) { //입력에 공백이 포함될 경우 차단
			cout << "입력은 공백을 허용하지 않습니다. 다시 입력하세요" << endl;
			cin.clear();
			continue;
		}
		// 삭제할 메뉴 입력에 대한 예외처리f
		int temp_num = atoi(temp_del.c_str());
		double temp_float = atof(temp_del.c_str());

		for (int i = 0; i < temp_del.size(); i++) {
			if (temp_del[i] == ' ') {
				cout << "입력은 공백을 허용하지 않습니다. 다시 입력하세요" << endl;
				ace = 1;
				break;
			}
		}
		if (temp_del.length() >= 1 && ace == -1) {
			if (temp_num >= 1 && temp_num <= menu_name.size() && (temp_float == temp_num) && (temp_del.find(".", 0) == string::npos)) { // 정상적인 입력임.
				choice_del = temp_num;
				choice_del--;
				break;
			}
			else if (temp_num == 0 && temp_del == "0") {
				system("cls");
				return;
			}

			if (temp_del.length() == 1) {
				if (temp_num == 0 && temp_del != "0" && temp_del != " ")
					cout << "< 0 ~ " << menu_name.size() << "사이의 정수를 입력하세요. >" << endl;
				else if (temp_num >= 1 && temp_num <= menu_name.size()) { // 정상적인 입력임.
					choice_del = temp_num;
					choice_del--;
					break;
				}
				else if (temp_num == 0 && temp_del == "0") {
					system("cls");
					return;
				}
				else {
					if (temp_del == " ")
						cout << "입력은 공백을 허용하지 않습니다. 다시 입력하세요" << endl;
					else
						cout << "< 0 ~ " << menu_name.size() << "사이의 정수를 입력하세요. >" << endl;

				}

			}
			else if (temp_num != atof(temp_del.c_str()) && (temp_num >= 1 && temp_num < menu_name.size() + 1)) {
				choice_del = temp_num;
				choice_del--;
				break;

			}
			else if (temp_num != atof(temp_del.c_str())) {
				cout << "< 0 ~ " << menu_name.size() << "사이의 정수를 입력하세요. >" << endl;
			}
			else if (temp_num == 0 && temp_del == "0 ")
				cout << "입력은 공백을 허용하지 않습니다. 다시 입력하세요" << endl;
			else if (temp_num < 0) {
				cout << "< 0 ~ " << menu_name.size() << "사이의 정수를 입력하세요. >" << endl;
			}
			else {

				if (temp_float == temp_num) cout << "< 0 ~ " << menu_name.size() << "사이의 정수를 입력하세요. >" << endl;
				else cout << "입력은 공백을 허용하지 않습니다. 다시 입력하세요" << endl;
			}
		}
		else if (ace == -1) {
			cout << "< 0 ~ " << menu_name.size() << "사이의 정수를 입력하세요. >" << endl;
		}
	}

	delete_menu_txt(menu_name[choice_del]);
	// update_ingre();

	int ingre_name_index =
		find(ingre_name.begin(), ingre_name.end(), menu_info[menu_name[choice_del]].ingre)
		- ingre_name.begin();
	ingre_info.erase(ingre_name[ingre_name_index]);
	ingre_name.erase(ingre_name.begin() + ingre_name_index);
	menu_info.erase(menu_name[choice_del]); // 참고 한 것 : http://www.cplusplus.com/reference/map/map/erase/
	menu_name.erase(menu_name.begin() + (choice_del)); // 참고 한 것 : https://inpages.tistory.com/134  

	update_ingre();

	cout << "< 바뀐 메뉴 >" << endl;
	for (int i = 0; i < menu_name.size(); i++)
	{
		cout << i + 1 << ". " << menu_name[i] << endl;
	}
	cout << endl << endl;

	system("pause");
	system("cls");

};

void menu_admin()
{
	string temp_admin_key = "";
	int menu_admin_key = -1;
	do
	{
		cout << "========== 메뉴 관리 ==========" << endl;
		cout << "< 수행할 작업을 선택해주세요 >" << endl;
		cout << "1. 메뉴 추가 \n2. 메뉴 삭제 \n0. 이전화면" << endl << endl;

		// 메뉴부분에서 어떤 작업을 할 지에 대한 입력 예외처리
		while (!cin.eof()) {
			cout << "메뉴를 선택해주세요: ";
			getline(cin, temp_admin_key);

			// 입력 예외처리
			if (cin.eof())
			{
				cin.clear();
				system("cls");
				break;
			}

			if (temp_admin_key.length() == 1) {
				int temp_num = temp_admin_key[0] - '0';

				if (temp_num == 1 || temp_num == 2 || temp_num == 0) {
					menu_admin_key = temp_num;
					break;
				}
				else {
					cout << "< 0 ~ 2 사이의 정수를 입력하세요. >" << endl;
				}
			}
			else {
				cout << "< 0 ~ 2 사이의 정수를 입력하세요. >" << endl;
			}
		}

		switch (menu_admin_key)
		{
		case 1:
			system("cls");
			cin.clear();
			menu_add();
			break;
		case 2:
			system("cls");
			cin.clear();
			menu_delete();
			break;
		}
	} while (menu_admin_key != 0);
	system("cls");
};


void ingredients_admin()
{
	char answer;
	bool isFirst = false;

	// 입력 예외처리
	do {
		cout << "========== 재고 관리 ==========" << endl;
		for (int i = 0; i < ingre_name.size(); i++) {
			cout << (i + 1) << ". " << ingre_name[i] << "\t" << ingre_info[ingre_name[i]] << endl;
		}
		cout << endl << endl;

		while (1) {
			cout << "재고 수량을 바꾸시겠습니까?(Y(y) / N(n)) : ";

			string temp_answer;
			getline(cin, temp_answer);//띄어쓰기까지 입력 받아서 구분하기 위해

			if (tokenizer(temp_answer).size() > 1) {
				cout << "< Y 또는 N을 입력하세요. >" << endl;
				cin.clear();
				continue;
			}
			if (temp_answer.length() == 1) {
				answer = toupper(temp_answer[0]);
				if (answer == 'Y' || answer == 'N') {
					break;
				}
				else {
					cout << "< Y 또는 N을 입력하세요. >" << endl;
					cin.clear();
				}
			}
			else {
				cout << "< Y 또는 N을 입력하세요. >" << endl;
				cin.clear();
			}
		}

		string ing_num;
		string ing_cnt;
		int count_num = 0; //num count(띄어쓰기 검사)
		int count_cnt = 0; //cnt count(띄어쓰기 검사)

		int cnt_dot = 0;
		switch (answer)
		{
		case 'Y':
			system("cls");
			cout << "========== 재고 관리 ==========" << endl;
			for (int i = 0; i < ingre_name.size(); i++) {
				cout << (i + 1) << ". " << ingre_name[i] << "\t" << ingre_info[ingre_name[i]] << endl;
			}
			cout << endl << endl;

			/*수정후2번*/

			while (1) {
				cout << "변경할 재고의 번호를 입력하세요." << endl;//재고 입력
				getline(cin, ing_num);

				if (ing_num.size() > 0) {
					for (int i = 0; i < ing_num.size(); i++) {
						if (!(ing_num[i] >= 49 && ing_num[i] <= 57)) {
							cout << "다시 입력하세요." << endl;
							count_num++;
							break;
						}
					}
				}
				else {
					cout << "다시 입력하세요." << endl;
					count_num++;
				}

				//cout << count;
				if (count_num != 0) {
					count_num = 0;
					continue;
				}
				else { //재고 입력에 성공했을 때
					if (stoi(ing_num) >= 1 && stoi(ing_num) <= ingre_name.size()) {
						cout << ingre_name[stoi(ing_num) - 1] + "를 선택했습니다." << endl;

						while (1) {//재고 수량 입력
							cout << "변경할 재고의 수량을 입력하세요." << endl;
							getline(cin, ing_cnt);
							if (ing_cnt.size() == 0) {
								cout << "다시 입력하세요." << endl;
								count_cnt++;
							}

							for (int i = 0; i < ing_cnt.size(); i++) {
								//cnt_dot
								if (ing_cnt[i] == '.') {//소수점일때 내림 하기 위함
									cnt_dot++;
									break;
								}
								else if (ing_cnt[i] == '-') {//음수 소수점일때 막기 위함
									count_cnt++;
									break;
								}
								else
									continue;

							}
							int dot;
							if (cnt_dot != 0) {
								dot = stoi(ing_cnt);//소수점이면 그냥 int로 바꿔서 소수점 이하 버림
								ing_cnt = to_string(dot);//다시 string으로
							}

							for (int i = 0; i < ing_cnt.size(); i++) {
								if (!(ing_cnt[i] >= 48 && ing_cnt[i] <= 57)) {//각 자리가 0에서부터 9까지가 아닐때
									cout << "다시 입력하세요." << endl;
									count_cnt++;
									break;
								}
							}
							if (count_cnt != 0) {
								count_cnt = 0;
								continue;
							}
							else {
								if (stoi(ing_cnt) < 0) {//음수일 때
									cout << "다시 입력하세요." << endl;
									cin.clear();
									continue;
								}
							}
							break;
						}
					}
					else {
						cout << "다시 입력하세요." << endl;
						continue;
					}
				}
				break;
			}

			ingre_info[ingre_name[stoi(ing_num) - 1]] = stoi(ing_cnt);
			update_ingre();

			cout << ingre_name[stoi(ing_num) - 1] << "의 수량이 " <<
				ingre_info[ingre_name[stoi(ing_num) - 1]] << "로 변경되었습니다." << endl;

			system("pause");
			system("cls");
			break;

		case 'N':
			break;

		}
	} while (answer != 'N');

	system("cls");
};

void mode_admin()
{
	string temp_admin_key;
	int admin_mode_key = -1;
	do
	{
		cout << "========== 운영자 모드 ==========" << endl;
		cout << "< 수행할 작업을 선택해주세요 >" << endl;
		cout << "1. 메뉴 관리 \n2. 재고 관리 \n0. 이전화면" << endl << endl;

		while (!cin.eof()) {
			cout << "메뉴를 선택해주세요: ";
			getline(cin, temp_admin_key);
			

			// 입력 예외처리
			if (cin.eof())
			{
				cin.clear();
				system("cls");
				break;
			}
			if (temp_admin_key.length() == 1) {
				int temp_num = temp_admin_key[0] - '0';

				if (temp_num == 1 || temp_num == 2 || temp_num == 0) {
					admin_mode_key = temp_num;
					break;
				}
				else {
					cout << "< 0 ~ 2 사이의 정수를 입력하세요. >" << endl;
				}
			}
			else {
				cout << "< 0 ~ 2 사이의 정수를 입력하세요. >" << endl;
			}
		}

		switch (admin_mode_key)
		{
		case 1:
			system("cls");
			cin.clear();
			menu_admin();
			break;
		case 2:
			system("cls");
			cin.clear();
			ingredients_admin();
			break;
		}
	} while (admin_mode_key != 0 && !cin.eof());
	system("cls");
	cin.clear();
};

void pay_by_cash(const int& price, bool& is_pay_success) {

	cout << "< 결제 금액은 " << price << "원 입니다. >" << endl;
	int left_price = price;
	int input_money = 0;
	int total_input_money = 0;

	while (left_price > 0) {
		cout << "투입 금액 : ";
		string temp_input_money;
		getline(cin, temp_input_money);

		if (temp_input_money.length() == 0) {
			cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
		}
		else {
			bool is_only_num = true;
			for (int i = 0; i < temp_input_money.length(); i++) {
				if (temp_input_money[i] < '0' || temp_input_money[i] > '9') {
					if (temp_input_money[i] != '.' && temp_input_money[i] != '-') {
						cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			input_money = atoi(temp_input_money.c_str());

			// 2147483647보다 큰 수를 입력하면 input_money에 2147483647이 저장됨.
			// -2147483648보다 큰 수를 입력하면 input_money에 -2147483648이 저장됨.
			if (input_money < 0) {
				cout << "< 결제금액은 음수가 될 수 없습니다. >" << endl;
				input_money = 0;
			}
			else if (input_money >= INT_MAX) {
				cout << "< 결제 금액은 INT_MAX를 초과할 수 없습니다. >" << endl;
				input_money = 0;
			}
			else if (input_money % 1000 != 0) {
				cout << "< 결제 금액은 천 원 단위 이상으로 입력해야합니다. >" << endl;
			}
			else {
				total_input_money += input_money;

				if (total_input_money >= price) { // 입력한 금액이 결제 금액과 같거나 클 때 
					int change = total_input_money - price; // (input_money - left_price)만큼 거스름돈

					// 지폐 순서 ==> 50000원 -> 10000원 -> 5000원 -> 1000원 순서대로 
					if (change >= 50000) {
						int out_num_50000 = change / 50000;

						if (out_num_50000 > num_50000) {
							cout << "< 결제 오류 : 키오스크 내 지폐 부족 >" << endl;
							is_pay_success = false;
							return;
						}
						else {
							num_50000 -= out_num_50000;
							change -= 50000 * out_num_50000;
						}

					}

					if (change >= 10000) {
						int out_num_10000 = change / 10000;

						if (out_num_10000 > num_10000) {
							cout << "< 결제 오류 : 키오스크 내 지폐 부족 >" << endl;
							is_pay_success = false;
							return;
						}
						else {
							num_10000 -= out_num_10000;
							change -= 10000 * out_num_10000;
						}
					}

					if (change >= 5000) {
						int out_num_5000 = change / 5000;

						if (out_num_5000 > num_5000) {
							cout << "< 결제 오류 : 키오스크 내 지폐 부족 >" << endl;
							is_pay_success = false;
							return;
						}
						else {
							num_5000 -= out_num_5000;
							change -= 5000 * out_num_5000;
						}
					}

					if (change >= 1000) {
						int out_num_1000 = change / 1000;

						if (out_num_1000 > num_1000) {
							cout << "< 결제 오류 : 키오스크 내 지폐 부족 >" << endl;
							is_pay_success = false;
							return;
						}
						else {
							num_1000 -= out_num_1000;
							change -= 1000 * out_num_1000;
						}
					}

					int change_to_print = total_input_money - price;
					if (change_to_print == 0) {
						cout << "< 결제가 완료되었습니다. >" << endl;
					}
					cout << "< 거스름돈 : " << change_to_print << "원 >" << endl;
					break;
				}
				else { // 입력한 금액이 결제 금액보다 작을 때 ==> (left_price - input_money)만큼  
					left_price = price - total_input_money;

					cout << "< " << left_price << "원을 더 입력해야합니다. >" << endl;
				}
			}
		}
	}
}


void pay_by_card(const int& price, bool& is_pay_success) {
	cout << "< " << price << "원이 결제 완료 되었습니다. >" << endl;

	return;
}

void mode_user()
{
	int used_chicken_count = 0; // 몇 마리를 주문했나 저장 ==> 주문 취소할 때 닭 개수 복구할 때 사용
	vector<pair<string, int>> used_ingre_count; // 어떤 재료를 얼만큼 사용했는지 저장 ==> 주문 취소할 때 재료 개수 복구시 사용

	int user_select = 0; // 결제/추가주문/주문취소 여부를 저장.
	long long int price = 0; // 결제 금액

	while (1) {
		cout << "============ 메뉴 주문 ============" << endl;

		if (menu_name.size() == 0) {
			cout << "주문할 메뉴가 없습니다" << endl;
			Sleep(2000);
			system("cls");
			return;
		}
		else {
			// 판매중인 메뉴이름과 가격 출력
			for (int i = 0; i < menu_name.size(); i++) {
				cout << (i + 1) << ". " << menu_name[i] << "\t" <<
					menu_info[menu_name[i]].price << "원" << endl;
			}
		}

		cout << endl << endl;

		int menu_num, menu_count; // menu_num은 메뉴 번호, menu_count는 주문할 수량

		// 사용자 주문 예외처리
		while (1) {
			cout << "주문할 메뉴를 입력하세요 : ";
			string temp_menu_num;
			getline(cin, temp_menu_num);

			if (temp_menu_num.length() == 0) {
				cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
				continue;
			}

			// 입력에 ,와 같이 0~9사이의 숫자가 아닌 것을 입력하는지 체크함.
			bool is_only_num = true;
			for (int i = 0; i < temp_menu_num.length(); i++) {
				if (temp_menu_num[i] < '0' || temp_menu_num[i] > '9') {
					if (temp_menu_num[i] != '-') {
						cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			// 2147483647보다 큰 수를 입력하면 add_price에 2147483647이 저장됨.
			// -2147483648보다 큰 수를 입력하면 add_price에 -2147483648이 저장됨.
			menu_num = atoi(temp_menu_num.c_str());

			// 메뉴판 번호에 없는 번호를 입력
			if (menu_num > menu_name.size() || menu_num < 1) {
				cout << "< 메뉴에 있는 번호를 골라주세요. >" << endl;
				menu_num = -1;
				menu_count = 0;
				continue;
			}
			// 주문한 메뉴의 재고가 0개이면 초기메뉴로 나감.
			if (get_min(ingre_info[menu_info[menu_name[menu_num - 1]].ingre], ingre_info["닭"]) == 0) {
				cout << "< 관리자 재고 목록 확인 요망 > " << endl;

				// 지금껏 주문에 의해 줄어든 재고 원상복구 시키기.
				ingre_info["닭"] += used_chicken_count;

				for (int i = 0; i < used_ingre_count.size(); i++) {
					ingre_info[used_ingre_count[i].first] += used_ingre_count[i].second;
				}

				system("pause");
				system("cls");
				return;
			}

			break;
		}

		// 주문할 수량 입력
		while (1) {
			cout << "주문할 수량을 입력하세요 : ";
			string temp_menu_count;
			getline(cin, temp_menu_count);

			if (temp_menu_count.length() == 0) {
				cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
				continue;
			}

			bool is_only_num = true;
			for (int i = 0; i < temp_menu_count.length(); i++) {
				if (temp_menu_count[i] < '0' || temp_menu_count[i] > '9') {
					if (temp_menu_count[i] != '-' && temp_menu_count[i] != '.') {
						cout << "< 0 ~ 9이외의 문자는 입력하실 수 없습니다. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			menu_count = atoi(temp_menu_count.c_str());

			if (menu_count < 0) { // 치킨 마리로 음수를 입력
				cout << "< 수량은 음수가 될 수 없습니다. >" << endl;
				//menu_num = -1;
				menu_count = 0;
			}
			else if (menu_count == 0) {
				cout << "< 0마리는 주문할 수 없습니다. >" << endl;
			}
			else if (menu_count >= INT_MAX) { // 메뉴의 개수를 입력할 때 int 오버플로우 발생.
				cout << "< 2,147,483,647이상의 수는 입력할 수 없습니다. >" << endl;
				//menu_num = -1;
				menu_count = 0;
			}
			else if (get_min(ingre_info[menu_info[menu_name[menu_num - 1]].ingre], ingre_info["닭"]) < menu_count) {
				// 치킨을 만들기 위한 재료가 부족
				cout << "< 메뉴를 만들기 위한 재료가 부족합니다. >" << endl;
				cout << "< 최대 " << get_min(ingre_info[menu_info[menu_name[menu_num - 1]].ingre], ingre_info["닭"]) <<
					"마리 주문가능 >" << endl;
			}
			else { // 모든 입력이 정상적임.
				cout << "\n" << menu_name[menu_num - 1] << "(을)를 " <<
					menu_count << "마리 주문했습니다.\n" << endl;
				cout << endl << endl;

				// 결제 가격 갱신
				price += menu_info[menu_name[menu_num - 1]].price * menu_count;

				// 주문한 수량만큼 재료 감소시키기.
				ingre_info["닭"] -= menu_count;
				ingre_info[menu_info[menu_name[menu_num - 1]].ingre] -= menu_count;

				used_chicken_count += menu_count;
				used_ingre_count.push_back(make_pair(menu_info[menu_name[menu_num - 1]].ingre, menu_count));

				break;
			}
		}

		cout << "< 수행할 작업을 선택해주세요 >" << endl;
		cout << "1. 결제 \n2. 추가 주문 \n3. 주문 취소(나가기)" << endl << endl;

		// 주문 후 수행할 작업 선택에 대한 예외처리
		while (1) {
			cout << "메뉴를 선택해주세요 : ";

			string temp_user_select;
			getline(cin, temp_user_select);

			if (temp_user_select.length() == 0) {
				cout << "< 1 ~ 3사이의 정수를 입력하세요. >" << endl;
				continue;
			}

			bool is_only_num = true;
			for (int i = 0; i < temp_user_select.length(); i++) {
				if (temp_user_select[i] < '0' || temp_user_select[i] > '9') {
					cout << "< 1 ~ 3사이의 정수를 입력하세요. >" << endl;
					is_only_num = false;
					break;
				}
			}
			if (is_only_num == false) {
				continue;
			}

			user_select = atoi(temp_user_select.c_str());
			if (user_select >= 1 && user_select <= 3) {
				break;
			}
			else {
				cout << "< 1 ~ 3사이의 정수를 입력하세요. >" << endl;
				user_select = 0;
			}

		}

		if (user_select == 1) { // 결제
			system("cls");
			cout << "============== 결제 =============" << endl;
			cout << "1. 현금결제" << endl << "2. 카드결제" << endl << endl;
			int pay_way = -1;
			const int CASH = 1;
			const int CARD = 2;

			while (1) {
				cout << "결제 방법을 선택하세요 : ";
				string temp_pay_way;
				getline(cin, temp_pay_way);

				if (temp_pay_way.length() == 0) {
					cout << "< 1 ~ 2사이의 정수를 입력하세요. >" << endl;
					continue;
				}

				bool is_only_num = true;
				for (int i = 0; i < temp_pay_way.length(); i++) {
					if (temp_pay_way[i] < '0' || temp_pay_way[i] > '9') {
						cout << "< 1 ~ 2사이의 정수를 입력하세요. >" << endl;
						is_only_num = false;
						break;
					}
				}
				if (is_only_num == false) {
					continue;
				}

				pay_way = atoi(temp_pay_way.c_str());
				if (pay_way >= 1 && pay_way <= 2) {
					break;
				}
				else {
					cout << "< 1 ~ 2사이의 정수를 입력하세요. >" << endl;
					user_select = 0;
				}
			}

			bool is_pay_success = true;
			switch (pay_way) {
			case CASH:
				pay_by_cash(price, is_pay_success);
				break;

			case CARD:
				pay_by_card(price, is_pay_success);
				break;
			}

			if (!is_pay_success) {
				// 키오스크 내의 지폐가 부족해서 결제에 실패하면 재료들 원상복구하기.
				ingre_info["닭"] += used_chicken_count;

				for (int i = 0; i < used_ingre_count.size(); i++) {
					ingre_info[used_ingre_count[i].first] += used_ingre_count[i].second;
				}
			}


			system("pause");
			system("cls");
			break;
		}
		else if (user_select == 2) { // 추가 주문
			system("cls");
		}
		else { // 그동안 했던 주문 취소. 감소했던 재료들도 원상복구하기
			cout << "주문이 취소되었습니다." << endl;
			price = 0;
			ingre_info["닭"] += used_chicken_count;

			for (int i = 0; i < used_ingre_count.size(); i++) {
				ingre_info[used_ingre_count[i].first] += used_ingre_count[i].second;
			}

			used_chicken_count = 0;
			Sleep(1000);
			system("cls");
			break;
		}
	}
}

int main()
{
	load_data();

	string temp_select = "";
	int select_menu = 0;
	do
	{
		start_menu(temp_select);
		select_menu = atoi(temp_select.c_str());
		switch (select_menu)
		{
		case 1:
			mode_admin();
			break;
		case 2:
			mode_user();
			break;
		case 3:
			update_ingre();
			break;
		}
	} while (select_menu != 3);

	cout << "안녕히계세요. CHIOSK였습니다" << endl << endl;
	return 0;
}