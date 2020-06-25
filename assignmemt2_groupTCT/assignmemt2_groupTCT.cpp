
#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "Util.h"
#include "Node.h"
#include "Item.h"
#include "ItemNode.h"
#include "DVDAndRecordsNode.h"
#include "RentalNode.h"
#include "Customer.h"
#include "VIPCustomerNode.h"
#include "RegularCustomerNode.h"
#include "GuestCustomerNode.h"
#include "CustomerList.h"
#include "ItemList.h"

void openFile(fstream* itemf, fstream* customerf);
void openSaveFile(ofstream* customerof, ofstream* itemof);
void loadFileToLists(CustomerList* customerList, fstream* customerf, ItemList* itemList, fstream* itemf);
void uploadCustomer(fstream* customerf, string info[], CustomerList* customerList);
void createCustomer(string info[], CustomerList* customerList, fstream* customerf);
void updateCustomerRentals(Customer* newCustomer, fstream* customerf, string* rentalInfo);
void uploadItem(fstream* itemf, string info[], ItemList* itemList);
void getInfoItem(string info[], string itemInfo);
void createItem(string info[], ItemList* itemList);
void displayGroupOfCustomer(CustomerList* customerList);
void searchItemOrCustomer(CustomerList* customerList, ItemList* itemList);
void searchCustomer(CustomerList* customerList);
void searchItem(ItemList* itemList);
void rentItem(CustomerList* customerList, ItemList* itemList);
void returnItem(CustomerList* customerList, ItemList* itemList);
void addEditDeleteCustomer(CustomerList* customerList);
void addEditDeleteItem(ItemList* itemList);
void promoteCustomer(CustomerList* customerList);
void displayMenu();
void getFunction(string* function);
bool validateFunction(string function);
void exitMessage();
void saveListToFile(CustomerList* customerList, fstream* customerf, ItemList* itemList, fstream* itemf);
void closeFile(fstream* itemf, fstream* customerf);
void closeOuputFile(ofstream* itemof, ofstream* customerof);
void performFunction(string function, CustomerList* customerList, ItemList *itemList, fstream* customerf, fstream* itemf);

//open customer and item files
void openFile(fstream* itemf, fstream* customerf) {
	(*itemf).open("items.txt", ios::in || ios::out);
	(*customerf).open("customers.txt", ios::in || ios::out);
	if (!*itemf || !*customerf) {
		cerr << "Either file items.txt or file customers.txt is missing or both. Please include both files to run the program.";
		exit(-1);
	}
}

void openSaveFile(ofstream* customerof, ofstream* itemof) {
	(*customerof).open("customers.txt");
	(*itemof).open("items.txt");
}

//load customer and item file to customer list and item list
void loadFileToLists(CustomerList* customerList, fstream* customerf, ItemList* itemList, fstream* itemf) {
	string customerInfo[6];
	string itemInfo[7];
	uploadCustomer(customerf, customerInfo, customerList);
	uploadItem(itemf, itemInfo, itemList);
}

//load customer information from file
void uploadCustomer(fstream* customerf, string info[], CustomerList* customerList) {
	while (!(*customerf).eof()) {
		string customerInfo;
		getline(*customerf, customerInfo);
		Util::getInfo(info, &customerInfo);
		createCustomer(info, customerList, customerf);
	}
}

//create new customer and upload rental info from file
void createCustomer(string info[], CustomerList* customerList, fstream* customerf) {
	int noOfRentals = stoi(info[4]);
	string* rentalInfo = new string[noOfRentals];
	if (info[5].compare("VIP") == 0) {
		VIPCustomer newCustomer(info[0], info[1], info[2], info[3], stoi(info[4]));
		updateCustomerRentals(&newCustomer, customerf, rentalInfo);
		customerList->addCustomer(newCustomer);
	}
	else if (info[5].compare("Regular") == 0) {
		RegularCustomer newCustomer(info[0], info[1], info[2], info[3], stoi(info[4]));
		updateCustomerRentals(&newCustomer, customerf, rentalInfo);
		customerList->addCustomer(newCustomer);
	}
	else if (info[5].compare("Guest") == 0) {
		GuestCustomer newCustomer(info[0], info[1], info[2], info[3], stoi(info[4]));
		updateCustomerRentals(&newCustomer, customerf, rentalInfo);
		customerList->addCustomer(newCustomer);
	}
}

//update rental info from file
void updateCustomerRentals(Customer* newCustomer, fstream* customerf, string* rentalInfo) {
	int noOfRentals = newCustomer->getNoOfRentals();
	for (int i = 0; i < noOfRentals; ++i) {
		getline(*customerf, *(rentalInfo + i));
	}
	newCustomer->updateRental(rentalInfo);
}

//load item information from file
void uploadItem(fstream* itemf, string info[], ItemList* itemList) {
	while (!(*itemf).eof()) {
		string itemInfo;
		getline(*itemf, itemInfo);
		getInfoItem(info, itemInfo);
		createItem(info, itemList);
	}
}

