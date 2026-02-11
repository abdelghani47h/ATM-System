#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h>

const int BOX_WIDTH = 50;
const int BOX_LEFT_PADDING = 15;

using namespace std;

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enMainMenueOptions {
    eQucikWithdraw = 1, eNormalWithDraw = 2, eDeposit = 3,
    eCheckBalance = 4, eExit = 5
};

const string ClientsFileName = "Clients.txt";
sClient CurrentClient;

void ShowMainMenue();
void Login();
void ShowQuickWithdrawScreen();
void ShowNormalWithDrawScreen();
void ShowDepositScreen();

void PrintCenteredTwoOptions(string left, string right)
{
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << left
        << string(12, ' ')
        << right << endl;
}

void PrintCenteredMenuItem(string text)
{
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << text << endl;
}

void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void PrintCenteredLine()
{
    cout << string(BOX_LEFT_PADDING, ' ')
        << string(BOX_WIDTH, '=')
        << endl;
}

void PrintCenteredTitle(string title)
{
    int titlePadding = (BOX_WIDTH - title.length()) / 2;
    cout << string(BOX_LEFT_PADDING + titlePadding, ' ')
        << title << endl;
}

void PrintCenteredInput(string label)
{
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << left << setw(18) << label << ": ";
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClients;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients, bool SaveToFile = true)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.AccountBalance += Amount;
            if (SaveToFile)
                SaveCleintsDataToFile(ClientsFileName, vClients);
            return true;
        }
    }
    return false;
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice>9)
    {
        cin >> Choice;
    }
    return Choice;
}

short getQuickWithDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1: return 20;
    case 2: return 50;
    case 3: return 100;
    case 4: return 200;
    case 5: return 400;
    case 6: return 600;
    case 7: return 800;
    case 8: return 1000;
    default: return 0;
    }
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
    if (QuickWithDrawOption == 9) return;

    if (QuickWithDrawOption < 1 || QuickWithDrawOption > 8)
    {
        cout << endl;
        SetColor(12);
        cout << string(BOX_LEFT_PADDING + 5, ' ')
            << "Invalid option! Please choose between 1 and 9.";
        SetColor(7);
        cout << endl << string(BOX_LEFT_PADDING + 5, ' ')
            << "Press any key to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);

    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << endl;
        SetColor(12);
        cout << string(BOX_LEFT_PADDING + 5, ' ')
            << "The amount exceeds your balance!";
        SetColor(7);
        cout << endl << string(BOX_LEFT_PADDING + 5, ' ')
            << "Press any key to choose again...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;

    cout << endl;
    SetColor(10);
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Withdrawal successful! New balance: " << CurrentClient.AccountBalance;
    SetColor(7);
    cout << endl << string(BOX_LEFT_PADDING + 5, ' ')
        << "Press any key to continue...";
    system("pause>0");
}

double ReadDepositAmount() {
    double Amount;
    while (true) {
        cout << endl << string(BOX_LEFT_PADDING + 5, ' ') << "Enter deposit amount: ";
        cin >> Amount;
        if (!cin) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (Amount > 0) break;
    }
    return Amount;
}

void PerfromDepositOption()
{
    double DepositAmount = ReadDepositAmount();
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    char Answer;
    cout << endl << string(BOX_LEFT_PADDING + 5, ' ')
        << "Are you sure you want to perform this transaction? y/n ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
        CurrentClient.AccountBalance += DepositAmount;

        SetColor(10); 
        cout << endl << string(BOX_LEFT_PADDING + 5, ' ')
            << "Transaction completed successfully! New balance: "
            << CurrentClient.AccountBalance << endl;
        SetColor(7);
    }
}

void ShowDepositScreen()
{
    system("cls");

    SetColor(11);
    PrintCenteredLine();
    PrintCenteredTitle("DEPOSIT");
    PrintCenteredLine();
    SetColor(7);

    cout << endl;

    PerfromDepositOption();

    system("pause>0");
}

