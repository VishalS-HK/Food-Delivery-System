
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class DeliveryOrder {
public:
    int orderid;
    std::string name;
    std::string address;
    double price;
    std::string status;

    DeliveryOrder(int id, const std::string& n, const std::string& addr, double p, const std::string& stat)
        : orderid(id), name(n), address(addr), price(p), status(stat) {}

    void displayOrder() const {
        std::cout << "Order ID: " << orderid << "\n"
                  << "Name: " << name << "\n"
                  << "Address: " << address << "\n"
                  << "Price: Rs " << price << "\n"
                  << "Status: " << status << "\n\n";
    }

    void updateStatus(const std::string& newStatus) {
        status = newStatus;
    }
};

std::vector<DeliveryOrder> readDataFromCSV(const std::string& filename) {
    std::vector<DeliveryOrder> orders;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return orders;
    }

    std::string line;
    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string orderIdStr, name, address, priceStr, status;
        double price;

        std::getline(ss, orderIdStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, address, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, status, ',');

        int orderId = std::stoi(orderIdStr);
        price = std::stod(priceStr);

        orders.emplace_back(orderId, name, address, price, status);
    }

    file.close();
    return orders;
}

void writeDataToCSV(const std::string& filename, const std::vector<DeliveryOrder>& orders) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "order id,name,address,price,status\n";
    for (const auto& order : orders) {
        file << order.orderid << ","
             << order.name << ","
             << order.address << ","
             << order.price << ","
             << order.status << "\n";
    }

    file.close();
}

int main() {
    std::vector<DeliveryOrder> orders = readDataFromCSV("delivery.csv");

    std::cout << "Delivery Details:\n";
    for (const auto& order : orders) {
        order.displayOrder();
    }

    int choice;
    do {
        std::cout << "Choose an option:\n"
                  << "1. Display delivery details\n"
                  << "2. Update order status\n"
                  << "3. Exit\n";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Delivery Details:\n";
                for (const auto& order : orders) {
                    order.displayOrder();
                }
                break;
            case 2: {
                int orderId;
                std::string newStatus;
                std::cout << "Enter the Order ID to update its status: ";
                std::cin >> orderId;

                bool found = false;
                for (auto& order : orders) {
                    if (order.orderid == orderId) {
                        found = true;
                        std::cout << "Enter the new status: ";
                        std::cin >> newStatus;
                        order.updateStatus(newStatus);
                        writeDataToCSV("delivery.csv", orders); // Save updated data to the CSV file
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Order ID not found.\n";
                }
                break;
            }
            case 3:
                std::cout << "Exiting the program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
