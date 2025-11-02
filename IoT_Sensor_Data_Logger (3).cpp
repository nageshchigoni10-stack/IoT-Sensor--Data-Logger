#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <vector>
using namespace std;

// Structure to hold a sensor reading
struct SensorData {
    string timestamp;
    float temperature;
    float humidity;
};

// Function to get current time as string
string getTimeStamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[30];
    sprintf(buffer, "%02d-%02d-%d %02d:%02d:%02d",
            ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

// Function to simulate temperature and humidity readings
SensorData generateReading() {
    SensorData data;
    data.timestamp = getTimeStamp();
    data.temperature = (rand() % 4000) / 100.0;  // 0.00 - 40.00 °C
    data.humidity = (rand() % 10000) / 100.0;    // 0.00 - 100.00 %
    return data;
}

// Function to log data into a CSV file
void logData(const SensorData &data) {
    ofstream file("sensor_log.csv", ios::app);
    if (file.tellp() == 0) {
        file << "Timestamp,Temperature (C),Humidity (%)\n";
    }
    file << data.timestamp << "," 
         << fixed << setprecision(2) 
         << data.temperature << "," 
         << data.humidity << "\n";
    file.close();
}

// Function to analyze logged data
void analyzeData() {
    ifstream file("sensor_log.csv");
    string line;
    vector<SensorData> readings;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        SensorData data;
        sscanf(line.c_str(), "%*[^,],%f,%f", &data.temperature, &data.humidity);
        readings.push_back(data);
    }
    file.close();

    if (readings.empty()) {
        cout << "No data available to analyze.\n";
        return;
    }

    float tempSum = 0, humSum = 0, maxTemp = -1e9, minTemp = 1e9;

    for (auto &d : readings) {
        tempSum += d.temperature;
        humSum += d.humidity;
        maxTemp = max(maxTemp, d.temperature);
        minTemp = min(minTemp, d.temperature);
    }

    cout << fixed << setprecision(2);
    cout << "\n--- Sensor Data Analysis ---\n";
    cout << "Total Readings: " << readings.size() << endl;
    cout << "Average Temperature: " << tempSum / readings.size() << " °C\n";
    cout << "Average Humidity: " << humSum / readings.size() << " %\n";
    cout << "Max Temperature: " << maxTemp << " °C\n";
    cout << "Min Temperature: " << minTemp << " °C\n";
}

// Main menu
int main() {
    srand(time(0));
    int choice;

    cout << "============================\n";
    cout << " IoT Sensor Data Logger\n";
    cout << "============================\n";

    do {
        cout << "\n1. Generate and Log Sensor Data";
        cout << "\n2. Analyze Logged Data";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                SensorData data = generateReading();
                logData(data);
                cout << "\nData logged successfully:\n";
                cout << "Time: " << data.timestamp 
                     << " | Temp: " << data.temperature 
                     << " °C | Humidity: " << data.humidity << " %\n";
                break;
            }
            case 2:
                analyzeData();
                break;
            case 3:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}