void ShowCheckBalanceScreen()
{
    system("cls");
    SetColor(11);
    PrintCenteredLine();
    PrintCenteredTitle("CHECK BALANCE");
    PrintCenteredLine();
    SetColor(7);

    cout << endl;
    SetColor(10);

    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Your Balance : " << CurrentClient.AccountBalance << endl;

    SetColor(7);
}

int ReadWithDrawAmont()
{
    int Amount = 0;
    while (true)
    {
     
        cin >> Amount;

        if (!cin) 
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (Amount > 0 && Amount % 5 == 0)
            break;
    }
    return Amount;
}

void PerfromNormalWithdrawOption()
{
    int WithDrawBalance = ReadWithDrawAmont();

    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << endl;
        SetColor(12); 
        cout << string(BOX_LEFT_PADDING + 5, ' ')
            << "The amount exceeds your balance, make another choice.";
        SetColor(7);
        cout << endl << string(BOX_LEFT_PADDING + 5, ' ')
            << "Press any key to continue...";
        system("pause>0");
        ShowNormalWithDrawScreen();
        return;
    }

    char Answer;
    cout << endl;
    SetColor(11);
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Are you sure you want to perform this transaction? y/n ? ";
    SetColor(7);
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients, false);
        CurrentClient.AccountBalance -= WithDrawBalance;

        cout << endl;
        SetColor(10); 
        cout << string(BOX_LEFT_PADDING + 5, ' ')
            << "Transaction completed successfully! New balance: " << CurrentClient.AccountBalance;
        SetColor(7);
   
        system("pause>0");
    }
    else
    {
        ShowNormalWithDrawScreen(); 
    }
}

void ShowNormalWithDrawScreen()
{
    system("cls");
    SetColor(11);
    PrintCenteredLine();
    PrintCenteredTitle("NORMAL WITHDRAW");
    PrintCenteredLine();
    SetColor(7);

    cout << endl;
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Enter amount (multiple of 5): ";
    PerfromNormalWithdrawOption();
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    SetColor(11);
    PrintCenteredLine();
    PrintCenteredTitle("QUICK WITHDRAW");
    PrintCenteredLine();
    SetColor(7);

    cout << endl;
    PrintCenteredTwoOptions("[1] 20", "[2] 50");
    PrintCenteredTwoOptions("[3] 100", "[4] 200");
    PrintCenteredTwoOptions("[5] 400", "[6] 600");
    PrintCenteredTwoOptions("[7] 800", "[8] 1000");

    cout << endl;
    PrintCenteredMenuItem("[9] Exit");
    cout << endl;

    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Your Balance : " << CurrentClient.AccountBalance << endl << endl;

    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Choose option [1 - 9]: ";
    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eQucikWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eNormalWithDraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eExit:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    SetColor(11);
    PrintCenteredLine();
    PrintCenteredTitle("ATM MAIN MENU");
    PrintCenteredLine();
    SetColor(7);

    cout << endl;
    PrintCenteredMenuItem("[1] Quick Withdraw");
    PrintCenteredMenuItem("[2] Normal Withdraw");
    PrintCenteredMenuItem("[3] Deposit");
    PrintCenteredMenuItem("[4] Check Balance");
    PrintCenteredMenuItem("[5] Logout");

    cout << endl;
    cout << string(BOX_LEFT_PADDING + 5, ' ')
        << "Choose option [1 - 5]: ";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");
        SetColor(11);
        PrintCenteredLine();
        PrintCenteredTitle("LOGIN SCREEN");
        PrintCenteredLine();
        SetColor(7);

        if (LoginFaild)
        {
            cout << endl;
            SetColor(12);
            cout << string(BOX_LEFT_PADDING + 6, ' ')
                << "Invalid Account Number or Pin Code!";
            SetColor(7);
        }

        cout << endl << endl;
        PrintCenteredInput("Account Number");
        cin >> AccountNumber;

        cout << endl << endl;
        PrintCenteredInput("Pin Code");
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowMainMenue();
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}
