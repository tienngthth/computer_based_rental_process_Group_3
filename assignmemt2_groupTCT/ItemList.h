class ItemList
{
private:
	ItemNode *firstItem = NULL;
	DVDAndRecordsNode *firstDVDAndRecordsItem = NULL;
	int noOfItems = 0;

	//get information for new item
	void getNewItemInfo(string* info) {
		cout << endl << "Enter ID as format Ixxx-xxxx (xxx-xxxx is unique code of 7 digits): ";
		getItemID(&info[0]);
		Util::getString(itemTitle, &info[1]);
		getRentalType(&info[2]);
		Util::getLoanType(&info[3]);
		Util::getNoOfCopies(&info[4]);
		Util::getFee(&info[5]);
	}

	//get new item ID
	void getItemID(string *ID) {
		Util::getString(ID);
		while (!validateNewID(*ID)) {
			Util::getString(itemID, ID);
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
		if (ID.length() == 9) {
			if ((ID[0] == 'I') && (ID[4] == '-')) {
				for (int i = 1; i <= 3; ++i) {
					if (isdigit(ID[i]) == 0) { return false;}
				}
				for (int i = 5; i < 9; i++)
				{
					if (!isdigit(ID[i])) {return false;}
				}
				return true;
			}
		}
		return false;
	}

	//check if new ID already exists in the list
	bool isDuplicateID(string iID) {
		if (noOfItems == 0) {return false;}
		else {
			DVDAndRecordsNode* item = searchDVDAndRecords(iID);
			if (item != NULL && item->getItem()->iID.compare(iID) == 0) {
				return true;
			} else {
				ItemNode* item = searchItem(iID);
				if (item != NULL && item->getItem()->iID.compare(iID) == 0) {
					return true;
				}
				else {return false;}
			}
		}
	}

	//get rental type for new item
	void getRentalType(string* type) {
		cout << endl << "Enter rental type (enter a number): " << endl << "1. DVD" << endl << "2. Game" << endl << "3. Records" << endl;
		Util::getString(type);
		while (type->compare("1") != 0 && type->compare("2") != 0 && type->compare("3") != 0) {
			Util::errorDisplaying(invalidInput);
			Util::getString(rentalType, type);
		}
		if (type->compare("1") == 0) { *type = "DVD"; }
		else if (type->compare("2") == 0) { *type = "Game"; }
		else { *type = "Records"; }
	}

	//get genre for new dvd and record item
	void getGenre(string* genre) {
		cout << endl << "Enter genre (enter a number): " << endl << "1. Action" << endl << "2. Horror" << endl << "3. Drama" << endl << "4. Comedy" << endl;
		Util::getString(genre);
		while (genre->compare("1") != 0 && genre->compare("2") != 0 && genre->compare("3") != 0 && genre->compare("4") != 0) {
			Util::errorDisplaying(invalidInput);
			Util::getString(itemGenre, genre);
		}
		if (genre->compare("1") == 0) { *genre = "Action"; }
		else if (genre->compare("2") == 0) { *genre = "Horror"; }
		else if (genre->compare("3") == 0) { *genre = "Drama"; }
		else { *genre = "Comedy"; }
	}
	
	//search and get matched Game items full info by a given piece of info
	void getMatchedItem(string info, int field, int* count, string* itemInfo) {
		ItemNode* current = this->firstItem;
		while (current != NULL) {
			if (compareInfo(current->getItem(), info, field)) {
				++*count;
				*itemInfo = current->getItem()->getInfo();
				++itemInfo;
			}
			current = (ItemNode*)current->getNext();
		}
	};

	//search and get matched DVD and Records items full info by a given piece of info
	void getMatchedDVDAndRecordsItem(string info, int field, int* count, string* itemInfo) {
		DVDAndRecordsNode* current = this->firstDVDAndRecordsItem;
		while (current != NULL) {
			if (compareInfo(current->getItem(), info, field)) {
				++*count;
				*itemInfo = current->getItem()->getInfo();
				++itemInfo;
			}
			current = (DVDAndRecordsNode*)current->getNext();
		}
	}
	
	//compare an item's info with a given info
	bool compareInfo(Item* item, string info, int field) {
		switch (field) {
		case title:
			return item->getTitle().compare(info) == 0;
			break;
		case ID:
			return item->iID.compare(info) == 0;
			break;
		case noOfCopies:
			return item->getNoOfCopies() == 0;
			break;
		case all:
			return true;
			break;
		default:
			return false;
		}
	}

public:
	ItemList() {};
	~ItemList() {};

	//add new Game item to list
	void addItem(Item item) {
		if (this->firstItem == NULL) {
			this->firstItem = new ItemNode(item);
		}
		else {
			ItemNode* current = this->firstItem;
			ItemNode* newItem = new ItemNode(item);
			while (current->getNext() != NULL) {
				current = (ItemNode*)current->getNext();
			}
			current->setNext(newItem);
		}
		++this->noOfItems;
	}
	
	//add new DVD and Record item to list
	void addItem(DVDAndRecords item) {
		if (this->firstDVDAndRecordsItem == NULL) {
			this->firstDVDAndRecordsItem = new DVDAndRecordsNode(item);
		}
		else {
			DVDAndRecordsNode* current = this->firstDVDAndRecordsItem;
			DVDAndRecordsNode* newItem = new DVDAndRecordsNode(item);
			while (current->getNext() != NULL) {
				current = (DVDAndRecordsNode*)current->getNext();
			}
			current->setNext(newItem);
		}
		++this->noOfItems;
	}

	//add new item 
	void addNewItem() {
		string info[7];
		getNewItemInfo(info);
		if (info[2].compare("DVD") == 0) {
			getGenre(&info[6]);
			DVDAndRecords newItem(info[0], info[1], info[2], info[3], info[4], info[5], info[6]);
			addItem(newItem);
		}
		else if (info[2].compare("Game") == 0) {
			Item newItem(info[0], info[1], info[2], info[3], info[4], info[5]);
			addItem(newItem);
		}
		else if (info[2].compare("Records") == 0) {
			getGenre(&info[6]);
			DVDAndRecords newItem(info[0], info[1], info[2], info[3], info[4], info[5], info[6]);
			addItem(newItem);
		}
		Util::announceMessage(added);
	}

	//edit an item
	void editItem() {
		string iID;
		Util::getString(itemID, &iID);
		ItemNode* editedItem = searchItem(iID);
		if (editedItem != NULL && editedItem->getItem()->iID.compare(iID) == 0) {
			editedItem->getItem()->editItem();
			Util::announceMessage(editted);
		}
		else {
			DVDAndRecordsNode* editedDVDAndRecords = searchDVDAndRecords(iID);
			if (editedDVDAndRecords != NULL && editedDVDAndRecords->getItem()->iID.compare(iID) == 0) {
				editedDVDAndRecords->getItem()->editItem();
				Util::announceMessage(editted);
			}
			else {
				Util::errorDisplaying(noItemFound);
			}
		}
	}

	//search for game item by given id
	ItemNode* searchItem(string iID) {
		ItemNode* current = this->firstItem;
		while (current != NULL) {
			if (current->getItem()->iID.compare(iID) == 0) { break;}
			else {current = (ItemNode*)current->getNext();}
		} return current;
	}

	//search for dvd and record item by given id
	DVDAndRecordsNode* searchDVDAndRecords(string iID) {
		DVDAndRecordsNode* current = this->firstDVDAndRecordsItem;
		while (current != NULL) {
			if (current->getItem()->iID.compare(iID) == 0) {break;}
			else {current = (DVDAndRecordsNode*)current->getNext();}
		}
		return current;
	}

	//delete an item
	void deleteItem() {
		string iID;
		Util::getString(itemID, &iID);
		ItemNode* itemDeleted = searchItem(iID);
		if (itemDeleted != NULL && itemDeleted->getItem()->iID.compare(iID) == 0) {
			itemDeleted->getItem()->deleteAllCopies();
			Util::announceMessage(deleted);
		} else {
			DVDAndRecordsNode* DVDAndRecordsDeleted = searchDVDAndRecords(iID);
			if (DVDAndRecordsDeleted != NULL && DVDAndRecordsDeleted->getItem()->iID.compare(iID) == 0) {
				DVDAndRecordsDeleted->getItem()->deleteAllCopies();
				Util::announceMessage(deleted);
			} else {Util::errorDisplaying(noItemFound);}
		}
	}

	//display list of items searched by a given way
	void displayItemList(string info, int field) {
		string* itemInfo = new string[this->noOfItems];
		string* p = itemInfo;
		if (this->noOfItems == 0) {Util::errorDisplaying(noItemFound);} 
		else {
			int countItem = 0, countDVDAndRecords = 0, matchedItems = 0;
			getMatchedItem(info, field, &countItem, itemInfo);
			getMatchedDVDAndRecordsItem(info, field, &countDVDAndRecords, itemInfo + countItem);
			matchedItems = countItem + countDVDAndRecords;
			if (matchedItems == 0) {Util::errorDisplaying(noItemFound);}
			else {
				Util::announceMessage(itemList);
				if (matchedItems == 1) {cout << *itemInfo << endl;}
				else { displaySortedItemInfo(itemInfo, matchedItems); }
			}
		}
	}

	void sortItemInfo(string* itemInfo, int matchedItems) {
		string iTitle1, iTitle2, temp;
		for (int i = matchedItems - 1; i > 0; --i) {
			for (int j = 0; j < i; ++j) {
				iTitle1 = (*(itemInfo + j)).substr(0, 9);
				iTitle2 = (*(itemInfo + j + 1)).substr(0, 9);
				if (iTitle1.compare(iTitle2) > 0) {
					temp = *(itemInfo + j);
					*(itemInfo + j) = *(itemInfo + j + 1);
					*(itemInfo + j + 1) = temp;
				}
			}
		}
	}

	//sort matched items infos and display
	void displaySortedItemInfo(string* itemInfo, int matchedItems) {
		sortItemInfo(itemInfo, matchedItems);
		for (int i = 0; i < matchedItems; ++i) {
			cout << *(itemInfo + i) << endl;
		}
	}

	//check if the given item is valid to be rent
	bool checkItemToRent(string iID, string* loanType) {
		ItemNode* itemRent = searchItem(iID);
		if (itemRent != NULL && itemRent->getItem()->iID.compare(iID) == 0) {
			if (itemRent->getItem()->getNoOfCopies() == 0) {
				Util::errorDisplaying(itemOutOfStock);
				return false;
			} else {
				*loanType = itemRent->getItem()->getLoanType();
				return true;
			}
		} else {
			DVDAndRecordsNode* DVDAndRecordRent = searchDVDAndRecords(iID);
			if (DVDAndRecordRent != NULL && DVDAndRecordRent->getItem()->iID.compare(iID) == 0) {
				if (DVDAndRecordRent->getItem()->getNoOfCopies() == 0) {
					Util::errorDisplaying(itemOutOfStock);
					return false;
				} else {
					*loanType = DVDAndRecordRent->getItem()->getLoanType();
					return true;
				}
			} else {
				Util::errorDisplaying(noItemFound);
				return false;
			}
		}
	}

	void rentItem(string iID) {
		ItemNode* itemReturned = searchItem(iID);
		if (itemReturned != NULL && itemReturned->getItem()->iID.compare(iID) == 0) {
			itemReturned->getItem()->rentItem();
		} else {
			DVDAndRecordsNode* DVDAndRecordReturned = searchDVDAndRecords(iID);
			if (DVDAndRecordReturned != NULL && DVDAndRecordReturned->getItem()->iID.compare(iID) == 0) {
				DVDAndRecordReturned->getItem()->rentItem();
			} else {
				Util::errorDisplaying(noItemFound);
			}
		}
	}

	void returnItem(string iID) {
		ItemNode* itemReturned = searchItem(iID);
		if (itemReturned != NULL && itemReturned->getItem()->iID.compare(iID) == 0) {
			itemReturned->getItem()->returnItem();
		}
		else {
			DVDAndRecordsNode* DVDAndRecordReturned = searchDVDAndRecords(iID);
			if (DVDAndRecordReturned != NULL && DVDAndRecordReturned->getItem()->iID.compare(iID) == 0) {
				DVDAndRecordReturned->getItem()->returnItem();
			}
		}
	}	

	void saveItem(string info, int field, ofstream* itemof) {
		string* itemInfo = new string[this->noOfItems];
		string* p = itemInfo;
		if (this->noOfItems == 0) { Util::errorDisplaying(noItemFound); }
		else {
			int countItem = 0, countDVDAndRecords = 0, matchedItems = 0;
			getMatchedItem(info, field, &countItem, itemInfo);
			getMatchedDVDAndRecordsItem(info, field, &countDVDAndRecords, itemInfo + countItem);
			matchedItems = countItem + countDVDAndRecords;
			if (matchedItems == 0) { *itemof << ""; }
			else {
				if (matchedItems == 1) { *itemof << *itemInfo << endl; }
				else { 
					sortItemInfo(itemInfo, matchedItems);
					for (int i = 0; i < matchedItems-1; ++i) {
						*itemof << *(itemInfo + i) << endl;
					}
					*itemof << *(itemInfo + matchedItems - 1);
				}
			}
		}
	}
};
