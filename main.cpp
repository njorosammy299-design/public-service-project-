#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// 1. The Blueprint for a Public Service Request
struct ServiceRequest {
    int id;
    string category;     // e.g., "Streetlight", "Water"
    string location;     // e.g., "Kenyatta Avenue"
    string description;  // Details of the issue
    string status;       // "Pending", "In Progress", "Resolved"
};

// 2. Save a new request to a text file
void saveRequest(const ServiceRequest& req) {
    ofstream outFile("requests.txt", ios::app);
    if (!outFile) {
        cout << "Error opening file for writing!\n";
        return;
    }

    outFile << req.id << "|" 
            << req.category << "|" 
            << req.location << "|" 
            << req.description << "|" 
            << req.status << "\n";
            
    outFile.close();
    cout << "Success: Request #" << req.id << " saved to file!\n";
}

// 3. Read and display all requests from the file
void viewAllRequests() {
    ifstream inFile("requests.txt");
    if (!inFile) {
        cout << "No requests found yet.\n";
        return;
    }

    string line;
    cout << "\n--- ALL PUBLIC SERVICE REQUESTS ---\n";
    while (getline(inFile, line)) {
        cout << line << "\n";
    }
    cout << "-----------------------------------\n";
    inFile.close();
}

// 4. Update Request Status by ID
void updateRequestStatus(int targetId, string newStatus) {
    ifstream inFile("requests.txt");
    if (!inFile) {
        cout << "No requests found!\n";
        return;
    }

    vector<ServiceRequest> requests;
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);
        size_t pos4 = line.find('|', pos3 + 1);

        if (pos1 != string::npos && pos4 != string::npos) {
            ServiceRequest item; // Renamed from 'req' to avoid conflict
            item.id = stoi(line.substr(0, pos1));
            item.category = line.substr(pos1 + 1, pos2 - pos1 - 1);
            item.location = line.substr(pos2 + 1, pos3 - pos2 - 1);
            item.description = line.substr(pos3 + 1, pos4 - pos3 - 1);
            item.status = line.substr(pos4 + 1);

            if (item.id == targetId) {
                item.status = newStatus;
                found = true;
            }
            requests.push_back(item);
        }
    }
    inFile.close();

    if (!found) {
        cout << "Request ID #" << targetId << " not found!\n";
        return;
    }

    ofstream outFile("requests.txt");
    for (const auto& item : requests) {
        outFile << item.id << "|" 
                << item.category << "|" 
                << item.location << "|" 
                << item.description << "|" 
                << item.status << "\n";
    }
    outFile.close();
    cout << "Success: Request #" << targetId << " status updated to " << newStatus << "!\n";
}
int main() {
    int choice;
    do {
        cout << "\n=== PUBLIC SERVICE REQUEST SYSTEM ===\n";
        cout << "1. Submit a New Request\n";
        cout << "2. View All Requests (Admin View)\n";
        cout << "3. Update Request Status (Admin Action)\n";
        cout << "4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: {
                ServiceRequest newReq;
                cout << "Enter numeric ID (e.g., 102): ";
                cin >> newReq.id;
                cin.ignore();

                cout << "Enter Category (e.g., Streetlight, Water, Garbage): ";
                getline(cin, newReq.category);

                cout << "Enter Location (e.g., Kenyatta Avenue): ";
                getline(cin, newReq.location);

                cout << "Enter Description of the issue: ";
                getline(cin, newReq.description);

                newReq.status = "Pending";
                saveRequest(newReq);
                break;
            }
            case 2:
                viewAllRequests();
                break;
            case 3: {
                int searchId;
                int statusChoice;
                cout << "Enter Request ID to update (e.g., 23): ";
                cin >> searchId;
                cout << "Select new status:\n1. In Progress\n2. Resolved\nEnter choice (1-2): ";
                cin >> statusChoice;
                
                string newStatus = (statusChoice == 2) ? "Resolved" : "In Progress";
                updateRequestStatus(searchId, newStatus);
                break;
            }
            case 4:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please enter 1, 2, 3, or 4.\n";
        }
    } while (choice != 4);

    return 0;
}