//load item information from file
void getInfoItem(string info[], string itemInfo) {
	Util::getInfo(info, &itemInfo); //get all the information except genre
	int lastComma;
	size_t found = itemInfo.find(",");
	//check if there is genre information
	if (found != std::string::npos) {
		lastComma = itemInfo.find_last_of(",");
		info[0] = itemInfo.substr(lastComma + 1); //info[0] initially has 2 information
		itemInfo.erase(lastComma, itemInfo.length());
		info[6] = itemInfo; //info[6] now has the 1st information(item's ID)
	}
	else { info[6] = "game"; }
}

//create new item
void createItem(string info[], ItemList* itemList) {
	if (info[6].compare("game") == 0) {
		Item newItem(info[0], info[1], info[2], info[3], info[4], info[5]);
		itemList->addItem(newItem);
	}
	else if (info[6].compare("game") != 0) {
		DVDAndRecords newItem(info[6], info[0], info[1], info[2], info[3], info[4], info[5]);
		itemList->addItem(newItem);
	}
}

//display the menu of functions
void displayMenu() {
	cout << "Welcome to Genie’s video store" << endl;
	cout << "Enter an option below." << endl;
	cout << "1. Add a new item, update or delete an existing item" << endl;
	cout << "2. Add new customer or update an existing customer" << endl;
	cout << "3. Promote an existing customer" << endl;
	cout << "4. Rent an item" << endl;
	cout << "5. Return an item" << endl;
	cout << "6. Display all items" << endl;
	cout << "7. Display out - of - stock items" << endl;
	cout << "8. Display all customers" << endl;
	cout << "9. Display group of customers" << endl;
	cout << "10. Search items or customers" << endl;
	cout << "Exit." << endl;
}

//get a valid function
void getFunction(string* functionOption) {
	Util::getString(function, functionOption);
	while (!validateFunction(*functionOption)) {
		Util::errorDisplaying(invalidInput);
		Util::getString(function, functionOption);
	}
}

//check if a function is Exit or from 1 to 10
bool validateFunction(string function) {
	if (function.compare("Exit") == 0) { return true; }
	else {
		if (Util::isNumber(function)) {
			if (1 <= stoi(function) && stoi(function) <= 10) { return true; }
		}
	}
	return false;
}

void performFunction(string function, CustomerList* customerList, ItemList *itemList, fstream* customerf, fstream* itemf) {
	cout << endl << "--------------------------------" << endl;
	if (function.compare("Exit") == 0) {
		saveListToFile(customerList, customerf, itemList, itemf);
	}
	else {
		switch (stoi(function)) {
		case 1:
			addEditDeleteItem(itemList);
			break;
		case 2:
			addEditDeleteCustomer(customerList);
			break;
		case 3:
			promoteCustomer(customerList);
			break;
		case 4:
			rentItem(customerList, itemList);
			break;
		case 5:
			returnItem(customerList, itemList);
			break;
		case 6:
			itemList->displayItemList("all", all);
			break;
		case 7:
			itemList->displayItemList("0", noOfCopies);
			break;
		case 8:
			customerList->displayCustomer("all", all);
			break;
		case 9:
			displayGroupOfCustomer(customerList);
			break;
		case 10:
			searchItemOrCustomer(customerList, itemList);
			break;
		case 11:
			itemList->deleteItem();
			break;
		}
	}
	cout << endl << "--------------------------------" << endl;
}

void addEditDeleteItem(ItemList* itemList) {
	cout << "Full list of item" << endl;
	itemList->displayItemList("all", all);
	string function;
	Util::getString(newEditDeleteItem, &function);
	if (Util::isNumber(function)) {
		switch (stoi(function)) {
		case 1:
			itemList->addNewItem();
			break;
		case 2:
			itemList->editItem();
			break;
		case 3:
			itemList->deleteItem();
			break;
		default:
			Util::errorDisplaying(invalidInput);
			break;
		}
	}
	else { Util::errorDisplaying(invalidInput); }
}

void addEditDeleteCustomer(CustomerList* customerList) {
	customerList->displayCustomer("all", all);
	string function;
	Util::getString(newEditDeleteCustomer, &function);
	if (Util::isNumber(function)) {
		switch (stoi(function)) {
		case 1:
			customerList->addNewCustomer();
			break;
		case 2:
			customerList->editCustomer();
			break;
		case 3:
			customerList->deleteCustomer();
			break;
		default:
			Util::errorDisplaying(invalidInput);
			break;
		}
	}
	else { Util::errorDisplaying(invalidInput); }
}

void promoteCustomer(CustomerList* customerList) {
	if (customerList->displayCustomer("promote", promote)) {
		string cID;
		Util::getString(customerID, &cID);
		customerList->promoteCustomer(cID);
	}
}

