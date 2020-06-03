enum ERROR {
	invalidInput,
	duplicateID,
	invalidToPromote,
	nonVipPromote,
	noCustomerFound,
	noItemFound,
	itemOutOfStock,
	guestCustomerInvalidLoanType,
	guestCustomerMaxRent,
};

enum MESSAGE {
	rented,
	returned,
	added,
	deleted,
	upgraded,
	editted,
	freeRent,
	noFreeRent,
	customerInfo,
	itemInfo,
	rentalList,
	itemList,
	customerList,
	customerEditRange,
	itemEditRange,
	notRenting,
};

enum OPTION {
	searchInfo,
	searchCustomerBy,
	searchItemBy,
	itemOrCustomer,
	newEditDeleteCustomer,
	newEditDeleteItem,
	function,
	customerID,
	itemID,
	itemTitle,
	rentalType,
	loan,
	itemNoOfCopies,
	itemFee,
	itemGenre,
	customerPhone,
	customerName,
	customerAddress,
	customerType,
	confirmUsePoint,
	confirmEditLoanType,
	confirmEditNoOfCopies,
	confirmEditRentalFee,
	confirmEditName,
	confirmEditPhone,
	confirmEditAddress,
};

class Util {
private:
	//check if a number is a valid float
	static bool validFloat(string number)
	{
		int dotCounting = 0;
		for (int i = 0; i < number.length(); i++)
		{	//check if a character is not a number and is not a dot
			if (isdigit(number[i]) == 0 && number[i] != '.') { return false; }
			//check if number of dots is more than 1
			if (dotCounting > 1) { return false; }
			if (number[i] == '.') { dotCounting++; }
		}
	}

	//check if phone is valid
	static bool validatePhone(string phone) {
		if (phone.length() > 15 || phone.length() < 5) {
			Util::errorDisplaying(invalidInput);
			return false;
		}
		else if (!Util::isNumber(phone)) {
			Util::errorDisplaying(invalidInput);
			return false;
		}
		else { return true; }
	}
public:

	static string removeWhiteSpace(string text) {
		int lastIndex, firstIndex;
		for (lastIndex = text.length() - 1; lastIndex > 0; lastIndex--) {
			if (!(text[lastIndex] == ' ') && !(text[lastIndex] == '\t')) {
				break;
			}
		}
		for (firstIndex = 0; firstIndex < lastIndex; ++firstIndex) {
			if (!(text[firstIndex] == ' ') && !(text[firstIndex] == '\t')) {
				break;
			}
		}
		string newString;
		for (int i = firstIndex; i <= lastIndex; ++i) {
			newString += text[i];
		}
		return newString;
	}

	static void getInfo(string listOfInfo[], string* info) {
		int lastComma;
		for (int i = 5; i > 0; --i) {
			lastComma = (*info).find_last_of(",");
			listOfInfo[i] = (*info).substr(lastComma + 1);
			(*info).erase(lastComma, (*info).length());
		}
		listOfInfo[0] = *info;
	}

	//check if a string is a number
	static bool isNumber(string input) {
		if (input.length() < 1) { return false; }
		else {
			for (int i = 0; i < input.length(); ++i) {
				if (!isdigit(input[i])) { return false; }
			}
		}
		return true;
	}

	static string trunkateFloat(float number) {
		string trunkatedNumber = to_string(number);
		int decimal = trunkatedNumber.find_last_of(".");
		if (decimal != -1) {
			trunkatedNumber.erase(decimal + 3, trunkatedNumber.length());
		}
		return trunkatedNumber;
	}

	static void errorDisplaying(int error) {
		switch (error) {
		case invalidInput:
			cout << endl << "Invalid input." << endl;
			break;
		case duplicateID:
			cout << endl << "This ID already exists" << endl;
			break;
		case invalidToPromote:
			cout << endl << "This account hasn't returned enough books to be promoted" << endl;
			break;
		case nonVipPromote:
			cout << endl << "Return more items to be promoted" << endl;
			break;
		case noCustomerFound:
			cout << endl << "No customer found" << endl;
			break;
		case noItemFound:
			cout << endl << "No item found" << endl;
			break;
		case itemOutOfStock:
			cout << endl << "Item out of stock" << endl;
			break;
		case guestCustomerInvalidLoanType:
			cout << endl << "Guest customer can not rent a 2-day item" << endl;
			break;
		case guestCustomerMaxRent:
			cout << endl << "You have reached the maximum number of rentals. Upgrade to rent more" << endl;
			break;
		default	:
			errorDisplaying(invalidInput);
		}
	}

