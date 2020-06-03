enum findingField {
	name,
	ID,
	type,
	all,
	title,
	noOfCopies,
	promote,
};

class CustomerList
{
private:
	VIPCustomerNode *firstVIPCustomer = NULL;
	RegularCustomerNode *firstRegularCustomer = NULL;
	GuestCustomerNode *firstGuestCustomer = NULL;

	int noOfCustomers = 0;

	//get information for new customer
	void getNewCustomerInfo(string* info) {
		cout << endl << "Enter ID as format Cxxx (xxx is unique code of 3 digits): ";
		getCustomerID(&info[0]);
		Util::getString(customerName, &info[1]);
		Util::getString(customerAddress, &info[2]);
		Util::getPhone(&info[3]);
		getCustomerType(&info[4]);
	}

	//get new customer ID
	void getCustomerID(string *ID) {
		Util::getString(ID);
		while (!validateNewID(*ID)) {
			Util::getString(customerID, ID);
		}
	}

	//check if new ID is valid
	bool validateNewID(string ID) {
		if (!validateID(ID)) {
			Util::errorDisplaying(invalidInput);
			return false;
		} else if (isDuplicateID(ID)) {
			Util::errorDisplaying(duplicateID);
			return false;
		} else {return true;}
	}

	//check if ID is in valid format
	bool validateID(string ID) {
		if (ID.length() == 4) {
			if (ID[0] == 'C') {
				for (int i = 1; i <= 3; ++i) {
					if (isdigit(ID[i]) == 0) {return false;}
				}
				return true;
			}
		}
		return false;
	}

	//check if new ID already exists in the list
	bool isDuplicateID(string cID) {
		if (this->noOfCustomers == 0) {return false;}
		else {
			VIPCustomerNode* customer = searchVIP(cID);
			if (customer != NULL && customer->getCustomer()->cID.compare(cID) == 0) {
				return true;
			}
			else {
				RegularCustomerNode* regularReturner = searchRegular(cID);
				if (regularReturner != NULL && regularReturner->getCustomer()->cID.compare(cID) == 0) {
					return true;
				}
				else {
					GuestCustomerNode* guestRenterner = searchGuest(cID);
					if (guestRenterner != NULL && guestRenterner->getCustomer()->cID.compare(cID) == 0) {
						return true;
					}
					else {return false;}
				}
			}
		}
	}

	//get customer type for new customer
	void getCustomerType(string* type) {
		cout << endl << "Enter customer type (enter a number): " << endl << "1. VIP" << endl << "2. Regular" << endl << "3. Guest" << endl;
		Util::getString(type);
		while (type->compare("1") != 0 && type->compare("2") != 0 && type->compare("3") != 0) {
			Util::errorDisplaying(invalidInput);
			Util::getString(customerType, type);
		}
		if (type->compare("1") == 0) { *type = "VIP";
		}
		else if (type->compare("2") == 0) { *type = "Regular"; }
		else { *type = "Guest"; }
	}

	//search and get matched VIP customers full info by a given piece of info
	void getMatchedVIPInfo(string info, int field, int* count, string* customerInfo) {
		VIPCustomerNode* current = this->firstVIPCustomer;
		while (current != NULL) {
			if (compareInfo(current->getCustomer(), info, field)) {
				++*count;
				*customerInfo = current->getCustomer()->getInfo();
				++customerInfo;
			}
			current = (VIPCustomerNode*)current->getNext();
		}
	};

	//search and get matched Regular customers full info by a given piece of info
	void getMatchedRegularInfo(string info, int field, int* count, string* customerInfo) {
		RegularCustomerNode* current = this->firstRegularCustomer;
		while (current != NULL) {
			if (compareInfo(current->getCustomer(), info, field)) {
				++*count;
				*customerInfo = current->getCustomer()->getInfo();
				++customerInfo;
			}
			current = (RegularCustomerNode*)current->getNext();
		}
	};
	
	//search and get matched Guest customers full info by a given piece of info
	void getMatchedGuestInfo(string info, int field, int* count, string* customerInfo) {
		GuestCustomerNode* current = this->firstGuestCustomer;
		while (current != NULL) {
			if (compareInfo(current->getCustomer(), info, field)) {
				++*count;
				*customerInfo = current->getCustomer()->getInfo();
				++customerInfo;
			}
			current = (GuestCustomerNode*)current->getNext();
		}
	};

