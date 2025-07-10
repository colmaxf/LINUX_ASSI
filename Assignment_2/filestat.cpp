#include <iostream>   
#include <string>     
#include <sys/stat.h> 
#include <ctime>      
#include <iomanip>    
#include <cstring>    

using namespace std;

int main(int argc, char *argv[]) {
    // 1. Check the number of command line parameters
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1; 
    }

    // file path
    std::string file_path = argv[1];

    struct stat file_stat; // store file information

    // 2. get file information
    if (lstat(file_path.c_str(), &file_stat) == -1) {
        cerr << "Error getting file stats for '" << file_path << "': " << strerror(errno) << endl;
        return 1; 
    }

    // 3. show file information
    cout << "--- File Metadata ---" << endl;
    cout << "File Path: " << file_path << endl;

    // file type
    cout << "File Type: ";
    if (S_ISREG(file_stat.st_mode)) {
        cout << "Regular File" << endl;
    } else if (S_ISDIR(file_stat.st_mode)) {
        cout << "Directory" << endl;
    } else if (S_ISLNK(file_stat.st_mode)) {
        cout << "Symbolic Link" << endl;
    } else {
        cout << "Other Type (e.g., FIFO, Socket, Block Device, Character Device)" << endl;
    }

    // file size
    cout << "Size: " << (long long)file_stat.st_size << " bytes" << endl;

    // last modified time
    char *last_modified_time_c = ctime(&file_stat.st_mtime);
    string last_modified_time_str = (last_modified_time_c != nullptr) ? last_modified_time_c : "N/A";

    // delete \n
    if (!last_modified_time_str.empty() && last_modified_time_str.back() == '\n') {
        last_modified_time_str.pop_back();
    }
    cout << "Last Modified: " << last_modified_time_str << endl;

    cout << "---------------------" << endl;

    return 0; // Thoát thành công
}