#include<iostream>
#include<string>
#include<ctime>

using namespace std;


string type1 = "credit";
string type2 = "debit";


template<class T>
class List
{

private:
	class Node
	{
	public:
		T data;
		Node* next;
		Node* prev;
		Node()
		{
			next = NULL;
			prev = NULL;
		}
	};

	Node* head;
	Node* tail;
public:


	List()
	{
		head = new Node;
		tail = new Node;
		head->next = tail;
		head->prev = NULL;
		tail->next = NULL;
		tail->prev = head;
	}

	class Iterator
	{

		friend class List<T>;
		friend class Bank;

	private:
		Node* curr;
	public:
		Iterator(Node* n = NULL)
		{
			curr = n;
		}
		Iterator(Iterator& rhs)
		{
			curr = rhs.curr;
		}
		Iterator& operator++()//post
		{
			curr = curr->next;
			return *this;
		}
		Iterator& operator--()//post
		{
			curr = curr->prev;
			return *this;
		}

		Iterator operator++(int)//pre
		{
			Iterator old = *this;
			++(*this);
			return old;

		}
		Iterator operator--(int)//pre
		{
			Iterator old = *this;
			--(*this);
			return old;
		}
		bool operator==(const Iterator& rhs)
		{
			if (curr == rhs.curr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator!=(const Iterator& rhs)
		{
			if (curr != rhs.curr)
			{
				return true;
			}
			else
			{
				return	false;
			}
		}


		T& operator*()
		{
			return curr->data;
		}


	};

	Iterator& begin()
	{
		Iterator itr(head->next);
		return itr;
	}
	Iterator& end()
	{
		Iterator itr(tail);
		return itr;
	}



	void print()
	{
		Iterator itr;
		for (itr = begin(); itr != end(); itr++)
		{
			cout << *itr << " ";
		}
	}

	Iterator Insert(Iterator it, T& d)
	{
		Node* p = it.curr;
		Node* q = p->prev;
		Node* temp = new Node;
		temp->data = d;
		temp->next = p;
		temp->prev = q;
		p->prev = temp;
		q->next = temp;
		Iterator ret(temp);
		return temp;
	}
	void insertAtHead(T& d)
	{
		Iterator it(head->next);
		Insert(it, d);
	}
	void insertAtTail(T& d)
	{
		Iterator it(tail);
		Insert(it, d);
	}

	void RemoveHead()
	{
		Node* temp = head->next;
		Node* h = temp->prev;
		h->next = temp->next;
		temp->next->prev = h;
		delete temp;
	}
	void RemoveTail()
	{
		Node* temp = tail->prev;
		Node* t = temp->next;
		t->prev = temp->prev;
		temp->prev->next = t;
		delete temp;
	}

	void deleteNode(Iterator& itr)
	{
		Node* temp = itr.curr;
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		itr = ++itr;
		delete temp;
	}

	Iterator& Search(int no)
	{
		Iterator itr;
		for (itr = begin(); itr != end(); itr++)
		{
			if (*itr == no)
			{
				return itr;
			}
		}

		itr.curr = 0;
		return itr;
	}


	/*~List()
	{
		Iterator itr1 = begin();
		Iterator itr2 = end();
		while (itr1 != itr2)
		{
			if (itr1 != NULL)
			{
				deleteNode(itr1);
					delete head;
					delete tail;
			}
		}
	}*/

};


class Bank;
class Transaction;
class Accounts;

class Transaction
{
private:
	int transaction_id;
	string transaction_time;
	string transaction_type;
	float ammount_transferd;
	int no;


public:
	Transaction()
	{

		transaction_id = -1;
		time_t curr_time;
		curr_time = time(NULL);
		char buff[26];

		ctime_s(buff, sizeof buff, &curr_time);
		transaction_time = buff;

		transaction_type = "0";
		ammount_transferd = 0.0;
		no = 0;
	}

	Transaction(int id, string ty, float a, int num)
	{

		transaction_id = id;
		time_t curr_time;
		curr_time = time(NULL);
		char buff[26];

		ctime_s(buff, sizeof buff, &curr_time);
		transaction_time = buff;

		transaction_type = ty;
		ammount_transferd = a;
		no = num;

	}