	//compare a customer's info with a given info
	bool compareInfo(Customer* customer, string info, int field) {
		switch (field) {
		case name:
			return customer->getName().compare(info) == 0;
		case ID:
			return customer->cID.compare(info) == 0;
		case type:
			return customer->getCustomerType().compare(info) == 0;
		case promote:
			if (customer->getCustomerType().compare("VIP") == 0) {return false;}
			return customer->getNoOfRenturnedRentals() > 3;
		case all:
			return true;
		default:
			return false;
		}
	}

	//delete a Gueset customer
	void deleteGuestCustomer(string cID) {
		GuestCustomerNode* current = this->firstGuestCustomer;
		GuestCustomerNode* deleteNode = searchGuest(cID);
		if (this->firstGuestCustomer != deleteNode) {
			while (current->getNext() != deleteNode) {
				current = (GuestCustomerNode*)current->getNext();
			}
			current->setNext(deleteNode->getNext());
			delete deleteNode;
		} else {
			this->firstGuestCustomer = (GuestCustomerNode*)current->getNext();
			delete current;
			current = NULL;
		}
	}

	//delete a Regular customer
	void deleteRegularCustomer(string cID) {
		RegularCustomerNode* current = this->firstRegularCustomer;
		RegularCustomerNode* deleteNode = searchRegular(cID);
		if (this->firstRegularCustomer != deleteNode) {
			while (current->getNext() != deleteNode) {
				current = (RegularCustomerNode*)current->getNext();
			}
			current->setNext(deleteNode->getNext());
			delete deleteNode;
		} else {
			this->firstRegularCustomer = (RegularCustomerNode*)current->getNext();
			delete current;
			current = NULL;
		}
	}

	//delete a VIP customer
	void deleteVIPCustomer(string cID) {
		VIPCustomerNode* current = this->firstVIPCustomer;
		VIPCustomerNode* deleteNode = searchVIP(cID);
		if (this->firstVIPCustomer != deleteNode) {
			while (current->getNext() != deleteNode) {
				current = (VIPCustomerNode*)current->getNext();
			}
			current->setNext(deleteNode->getNext());
			delete deleteNode;
		} else {
			this->firstVIPCustomer = (VIPCustomerNode*)current->getNext();
			delete current;
			current = NULL;
		}
	}

public:
	CustomerList() {};
	~CustomerList() {};

	//add new VIP customer to list
	void addCustomer(VIPCustomer customer) {
		if (this->firstVIPCustomer == NULL) {
			this->firstVIPCustomer = new VIPCustomerNode(customer);
		}
		else {
			VIPCustomerNode* current = this->firstVIPCustomer;
			VIPCustomerNode* newCustomer = new VIPCustomerNode(customer);
			while (current->getNext() != NULL) {
				current = (VIPCustomerNode*)current->getNext();
			}
			current->setNext(newCustomer);
		}
		++this->noOfCustomers;
	}

	//add new Regular customer to list
	void addCustomer(RegularCustomer customer) {
		if (this->firstRegularCustomer == NULL) {
			this->firstRegularCustomer = new RegularCustomerNode(customer);
		}
		else {
			RegularCustomerNode* current = this->firstRegularCustomer;
			RegularCustomerNode* newCustomer = new RegularCustomerNode(customer);
			while (current->getNext() != NULL) {
				current = (RegularCustomerNode*)current->getNext();
			}
			current->setNext(newCustomer);
		}
		++this->noOfCustomers;
	}

	//add new Guest customer to list
	void addCustomer(GuestCustomer customer) {
		if (this->firstGuestCustomer == NULL) {
			this->firstGuestCustomer = new GuestCustomerNode(customer);
		}
		else {
			GuestCustomerNode* current = this->firstGuestCustomer;
			GuestCustomerNode* newCustomer = new GuestCustomerNode(customer);
			while (current->getNext() != NULL) {
				current = (GuestCustomerNode*)current->getNext();
			}
			current->setNext(newCustomer);
		}
		++this->noOfCustomers;
	}

	//add new customer
	void addNewCustomer() {
		string info[5];
		getNewCustomerInfo(info);
		if (info[4].compare("VIP") == 0) {
			VIPCustomer newCustomer(info[0], info[1], info[2], info[3], 0);
			addCustomer(newCustomer);
		}
		else if (info[4].compare("Regular") == 0) {
			RegularCustomer newCustomer(info[0], info[1], info[2], info[3], 0);
			addCustomer(newCustomer);
		}
		else if (info[4].compare("Guest") == 0) {
			GuestCustomer newCustomer(info[0], info[1], info[2], info[3], 0);
			addCustomer(newCustomer);
		}
		Util::announceMessage(added);
	}