	static void announceMessage(int message) {
		switch (message) {
		case returned:
			cout << endl << "Successfully returned!" << endl;
			break;
		case rented:
			cout << endl << "Successfully rented!" << endl;
			break;
		case added:
			cout << endl << "Successfully added!" << endl;
			break;
		case editted:
			cout << endl << "Successfully editted" << endl;
			break;
		case deleted:
			cout << endl << "Successfully deleted" << endl;
			break;
		case upgraded:
			cout << endl << "Successfully upgraded" << endl;
			break;
		case freeRent:
			cout << endl << "This item is rented for free. You have used 100 points" << endl;
			break;
		case noFreeRent:
			cout << endl << "This item is not rented for free. You have saved your points" << endl;
			break;
		case customerInfo:
			cout << endl << "Customer's current full info" << endl;
			break;
		case customerEditRange:
			cout << endl << "You are allowed to edit name, home address and phone!" << endl;
			break;
		case notRenting:
			cout << endl << "This customer is not renting any items" << endl;
			break;
		case rentalList:
			cout << endl << "Full list of rentals" << endl;
			break;
		case itemList:
			cout << endl << "List of matched items (iID/title/rentalType/loanType/noOfCopies/rentalFee/rentalStatus/genre(optional))" << endl;
			break;
		case customerList:
			cout << endl << "List of matched customers (cID/name/address/phone/noOfRentals/customerType)" << endl;
			break;
		}
	}

	//get loan type for new item
	static void getLoanType(string* type) {
		cout << endl << "Enter loan type (enter a number): " << endl << "1. 1-week loan" << endl << "2. 2-day loan" << endl;
		getString(type);
		while (type->compare("1") != 0 && type->compare("2") != 0) {
			Util::errorDisplaying(invalidInput);
			Util::getString(loan, type);
		}
		if (type->compare("1") == 0) { *type = "1-week"; }
		else { *type = "2-day"; }
	}

	//get number of copies for new item
	static void getNoOfCopies(string* number) {
		cout << endl << "Enter number of copies (an integer): ";
		getString(number);
		while (!isNumber(*number)) {
			Util::errorDisplaying(invalidInput);
			Util::getString(itemNoOfCopies, number);
		}
	}

	static void getFee(string* number) {
		cout << endl << "Enter rental fee (a float): ";
		getString(number);
		while (!validFloat(*number)) {
			getString(itemFee, number);
		}
	}

	//get phone for new customer
	static void getPhone(string* phone) {
		cout << endl << "Enter phone (include numbers only, from 5 - 15 numbers): ";
		Util::getString(phone);
		while (!validatePhone(*phone)) {
			getString(customerPhone, phone);
		}
	}

	static void getString(string* aString) {
		getline(cin, *aString);
		*aString = removeWhiteSpace(*aString);
	}

	static void getString(int askFor, string* aString) {
		switch (askFor) {
		case (searchItemBy):
			cout << endl << "Search by (enter number): " << endl << "1. By title" << endl << "2. By ID" << endl;
			break;
		case (itemOrCustomer):
			cout << "Search for (enter a number): " << endl << "1. Search for item" << endl << "2. Search for customer" << endl;
			break;
		case (searchCustomerBy):
			cout << endl << "Search by (enter number): " << endl << "1. By name" << endl << "2. By ID" << endl;
			break;
		case (searchInfo) :
			cout << endl << "Enter information to search: ";
			break;
		case (newEditDeleteCustomer):
			cout << endl << "Function (enter number):" << endl << "1. Add new customer" << endl << "2. Edit customer" << endl << "3. Delete customer" << endl;
			break;
		case (newEditDeleteItem):
			cout << endl << "Function (enter number):" << endl << "1. Add new item" << endl << "2. Edit item" << endl << "3. Delete item" << endl;
			break;
		case (function):
			cout << endl << "Enter function's number: ";
			break;
		case (customerID):
			cout << endl << "Enter Customer ID: ";
			break;
		case (itemID):
			cout << endl << "Enter Item ID: ";
			break;
		case (itemTitle):
			cout << endl << "Enter title: ";
			break;
		case (rentalType):
			cout << endl << "Enter rental type: ";
			break;
		case (loan):
			cout << endl << "Enter loan type: ";
			break;
		case (itemNoOfCopies):
			cout << endl << "Enter number of copies: ";
			break;
		case (itemFee):
			cout << endl << "Enter fee: ";
			break;
		case (itemGenre):
			cout << endl << "Enter genre: ";
			break;
		case (customerName):
			cout << endl << "Enter name: ";
			break;
		case (customerAddress):
			cout << endl << "Enter address: ";
			break;
		case (customerPhone):
			cout << endl << "Enter phone: ";
			break;
		case (customerType):
			cout << endl << "Customer type: ";
			break;
		case (confirmUsePoint):
			cout << endl << "Congratulation! You can rent 1 item for free!!" << endl;
			cout << "Do you want to rent this item for free?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		case (confirmEditLoanType):
			cout << endl << "Do you want to edit loan type?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		case (confirmEditRentalFee):
			cout << endl << "Do you want to edit rental fee?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		case (confirmEditNoOfCopies):
			cout << endl << "Do you want to edit number of copies?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		case (confirmEditName):
			cout << endl << "Do you want to edit name?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		case (confirmEditPhone):
			cout << endl << "Do you want to edit phone?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		case (confirmEditAddress):
			cout << endl << "Do you want to edit home address?" << endl;
			cout << "Enter Y for Yes (anything else means No)" << endl;
			break;
		default:
			errorDisplaying(invalidInput);
		}
		getString(aString);
	}
};
