#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;

class FoodItem {
public:
    string name;
    double price;
    int quantity; 

    FoodItem(const string& name, double price, int quantity = 0)
        : name(name), price(price), quantity(quantity) {}

    
    double getTotalCost() const {
        return price * quantity;
    }
};

vector<string> splitCSVRow(const string& row, char delimiter) {
    vector<string> fields;
    stringstream ss(row);
    string field;

    while (getline(ss, field, delimiter)) {
        fields.push_back(field);
    }

    return fields;
}

vector<FoodItem> readMenuFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file: " << filename << endl;
        return {};
    }

    vector<FoodItem> menu;
    string line;
    // getline(file, line);

    while (getline(file, line)) {
        vector<string> fields = splitCSVRow(line, ',');
        if (fields.size() != 3) {
            continue;
        }

        string name = fields[1];
        double price = stod(fields[2]);

        menu.emplace_back(name, price);
    }

    file.close();
    return menu;
}

namespace FoodDeliverySystem {
    void displayMenu(const vector<FoodItem>& menu) {
        cout << "Menu:" << endl;
        cout << left << setw(8) << "Sl No" << setw(30) << "Food Item" << setw(6) << "Price" << endl;
        cout << string(50, '-') << endl;

        for (int i = 0; i < menu.size(); ++i) {
            cout << left << setw(8) << i + 1 << setw(30) << menu[i].name << setw(4) << "Rs." << menu[i].price << endl;
        }
    }
}

class Order {
public:
    vector<FoodItem> selectedItems;

    void addItem(const FoodItem& item) {
        selectedItems.push_back(item);
    }

    double getTotalCost() const {
        double totalCost = 0.0;
        for (const auto& item : selectedItems) {
            totalCost += item.getTotalCost();
        }
        return totalCost;
    }

    void takeOrder(const vector<FoodItem>& menu) {
        char choice;
        char choice2;
        do {
            cout << "Enter the Sl No of the food item you want to order: ";
            int slNo;
            cin >> slNo;

            if (cin.fail()) {
            cout << "Invalid input. Please enter a valid Sl No." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
            }
            
            if (slNo <= 0 || slNo > static_cast<int>(menu.size())) {
                cout << "Invalid Sl No. Please try again." << endl;
                continue;
            }
            
            cout << "Enter the quantity: ";
            int quantity;
            cin >> quantity;

            FoodItem selectedItem = menu[slNo - 1];
            selectedItem.quantity = quantity;
            addItem(selectedItem);

            cout << "Do you want to add more items? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }
};

class OrderDetails {
public:
    int orderId;
    double totalBillAmount;
    string address;
    string custName;
    string status;

    OrderDetails(int orderId, const string& custName,const string& address, double totalBillAmount)
        : orderId(orderId), custName(custName),address(address), totalBillAmount(totalBillAmount),status("Pending") {}

    void writeToFile(const string& filename) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cout << "Error opening the file: " << filename << endl;
            return;
        }
        file << orderId << "," << custName << "," << address << "," << totalBillAmount << "," <<status << endl;
        file.close();
    }
};

int generateRandomOrderId() {
    srand(static_cast<unsigned>(time(0)));
    int randomOrderId = rand() % 100000 + 1;
    return randomOrderId;
}

void clearScreen(){
    system("clear");
    //system("cls"); on Windows 
}

int main() {
    string filename = "menu.csv";

    vector<FoodItem> menu = readMenuFromCSV(filename);

    if (menu.empty()) {
        cout << "No menu items found." << endl;
        return 0;
    }

    string custName, address;

    cout << setw(80) << "**********************************************" << endl;
    cout << setw(80) << "\t                    Hey There 👋                " << endl;                 
    cout << setw(80) << " *                  WELCOME TO                *" << endl;
    cout << setw(80) << " *          ONLINE FOOD DELIVERY SYSTEM       *" << endl;
    cout << setw(80) << "**********************************************" << endl;
    cout << endl;
    cout << "Please log in to continue." << endl;

    cout << "Please enter your name: ";
    getline(cin, custName);

    cout << "Please enter your address: ";
    getline(cin, address);

    FoodDeliverySystem::displayMenu(menu);

    Order order;
    order.takeOrder(menu);
    clearScreen();
    
    cout << string(50, '-') << endl;
    cout << "Order details for " << custName << ":" << endl;
    for (const auto& item : order.selectedItems) {
        cout << item.name << " x " << item.quantity << " = Rs." << fixed << setprecision(2) << item.getTotalCost() << endl;
    }
    cout << "Total cost: Rs." << fixed << setprecision(2) << order.getTotalCost() << endl;
    cout << string(50, '-') << endl;

    int orderId = generateRandomOrderId();
    OrderDetails orderDetails(orderId, custName, address, order.getTotalCost());
    orderDetails.writeToFile("orders.csv");
    return 0;
}