	//edit aa customer
	void editCustomer() {
		string cID;
		Util::getString(customerID, &cID);
		VIPCustomerNode* editedVIPCustomer = searchVIP(cID);
		if (editedVIPCustomer != NULL && editedVIPCustomer->getCustomer()->cID.compare(cID) == 0) {
			editedVIPCustomer->getCustomer()->editCustomer();
			Util::announceMessage(editted);
		} else {
			RegularCustomerNode* editedRegularCustomer = searchRegular(cID);
			if (editedRegularCustomer != NULL && editedRegularCustomer->getCustomer()->cID.compare(cID) == 0) {
				editedRegularCustomer->getCustomer()->editCustomer();
				Util::announceMessage(editted);
			} else {
				GuestCustomerNode* editedGuestCustomer = searchGuest(cID);
				if (editedGuestCustomer != NULL && editedGuestCustomer->getCustomer()->cID.compare(cID) == 0) {
					editedGuestCustomer->getCustomer()->editCustomer();
					Util::announceMessage(editted);
				} else { Util::errorDisplaying(noCustomerFound); }
			}
		}
	}

	//delete a customer by a given ID
	void deleteCustomer() {
		string cID;
		Util::getString(customerID, &cID);
		VIPCustomerNode* vipCustomer = searchVIP(cID);
		if (vipCustomer != NULL && vipCustomer->getCustomer()->cID.compare(cID) == 0) {
			deleteVIPCustomer(cID);
			Util::announceMessage(deleted);
		} else {
			RegularCustomerNode* regularCustomer = searchRegular(cID);
			if (regularCustomer != NULL && regularCustomer->getCustomer()->cID.compare(cID) == 0) {
				deleteRegularCustomer(cID);
				Util::announceMessage(deleted);
			} else {
				GuestCustomerNode* guestCustomer = searchGuest(cID);
				if (guestCustomer != NULL && guestCustomer->getCustomer()->cID.compare(cID) == 0) {
					deleteGuestCustomer(cID);
					Util::announceMessage(deleted);
				} else {Util::errorDisplaying(noCustomerFound);}
			}
		}
	}
	
	//display list of customers searched by a given way
	bool displayCustomer(string info, int field) {
		string* customerInfo = new string[this->noOfCustomers];
		string* tempPointer = customerInfo;
		if (this->noOfCustomers == 0) {
			Util::errorDisplaying(noCustomerFound);
			return false;
		} else {
			int countVIP = 0, countRegular = 0, countGuest = 0, matchedCustomers = 0;
			getMatchedVIPInfo(info, field, &countVIP, tempPointer);
			getMatchedRegularInfo(info, field, &countRegular, tempPointer + countVIP);
			getMatchedGuestInfo(info, field, &countGuest, tempPointer + countVIP + countRegular);
			matchedCustomers = countVIP + countRegular + countGuest;
			if (matchedCustomers == 0) {
				Util::errorDisplaying(noCustomerFound);
				return false;
			} else {
				Util::announceMessage(customerList);
				if (matchedCustomers == 1) {
					cout << *customerInfo << endl;
				} else {
					displaySortedCustomerInfo(customerInfo, matchedCustomers);
				} 
				return true;
			}
		}
	}

	void saveCustomer(string info, int field, ofstream* customerof) {
		string* customerInfo = new string[this->noOfCustomers];
		string* tempPointer = customerInfo;
		if (this->noOfCustomers == 0) {
		}
		else {
			int countVIP = 0, countRegular = 0, countGuest = 0, matchedCustomers = 0;
			getMatchedVIPInfo(info, field, &countVIP, tempPointer);
			getMatchedRegularInfo(info, field, &countRegular, tempPointer + countVIP);
			getMatchedGuestInfo(info, field, &countGuest, tempPointer + countVIP + countRegular);
			matchedCustomers = countVIP + countRegular + countGuest;
			if (matchedCustomers == 0) {
				Util::errorDisplaying(noCustomerFound);
			}
			else {
				if (matchedCustomers == 1) {
					*customerof << customerInfo << endl;
				}
				else {
					sortCustomerInfo(customerInfo, matchedCustomers);
					saveCustomerInfo(customerInfo, matchedCustomers,customerof);
				}
			}
		}
	}

