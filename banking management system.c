#include <iostream>
#include <fstream>
#include <conio.h>
#include <cstring>

using namespace std;

char password[11]; // Global declaration for password

class bankStatement {
public:
    char name[30];
    char accNo[11];
    char userPassword[20];
    float currentBalance;
    float creditedAmount;
    float debitedAmount;
} statement;

class Bank {
private:
    char name[30];
    char accountNo[11];
    char password[11];
    float balance;

public:
    void openAccount();
    void checkBalance();
    void depositMoney();
    void withdrawMoney();
    void transferMoney();
    void showStatement();
};

void Bank::openAccount() {
    Bank user; // Use local variables
    cout << "\nPlease provide some information\n";
    cout << "Enter Your Name: ";
   // cin.ignore(); // Ignore any leftover newline characters
    cin.getline(name, 29);
    cout << "Choose Account No: ";
    cin.getline(accountNo, 10);
    cout << "Create Password: ";
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        password[i] = ch;
        i++;
        cout << '*';
    }
    password[i] = '\0';
    cout << "\n";
    cout << "Deposit Account opening balance: ";
    cin >> balance;

    ofstream writeRecord("bankRecord.txt", ios::app);
    if (!writeRecord) {
        cout << "\nFile is not found\n";
        return;
    }
    writeRecord << name << " " << accountNo << " " << password << " " << balance << endl;
    writeRecord.close();
    cout << "\nYour Account is Opened successfully\n\n";
}

void Bank::checkBalance() {
    char na[30];
    char accNo[11];
    char pass[11];
    float bal;
    char userAccNo[11];
    cout << "Enter Your Account No: ";
   // cin.ignore();
    cin >> userAccNo;
    cout << "Enter Your Password: ";
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        password[i] = ch;
        i++;
        cout << '*';
    }
    password[i] = '\0';
    cout << "\n";
    ifstream readRecord("bankRecord.txt", ios::in);
    if (!readRecord) {
        cout << "\nFile is not found\n";
        return;
    }


    while (readRecord >> na >> accNo >> pass >> bal) {
        //cout << accNo <<" "<< userAccNo<<" " << pass<<" " << password;

        if (!strcmp(accNo, userAccNo) && !strcmp(pass, password)) {

            cout << "\nYour Balance is " << balance << " Rupees\n\n";
            readRecord.close();
            return;
        }
    }

    readRecord.close();
    cout << "\nBalance check Proccess is failed due to incorrect information. Please try again\n\n";
}

void Bank::depositMoney() {
    char userAccNo[11];
    float depositAmount;
    cout << "Enter Your Account No: ";
    //cin.ignore();
    cin.getline(userAccNo, 10);
    cout << "Enter Your Password: ";
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        password[i] = ch;
        i++;
        cout << '*';
    }
    password[i] = '\0';
    cout << "\n";

    fstream updateRecord, tranStatement;
    updateRecord.open("bankRecord.txt", ios::in | ios::out);
    tranStatement.open("bankStatement.txt", ios::out | ios::app);
    while (updateRecord >> name >> accountNo >> password >> balance) {
        if (!strcmp(accountNo, userAccNo) && !strcmp(password, password)) {
            cout << "Enter Deposit Amount: ";
            cin >> depositAmount;
            balance += depositAmount;



            strcpy(statement.name, name);
            strcpy(statement.accNo, accountNo);
            statement.currentBalance = balance;
            statement.creditedAmount = depositAmount;
            statement.debitedAmount = 0.00;

            int pos = updateRecord.tellg();
            cout << updateRecord.tellg();
            updateRecord.seekg(pos - sizeof(*this));
            updateRecord << name << " " << accountNo << " " << password << " " << balance << endl;
            tranStatement << name << " " << accountNo << " " << balance << " " << depositAmount << " 0.00" << endl;

            cout << "\n" << depositAmount << " Rupees is deposited successfully\n\n";
            updateRecord.close();
            tranStatement.close();
            return;
        }
    }
    cout << "\nDeposit Proccess is failed due to incorrect information please try again\n\n";
    tranStatement.close();
    updateRecord.close();
}

void Bank::withdrawMoney() {
    char userAccNo[11];
    float withdrawAmount;
    cout << "Enter Your Account No: ";
    //cin.ignore();
    cin.getline(userAccNo, 10);
    cout << "Enter Your Password: ";
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        password[i] = ch;
        i++;
        cout << '*';
    }
    password[i] = '\0';
    cout << "\n";

    fstream updateRecord, tranStatement;
    updateRecord.open("bankRecord.txt", ios::in | ios::out);
    tranStatement.open("bankStatement.txt", ios::out | ios::app);
    while (updateRecord >> name >> accountNo >> password >> balance) {
        if (!strcmp(accountNo, userAccNo) && !strcmp(password,password)) {
            cout << "Enter Withdrawable Amount: ";
            cin >> withdrawAmount;
            if (balance < withdrawAmount) {
                cout << "\nSorry! You don't have enough Money to withdraw\n\n";
                return;
            }
            balance = balance-withdrawAmount;

            strcpy(statement.name, name);
            strcpy(statement.accNo, accountNo);
            statement.currentBalance = balance;
            statement.creditedAmount = 0.00;
            statement.debitedAmount = withdrawAmount;

            int pos = updateRecord.tellg();
            updateRecord.seekg(pos - sizeof(*this));
            updateRecord << name << " " << accountNo << " " << password << " " << balance << endl;
            tranStatement << name << " " << accountNo << " " << balance << " 0.00 " << withdrawAmount << endl;

            cout << "\n" << withdrawAmount << " Rupees is withdrawal successfully\n\n";
            updateRecord.close();
            tranStatement.close();
            return;
        }
    }
    cout << "\nWithdraw Proccess is failed due to incorrect information please try again\n\n";
    tranStatement.close();
    updateRecord.close();
}

