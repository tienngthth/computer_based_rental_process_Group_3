class Customer {
private:
	string name, address, phone, customerType;
	int noOfRentals = 0;
	int noOfRentalsReturned = 0;
	RentalNode *firstRental = NULL;

	RentalNode* search(string iID) {
		RentalNode* current = this->firstRental;
		while (current != NULL)
		{
			if (current->getData().compare(iID) == 0)
				break;
			else
				current = (RentalNode*)current->getNext();
		}
		return current;
	}

public:
	string cID;

	Customer(string cID, string name, string address, string phone, int noOfRentals) {
		this->cID = cID;
		this->name = name;
		this->address = address;
		this->phone = phone;
		this->noOfRentals = noOfRentals;
	};

	Customer() {};

	~Customer() {};

	int getNoOfRentals() {
		return this->noOfRentals;
	};

	//update rental from file, do not increase the total number of rentals
	void updateRental(string* rentalInfo) {
		for (int i = 0; i < this->noOfRentals; ++i) {
			rentItem(*(rentalInfo + i));
			--this->noOfRentals;
		}
	}

	//rent a new item according to customer type
	virtual	void rentAnItem() {}

	// rent a new item
	void rentItem(string iID) {
		if (this->firstRental == NULL) {
			this->firstRental = new RentalNode(iID);
		}
		else {
			RentalNode* current = this->firstRental;
			RentalNode* newRental = new RentalNode(iID);
			while (current->getNext() != NULL) {
				current = (RentalNode*)current->getNext();
			}
			current->setNext(newRental);
		}
		++this->noOfRentals;
	};

	//edit a customer info
	void editCustomer() {
		Util::announceMessage(customerInfo);
		displayInfo();
		Util::announceMessage(customerEditRange);
		string newInfo;
		string confirmation;
		Util::getString(confirmEditName, &confirmation);
		if (confirmation.compare("Y") == 0) {
			Util::getString(customerName, &newInfo);
			this->name = newInfo;
		}
		Util::getString(confirmEditAddress, &confirmation);
		if (confirmation.compare("Y") == 0) {
			Util::getString(customerAddress, &newInfo);
			this->address= newInfo;
		}
		Util::getString(confirmEditPhone, &confirmation);
		if (confirmation.compare("Y") == 0) {
			Util::getPhone(&newInfo);
			this->phone = stoi(newInfo);
		}
	}

	//get number of current rentals
	int getNoOfRenturnedRentals() {
		return this->noOfRentalsReturned;
	}

	//new number of returned rentals after promoted is reduced by 4 (customer has to returned equals or more than 4 to be promoted)
	void resetNoOfRentalsReturned(int noOfRentalsReturned) {
		this->noOfRentalsReturned = noOfRentalsReturned - 4;
	}

	//get all current rentals
	void getFullRentals(string* rentalInfo) {
		RentalNode* current = this->firstRental;
		while (current != NULL) {
			*rentalInfo = current->getData();
			++rentalInfo;
			current = (RentalNode*)current->getNext();
		}
	}

	//display all rentals
	void displayRental() {
		if (this->noOfRentals == 0) {
			Util::announceMessage(notRenting);
		}
		else {
			Util::announceMessage(rentalList);
			RentalNode* current = this->firstRental;
			while (current->getNext() != NULL) {
				cout << current->getData() << endl;
				current = (RentalNode*)current->getNext();
			}
			cout << current->getData() << endl;
		}
	}

	void saveRental(ofstream* customerof) {
		if (this->noOfRentals == 0) {
		}
		else {
			RentalNode* current = this->firstRental;
			while (current->getNext() != NULL) {
				*customerof << endl << current->getData() ;
				current = (RentalNode*)current->getNext();
			}
			*customerof << endl << current->getData();
		}
	}

	//return an item
	bool returnItem(string iID) {
		RentalNode* current = this->firstRental;
		RentalNode* deleteNode = search(iID);
		if (deleteNode == NULL || deleteNode->getData().compare(iID) != 0) {
			Util::errorDisplaying(noItemFound);
			return false;
		}
		else {
			++this->noOfRentalsReturned;
			--this->noOfRentals;
			Util::announceMessage(returned);
			if (this->firstRental != deleteNode) {
				while (current->getNext() != deleteNode) {
					current = (RentalNode*)current->getNext();
				}
				current->setNext(deleteNode->getNext());
				delete deleteNode;
			}
			else {
				this->firstRental = (RentalNode*)current->getNext();
				delete current;
				current = NULL;
			}
			return true;
		}
	};

	void setCustomerType(string type) {
		this->customerType = type;
	}

	void displayInfo() {
		cout << getInfo();
		displayRental();
	}

	string getInfo() {
		return this->cID + "," + this->name + "," + this->address + "," + this->phone + "," + to_string(noOfRentals) + "," + this->customerType;
	}

	string getName() {
		return this->name;
	}

	string getCustomerType() {
		return this->customerType;
	}

};

class VIPCustomer : public Customer {
private:
	int rewardPoint = 100;

	void usePoint() {
		this->rewardPoint -= 100;
	}

	void accumulatePoint() {
		this->rewardPoint += 10;
	}

public:
	VIPCustomer(string cID, string name, string address, string phone, int noOfRentals) : Customer(cID, name, address, phone, noOfRentals) {
		setCustomerType("VIP");
	};

	VIPCustomer(string cID, string name, string address, string phone, int noOfRentals, int noOfRentalsReturned) : Customer(cID, name, address, phone, noOfRentals) {
		setCustomerType("VIP");
		resetNoOfRentalsReturned(noOfRentalsReturned);
	};

	VIPCustomer() {};

	~VIPCustomer() {};

	void rentAnItem(string iID) {
		if (this->rewardPoint >= 100) {
			string confirmation;
			Util::getString(confirmUsePoint, &confirmation);
			if (confirmation.compare("Y") == 0) {
				Util::announceMessage(freeRent);
				usePoint();
			} else {
				Util::announceMessage(noFreeRent);
			}
		}
		rentItem(iID);
		accumulatePoint();
	}
};

class RegularCustomer : public Customer {
public:
	RegularCustomer(string cID, string name, string address, string phone, int noOfRentals) : Customer(cID, name, address, phone, noOfRentals) {
		setCustomerType("Regular");
	};

	RegularCustomer(string cID, string name, string address, string phone, int noOfRentals, int noOfRentalsReturned) : Customer(cID, name, address, phone, noOfRentals) {
		setCustomerType("Regular");
		resetNoOfRentalsReturned(noOfRentalsReturned);
	};

	RegularCustomer() {};

	~RegularCustomer() {};

	void rentAnItem(string iID) {
		rentItem(iID);
	}
};

class GuestCustomer : public Customer {
public:
	GuestCustomer(string cID, string name, string address, string phone, int noOfRentals) : Customer(cID, name, address, phone, noOfRentals) {
		setCustomerType("Guest");
	};

	GuestCustomer() {};

	~GuestCustomer() {};

	bool rentAnItem(string iID, string loanType) {
		if (getNoOfRentals() >= 2) {
			Util::errorDisplaying(guestCustomerMaxRent);
			return false;
		}
		else if (loanType.compare("2-day") == 0) {
			Util::errorDisplaying(guestCustomerInvalidLoanType);
			return false;
		}
		else {
			rentItem(iID);
			return true;
		}
		return false;
	}
};