	//sort matched customer infos and display
	void displaySortedCustomerInfo(string* customerInfo, int matchedCustomers) {
		sortCustomerInfo(customerInfo, matchedCustomers);
		for (int i = 0; i < matchedCustomers; ++i) {
			cout << *(customerInfo + i) << endl;
		}
	}

	void saveCustomerInfo(string* customerInfo, int matchedCustomers, ofstream* customerof) {
		string cName;
		cName = (*(customerInfo)).substr(0, 4);
		VIPCustomerNode* matchVIPCustomer = searchVIP(cName);
		RegularCustomerNode* matchRegularCustomer = searchRegular(cName);
		GuestCustomerNode* matchGuestCustomer = searchGuest(cName);
		if (matchVIPCustomer != NULL)
		{
			*customerof << *(customerInfo) ;
			matchVIPCustomer->getCustomer()->saveRental(customerof);
		}
		else if (matchRegularCustomer != NULL)
		{
			*customerof << *(customerInfo);
			matchRegularCustomer->getCustomer()->saveRental(customerof);
		}
		else
		{
			*customerof << *(customerInfo);
			matchGuestCustomer->getCustomer()->saveRental(customerof);
		}
		for (int i = 1; i < matchedCustomers; i++) {
			cName = (*(customerInfo + i)).substr(0, 4);
			matchVIPCustomer = searchVIP(cName);
			matchRegularCustomer = searchRegular(cName);
			matchGuestCustomer = searchGuest(cName);
			if (matchVIPCustomer != NULL)
			{
				*customerof << endl << *(customerInfo + i) ;
				matchVIPCustomer->getCustomer()->saveRental(customerof);
			}
			else if(matchRegularCustomer != NULL)
			{
				*customerof << endl << *(customerInfo + i) ;
				matchRegularCustomer->getCustomer()->saveRental(customerof);
			}
			else
			{
				*customerof << endl << *(customerInfo + i);
				matchGuestCustomer->getCustomer()->saveRental(customerof);
			}
		}
	}


	void sortCustomerInfo(string* customerInfo, int matchedCustomers) {
		string cName1, cName2, temp;
		for (int i = matchedCustomers - 1; i > 0; --i) {
			for (int j = 0; j < i; ++j) {
				cName1 = (*(customerInfo + j)).substr(0, 4);
				cName2 = (*(customerInfo + j + 1)).substr(0, 4);
				if (cName1.compare(cName2) > 0) {
					temp = *(customerInfo + j);
					*(customerInfo + j) = *(customerInfo + j + 1);
					*(customerInfo + j + 1) = temp;
				}
			}
		}
	}

	//promote a customer by a given id
	void promoteCustomer(string cID) {
		string info[6];
		int noOfCurrentReturnedRentals;
		string* rentalInfo;
		VIPCustomerNode* customer = searchVIP(cID);
		RegularCustomerNode* regularCustomer = searchRegular(cID);
		if (regularCustomer != NULL && regularCustomer->getCustomer()->cID.compare(cID) == 0) {
			if (regularCustomer->getCustomer()->getNoOfRenturnedRentals() > 3) {
				string customerInfo = regularCustomer->getCustomer()->getInfo();
				noOfCurrentReturnedRentals = regularCustomer->getCustomer()->getNoOfRenturnedRentals();
				Util::getInfo(info, &customerInfo);
				VIPCustomer upgradedCustomer(info[0], info[1], info[2], info[3], stoi(info[4]), noOfCurrentReturnedRentals);
				rentalInfo = new string[upgradedCustomer.getNoOfRentals()];
				regularCustomer->getCustomer()->getFullRentals(rentalInfo);
				upgradedCustomer.updateRental(rentalInfo);
				addCustomer(upgradedCustomer);
				deleteRegularCustomer(cID);
				Util::announceMessage(upgraded);
			} else {Util::errorDisplaying(nonVipPromote);}
		} else {
			GuestCustomerNode* guestCustomer = searchGuest(cID);
			if (guestCustomer != NULL && guestCustomer->getCustomer()->cID.compare(cID) == 0) {
				if (guestCustomer->getCustomer()->getNoOfRenturnedRentals() > 3) {
					string customerInfo = guestCustomer->getCustomer()->getInfo();
					noOfCurrentReturnedRentals = guestCustomer->getCustomer()->getNoOfRenturnedRentals();
					Util::getInfo(info, &customerInfo);
					RegularCustomer upgradedCustomer(info[0], info[1], info[2], info[3], stoi(info[4]), noOfCurrentReturnedRentals);
					rentalInfo = new string[upgradedCustomer.getNoOfRentals()];
					guestCustomer->getCustomer()->getFullRentals(rentalInfo);
					upgradedCustomer.updateRental(rentalInfo);
					addCustomer(upgradedCustomer);
					deleteGuestCustomer(cID);
					Util::announceMessage(upgraded);
				} else {Util::errorDisplaying(nonVipPromote);}
			} else {Util::errorDisplaying(noCustomerFound);}
		}
	}

