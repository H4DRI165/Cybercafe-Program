#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <mysql.h>
#include <chrono>
#include <thread>
#include <cctype> // For isalpha() function

using namespace std;

void Login();
int Admin();
int AdminMenu();
int UserMenu();
void AddAdmin();
void DeleteAdmin();
void ViewAdmin();
void Pc();
void OpenPc();
void ClosePc();
void PcReserve();
void RemoveReserve();
void ViewPayment();

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

class db_response
{

public:
	static void ConnectionFunction()
	{
		//Start the program
		conn = mysql_init(0);
		cout << "Testing connection to database..." << std::endl;

		//Check the connection to the mysql database
		conn = mysql_real_connect(conn, "localhost", "root", "", "cybercafe management system", 3306, NULL, 0);
		if (conn) {
			cout << "Database Connected To MySql" << std::endl;
		}
		else {
			cout << "Failed To Connect!" << std::endl;

			// Delay for 3 seconds
			std::this_thread::sleep_for(std::chrono::seconds(3));
			//Exit the program
			exit(1);
		}
	}
};



int main()
{
	//Clear the screen
	system("cls");
	db_response::ConnectionFunction();

	int select;

	cout << "-----------------------------------------\n";
	cout << "\tWELCOME TO IMAX CYBERCAFE\n";
	cout << "\nBangunan 2 Fasa 1E Seri Manjung, Perak\n\n";
	cout << "-----------------------------------------\n\n";
	cout << "SELECT 1 for | LOGIN | " << endl;
	cout << "SELECT 2 for | EXIT | \n\n";
	cout << "SELECT: ";
	cin >> select;

	switch (select)
	{
	case 1:
		Login();
		break;
	case 2:
		cout << "Program Exiting..." << endl;
		exit(0);
	default:
		cout << "Invalid number. Press Enter To Continue... ";
		_getch();
		system("cls");
		main();
		break;
	}
	return 0;
}

//Login purpose
void Login()
{
	string Username, Password, AdminID;
	system("cls");

	cout << endl;
	cout << "--------------------------------------------------\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n";
	cout << "\n-----------------\tLOGIN\t------------------\n\n\n";
	cout << "USERNAME: ";
	cin >> Username;
	cout << "PASSWORD: ";
	char ch;
	while ((ch = _getch()) != 13)
	{
		Password += ch;
		cout << '*';
	}

	string checkUser_query = "select AdminID from Userdata where Username = '" + Username + "' and password = '" + Password + "'";
	const char* cu = checkUser_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			while (row = mysql_fetch_row(res))
				AdminID = row[0];
			Admin();
		}
		else
		{
			cout << endl;
			cout << "Invalid username or password. Please retry again " << endl;
			system("pause");
			Login();
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}


int Admin()
{
	int select;

	system("cls");
	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "--------------------------------------------------\n\n\n";

	cout << "SELECT 1 for | ADMIN MANAGEMENT |" << endl;
	cout << "SELECT 2 for | USER MANAGEMENT |" << endl;
	cout << "SELECT 3 for | LOGOUT |" << endl;
	cout << "\nSELECT: ";
	cin >> select;

	switch (select)
	{
	case 1:
		AdminMenu();
		break;
	case 2:
		UserMenu();
		break;
	case 3:
		main();
		break;
	default:
		cout << "Invalid number. Press Enter To Continue... ";
		_getch();
		system("cls");
		Admin();
		break;
	}
	return 0;
}


int AdminMenu()
{
	system("cls");
	int select;

	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "----------\tADMIN MANAGEMENT\t----------\n\n\n";
	cout << "SELECT 1 for | CREATE NEW ADMIN ACCOUNT |" << endl;
	cout << "SELECT 2 for | DELETE ADMIN ACCOUNT |" << endl;
	cout << "SELECT 3 for | VIEW ALL ADMIN ACCOUNT |" << endl;
	cout << "SELECT 4 for | PREVIOUS MENU |" << endl;
	cout << "\nSELECT: ";
	cin >> select;

	switch (select)
	{
	case 1:
		AddAdmin();
		break;
	case 2:
		DeleteAdmin();
		break;
	case 3:
		ViewAdmin();
		break;
	case 4:
		Admin();
		break;
	default:
		cout << "Invalid number. Press Enter To Continue... " << endl;
		_getch();
		system("cls");
		AdminMenu();
		break;
	}
	return 0;
}

