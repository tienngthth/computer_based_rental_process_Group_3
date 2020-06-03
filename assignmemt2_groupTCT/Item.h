class Item
{
private:
	string title, rentalType, loanType, rentalStatus;
	string genre = "";
	float rentalFee = 0;
	int noOfCopies = 0;
	
public:
	string iID;

	Item() {};

	~Item() {};

	Item(string iID, string title, string rentalType, string loanType, string noOfCopies, string fee) {
		this->iID = iID;
		this->title = title;
		this->rentalType = rentalType;
		this->loanType = loanType;
		this->noOfCopies = stoi(noOfCopies);
		this->rentalFee = stof(fee);
		if (noOfCopies.compare("0") == 0) {
			rentalStatus = "Borrowed";
		}
		else {
			rentalStatus = "Available";
		}
	}

	//edit an item information (loan type, number of copies and rental fee)
	void editItem() {
		cout << endl << "Item current full info" << endl;
		cout << getInfo();
		cout << endl << endl << "You are allowed to edit loan type, number of copies and rental fee!" << endl;
		string newInfo;
		string confirmation;
		Util::getString(confirmEditLoanType, &confirmation);
		if (confirmation.compare("Y") == 0) {
			Util::getLoanType(&newInfo);
			this->loanType = newInfo;
		}
		Util::getString(confirmEditNoOfCopies, &confirmation);
		if (confirmation.compare("Y") == 0) {
			Util::getNoOfCopies(&newInfo);
			this->noOfCopies = stoi(newInfo);
		}
		Util::getString(confirmEditRentalFee, &confirmation);
		if (confirmation.compare("Y") == 0) {
			Util::getFee(&newInfo);
			this->rentalFee = stof(newInfo);
		}
	}
	
	//delete an item by deleting all of its copies
	void deleteAllCopies() {
		this->noOfCopies = 0;
	}

	//an item is rent
	void rentItem() {
		--this->noOfCopies;
		if (this->noOfCopies == 0) {
			this->rentalStatus = "Borrowed";
		} else { this->rentalStatus = "Available"; }
	}

	void returnItem() {
		if (this->noOfCopies == 0) {
			this->rentalStatus = "Available";
		}
		++this->noOfCopies;
	}

	string getBasicInfo() {
		return this->iID + "," + this->title + "," + this->rentalType + ","
			+ this->loanType + "," + to_string(this->noOfCopies) + "," + Util::trunkateFloat(this->rentalFee);
	}

	virtual string getInfo() {
		return getBasicInfo();
	}

	string getTitle() {
		return this->title;
	}

	int getNoOfCopies() {
		return this->noOfCopies;
	}

	string getLoanType() {
		return loanType;
	}

	string getRentalStatus() {
		return rentalStatus;
	}


};

class DVDAndRecords : public Item
{
public:
	DVDAndRecords() {};
	~DVDAndRecords() {};
	string genre;

	DVDAndRecords(string ID, string title, string rentalType, string loanType, string numOfCopies, string fee, string genre) : Item(ID, title, rentalType, loanType, numOfCopies, fee) {
		this->genre = genre;
	}

	string getInfo() {
		return getBasicInfo() + "," + this->genre;
	}
};
