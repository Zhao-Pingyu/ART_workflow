#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <regex>
#include <unistd.h>  // For chdir on UNIX-based systems
#include <set>       // Include for set container
#include <iomanip>   // Include for setprecision

using namespace std;

// Function to calculate total energy from file
double calc_en(const string& file) {
    ifstream infile(file);
    string content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    regex e("total_energy:\\s*([\\d\\.-]+)");
    smatch match;
    
    if (regex_search(content, match, e)) {
        return stod(match[1].str());
    } else {
        cout << "Total Energy not found in the file." << endl;
        return 0;
    }
}

// Function to calculate the distance between two files (files should contain numerical data)
double calc_dis(const string& file1, const string& file2) {
    vector<vector<double>> data1, data2;
    
    ifstream infile1(file1), infile2(file2);
    string line;  // Declare 'line' here for both files
    
    // Read data from file1
    while (getline(infile1, line)) {
        stringstream ss(line);
        vector<double> row;
        double value;
        while (ss >> value) row.push_back(value);
        data1.push_back(row);
    }
    
    // Read data from file2
    while (getline(infile2, line)) {
        stringstream ss(line);
        vector<double> row;
        double value;
        while (ss >> value) row.push_back(value);
        data2.push_back(row);
    }

    double diff = 0.0;
    for (size_t i = 0; i < data1.size(); ++i) {
        for (size_t j = 0; j < data1[i].size(); ++j) {
            diff += pow(data1[i][j] - data2[i][j], 2);
        }
    }
    
    return sqrt(diff);
}


// Function to process events in a folder
vector<vector<string>> loop_folder(vector<vector<string>>& data) {
    vector<vector<string>> data1 = data;
    vector<int> delete_indices;
    vector<int> delete_indices1;
    
    for (auto& row : data1) row.push_back("0");
    for (auto& row : data1) row.push_back("0");
    //int rows = data.size();
    //int cols = data[0].size(); // Assuming data has columns (e.g., 3 initial columns and 3 calculated values)

    // Add extra columns (similar to np.hstack((data1, np.zeros((len(data), 2)))))
    //for (int i = 0; i < rows; ++i) {
    //    data[i].resize(cols + 2, "0.0"); // Add 2 placeholder columns for delta, E_fin_init, E_sad_init
    //}
    
    for (size_t i = 0; i < data1.size(); ++i) {
        string init = data1[i][0], sad = data1[i][1], fin = data1[i][2];
        double E_init = calc_en(init), E_sad = calc_en(sad), E_fin = calc_en(fin);
        
        double E_sad_init = E_sad - E_init;
        double E_sad_fin = E_sad - E_fin;
        double E_fin_init = E_fin - E_init;
        double delta = calc_dis(fin, init);
        
        data1[i][3] = to_string(delta);
        data1[i][4] = to_string(E_fin_init);
        data1[i][5] = to_string(E_sad_init);
        
        if (E_sad_init < 0 || E_sad_fin < 0) {
            delete_indices.push_back(i);
            continue;
        }
        if (E_fin_init < 0.02 && delta < 1) {
            delete_indices.push_back(i);
            continue;
        }
    }
    /*
    for (int idx : delete_indices) {
        data1.erase(data1.begin() + idx);
    }
    */
    // Delete indices in reverse order to avoid skipping rows
    for (auto it = delete_indices.rbegin(); it != delete_indices.rend(); ++it) {
        data1.erase(data1.begin() + *it);
    }
    
    size_t n = data1.size();
    // Delete repeats
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double delta_d = abs(stod(data1[i][3]) - stod(data1[j][3]));
            double E_delta_fin_init = abs(stod(data1[i][4]) - stod(data1[j][4]));
            double E_delta_sad_init = abs(stod(data1[i][5]) - stod(data1[j][5]));

            if (delta_d < 0.1 && E_delta_fin_init < 0.005 && E_delta_sad_init < 0.01) {
                delete_indices1.push_back(j);
	    }
        }
    }

    // Remove duplicates in delete_indices1
    sort(delete_indices1.begin(), delete_indices1.end());
    delete_indices1.erase(unique(delete_indices1.begin(), delete_indices1.end()), delete_indices1.end());

    for (auto it = delete_indices1.rbegin(); it != delete_indices1.rend(); ++it) {
        data1.erase(data1.begin() + *it);
    }
    
    return data1;
}