	//a customer rent an item
	bool rentItem(string cID, string iID, string loanType) {
		VIPCustomerNode* VIPReturner = searchVIP(cID);
		if (VIPReturner != NULL && VIPReturner->getCustomer()->cID.compare(cID) == 0) {
			VIPReturner->getCustomer()->rentAnItem(iID);
		} else {
			RegularCustomerNode* regularReturner = searchRegular(cID);
			if (regularReturner != NULL && regularReturner->getCustomer()->cID.compare(cID) == 0) {
				regularReturner->getCustomer()->rentAnItem(iID);
			} else {
				GuestCustomerNode* guestRenterner = searchGuest(cID);
				if (guestRenterner != NULL && guestRenterner->getCustomer()->cID.compare(cID) == 0) {
					return guestRenterner->getCustomer()->rentAnItem(iID, loanType);
				} else {
					Util::errorDisplaying(noCustomerFound);
					return false;
				}
			}
		}
		return true;
	}

	//check if a customer is valid to return an item
	bool checkCustomerToReturnItem(string cID) {
		VIPCustomerNode* VIPReturner = searchVIP(cID);
		if (VIPReturner != NULL && VIPReturner->getCustomer()->cID.compare(cID) == 0) {
			VIPReturner->getCustomer()->displayRental();
			if (VIPReturner->getCustomer()->getNoOfRentals() != 0) {
				return true;
			} else {return false;}
		} else {
			RegularCustomerNode* regularReturner = searchRegular(cID);
			if (regularReturner != NULL && regularReturner->getCustomer()->cID.compare(cID) == 0) {
				regularReturner->getCustomer()->displayRental();
				if (regularReturner->getCustomer()->getNoOfRentals() != 0) {
					return true;
				} else {return false;}
			} else {
				GuestCustomerNode* guestRenterner = searchGuest(cID);
				if (guestRenterner != NULL && guestRenterner->getCustomer()->cID.compare(cID) == 0) {
					guestRenterner->getCustomer()->displayRental();
					if (guestRenterner->getCustomer()->getNoOfRentals() != 0) {
						return true;
					} else {return false;}
				} else {
					Util::errorDisplaying(noCustomerFound);
					return false; 
				}
			}
		}
	}

	//a customer return an item
	bool returnItem(string cID, string iID) {
		VIPCustomerNode* VIPReturner = searchVIP(cID);
		if (VIPReturner != NULL && VIPReturner->getCustomer()->cID.compare(cID) == 0) {
			return VIPReturner->getCustomer()->returnItem(iID);
		}
		else {
			RegularCustomerNode* regularReturner = searchRegular(cID);
			if (regularReturner != NULL && regularReturner->getCustomer()->cID.compare(cID) == 0) {
				return regularReturner->getCustomer()->returnItem(iID);
			}
			else {
				GuestCustomerNode* guestRenterner = searchGuest(cID);
				if (guestRenterner != NULL && guestRenterner->getCustomer()->cID.compare(cID) == 0) {
					return guestRenterner->getCustomer()->returnItem(iID);
				}
			}
		}
	}

	VIPCustomerNode* searchVIP(string cID) {
		VIPCustomerNode* current = this->firstVIPCustomer;
		while (current != NULL)
		{
			if (current->getCustomer()->cID.compare(cID) == 0) {
				break;
			}
			else {
				current = (VIPCustomerNode*)current->getNext();
			}
		} return current;
	}

	RegularCustomerNode* searchRegular(string cID) {
		RegularCustomerNode* current = this->firstRegularCustomer;
		while (current != NULL)
		{
			if (current->getCustomer()->cID.compare(cID) == 0) {
				break;
			}
			else {
				current = (RegularCustomerNode*)current->getNext();
			}
		}
		return current;
	}

	GuestCustomerNode* searchGuest(string cID) {
		GuestCustomerNode* current = this->firstGuestCustomer;
		while (current != NULL)
		{
			if (current->getCustomer()->cID.compare(cID) == 0) {
				break;
			}
			else {
				current = (GuestCustomerNode*)current->getNext();
			}
		}
		return current;
	}

};