	friend ostream& operator<<(ostream& out, Transaction& obj)
	{
		out << "Transaction id: " << obj.transaction_id << endl;
		out << "Date & Time: " << obj.transaction_time << endl;
		out << "Type:" << obj.transaction_type << endl;
		out << "Ammount Transfered   " << obj.ammount_transferd << endl;
		out << "Account No: " << obj.no;
		out << "\n_____________________________________________________________________________\n\n";
		return out;
	}
	bool operator<(const Transaction& rhs)
	{
		if (transaction_id < rhs.transaction_id)
			return 1;
		else
			return 0;
	}
	bool operator==(Transaction& rhs)
	{
		if (transaction_id == rhs.transaction_id)
			return 1;
		else
			return 0;
	}

};

class Accounts
{
	friend Bank;
	friend Transaction;
private:
	int account_no;
	string account_Title;
	float account_balance;
	List<Transaction>transaction_history;

public:
	Accounts()
	{
		account_no = 0;
		account_Title = "0";
		account_balance = 0.0;
	}
	Accounts(int n, string t, float b)
	{
		account_no = n;
		account_Title = t;
		account_balance = b;

	}
	Accounts(Accounts& rhs)
	{
		account_no = rhs.account_no;
		account_Title = rhs.account_Title;
		account_balance = rhs.account_balance;

	}

	void credittransaction(float money, int id)
	{
		account_balance += money;

		Transaction Tt(id, type1, money, account_no);
		transaction_history.insertAtTail(Tt);
	}
	void debittransaction(float money, int id)
	{
		if (money > account_balance)
		{
			cout << "\nSorry transaction failed due to insufficient balance :(\n";
		}
		else
		{
			account_balance -= money;
		}
		Transaction Tt(id, type2, money, account_no);
		transaction_history.insertAtTail(Tt);
	}

	bool operator == (int no)
	{
		return account_no == no;
	}

	friend ostream& operator<<(ostream& out, Accounts& obj)
	{
		out << "Account NO: " << obj.account_no << endl;
		out << "Account Title: " << obj.account_Title << endl;
		out << "Account Balance: " << obj.account_balance << endl;
		out << "Transaction History: \n\n";
		obj.transaction_history.print();
		out << endl;
		out << endl;

		return out;
	}


	void Merge(Accounts& A1, Accounts& A2)
	{
		List<Transaction>::Iterator itr1 = A1.transaction_history.begin();
		List<Transaction>::Iterator itr2 = A1.transaction_history.end();
		List<Transaction>::Iterator itr3 = A2.transaction_history.begin();
		List<Transaction>::Iterator itr4 = A2.transaction_history.end();

		while (itr1 != itr2 || itr3 != itr4)
		{
			if (*itr1 < *itr3 && itr1 != itr2)
			{
				transaction_history.insertAtTail(*itr1);
				itr1++;
			}
			else if (*itr3 < *itr1 && itr3 != itr4)
			{
				transaction_history.insertAtTail(*itr3);
				itr3++;
			}
			else if (itr1 == itr2 && itr3 != itr4)
			{
				transaction_history.insertAtTail(*itr3);
				itr3++;
			}
			else if (itr1 != itr2 && itr3 == itr4)
			{
				transaction_history.insertAtTail(*itr1);
				itr1++;
			}
			else if (*itr1 == *itr3)
			{
				transaction_history.insertAtTail(*itr1);
				transaction_history.insertAtTail(*itr3);
				itr1++; itr3++;
			}
		}
	}

};




class Bank
{
	friend Accounts;
private:
	List<Accounts>accounts;
	string name;

public:
	int account_no = 1;
	int transaction_no = 1;
	Bank(string n)
	{
		name = n;
	}
	void AddNewAcc(string n, float a)
	{
		Accounts A(account_no, n, a);
		account_no++;
		accounts.insertAtTail(A);

	}

	void Print()
	{
		cout << "-----------------------------------" << name << "---------------------------" << endl << endl;
		accounts.print();
		cout << endl;
		cout << endl;
	}

	void Remove(int no)
	{
		List<Accounts>::Iterator itr = accounts.Search(no);

		if (itr.curr != 0) {
			accounts.deleteNode(itr);
			account_no--;
			cout << "Account with Account No " << no << " is removed :)\n";
		}
		else
		{
			cout << "Account with Acocunt NO " << no << " Not Found :( \n";
		}
	}

