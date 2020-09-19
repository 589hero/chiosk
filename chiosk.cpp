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

// menu�̸��� ���� ���ݰ� ����µ� �ʿ��� ��Ḧ ������ ����ü
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
long long int sum = 0; // �ʿ� �����Ű���.

vector<string> ingre_name; // ��� �̸� ����. 
map<string, int> ingre_info; // ��� ���� ����.
vector<string> menu_name; //�޴��̸� ����
map<string, menu> menu_info; // �޴� ���� ����

void load_data()
{
	// ingredients.txt ���� �б�
	ifstream in("ingredients.txt");

	if (!in.is_open()) // �ȿ�����
	{
		return; // ����
	}

	string temp_ingre_name; // ��� �̸� ����
	int ingre_count; // ��� ���� ����
	int count = 0;
	while (1)
	{
		in >> temp_ingre_name;
		in >> ingre_count;
		//cin�� �ƴ϶� in(file ����� ��Ʈ����)

		if (in.fail()) //�����ߴٴ°� ���� �������̶�°ű��� �Ϸ��ߴٴ°�
		{
			break;
		}
		// ���� 30 ���Ŀ��� �ƹ��͵� ���ڳ� �װ� ���з� ����
		ingre_name.push_back(temp_ingre_name);
		ingre_info.insert(pair<string, int>(temp_ingre_name, ingre_count)); //����̸��� ������ ��ᰳ���� ���̴°���!
		// map ���� ����: ��� �̸��� �����ϸ� ��� ���� ������

		/*
		ingredients.txt//��ü���� ����
		�� 40
		��� 20
		���� 30

		�� 40 �̷��� ���� ������
		���� temp_ingre_name/ingre_count�� �Է�
		�̰� ���� �ݺ�
		*/
	}

	in.close(); // in���ٰ� �ٸ� path�� �ֱ� ���� close()

	// chicken_menu ���丮�� �ִ� txt���� ���� �о menu���ٰ� ����.
	const string menu_dir_path = "./chicken_menu/*.txt";
	_finddata_t fd;//
	long handle = _findfirst(menu_dir_path.c_str(), &fd); // ó�� txt���� ����.
	//_findfirst��� �Լ��� return���� fd��� ���� ����ü�� ���� => �̰� ���ؼ� ��������
	//handle: ������
	if (handle == -1)
	{
		return;
	}

	int result = 0;
	do
	{
		// fd.name ==> ���� �̸�(ex : ����ġŲ.txt)
		string menu_path = "./chicken_menu/"; //�⺻����̰�
		menu_path += fd.name; //�̰����� �ڿ� �� ����

		in.open(menu_path);

		if (!in.is_open())
		{
			return;
		}

		string temp_menu_name = fd.name;
		// .txt�κ� �����(ex : ����ġŲ.txt ==> ����ġŲ)
		temp_menu_name.erase(temp_menu_name.length() - 4, temp_menu_name.length() - 1);

		int menu_price; // �� �޴� ���� ����
		string menu_ingre; // �� �޴��� ����µ� �Ҹ�Ǵ� ��� ����.
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
	// �߰��� �޴��� txt���� ����
	ofstream out;
	string menu_path = "./chicken_menu/";

	menu_path += add_menu_name + ".txt";

	out.open(menu_path);
	out << menu_info[add_menu_name].price << " " << menu_info[add_menu_name].ingre << endl;
	menu_path = "./chicken_menu/";
	out.close();
}

void delete_menu_txt(const string& del_menu_name) {
	// ������ �޴��� txt���� �����ϱ�
	string remove_path = "./chicken_menu/" + del_menu_name + ".txt";

	if (remove(remove_path.c_str()) != 0) {
		perror("Error deleting file!!");
	}
	else {
		cout << del_menu_name << "(��)�� �����Ǿ����ϴ�." << endl;
	}
}

void update_ingre() { // ������ �� txt���� �����ϱ�
   // ingredients.txt ���� ����
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
	cout << "���ݱ����� �̷� Ű����ũ�� ������.(������)" << endl;
	cout << "�̰��� ġŲŰ����ũ�ΰ� ġ����ũŰģ�ΰ�" << endl;
	cout << "�ȳ��ϼ��� CHIOSK�Դϴ�." << endl << endl;
	cout << "�ѹ� �����غ����?" << endl;
	cout << "1. ������ \n2. ����� \n3. ����" << endl << endl;
	cout << endl;


	while (1) {
		if (cin.fail()) {
			cin.ignore(256, '\n');
			continue;
		}
		cout << "��� ���� �����Ͻðڽ��ϱ�? : ";
		getline(cin, temp_select);

		// ó�� ���� �޴� �Է¿� ���� ����ó��
		if (temp_select.length() == 1) {
			int temp_num = atoi(temp_select.c_str());
			if (temp_num >= 1 && temp_num <= 3) {
				break;
			}
			else {
				cout << "< 1 ~ 3������ ������ �Է��ϼ���. >" << endl;
			}
		}
		else {
			cout << "< 1 ~ 3������ ������ �Է��ϼ���. >" << endl;
		}
	}

	system("cls");
};

void menu_add()
{
	string add_menu_name; // �߰��� �޴� �̸��� �����ϴ� ���� ==> 20���� ����

	cout << "========== �޴� �߰� ==========" << endl;
	while (1) {
		cout << "�߰��� �޴� �̸��� �Է��ϼ��� : ";
		getline(cin, add_menu_name);

		// �޴� �߰��� �� �޴� �̸� �Է¿� ���� ����ó��
		if (add_menu_name.length() > 40) {
			cout << "< �޴� �̸��� �ִ� 40����Ʈ �Դϴ�. >" << endl;
		}
		else if (add_menu_name.length() == 0) {  // �ƹ� ���� �Է� ���ϰ� ���� ����.
			cout << "< ��� 1�� �̻��� ���ڸ� �Է��ϼ���. >" << endl;
		}
		else if (add_menu_name == " ") {
			cout << "< �޴� �̸����� ������ ������ �� �����ϴ�. >" << endl;
		}
		else if (add_menu_name[add_menu_name.length() - 1] == ' ') {
			cout << "< ��� �̸����� ������ ������ �� �����ϴ�. >" << endl;
		}
		else if (tokenizer(add_menu_name).size() > 1) {
			cout << "< �޴� �̸����� ������ ������ �� �����ϴ�. >" << endl;
		}
		else { // �߰��� �޴� �̸��� 20���� �����̰�, ������ ����.
			int menu_name_index =
				find(menu_name.begin(), menu_name.end(), add_menu_name) - menu_name.begin();
			// �޴��� �����ϸ� menu_name������ index ����, �������� ������ menu_name.size()����.

			if (menu_name_index >= 0 && menu_name_index < menu_name.size()) { // �̹� �����ϴ� �޴�
				cout << "< �̹� �����ϴ� �޴��Դϴ�. >" << endl;
			}
			else { // �������� �ʴ� �޴��� ==> �߰��ϱ�!
				break;
			}
		}
	}

	string add_menu_ingre;
	while (1) {
		cout << "�޴����ۿ� �ʿ��� ��Ḧ �Է��ϼ��� : ";
		getline(cin, add_menu_ingre);

		// �޴� �߰��� ���ο� �޴��� ����µ� �ʿ��� ��� �Է¿� ���� ����ó��
		if (add_menu_ingre.length() == 0) { // �ƹ� �Է� ���� ����Ű�� ������ ��
			cout << "< ��� 1�� �̻��� ���ڸ� �Է��ϼ���. >" << endl;
		}
		else if (add_menu_ingre == " ") {
			cout << "< ��� �̸����� ������ ������ �� �����ϴ�. >" << endl;
		}
		else if (add_menu_ingre[add_menu_ingre.length() - 1] == ' ') {
			cout << "< ��� �̸����� ������ ������ �� �����ϴ�. >" << endl;
		}
		else if (tokenizer(add_menu_ingre).size() > 1) {
			cout << "< ��� �̸����� ������ ������ �� �����ϴ�. >" << endl;
		}
		else {
			int ingre_name_index =
				find(ingre_name.begin(), ingre_name.end(), add_menu_ingre) - ingre_name.begin();

			if (ingre_name_index >= 0 && ingre_name_index < ingre_name.size()) {
				cout << "< �̹� �����ϴ� ����Դϴ�. >" << endl;
			}
			else {
				break;
			}
		}
	}

	// �߰��� �޴� ���� �Է¿� ���� ����ó��
	int add_price = -1;
	while (1) {
		cout << "�߰��� �޴��� ������ �Է��ϼ��� : ";
		string temp_price;
		getline(cin, temp_price);

		if (tokenizer(temp_price).size() > 1) {
			cout << "< ���ݿ��� ������ ������ �� �����ϴ�. >" << endl;
			continue;
		}
		else if (temp_price.length() == 0) {
			cout << "< ��� 1000�� �̻��� ������ �Է��ϼ���. >" << endl;
		}
		else if (temp_price[temp_price.length() - 1] == ' ') {
			cout << "< ���ݿ��� ������ �����ؼ��� �ȵ˴ϴ�. >" << endl;
			continue;
		}
		else {
			bool is_only_num = true;
			for (int i = 0; i < temp_price.length(); i++) {
				if (temp_price[i] < '0' || temp_price[i] > '9') {
					if (temp_price[i] != '.' && temp_price[i] != '-') {
						cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			add_price = atoi(temp_price.c_str());

			// 2147483647���� ū ���� �Է��ϸ� add_price�� 2147483647�� �����.
			// -2147483648���� ū ���� �Է��ϸ� add_price�� -2147483648�� �����.
			if (add_price < 0) {
				cout << "< ������ ������ �� �� �����ϴ�. >" << endl;
				add_price = 0;
			}
			else if (add_price == 0) {
				cout << "< ������ 0���� �� �� �����ϴ�. >" << endl;
				add_price = 0;
			}
			else if (add_price >= INT_MAX) {
				cout << "< 2,147,483,647�̻��� ���� �Է��� �� �����ϴ�. >" << endl;
				add_price = 0;
			}
			else if (add_price % 1000 != 0) {
				cout << "< ������ 1000�� �̻��� ������ �Է��ؾ� �մϴ�. >" << endl;
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

	cout << "< �޴� �߰��� �Ϸ�Ǿ����ϴ�. >" << endl << endl;
	system("pause");
	system("cls");
};

void menu_delete()
{
	int choice_del = -1;

	cout << "========== �޴� ���� ==========" << endl;
	cout << "< ���� �Ǹ����� �޴��� ������ �����ϴ�.>" << endl;

	cout << "0. ����ȭ��" << endl;
	for (int i = 0; i < menu_name.size(); i++)
	{
		cout << i + 1 << ". " << menu_name[i] << endl;
	}
	cout << endl << endl;

	while (1) {
		cout << "������ �޴��� �����ϼ��� : ";
		string temp_del;
		int ace = -1;

		getline(cin, temp_del);
		if (tokenizer(temp_del).size() > 1) { //�Է¿� ������ ���Ե� ��� ����
			cout << "�Է��� ������ ������� �ʽ��ϴ�. �ٽ� �Է��ϼ���" << endl;
			cin.clear();
			continue;
		}
		// ������ �޴� �Է¿� ���� ����ó��f
		int temp_num = atoi(temp_del.c_str());
		double temp_float = atof(temp_del.c_str());

		for (int i = 0; i < temp_del.size(); i++) {
			if (temp_del[i] == ' ') {
				cout << "�Է��� ������ ������� �ʽ��ϴ�. �ٽ� �Է��ϼ���" << endl;
				ace = 1;
				break;
			}
		}
		if (temp_del.length() >= 1 && ace == -1) {
			if (temp_num >= 1 && temp_num <= menu_name.size() && (temp_float == temp_num) && (temp_del.find(".", 0) == string::npos)) { // �������� �Է���.
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
					cout << "< 0 ~ " << menu_name.size() << "������ ������ �Է��ϼ���. >" << endl;
				else if (temp_num >= 1 && temp_num <= menu_name.size()) { // �������� �Է���.
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
						cout << "�Է��� ������ ������� �ʽ��ϴ�. �ٽ� �Է��ϼ���" << endl;
					else
						cout << "< 0 ~ " << menu_name.size() << "������ ������ �Է��ϼ���. >" << endl;

				}

			}
			else if (temp_num != atof(temp_del.c_str()) && (temp_num >= 1 && temp_num < menu_name.size() + 1)) {
				choice_del = temp_num;
				choice_del--;
				break;

			}
			else if (temp_num != atof(temp_del.c_str())) {
				cout << "< 0 ~ " << menu_name.size() << "������ ������ �Է��ϼ���. >" << endl;
			}
			else if (temp_num == 0 && temp_del == "0 ")
				cout << "�Է��� ������ ������� �ʽ��ϴ�. �ٽ� �Է��ϼ���" << endl;
			else if (temp_num < 0) {
				cout << "< 0 ~ " << menu_name.size() << "������ ������ �Է��ϼ���. >" << endl;
			}
			else {

				if (temp_float == temp_num) cout << "< 0 ~ " << menu_name.size() << "������ ������ �Է��ϼ���. >" << endl;
				else cout << "�Է��� ������ ������� �ʽ��ϴ�. �ٽ� �Է��ϼ���" << endl;
			}
		}
		else if (ace == -1) {
			cout << "< 0 ~ " << menu_name.size() << "������ ������ �Է��ϼ���. >" << endl;
		}
	}

	delete_menu_txt(menu_name[choice_del]);
	// update_ingre();

	int ingre_name_index =
		find(ingre_name.begin(), ingre_name.end(), menu_info[menu_name[choice_del]].ingre)
		- ingre_name.begin();
	ingre_info.erase(ingre_name[ingre_name_index]);
	ingre_name.erase(ingre_name.begin() + ingre_name_index);
	menu_info.erase(menu_name[choice_del]); // ���� �� �� : http://www.cplusplus.com/reference/map/map/erase/
	menu_name.erase(menu_name.begin() + (choice_del)); // ���� �� �� : https://inpages.tistory.com/134  

	update_ingre();

	cout << "< �ٲ� �޴� >" << endl;
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
		cout << "========== �޴� ���� ==========" << endl;
		cout << "< ������ �۾��� �������ּ��� >" << endl;
		cout << "1. �޴� �߰� \n2. �޴� ���� \n0. ����ȭ��" << endl << endl;

		// �޴��κп��� � �۾��� �� ���� ���� �Է� ����ó��
		while (!cin.eof()) {
			cout << "�޴��� �������ּ���: ";
			getline(cin, temp_admin_key);

			// �Է� ����ó��
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
					cout << "< 0 ~ 2 ������ ������ �Է��ϼ���. >" << endl;
				}
			}
			else {
				cout << "< 0 ~ 2 ������ ������ �Է��ϼ���. >" << endl;
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

	// �Է� ����ó��
	do {
		cout << "========== ��� ���� ==========" << endl;
		for (int i = 0; i < ingre_name.size(); i++) {
			cout << (i + 1) << ". " << ingre_name[i] << "\t" << ingre_info[ingre_name[i]] << endl;
		}
		cout << endl << endl;

		while (1) {
			cout << "��� ������ �ٲٽðڽ��ϱ�?(Y(y) / N(n)) : ";

			string temp_answer;
			getline(cin, temp_answer);//������� �Է� �޾Ƽ� �����ϱ� ����

			if (tokenizer(temp_answer).size() > 1) {
				cout << "< Y �Ǵ� N�� �Է��ϼ���. >" << endl;
				cin.clear();
				continue;
			}
			if (temp_answer.length() == 1) {
				answer = toupper(temp_answer[0]);
				if (answer == 'Y' || answer == 'N') {
					break;
				}
				else {
					cout << "< Y �Ǵ� N�� �Է��ϼ���. >" << endl;
					cin.clear();
				}
			}
			else {
				cout << "< Y �Ǵ� N�� �Է��ϼ���. >" << endl;
				cin.clear();
			}
		}

		string ing_num;
		string ing_cnt;
		int count_num = 0; //num count(���� �˻�)
		int count_cnt = 0; //cnt count(���� �˻�)

		int cnt_dot = 0;
		switch (answer)
		{
		case 'Y':
			system("cls");
			cout << "========== ��� ���� ==========" << endl;
			for (int i = 0; i < ingre_name.size(); i++) {
				cout << (i + 1) << ". " << ingre_name[i] << "\t" << ingre_info[ingre_name[i]] << endl;
			}
			cout << endl << endl;

			/*������2��*/

			while (1) {
				cout << "������ ����� ��ȣ�� �Է��ϼ���." << endl;//��� �Է�
				getline(cin, ing_num);

				if (ing_num.size() > 0) {
					for (int i = 0; i < ing_num.size(); i++) {
						if (!(ing_num[i] >= 49 && ing_num[i] <= 57)) {
							cout << "�ٽ� �Է��ϼ���." << endl;
							count_num++;
							break;
						}
					}
				}
				else {
					cout << "�ٽ� �Է��ϼ���." << endl;
					count_num++;
				}

				//cout << count;
				if (count_num != 0) {
					count_num = 0;
					continue;
				}
				else { //��� �Է¿� �������� ��
					if (stoi(ing_num) >= 1 && stoi(ing_num) <= ingre_name.size()) {
						cout << ingre_name[stoi(ing_num) - 1] + "�� �����߽��ϴ�." << endl;

						while (1) {//��� ���� �Է�
							cout << "������ ����� ������ �Է��ϼ���." << endl;
							getline(cin, ing_cnt);
							if (ing_cnt.size() == 0) {
								cout << "�ٽ� �Է��ϼ���." << endl;
								count_cnt++;
							}

							for (int i = 0; i < ing_cnt.size(); i++) {
								//cnt_dot
								if (ing_cnt[i] == '.') {//�Ҽ����϶� ���� �ϱ� ����
									cnt_dot++;
									break;
								}
								else if (ing_cnt[i] == '-') {//���� �Ҽ����϶� ���� ����
									count_cnt++;
									break;
								}
								else
									continue;

							}
							int dot;
							if (cnt_dot != 0) {
								dot = stoi(ing_cnt);//�Ҽ����̸� �׳� int�� �ٲ㼭 �Ҽ��� ���� ����
								ing_cnt = to_string(dot);//�ٽ� string����
							}

							for (int i = 0; i < ing_cnt.size(); i++) {
								if (!(ing_cnt[i] >= 48 && ing_cnt[i] <= 57)) {//�� �ڸ��� 0�������� 9������ �ƴҶ�
									cout << "�ٽ� �Է��ϼ���." << endl;
									count_cnt++;
									break;
								}
							}
							if (count_cnt != 0) {
								count_cnt = 0;
								continue;
							}
							else {
								if (stoi(ing_cnt) < 0) {//������ ��
									cout << "�ٽ� �Է��ϼ���." << endl;
									cin.clear();
									continue;
								}
							}
							break;
						}
					}
					else {
						cout << "�ٽ� �Է��ϼ���." << endl;
						continue;
					}
				}
				break;
			}

			ingre_info[ingre_name[stoi(ing_num) - 1]] = stoi(ing_cnt);
			update_ingre();

			cout << ingre_name[stoi(ing_num) - 1] << "�� ������ " <<
				ingre_info[ingre_name[stoi(ing_num) - 1]] << "�� ����Ǿ����ϴ�." << endl;

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
		cout << "========== ��� ��� ==========" << endl;
		cout << "< ������ �۾��� �������ּ��� >" << endl;
		cout << "1. �޴� ���� \n2. ��� ���� \n0. ����ȭ��" << endl << endl;

		while (!cin.eof()) {
			cout << "�޴��� �������ּ���: ";
			getline(cin, temp_admin_key);
			

			// �Է� ����ó��
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
					cout << "< 0 ~ 2 ������ ������ �Է��ϼ���. >" << endl;
				}
			}
			else {
				cout << "< 0 ~ 2 ������ ������ �Է��ϼ���. >" << endl;
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

	cout << "< ���� �ݾ��� " << price << "�� �Դϴ�. >" << endl;
	int left_price = price;
	int input_money = 0;
	int total_input_money = 0;

	while (left_price > 0) {
		cout << "���� �ݾ� : ";
		string temp_input_money;
		getline(cin, temp_input_money);

		if (temp_input_money.length() == 0) {
			cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
		}
		else {
			bool is_only_num = true;
			for (int i = 0; i < temp_input_money.length(); i++) {
				if (temp_input_money[i] < '0' || temp_input_money[i] > '9') {
					if (temp_input_money[i] != '.' && temp_input_money[i] != '-') {
						cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			input_money = atoi(temp_input_money.c_str());

			// 2147483647���� ū ���� �Է��ϸ� input_money�� 2147483647�� �����.
			// -2147483648���� ū ���� �Է��ϸ� input_money�� -2147483648�� �����.
			if (input_money < 0) {
				cout << "< �����ݾ��� ������ �� �� �����ϴ�. >" << endl;
				input_money = 0;
			}
			else if (input_money >= INT_MAX) {
				cout << "< ���� �ݾ��� INT_MAX�� �ʰ��� �� �����ϴ�. >" << endl;
				input_money = 0;
			}
			else if (input_money % 1000 != 0) {
				cout << "< ���� �ݾ��� õ �� ���� �̻����� �Է��ؾ��մϴ�. >" << endl;
			}
			else {
				total_input_money += input_money;

				if (total_input_money >= price) { // �Է��� �ݾ��� ���� �ݾװ� ���ų� Ŭ �� 
					int change = total_input_money - price; // (input_money - left_price)��ŭ �Ž�����

					// ���� ���� ==> 50000�� -> 10000�� -> 5000�� -> 1000�� ������� 
					if (change >= 50000) {
						int out_num_50000 = change / 50000;

						if (out_num_50000 > num_50000) {
							cout << "< ���� ���� : Ű����ũ �� ���� ���� >" << endl;
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
							cout << "< ���� ���� : Ű����ũ �� ���� ���� >" << endl;
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
							cout << "< ���� ���� : Ű����ũ �� ���� ���� >" << endl;
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
							cout << "< ���� ���� : Ű����ũ �� ���� ���� >" << endl;
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
						cout << "< ������ �Ϸ�Ǿ����ϴ�. >" << endl;
					}
					cout << "< �Ž����� : " << change_to_print << "�� >" << endl;
					break;
				}
				else { // �Է��� �ݾ��� ���� �ݾ׺��� ���� �� ==> (left_price - input_money)��ŭ  
					left_price = price - total_input_money;

					cout << "< " << left_price << "���� �� �Է��ؾ��մϴ�. >" << endl;
				}
			}
		}
	}
}


void pay_by_card(const int& price, bool& is_pay_success) {
	cout << "< " << price << "���� ���� �Ϸ� �Ǿ����ϴ�. >" << endl;

	return;
}

void mode_user()
{
	int used_chicken_count = 0; // �� ������ �ֹ��߳� ���� ==> �ֹ� ����� �� �� ���� ������ �� ���
	vector<pair<string, int>> used_ingre_count; // � ��Ḧ ��ŭ ����ߴ��� ���� ==> �ֹ� ����� �� ��� ���� ������ ���

	int user_select = 0; // ����/�߰��ֹ�/�ֹ���� ���θ� ����.
	long long int price = 0; // ���� �ݾ�

	while (1) {
		cout << "============ �޴� �ֹ� ============" << endl;

		if (menu_name.size() == 0) {
			cout << "�ֹ��� �޴��� �����ϴ�" << endl;
			Sleep(2000);
			system("cls");
			return;
		}
		else {
			// �Ǹ����� �޴��̸��� ���� ���
			for (int i = 0; i < menu_name.size(); i++) {
				cout << (i + 1) << ". " << menu_name[i] << "\t" <<
					menu_info[menu_name[i]].price << "��" << endl;
			}
		}

		cout << endl << endl;

		int menu_num, menu_count; // menu_num�� �޴� ��ȣ, menu_count�� �ֹ��� ����

		// ����� �ֹ� ����ó��
		while (1) {
			cout << "�ֹ��� �޴��� �Է��ϼ��� : ";
			string temp_menu_num;
			getline(cin, temp_menu_num);

			if (temp_menu_num.length() == 0) {
				cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
				continue;
			}

			// �Է¿� ,�� ���� 0~9������ ���ڰ� �ƴ� ���� �Է��ϴ��� üũ��.
			bool is_only_num = true;
			for (int i = 0; i < temp_menu_num.length(); i++) {
				if (temp_menu_num[i] < '0' || temp_menu_num[i] > '9') {
					if (temp_menu_num[i] != '-') {
						cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			// 2147483647���� ū ���� �Է��ϸ� add_price�� 2147483647�� �����.
			// -2147483648���� ū ���� �Է��ϸ� add_price�� -2147483648�� �����.
			menu_num = atoi(temp_menu_num.c_str());

			// �޴��� ��ȣ�� ���� ��ȣ�� �Է�
			if (menu_num > menu_name.size() || menu_num < 1) {
				cout << "< �޴��� �ִ� ��ȣ�� ����ּ���. >" << endl;
				menu_num = -1;
				menu_count = 0;
				continue;
			}
			// �ֹ��� �޴��� ��� 0���̸� �ʱ�޴��� ����.
			if (get_min(ingre_info[menu_info[menu_name[menu_num - 1]].ingre], ingre_info["��"]) == 0) {
				cout << "< ������ ��� ��� Ȯ�� ��� > " << endl;

				// ���ݲ� �ֹ��� ���� �پ�� ��� ���󺹱� ��Ű��.
				ingre_info["��"] += used_chicken_count;

				for (int i = 0; i < used_ingre_count.size(); i++) {
					ingre_info[used_ingre_count[i].first] += used_ingre_count[i].second;
				}

				system("pause");
				system("cls");
				return;
			}

			break;
		}

		// �ֹ��� ���� �Է�
		while (1) {
			cout << "�ֹ��� ������ �Է��ϼ��� : ";
			string temp_menu_count;
			getline(cin, temp_menu_count);

			if (temp_menu_count.length() == 0) {
				cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
				continue;
			}

			bool is_only_num = true;
			for (int i = 0; i < temp_menu_count.length(); i++) {
				if (temp_menu_count[i] < '0' || temp_menu_count[i] > '9') {
					if (temp_menu_count[i] != '-' && temp_menu_count[i] != '.') {
						cout << "< 0 ~ 9�̿��� ���ڴ� �Է��Ͻ� �� �����ϴ�. >" << endl;
						is_only_num = false;
						break;
					}
				}
			}
			if (is_only_num == false) {
				continue;
			}

			menu_count = atoi(temp_menu_count.c_str());

			if (menu_count < 0) { // ġŲ ������ ������ �Է�
				cout << "< ������ ������ �� �� �����ϴ�. >" << endl;
				//menu_num = -1;
				menu_count = 0;
			}
			else if (menu_count == 0) {
				cout << "< 0������ �ֹ��� �� �����ϴ�. >" << endl;
			}
			else if (menu_count >= INT_MAX) { // �޴��� ������ �Է��� �� int �����÷ο� �߻�.
				cout << "< 2,147,483,647�̻��� ���� �Է��� �� �����ϴ�. >" << endl;
				//menu_num = -1;
				menu_count = 0;
			}
			else if (get_min(ingre_info[menu_info[menu_name[menu_num - 1]].ingre], ingre_info["��"]) < menu_count) {
				// ġŲ�� ����� ���� ��ᰡ ����
				cout << "< �޴��� ����� ���� ��ᰡ �����մϴ�. >" << endl;
				cout << "< �ִ� " << get_min(ingre_info[menu_info[menu_name[menu_num - 1]].ingre], ingre_info["��"]) <<
					"���� �ֹ����� >" << endl;
			}
			else { // ��� �Է��� ��������.
				cout << "\n" << menu_name[menu_num - 1] << "(��)�� " <<
					menu_count << "���� �ֹ��߽��ϴ�.\n" << endl;
				cout << endl << endl;

				// ���� ���� ����
				price += menu_info[menu_name[menu_num - 1]].price * menu_count;

				// �ֹ��� ������ŭ ��� ���ҽ�Ű��.
				ingre_info["��"] -= menu_count;
				ingre_info[menu_info[menu_name[menu_num - 1]].ingre] -= menu_count;

				used_chicken_count += menu_count;
				used_ingre_count.push_back(make_pair(menu_info[menu_name[menu_num - 1]].ingre, menu_count));

				break;
			}
		}

		cout << "< ������ �۾��� �������ּ��� >" << endl;
		cout << "1. ���� \n2. �߰� �ֹ� \n3. �ֹ� ���(������)" << endl << endl;

		// �ֹ� �� ������ �۾� ���ÿ� ���� ����ó��
		while (1) {
			cout << "�޴��� �������ּ��� : ";

			string temp_user_select;
			getline(cin, temp_user_select);

			if (temp_user_select.length() == 0) {
				cout << "< 1 ~ 3������ ������ �Է��ϼ���. >" << endl;
				continue;
			}

			bool is_only_num = true;
			for (int i = 0; i < temp_user_select.length(); i++) {
				if (temp_user_select[i] < '0' || temp_user_select[i] > '9') {
					cout << "< 1 ~ 3������ ������ �Է��ϼ���. >" << endl;
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
				cout << "< 1 ~ 3������ ������ �Է��ϼ���. >" << endl;
				user_select = 0;
			}

		}

		if (user_select == 1) { // ����
			system("cls");
			cout << "============== ���� =============" << endl;
			cout << "1. ���ݰ���" << endl << "2. ī�����" << endl << endl;
			int pay_way = -1;
			const int CASH = 1;
			const int CARD = 2;

			while (1) {
				cout << "���� ����� �����ϼ��� : ";
				string temp_pay_way;
				getline(cin, temp_pay_way);

				if (temp_pay_way.length() == 0) {
					cout << "< 1 ~ 2������ ������ �Է��ϼ���. >" << endl;
					continue;
				}

				bool is_only_num = true;
				for (int i = 0; i < temp_pay_way.length(); i++) {
					if (temp_pay_way[i] < '0' || temp_pay_way[i] > '9') {
						cout << "< 1 ~ 2������ ������ �Է��ϼ���. >" << endl;
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
					cout << "< 1 ~ 2������ ������ �Է��ϼ���. >" << endl;
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
				// Ű����ũ ���� ���� �����ؼ� ������ �����ϸ� ���� ���󺹱��ϱ�.
				ingre_info["��"] += used_chicken_count;

				for (int i = 0; i < used_ingre_count.size(); i++) {
					ingre_info[used_ingre_count[i].first] += used_ingre_count[i].second;
				}
			}


			system("pause");
			system("cls");
			break;
		}
		else if (user_select == 2) { // �߰� �ֹ�
			system("cls");
		}
		else { // �׵��� �ߴ� �ֹ� ���. �����ߴ� ���鵵 ���󺹱��ϱ�
			cout << "�ֹ��� ��ҵǾ����ϴ�." << endl;
			price = 0;
			ingre_info["��"] += used_chicken_count;

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

	cout << "�ȳ����輼��. CHIOSK�����ϴ�" << endl << endl;
	return 0;
}