//Standard Pre-Processors 
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<string>
#include<ctime>
#include<memory>

//SQL Preprocessors
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include<cppconn/resultset.h>
#include <mysql_connection.h>
#include<cppconn/prepared_statement.h>
#include <mysql_driver.h>
using namespace std;

struct MemberDetails {
	string name;
	string email;
	string phoneNumber;
	string paymentType;
	string memberStart;
};
vector<MemberDetails> members;

string getDate() {
	time_t now = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &now);
	char date[11];
	strftime(date, sizeof(date), "%Y-%m-%d", &localTime);
	return string(date);
}

void addMember(const MemberDetails& m) {
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* conn;

		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1", "cpp", "localpassword");
		conn->setSchema("members_database");
		unique_ptr<sql::PreparedStatement> prpdState(
			conn->prepareStatement("INSERT INTO member_details (Name, Email, PhoneNumber, PaymentType, MembershipAdded) VALUES (?,?,?,?,?)")
		);
		prpdState->setString(1, m.name);
		prpdState->setString(2, m.email);
		prpdState->setString(3, m.phoneNumber);
		prpdState->setString(4, m.paymentType);
		prpdState->setString(5, m.memberStart);
		prpdState->execute();

		cout << "\nMEMBER HAS BEEN SUCCESFULLY ADDED!!\n";
		delete conn;

	}
	catch (sql::SQLException& e) {
		cerr << "Something WENT WRONG Please Contanct an Admin or Developer~ UwU \n\n"
			<< e.what() << endl;
	}
	return;
}

void createMember() {
	MemberDetails m;
	cin.ignore();
	cout << "Enter Name: ";
	getline(cin, m.name);
	cout << "\nEnter Email: ";
	getline(cin, m.email);
	cout << "\nEnter Phone Number: ";
	getline(cin, m.phoneNumber);
	cout << "\nEnter Payment Type: ";
	getline(cin, m.paymentType);
	cout << "\nMembership Date Automatically Added! \n";
	cout << "Became a member by " << getDate() << "\n";
	m.memberStart = getDate();
	members.push_back(m);
	addMember(m);
}

void readMember() {
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* conn;
		sql::Statement* stmt;
		sql::ResultSet* res;

		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1", "cpp", "localpassword");
		conn->setSchema("members_database");

		cout << "Retreived Details Sucessfully! \n\n";

		stmt = conn->createStatement();
		res = stmt->executeQuery("SELECT * FROM member_details");

		while (res->next()) {
			cout << "ID: " << res->getInt("ID")
				<< " | Name: " << res->getString("Name")
				<< " | Email: " << res->getString("Email")
				<< " | Phone Number: " << res->getString("PhoneNumber")
				<< " | Payment Type: " << res->getString("PaymentType")
				<< " | Member Since: " << res->getString("MembershipAdded");
			cout << "\n";
		}
		cout << "\n";
		delete conn;
		delete res;
		delete stmt;
	}

	catch (sql::SQLException& e) {

		cerr << "Something WENT WRONG Please Contanct an Admin or Developer~ UwU \n\n"
			<< e.what() << endl;

	}
	return;
}
void sysAdminLogin() {
	string username, password;
	int option;
	while (true) {
		cout << "Logging You in As a System Administrator..\n";
		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		cin >> password;
		if (username == "localhost" && password == "root_pw") {
			cout << "\nThank you for logging in, Admin " << username << "\n";
			cout << "Administrator Panel Options:\n";
			cout << "1 - See All Members\n";
			cout << "2 - Remove a Member\n";
			cout << "3 - Update a Member's Details\n";
			cout << "Enter Option: ";
			cin >> option;
			switch (option) {
			case 1:
				readMember();
				break;
			case 2: 
				break;
			case 3:
				break;
			}
		}
		else {
			cout << "Wrong Password or Username, Attempt Again.";
		}
	}
}

int main() {
	int option;
	char select;
	string username, password;
	cout << "================= WELCOME TO THE ORGANIZATION MEMBERSHIP MANAGEMENT SYSTEM =================\n\n";
	cout << "Firstly are you a member signing up or a system administrator?\n";
	do {
		cout << "(1 for Member, 2 for System Administrator): ";
		cin >> option;
		cout << "\n\n";
		switch (option) {
		case 1:
			cout << "Membership Sign Up..\n\n";
			createMember();
			break;
		case 2:
			sysAdminLogin();
			break;
		default:
			cout << "The Option you have give is an invalid input.\n";
		}
		cout << "Would you like to keep using the program? (Y or N): ";
		cin >> select;
	} while (select == 'Y' || select == 'y');

	return 0;
}

