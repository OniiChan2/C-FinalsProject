//Developed by @kleinuu
//Developed in no relations to "" of COE253
//Credits to those who need credits XD XD

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

//Structure for the Member's details 
struct MemberDetails {
	string name;
	string email;
	string phoneNumber;
	string paymentType;
	string memberStart;
};
vector<MemberDetails> members;


//For getting the current date on the computer 
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
		cout << "\n============================ Member's List ============================ \n";
		while (res->next()) {
			cout << "ID: " << res->getInt("ID")
				<< " | Name: " << res->getString("Name")
				<< " | Email: " << res->getString("Email")
				<< " | Phone Number: " << res->getString("PhoneNumber")
				<< " | Payment Type: " << res->getString("PaymentType")
				<< " | Member Since: " << res->getString("MembershipAdded");
			cout << "\n\n";
		}
		cout << "\n";
		delete conn;
		delete res;
		delete stmt;
		return;
	}
	catch (sql::SQLException& e) {

		cerr << "Something WENT WRONG Please Contanct an Admin or Developer~ UwU \n\n"
			<< e.what() << endl;

	}
	return;
}

void updateDetails(int ID, const MemberDetails& m) {
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* conn;

		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1", "cpp", "localpassword");
		conn->setSchema("members_database");
		unique_ptr<sql::PreparedStatement> prpdState(
			conn->prepareStatement("UPDATE member_details SET Name = ?, Email = ?, PhoneNumber = ?, PaymentType = ? WHERE ID = ?"));
		prpdState->setString(1, m.name);
		prpdState->setString(2, m.email);
		prpdState->setString(3, m.phoneNumber);
		prpdState->setString(4, m.paymentType);
		prpdState->setInt(5, ID);
		prpdState->execute();
	}
	catch (sql::SQLException& e) {
		cerr << "Something WENT WRONG Please Contanct an Admin or Developer~ UwU \n\n"
			<< e.what() << endl;
	}
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

void updateMember() {
	int ID;
	MemberDetails m;

	cout << "\n\n";
	cout << "Understood! Updating the Details Of The Member\n";
	cout << "Enter Member ID # To Be Updated: ";
	cin >> ID;
	cin.ignore();
	cout << "Enter New Name: ";
	getline(cin, m.name);
	cout << "Enter Email: ";
	getline(cin, m.email);
	cout << "Enter Phone Number: ";
	getline(cin, m.phoneNumber);
	cout << "Enter Payment Type: ";
	getline(cin, m.paymentType);
	cout << "\nUpdate Member's Details Successfully!\n";
	members.push_back(m);
	updateDetails(ID, m);
	cout << "\n\nShowing Updated List Of Members.\n\n";
	return;
}

void deleteMember() {
	int ID;

	cout << "Enter the ID # To Be Deleted: ";
	cin >> ID;
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* conn;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1", "cpp", "localpassword");
		conn->setSchema("members_database");
		unique_ptr<sql::PreparedStatement> prpdState(conn->prepareStatement("DELETE FROM member_details WHERE ID = ?"));
		prpdState->setInt(1, ID);
		prpdState->execute();
		cout << "Sucesfully Deleted From Database! Showing Updated List..\n";
		readMember();
		cout << "\nGoing Back to Admin Panel. \n";
	}

	catch (sql::SQLException& e) {
		cerr << "Something WENT WRONG Please Contanct an Admin or Developer~ UwU \n\n"
			<< e.what() << endl;
	}

	return;
}

bool adminPanel() {
	while (true) {
		int option;

		cout << "\nAdministrator Panel Options:\n";
		cout << "1 - See All Members\n";
		cout << "2 - Remove a Member\n";
		cout << "3 - Update a Member's Details\n";
		cout << "4 - Log out as Admin & Exit\n";
		cout << "Enter Option: ";
		cin >> option;
		switch (option) {
		case 1:
			readMember();
			break;
		case 2:
			deleteMember();
			break;
		case 3:
			updateMember();
			break;
		case 4 :
			cout << "Logging Out Now..\n";
			cout << "Thank you for using! Exiting Now.\n";
			return false;
			break;
		default:
			cout << "Entered Option Does not Exist?..\n";
		}
	}
}

bool adminLogin(string Username, string Password) {
	try {
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* conn;
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1", "cpp", "localpassword");
		conn->setSchema("members_database");
		unique_ptr<sql::PreparedStatement>prpdState(conn->prepareStatement("SELECT COUNT(*) FROM adminaccounts WHERE Username = ? AND Password = ?"));
		prpdState->setString(1, Username);
		prpdState->setString(2, Password);

		unique_ptr <sql::ResultSet> rslt(prpdState->executeQuery());
		if (rslt->next() && rslt->getInt(1) == 1) {
			return true;
		}
		else {
			return false;
		}

	}

	catch (sql::SQLException& e) 
	{
		cerr << "Something WENT WRONG Please Contanct an Admin or Developer~ UwU \n\n"
			<< e.what() << endl;
	}

}

void sysAdminLogin() {
	string username, password;
	bool isLoggedIn;
	int count = 0;
	const int max_login_attempt = 3;

	while (count < max_login_attempt) {
		cout << "\nLogging You in As a System Administrator..\n";
		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		cin >> password;
		if (adminLogin(username, password)) {
			cout << "Logging you In now! \n";
			bool stayLoggedIn = adminPanel();
			if (!stayLoggedIn) {
				return;
			}

		}
		else if (!adminLogin(username, password)) {
			count++;
			cout << "You are now on Attempt #" << count << ".\n";
		}
	
	}
	string output = (count < max_login_attempt) ? "Thank you for using, Logging out now.." : "TOO MANY FAILED LOGIN ATTEMPTS GOING BACK TO MAIN MENU";
	cout << output << "\n";
	return;
}

int main() {
	int option;
	char select;

	cout << "================= WELCOME TO THE ORGANIZATION MEMBERSHIP MANAGEMENT SYSTEM =================\n\n";
	cout << "Firstly are you a member signing up or a system administrator?\n\n";
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
		cout << "Would you like to keep using the application? (Y or N): ";
		cin >> select;
	} while (select == 'Y' || select == 'y');
	cout << "Thank you for using! Exiting now.\n";
	return 0;

}