void Bank::transferMoney() {
    char userAccNo[11], transferAccNo[11], accHolderName[30];
    float transferAmount;
    int isFound = 0;

    cout << "Enter Your Account No: ";
   // cin.ignore();
    cin.getline(userAccNo, 10);
    cout << "Enter Your Password: ";
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        password[i] = ch;
        i++;
        cout << '*';
    }
    password[i] = '\0';
    cout << "\n";

    fstream updateRecord, tranStatement;
    updateRecord.open("bankRecord.txt", ios::in | ios::out);
    tranStatement.open("bankStatement.txt", ios::out | ios::app);

    while (updateRecord >> name >> accountNo >> password >> balance) {
        if (!strcmp(accountNo, userAccNo) && !strcmp(password, password)) {
            cout << "Enter receiver's acc. Name: ";
            //cin.ignore();
            cin.getline(accHolderName, 29);
            cout << "Enter Account No: ";
            cin.getline(transferAccNo, 10);
            cout << "Enter Transfer Amount: ";
            cin >> transferAmount;

            if (balance < transferAmount) {
                cout << "\nSorry! You don't have enough Money to Transfer\n\n";
                return;
            }

            balance -= transferAmount;

            int pos = updateRecord.tellg();
            updateRecord.seekg(pos - sizeof(*this));
            updateRecord << name << " " << accountNo << " " << password << " " << balance << endl;
            tranStatement << name << " " << accountNo << " " << balance << " 0.00 " << transferAmount << endl;

            updateRecord.close();
            isFound = 1;
            break;
        }
    }

    if (isFound == 0) {
        cout << "\n senders Transfer Process is failed due to incorrect information. Please try again\n\n";
        updateRecord.close();
        return;
    }

    updateRecord.open("bankRecord.txt", ios::in | ios::out);

    while (updateRecord >> name >> accountNo >> password >> balance) {
        if (!strcmp(accountNo, transferAccNo) && !strcmp(name, accHolderName)) {
            balance += transferAmount;

            strcpy(statement.name, name);
            strcpy(statement.accNo, accountNo);
            statement.currentBalance = balance;
            statement.creditedAmount = transferAmount;
            statement.debitedAmount = 0.00;

            int pos = updateRecord.tellg();
            updateRecord.seekg(pos - sizeof(*this));
            updateRecord << name << " " << accountNo << " " << password << " " << balance << endl;
            tranStatement << name << " " << accountNo << " " << balance << " " << transferAmount << " 0.00" << endl;

            cout << "\n" << transferAmount << " Rupees is transferred successfully\n\n";
            updateRecord.close();
            tranStatement.close();
            return;
        }
    }

    cout << "\nrecievers Transfer Process is failed due to incorrect information. Please try again\n\n";
    updateRecord.close();
    tranStatement.close();
}

void Bank::showStatement() {
    char na[30];
    char accNo[11];
    char pass[11],creditedAmount[10],debitedAmount[10];
    float bal;
    char userAccNo[11] , userPassword[11];
    int isFound = 0;

    cout << "Enter Your Account No: ";
    //cin.ignore();
    cin.getline(userAccNo, 10);
    cout << "Enter Your Password: ";

    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        password[i] = ch;
        i++;
        cout << '*';
    }
    password[i] = '\0';
    cout << "\n";

    ifstream readRecord("bankStatement.txt", ios::in);

    if (!readRecord) {
        cout << "\nFile is not found\n";
        return;
    }

    cout << "\n  Name   \tAccount No  \tCredit  \tDebited  \tBalance\n\n";

     while (readRecord >> name >> accountNo >> balance >> creditedAmount >> debitedAmount ) {
        // cout << accountNo <<" "<< userAccNo<<" " << password <<" " << password;
        if (!strcmp(accountNo, userAccNo)) {
            cout << " " << name << "  \t" << accountNo << "     \t";
            cout << creditedAmount << "          \t" << debitedAmount << "    \t\t"
                 << balance << endl;
            isFound = 1;
        }
    }
    cout << endl;
    readRecord.close();

    if (!isFound) {
        cout << "\nStatement check failed due to incorrect information. Please try again\n\n";
    }
}




int main() {
    int choose;
    Bank account;
    cout << "\nBANKING MANAGEMENT SYSTEM\n\n";
    do {
        cout << "Press 1 to Open Account\n";
        cout << "Press 2 to Check Balance\n";
        cout << "Press 3 to Deposit Money\n";
        cout << "Press 4 to Withdraw Money\n";
        cout << "Press 5 to Transfer Money\n";
        cout << "Press 6 to Show Statement\n";
        cout << "Press 7 to Exit\n";
        cout << "Choose your choice: ";
        cin >> choose;
        cin.ignore(); // Ignore any leftover newline characters
        switch (choose) {
            case 1:
                account.openAccount();
                break;
            case 2:
                account.checkBalance();
                break;

            case 3:
	             account.depositMoney();
	              break;
	        case 4:
	             account.withdrawMoney();
	             break;
	        case 5:
	             account.transferMoney();
	            break;
	        case 6:
	             account.showStatement();
	             break;

            case 7:
                break;
            default:
                cout << "Invalid choice. Try again\n";
                break;
        }
    } while (choose != 7);

    return 0;
}