	void PerformTransaction(int id, string type, float a)
	{
		List<Accounts>::Iterator itr = accounts.Search(id);

		if (itr != 0)
		{
			Accounts* A = &*itr;
			if (type == "credit")
			{
				A->credittransaction(a, transaction_no);
				transaction_no++;
			}
			else if (type == "debit")
			{
				A->debittransaction(a, transaction_no);
				transaction_no++;
			}
			else
			{
				cout << "No such transaction type exists\n";
			}
		}
	}

	void Merge(int a, int b, string n)
	{
		Accounts A1;
		Accounts A2;
		List<Accounts> ::Iterator itr = accounts.Search(a);
		List<Accounts> ::Iterator itr2 = accounts.Search(b);

		if (itr.curr != 0 && itr2.curr != 0)
		{
			A1 = *itr;
			A2 = *itr2;
			if (A1.account_no != A2.account_no)
			{
				float ammount = A1.account_balance + A2.account_balance;

				Accounts A3(account_no, n, ammount);
				A3.Merge(A1, A2);
				accounts.insertAtTail(A3);
				accounts.deleteNode(itr);
				accounts.deleteNode(itr2);
				account_no--;
				cout << "Accounts " << A1.account_no << " and " << A2.account_no << " are merged\n";
			}
			else
			{
				cout << "same accounts can not be merge\n";
			}
		}
		else
		{
			cout << "\nNo account of such id is available\n";
		}

	}
};



int main()
{


	Bank bank("Zayn Bank limited");
	/*bank.AddNewAcc("Ali", 100);
	bank.AddNewAcc("Zain", 100);
	bank.AddNewAcc("chomu", 2000);
	bank.Print();
	bank.PerformTransaction(1, "credit", 100);
	bank.PerformTransaction(2, "credit", 100);
	bank.Print();
	bank.Merge(1, 3, "Ali chomu");
	bank.Print();*/

	int choice;
	int ammount;
	int accno;
	int accno2;

	string title;
	string type;
	cout << "\t\t---------------------- Welcome to Zayn Bank Limited ----------------------  \n\n";


	while (1)
	{

		cout << "\nChoose a number for the respective operation and ZERO ' 0 ' to terminate the programme : ";
		cout << "\n1 to Add account \n";
		cout << "2 to Perform Transaction\n";
		cout << "3 to Remove Account\n";
		cout << "4 to Merge Accounts\n";
		cout << "5 to Print Details\n";
		cout << "6 to clear screen\n";
		cout << "0 to Exit\n";
		cin >> choice;
		if (choice == 0)
		{
			cout << "\nProgram \n";
			break;
		}
		else if (choice == 1)
		{
			cout << "Enter the Account Title: ";
			cin >> title;
			cout << "Enter the Account balance: ";
			cin >> ammount;
			bank.AddNewAcc(title, ammount);
		}
		else if (choice == 2)
		{
			cout << "Enter the account number on which you want to perform transaction: ";
			cin >> accno;

			cout << "Enter the type of transaction you want to perform:  ";
			cin >> type;
			if (type == "credit" || type == "debit")
			{
				cout << "Enter the ammount to be " << type << "ed: ";
				cin >> ammount;
				bank.PerformTransaction(accno, type, ammount);
			}
			else
			{
				cout << "No such transaction type exists please enter 'credit' or 'debit' to process\n";
			}

		}
		else if (choice == 3)
		{
			cout << "Enter the account No to be removed: ";
			cin >> accno;
			bank.Remove(accno);
		}
		else if (choice == 4)
		{
			cout << "Enter the First Account n :";
			cin >> accno;
			cout << "Enter the Second Account n :";
			cin >> accno2;
			cout << "Enter the new account Title: ";
			cin >> title;

			bank.Merge(accno, accno2, title);
		}
		else if (choice == 5)
		{
			cout << "\n\nDETAILS: \n\n";
			bank.Print();
		}
		else if (choice == 6)
		{
			system("cls");
		}
		else
		{
			cout << "Please Enter the right Choice\n";
		}
	}
	cout << endl;
	system("pause");
	return 0;
}