void AddAdmin()
{
	system("cls");
	string Username, Password, Name;

	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "----------\tCREATE NEW ADMIN\t-----------\n\n\n";
	cout << "ENTER USERNAME: ";
	cin.ignore(1, '\n');
	getline(cin, Username);
	cout << "ENTER PASSWORD: ";
	getline(cin, Password);
	cout << "ENTER NAME: ";
	getline(cin, Name);

	// Check if each character in the input is alphabetic
	for (char c : Name) {
		if (!isalpha(c)) {
			cout << "Invalid input! Please enter alphabet characters only." << endl;
			return; // Return an error code
		}
	}

	string checkUser_query = "select * from userdata where Username = '" + Username + "'";
	const char* cu = checkUser_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			cout << "Username is already exist. Press Enter to Try Again...";
			_getch();
			AddAdmin();
		}
		else
		{
			int n = Password.length();

			// Checking lower alphabet in string
			bool hasLower = false, hasUpper = false;
			bool hasDigit = false, specialChar = false;
			string normalChars = "abcdefghijklmnopqrstu" "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

			for (int i = 0; i < n; i++)
			{
				if (islower(Password[i]))
					hasLower = true;
				if (isupper(Password[i]))
					hasUpper = true;
				if (isdigit(Password[i]))
					hasDigit = true;
				size_t special = Password.find_first_not_of(normalChars);
				if (special != string::npos)
					specialChar = true;
			}

			//Check password strength
			cout << "\nStrength of password: ";
			if (hasLower && hasUpper && hasDigit && specialChar && (n >= 8))
			{
				cout << "Strong" << endl;

				string insertUser_query = "insert into userdata (Username, Password, Name) values ('" + Username + "', '" + Password + "', '" + Name + "')";
				const char* q = insertUser_query.c_str();
				qstate = mysql_query(conn, q);

				if (!qstate)
				{
					cout << endl << "You have been registered. Press Enter to Continue...";
					_getch();
					AdminMenu();
				}
				else
				{
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
				}
			}
			else if ((hasLower || hasUpper) && specialChar && (n >= 6))
			{
				cout << "Moderate" << endl;

				string insertUser_query = "insert into userdata (Username, Password, Name) values ('" + Username + "', '" + Password + "', '" + Name + "')";
				const char* q = insertUser_query.c_str();
				qstate = mysql_query(conn, q);

				if (!qstate)
				{
					cout << endl << "You have been registered. Press Enter to Continue...";
					_getch();
					AdminMenu();
				}
				else
				{
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
				}
			}
			else
			{
				cout << "Weak" << endl;
				cout << "\nPlease try again";
				cout << "\n Tips: Use Upper and Lower alphabets, number and special character for maximum protection";
				_getch();
				AddAdmin();
			}
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}


void DeleteAdmin()
{
	system("cls");
	string Username;
	char choose;

	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "----------------\tDELETE ADMIN\t-----------------\n\n\n";

	qstate = mysql_query(conn, "select AdminID, Username, Password, Name from userdata");

	if (!qstate)
	{
		cout << setw(11) << "AdminID" << setw(20) << "Username" << setw(20) << "Password" << setw(50) << "Name\n";

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << setw(11) << row[0] << setw(20) << row[1] << setw(20) << row[2] << setw(50) << row[3] << endl;
		}

		cout << "\nChoose Username: ";
		cin >> Username;

		string delete_query = "delete from userdata where Username = '" + Username + "'";
		const char* q = delete_query.c_str();
		qstate = mysql_query(conn, q);

		cout << "Do you want to delete other admin? (Y|N): ";
		cin >> choose;

		if (choose == 'y' || choose == 'Y')
			DeleteAdmin();
		else
			ViewAdmin();
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}


