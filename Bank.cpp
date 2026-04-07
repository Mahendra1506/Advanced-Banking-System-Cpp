#include<iostream>
#include<vector>
#include<limits>
#include<fstream>
using namespace std;
 
class Account
{
    public:
        static int nextAccno;

        vector<string> history;

        int accNo;
        string name;
        float balance;
        int pin;

        void createaccount()
        {
            cout<<"===== Banking System ====="<<endl;

            cout<<"Enter Your Name :"<<endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin,name);

            cout<<"Set PIN :"<<endl;
            cin>>pin;

            cout<<"Enter The Balance :"<<endl;
            cin>>balance;

            accNo = nextAccno++;

            cout<<"Account Created Succesfully!!!"<<endl;
            cout<<"Your Account Number is :"<<accNo<<endl;
        }
        void deposit()
        {
            float amount;
            cout<<"Enter Amount to Deposite"<<endl;
            cin>>amount;
            if(amount <= 0)
            {
                cout<<"Invalid Amount"<<endl;
                return;
            }
            balance += amount;
            history.push_back("Deposited " + to_string(amount));
            savehistoryTofile();
        }
        void withdraw()
        {
            float amount;
            cout<<"Enter Amount to withdraw"<<endl;
            cin>>amount;
            if(amount > balance)
            {
                cout<<"Insufficient Balance!!!"<<endl;
            }
            else{
                balance -= amount;
                history.push_back("Withdrawn " + to_string(amount));
                savehistoryTofile();
            }
            
        }
        void display()
        {
            cout<<"!!Account Details!!"<<endl;
            cout<<" Account Number : "<<accNo<<endl;
            cout<<" Name : "<<name<<endl;
            cout<<" Balance : "<<balance<<endl;

        }
        void savehistoryTofile()
        {
            string filename="history_"+to_string(accNo)+".txt";
            ofstream file(filename, ios::app);

            for(string h : history)
            {
                file<<h<<endl;

            }
            file.close();

            
        }
        void loadHistoryFromfile()
        {
            string filename = "history_"+ to_string(accNo)+".txt";
            ifstream file(filename);

            string line;
            while(getline(file,line))
        {
            history.push_back(line);
        }
        file.close();
        }
        void showhistory()
        {
            cout<<"Transacion History:"<<endl;
            for(string h: history)
            {
                cout<<h<<endl;

            }
        }
        void transfer(Account &receiver)
        {
            float amount;
            cout<<"Enter Amount To Transfer"<<endl;
            cin>>amount;
            

            if(amount <= 0)
            {
                cout<<"Invalid Amount!!"<<endl;
                return ;
            }
            if(amount > balance)
            {
                cout<<"Insufficent Balance"<<endl;
                return ;
            }

            balance -= amount;
            receiver.balance += amount;

            history.push_back("Transferred "+to_string(amount)+" to AccNO "+ to_string(receiver.accNo));
            savehistoryTofile();

            receiver.history.push_back("Received "+to_string(amount)+" from AccNO "+to_string(accNo));
            savehistoryTofile();

            cout<<"Transfer Successful!"<<endl;
        }
};
vector<Account> accounts;
int findAccount(int accNo)
{
    for(int i=0;i<accounts.size();i++)
    {
        if(accounts[i].accNo == accNo)
        {
            return i;
        }
    }
    return -1;
}
int Login()
{
    
    int accNo;
    int pin;
    cout<<"Enter Account Number"<<endl;
    cin>>accNo;

    cout<<"Enter The Pin"<<endl;
    cin>>pin;

    int index=findAccount(accNo);

    if(index != -1 && accounts[index].pin == pin)
    {
        cout<<"Welcome, "<<accounts[index].name<<endl;
        return index;
    }
    else{
        cout<<"Invalid Credentials!!"<<endl;
        return -1;
    }
}
void saveToFile()
{
    ofstream file("account.txt");

    for(int i=0;i<accounts.size();i++)
    {
        file<<accounts[i].accNo<<" "
            <<accounts[i].name<<" "
            <<accounts[i].balance<<" "
            <<accounts[i].pin<<endl;

    }
    file.close();
}
void loadFromFile()
{
    ifstream file("account.txt");

    Account acc;
    int lastAccNo = 999;
    while(file>>acc.accNo>>acc.name>>acc.balance>>acc.pin)
    {
        accounts.push_back(acc);

        if(acc.accNo > lastAccNo)
        {
            lastAccNo = acc.accNo;
        }
    }
    file.close();

    Account ::nextAccno = lastAccNo + 1;
}
int Account::nextAccno = 1000; 

int main()
{
    loadFromFile();
    int choice;
    do{
        cout<<"---Banking System---"<<endl;
        cout<<"1.Create Account :"<<endl;
        cout<<"2.Login :"<<endl;
        cout<<"3. Exit :"<<endl;
        cout<<"Enter Choice : "<<endl;
        cin>>choice;

        switch(choice){
            case 1:{
                Account acc;
                acc.createaccount();
                accounts.push_back(acc);
                saveToFile();
                break;
            }
            case 2:{
                int index=Login();

                if(index!=-1)
                {
                    accounts[index].history.clear();
                    accounts[index].loadHistoryFromfile();
                    int ch;
                    do{
                        cout<<"1.Deposit\n2.Withdraw\n3.Check Balance\n4.History\n5.Tranfer\n6.Logout"<<endl;
                        cout<<"Enter Your Choice :"<<endl;
                        cin>>ch;

                        switch(ch)
                        {
                            case 1:
                            accounts[index].deposit();
                            saveToFile();
                            break;

                            case 2:
                            accounts[index].withdraw();
                            saveToFile();
                            break;

                            case 3:
                            accounts[index].display();
                            break;

                            case 4:
                            accounts[index].showhistory();
                            break;

                            case 5:
                            int receiverAccNo;
                            cout<<"Enter Receiver Account Number"<<endl;
                            cin>>receiverAccNo;

                            int receiverIndex = findAccount(receiverAccNo);

                            if(receiverAccNo == accounts[index].accNo)
                            {
                                cout<<"Cannot tranfer to same account "<<endl;
                                break;
                            }
                           

                            if(receiverIndex == -1)
                            {
                                cout<<"Account Not Found"<<endl;

                            }
                            else
                            {
                                accounts[index].transfer(accounts[receiverIndex]);
                                saveToFile();
                            }
                            break;
                        }
                    }while(ch != 6);
                }
                break;

            }
            case 3:{
                cout<<"Exiting...."<<endl;
                break;
            }
            default:
                cout<<"Invalid Choice...."<<endl;

        }
    }while(choice != 3);

    return 0;
}