vector<vector<string>> loop_all(vector<vector<string>>& events) {
    vector<vector<string>> data = events;  // Create a copy of the events data
    size_t n = data.size();  // Get the number of rows
    vector<pair<int, int>> pairs;
    vector<bool> to_delete(n, false);  // Mark rows to delete

    // Create all unique pairs of rows
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            pairs.push_back({i, j});
        }
    }

    // Check the condition for deletion based on the criteria
    for (auto& p : pairs) {
        int i = p.first, j = p.second;
        double delta_d = abs(stod(events[i][3]) - stod(events[j][3]));
        double E_delta_fin_init = abs(stod(events[i][4]) - stod(events[j][4]));
        double E_delta_sad_init = abs(stod(events[i][5]) - stod(events[j][5]));

        // If the condition is met, mark row `j` for deletion
        if (delta_d < 0.1 && E_delta_fin_init < 0.005 && E_delta_sad_init < 0.01) {
            to_delete[j] = true;
        }
    }

    // Create the result vector by keeping only non-deleted rows
    vector<vector<string>> result;
    for (int i = 0; i < n; ++i) {
        if (!to_delete[i]) {
            result.push_back(data[i]);
        }
    }

    // Print the final data size after deletion (for debugging)
    cout << "Final data size: " << result.size() << endl;

    return result;
}

int main() {
    ifstream folders_file("gb_id.txt");
    vector<int> folders;
    int folder;
    while (folders_file >> folder) {
        folders.push_back(folder);
    }
    
    string dir = get_current_dir_name();  // Save the initial working directory
    
    vector<vector<string>> data2;
    for (int folder : folders) {
        string folder_name = to_string(folder) + "/";
        chdir(folder_name.c_str());  // Change to the folder
        
        ifstream events_file("events.list");
        string line;  // Declare 'line' before the loop here
        
        vector<vector<string>> data;
        while (getline(events_file, line)) {  // 'line' is now declared here
            stringstream ss(line);
            string word;
            vector<string> row;
            while (ss >> word) row.push_back(word);
            data.push_back(row);
        }

        vector<vector<string>> data1 = loop_folder(data);
        
        if (!data1.empty()) {
            for (auto& row : data1) {
                vector<string> new_row = {folder_name};
                new_row.insert(new_row.end(), row.begin(), row.end());
                data2.push_back(new_row);
            }
        }
	chdir(dir.c_str());
    }
    
    chdir(dir.c_str());  // Go back to the original directory
    
    // Writing output to the final file
    ofstream output_file("events_all.list");
    for (auto& row : data2) {
        // Folder name is stored in the first element of the row
        string folder_name = row[0];

        // Output the folder and the file names
        output_file << folder_name + row[1] << " "; // min file
        output_file << folder_name + row[2] << " "; // sad file
        output_file << folder_name + row[3] << " "; // fin file

        // Output the computed values
        output_file << row[4] << " ";  // delta
        output_file << row[5] << " ";  // E_fin_init
        output_file << row[6] << endl; // E_sad_init
    }

    
    ifstream events_file("events_all.list");
    vector<vector<string>> events;
    string line2;  // Declare another 'line' variable here for reading events_all.list
    while (getline(events_file, line2)) {  // Use the new 'line2' variable
        stringstream ss(line2);
        string word;
        vector<string> row;
        while (ss >> word) row.push_back(word);
        events.push_back(row);
    }
    
    vector<vector<string>> non_repeat = loop_all(events);
    
    ofstream final_file("events_clean.list");
    for (auto& row : non_repeat) {
        for (auto& word : row) {
            final_file << word << " ";
        }
        final_file << endl;
    }

    return 0;
}