void ViewAdmin()
{
	system("cls");
	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "---------------\tVIEW ALL ADMIN\t------------------\n\n\n";

	qstate = mysql_query(conn, "select AdminID, Username, Password, Name from userdata");

	if (!qstate)
	{
		cout << setw(11) << "AdminID" << setw(20) << "Username" << setw(20) << "Password" << setw(50) << "Name\n";

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << setw(11) << row[0] << setw(20) << row[1] << setw(20) << row[2] << setw(50) << row[3] << endl;
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}

	cout << "\nPress Enter To Go Back...";
	_getch();
	AdminMenu();
}


int UserMenu()
{
	system("cls");
	int select;

	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "---------------\tUSER MANAGEMENT\t------------------\n\n\n";
	cout << "SELECT 1 for | PC |" << endl;
	cout << "SELECT 2 for | VIEW PAYMENT DETAIL |" << endl;
	cout << "SELECT 3 for | PREVIOUS MENU |" << endl;
	cout << "\nSELECT: ";
	cin >> select;

	switch (select)
	{
	case 1:
		Pc();
		break;
	case 2:
		ViewPayment();
		break;
	case 3:
		Admin();
		break;
	default:
		cout << "Invalid number. Press Enter To Continue... ";
		_getch();
		system("cls");
		UserMenu();
		break;
	}
	return 0;;
}

void Pc()
{
	system("cls");
	int select;

	cout << "--------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "-----------------\tPC\t------------------\n\n\n";

	qstate = mysql_query(conn, "select PcNumber, PcStatus from pc");

	if (!qstate)
	{
		cout << setw(15) << "PcNumber" << setw(20) << "PcStatus" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << setw(15) << row[0] << setw(20) << row[1] << endl;
		}

		cout << "\nSELECT 1 for | OPEN PC |" << endl;
		cout << "SELECT 2 for | CLOSE PC |" << endl;
		cout << "SELECT 3 for | PC RESERVATION |" << endl;
		cout << "SELECT 4 for | REMOVE PC RESERVATION |" << endl;
		cout << "SELECT 5 for | PREVIOUS MENU |" << endl;
		cout << "\nSELECT: ";
		cin >> select;

		switch (select)
		{
		case 1:
			OpenPc();
			break;
		case 2:
			ClosePc();
			break;
		case 3:
			PcReserve();
			break;
		case 4:
			RemoveReserve();
			break;
		case 5:
			UserMenu();
		default:
			cout << "Invalid number. Press Enter To Continue... ";
			_getch();
			system("cls");
			Pc();
			break;
		}
	}
}

void OpenPc()
{
	string PcNumber, CustomerName, AdminID, NotAvailable, No;
	int age;
	NotAvailable = "NotAvailable";
	No = "No";

	cout << "\nSELECT PC TO OPEN: ";
	cin >> PcNumber;

	cout << "\nADMIN ID: ";
	cin >> AdminID;
	cout << "ENTER CUSTOMER AGE: ";
	cin >> age;
	cout << "ENTER CUSTOMER NAME: ";
	cin.ignore(1, '\n');
	getline(cin, CustomerName);

	if (age <= 17)
	{
		cout << "Underage cannot use pc here !";
		_getch();
		Pc();
	}
	else if (age >= 18)
	{

		string update_query = "update pc set PcStatus = '" + NotAvailable + "' where PcNumber = '" + PcNumber + "'";
		const char* q = update_query.c_str();
		qstate = mysql_query(conn, q);

		if (!qstate)
		{
			cout << endl << "\nPc will open now and ";
		}
		else
		{
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}


		string insert_query = "insert into payment (AdminID, PcNumber, CustomerName, Paid) values ('" + AdminID + "', '" + PcNumber + "', '" + CustomerName + "', '" + No + "')";
		const char* p = insert_query.c_str();
		qstate = mysql_query(conn, p);

		if (!qstate)
		{
			cout << "payment detail have been saved." << endl;
			_getch();
		}
		else
		{
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}

		Pc();
	}
}