void rentItem(CustomerList* customerList, ItemList* itemList) {
	itemList->displayItemList("all", all);
	customerList->displayCustomer("all", all);
	string cID, iID;
	Util::getString(itemID, &iID);
	string loanType;
	if (itemList->checkItemToRent(iID, &loanType)) {
		Util::getString(customerID, &cID);
		if (customerList->rentItem(cID, iID, loanType)) {
			itemList->rentItem(iID);
			Util::announceMessage(rented);
		}
	}
}

void returnItem(CustomerList* customerList, ItemList* itemList) {
	customerList->displayCustomer("all", all);
	string cID, iID;
	Util::getString(customerID, &cID);
	if (customerList->checkCustomerToReturnItem(cID)) {
		Util::getString(itemID, &iID);
		if (customerList->returnItem(cID, iID)) {
			itemList->returnItem(iID);
		}
	}
}

void displayGroupOfCustomer(CustomerList* customerList) {
	string customerType;
	cout << "Customer type  (enter number):" << endl << "1. VIP" << endl << "2. Regular" << endl << "3. Guest" << endl;
	Util::getString(&customerType);
	if (Util::isNumber(customerType)) {
		switch (stoi(customerType)) {
		case 1:
			customerList->displayCustomer("VIP", type);
			break;
		case 2:
			customerList->displayCustomer("Regular", type);
			break;
		case 3:
			customerList->displayCustomer("Guest", type);
			break;
		default:
			Util::errorDisplaying(invalidInput);
			break;
		}
	}
	else {
		Util::errorDisplaying(invalidInput);
	}
}

void searchItemOrCustomer(CustomerList* customerList, ItemList* itemList) {
	string searchFor;
	Util::getString(itemOrCustomer, &searchFor);
	if (Util::isNumber(searchFor) && (stoi(searchFor) == 1 || stoi(searchFor) == 2)) {
		switch (stoi(searchFor)) {
		case 1:
			searchItem(itemList);
			break;
		case 2:
			searchCustomer(customerList);
			break;
		default:
			Util::errorDisplaying(invalidInput);
		}
	}
	else {
		Util::errorDisplaying(invalidInput);
	}

}

void searchCustomer(CustomerList* customerList) {
	customerList->displayCustomer("all", all);
	string searchBy;
	Util::getString(searchCustomerBy, &searchBy);
	if (Util::isNumber(searchBy) && (stoi(searchBy) == 1 || stoi(searchBy) == 2)) {
		string info;
		Util::getString(searchInfo, &info);
		switch (stoi(searchBy)) {
		case 1:
			customerList->displayCustomer(info, name);
			break;
		case 2:
			customerList->displayCustomer(info, ID);
			break;
		}
	}
	else { Util::errorDisplaying(invalidInput); }
}

void searchItem(ItemList* itemList) {
	itemList->displayItemList("all", all);
	string searchBy;
	Util::getString(searchItemBy, &searchBy);
	if (Util::isNumber(searchBy) && (stoi(searchBy) == 1 || stoi(searchBy) == 2)) {
		string info;
		Util::getString(searchInfo, &info);
		switch (stoi(searchBy)) {
		case 1:
			itemList->displayItemList(info, title);
			break;
		case 2:
			itemList->displayItemList(info, ID);
			break;
		}
	}
	else { Util::errorDisplaying(invalidInput); }
}

//print out exit message
void exitMessage() {
	cout << "ASSIGNMENT 2 GROUP TCT" << endl;
	cout << "s3748840, s3748840@rmit.edu.vn, Cuong, Nguyen" << endl;
	cout << "s3757934, s3757934@rmit.edu.vn, Tien, Nguyen" << endl;
	cout << "s3747274, s3747274@rmit.edu.vn, Tam, Nguyen" << endl;
}

void saveListToFile(CustomerList* customerList, fstream* customerf, ItemList* itemList, fstream* itemf) {
	closeFile(itemf, customerf);
	ofstream customerof("customers.txt");
	ofstream itemof("items.txt");
	customerList->saveCustomer("all", all, &customerof);
	itemList->saveItem("all", all, &itemof);
	closeOuputFile(&itemof, &customerof);
	exitMessage();
	exit(-1);
}


//close 2 files when finish using
void closeOuputFile(ofstream* itemof, ofstream* customerof) {
	if (itemof != NULL) { (*itemof).close(); }
	if (customerof != NULL) { (*customerof).close(); }
}

//close 2 files when finish using
void closeFile(fstream* itemf, fstream* customerf) {
	if (itemf != NULL) { (*itemf).close(); }
	if (customerf != NULL) { (*customerf).close(); }
}

int main()
{
	CustomerList customerList;
	ItemList itemList;
	fstream itemf;
	fstream customerf;
	string function;

	openFile(&itemf, &customerf);
	loadFileToLists(&customerList, &customerf, &itemList, &itemf);

	do {
		displayMenu();
		getFunction(&function);
		performFunction(function, &customerList, &itemList, &customerf, &itemf);
	} while (true);
	return 0;
}