void ClosePc()
{
	string PcNumber, Available, Yes, CustomerName, pymtID, Unpaid;
	Available = "Available";
	double PayAmount;
	Yes = "Yes";
	Unpaid = "Unpaid";
	char choice;

	cout << "SELECT PC TO CLOSE: ";
	cin >> PcNumber;

	string update_query = "update pc set PcStatus = '" + Available + "' where PcNumber = '" + PcNumber + "'";
	const char* u = update_query.c_str();
	qstate = mysql_query(conn, u);

	if (!qstate)
	{
		cout << endl << "*Pc have been closed and ";
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}

	string select_query1 = "SELECT PaymentID FROM payment WHERE PcNumber = '" + PcNumber + "' ORDER BY PaymentID DESC LIMIT 1";
	const char* w = select_query1.c_str();
	qstate = mysql_query(conn, w);

	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	pymtID = row[0];

	string update_query1 = "update payment set EndTime = now() where PcNumber = '" + PcNumber + "' AND '" + pymtID + "' ORDER BY PaymentID DESC LIMIT 1";
	const char* q = update_query1.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		cout << "Endtime database have been updated ";
	}
	else
	{
		cout << "Query Execution Problem! " << mysql_errno(conn) << endl;
		_getch();
		ClosePc();
	}

	string select_query2 = "select (ROUND(time_to_sec((TIMEDIFF(EndTime, StartTime))) / 60)) as duration from payment where PcNumber = '" + PcNumber + "' AND PaymentID = '" + pymtID + "' ORDER BY PaymentID DESC LIMIT 1";
	const char* o = select_query2.c_str();
	qstate = mysql_query(conn, o);

	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);

	float rm = 1;
	PayAmount = (rm / 30) * (atof(row[0]));

	string a = to_string(PayAmount);
	string update_query2 = "update payment set PayAmount = '" + a + "' where PcNumber = '" + PcNumber + "' AND PaymentID = '" + pymtID + "'";
	const char* z = update_query2.c_str();
	qstate = mysql_query(conn, z);

	cout << "\n\nPrice (RM): " << a;
	cout << "\n\nCustomer paid? (Y|N): ";
	cin >> choice;

	if (choice == 'Y' || choice == 'y')
	{
		string update_query3 = "update payment set Paid= '" + Yes + "' where PcNumber = '" + PcNumber + "' AND PaymentID = '" + pymtID + "'";
		const char* m = update_query3.c_str();
		qstate = mysql_query(conn, m);

		if (!qstate)
		{
			cout << "\n*Pay amount database have been updated ";
			_getch();
			Pc();

		}
		else
		{
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
	}
	else if (choice == 'N' || choice == 'n')
	{
		string update_query4 = "update payment set Paid= '" + Unpaid + "' where PcNumber = '" + PcNumber + "' AND PaymentID = '" + pymtID + "'";
		const char* n = update_query4.c_str();
		qstate = mysql_query(conn, n);

		if (!qstate)
		{
			cout << "\n*Pay amount database have been updated ";
			_getch();
			Pc();

		}
		else
		{
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
	}
}


void PcReserve()
{

	string Reserved, PcNumber;
	Reserved = "Reserved";

	cout << "\nSELECT PC TO RESERVE: ";
	cin >> PcNumber;

	string update_query = "update pc set PcStatus = '" + Reserved + "' where PcNumber = '" + PcNumber + "'";
	const char* q = update_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		cout << endl << "Pc have been reserved" << endl;
		_getch();
		Pc();
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}


void RemoveReserve()
{
	string Available, PcNumber;
	Available = "Available";

	cout << "\nSELECT PC TO REMOVE RESERVATION: ";
	cin >> PcNumber;

	string update_query = "update pc set PcStatus = '" + Available + "' where PcNumber = '" + PcNumber + "'";
	const char* q = update_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		cout << endl << "Pc reservation have been removed" << endl;
		_getch();
		Pc();
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}


void ViewPayment()

{
	system("cls");

	cout << "--------------------------------------------------------\n\n";
	cout << "\t\tIMAX CYBERCAFE SYSTEM\n\n";
	cout << "---------------\tVIEW PAYMENT DETAIL\t----------------\n\n\n";

	qstate = mysql_query(conn, "select PaymentID, AdminID, PcNumber, CustomerName, PayAmount, Paid, StartTime, EndTime from payment");

	if (!qstate)
	{
		cout << setw(11) << "Payment ID" << setw(11) << "Admin ID" << setw(15) << "Pc Number" << setw(30) << "Customer Name" << setw(20) << "Pay Amount (RM)" << setw(11) << "Paid" << setw(30) << "Start Time" << setw(30) << "End Time" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << setw(11) << row[0] << setw(11) << row[1] << setw(15) << row[2] << setw(30) << row[3] << setw(20) << row[4] << setw(11) << row[5] << setw(30) << row[6] << setw(30) << row[7] << endl;
		}

		string select_query = "select COUNT(*) from payment";
		const char* m = select_query.c_str();
		qstate = mysql_query(conn, m);

		if (!qstate)
		{
			int data;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			data = atof(row[0]);

			cout << "\n\n\nTotal Data : " << data;
		}

		string select_query1 = "select SUM(PayAmount) from payment where PcNumber = 1";
		const char* q = select_query1.c_str();
		qstate = mysql_query(conn, q);

		if (!qstate)
		{
			double Pc1;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			Pc1 = atof(row[0]);

			cout << "\n\n\nPc 1 (RM) : " << Pc1;
		}

		string select_query2 = "select SUM(PayAmount) from payment where PcNumber = 2";
		const char* w = select_query2.c_str();
		qstate = mysql_query(conn, w);

		if (!qstate)
		{
			double Pc2;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			Pc2 = atof(row[0]);

			cout << "\nPc 2 (RM) : " << Pc2;
		}

		string select_query3 = "select SUM(PayAmount) from payment where PcNumber = 3";
		const char* e = select_query3.c_str();
		qstate = mysql_query(conn, e);

		if (!qstate)
		{
			double Pc3;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			Pc3 = atof(row[0]);

			cout << "\nPc 3 (RM) : " << Pc3;
		}

		string select_query4 = "select SUM(PayAmount) from payment where PcNumber = 4";
		const char* r = select_query4.c_str();
		qstate = mysql_query(conn, r);

		if (!qstate)
		{
			double Pc4;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			Pc4 = atof(row[0]);

			cout << "\nPc 4 (RM) : " << Pc4;

		}

		string select_query5 = "select SUM(PayAmount) from payment where PcNumber = 5";
		const char* t = select_query5.c_str();
		qstate = mysql_query(conn, t);

		if (!qstate)
		{
			double Pc5;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			Pc5 = atof(row[0]);

			cout << "\nPc 5 (RM) : " << Pc5;
		}

		string select_query6 = "select SUM(PayAmount) from payment";
		const char* p = select_query6.c_str();
		qstate = mysql_query(conn, p);

		if (!qstate)
		{
			double TotalPrice;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			TotalPrice = atof(row[0]);

			cout << "\n\nTotal Collected (RM) : " << TotalPrice << endl;
		}

		string select_query7 = "select SUM(PayAmount) from payment where Paid = 'Unpaid'";
		const char* k = select_query7.c_str();
		qstate = mysql_query(conn, k);

		if (!qstate)
		{
			double Unpaid;
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			Unpaid = atof(row[0]);

			cout << "\nUncollected (RM) : " << Unpaid << endl;
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	cout << "\nPress Enter To Go Back...";
	_getch();
	UserMenu();
}

