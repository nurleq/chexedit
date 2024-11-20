#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "WindowsAPI.h"

using namespace std;

class HexEditor {
public:
    string exec(const char *args[]) {
        // parse command line arguments
        int argc = sizeof(args) / sizeof(args[0]);
        bool loadProcess = false;
        bool editMemory = false;
        string inputFilePath, outputFilePath;

        for (int i = 1; i < argc - 1; ++i) {
            if (string(args[i]) == "-l") {
                loadProcess = true;
                break;
            } else if (string(args[i]) == "-e") {
                editMemory = true;
                break;
            }
        }

        for (int i = argc - 2; i > 0; --i) {
            if (string(args[i]).find(".bin") != string::npos) {
                inputFilePath = args[i];
            } else if (string(args[i]).find(".out") != string::npos) {
                outputFilePath = args[i];
            }
        }

        // execute the command
        if (loadProcess) {
            return loadAndEditProcess(inputFilePath);
        } else if (editMemory) {
            return editMemory(inputFilePath, outputFilePath);
        } else {
            return openFileForEditing(inputFilePath, outputFilePath);
        }
    }

private:
    string openFileForEditing(string inputFilePath, string outputFilePath) {
        // open the file for editing
        ifstream file(inputFilePath, ios::binary);

        if (!file.is_open()) {
            cerr << "Error: unable to open file." << endl;
            return "";
        }

        char buffer[4096];
        int bytesRead = 0;

        cout << "Enter address to edit (in hex): ";
        string addressInput;
        getline(cin, addressInput);

        if (!(addressInput.length() == 8 && isxdigit(addressInput[0]) && isxdigit(addressInput[1])
             && isxdigit(addressInput[2]) && isxdigit(addressInput[3]) && isxdigit(addressInput[4])
             && isxdigit(addressInput[5]) && isxdigit(addressInput[6]) && isxdigit(addressInput[7]))) {
            cerr << "Error: invalid address." << endl;
            return "";
        }

        int address = (addressInput[0] - '0') * 16 + (addressInput[1] - '0') * 16 + (addressInput[2] - '0')
                     * 16 + (addressInput[3] - '0') * 16 + (addressInput[4] - '0') * 16
                     + (addressInput[5] - '0') * 16 + (addressInput[6] - '0') * 16 + (addressInput[7] - '0');

        char value;
        cout << "Enter new value for memory at address " << hex << address << ": ";
        cin >> value;

        // write the modified value back to the file
        while (bytesRead = file.read(buffer, sizeof(buffer))) {
            if (!file.eof()) {
                for (int i = 0; i < bytesRead; ++i) {
                    cout.write(buffer + i, 1);
                }
            } else {
                break;
            }

            cout << "Enter address to edit (in hex): ";
            getline(cin, addressInput);

            if (!(addressInput.length() == 8 && isxdigit(addressInput[0]) && isxdigit(addressInput[1])
                 && isxdigit(addressInput[2]) && isxdigit(addressInput[3]) && isxdigit(addressInput[4])
                 && isxdigit(addressInput[5]) && isxdigit(addressInput[6]) && isxdigit(addressInput[7]))) {
                cerr << "Error: invalid address." << endl;
                return "";
            }

            int addr = (addressInput[0] - '0') * 16 + (addressInput[1] - '0') * 16 + (addressInput[2] - '0')
                        * 16 + (addressInput[3] - '0') * 16 + (addressInput[4] - '0') * 16
                        + (addressInput[5] - '0') * 16 + (addressInput[6] - '0') * 16 + (addressInput[7] - '0');

            cout << "Enter new value for memory at address " << hex << addr << ": ";
            cin >> value;

            // write the modified value back to the file
            if ((addr & 0xFF) == (char)value) {
                char* ptr = (char*)malloc(sizeof(char));
                *ptr = value;
                memcpy(buffer + (addr & 0xFFFF), ptr, sizeof(char));

                free(ptr);
            }
        }

        // write any remaining data to the file
        if (!file.eof()) {
            for (int i = bytesRead; i < file.gcount(); ++i) {
                cout.write(&buffer[i], 1);
            }
        }

        return "";
    }

    string loadAndEditProcess(string inputFilePath) {
        // open the process using Windows API
        HANDLE hProcess;
        PROCESS_INFORMATION pi;

        AllocConsole();
        GetConsoleCursorPos(NULL);
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), EnableEnhancedMode());
        std::string command = "cmd.exe /c ";
        for (int i = 1; i < argc - 2; ++i) {
            if (string(args[i]).find(".bin") != string::npos) {
                command += inputFilePath + " ";
            }
        }

        // execute the program
        CreateProcess(NULL, command.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW,
                     NULL, NULL, &pi, NULL);

        // wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return "";
    }

    string editMemory(string inputFilePath, string outputFilePath) {
        // open the file for editing
        ifstream file(inputFilePath, ios::binary);

        if (!file.is_open()) {
            cerr << "Error: unable to open file." << endl;
            return "";
        }

        char buffer[4096];
        int bytesRead = 0;

        cout << "Enter address to edit (in hex): ";
        string addressInput;
        getline(cin, addressInput);

        if (!(addressInput.length() == 8 && isxdigit(addressInput[0]) && isxdigit(addressInput[1])
             && isxdigit(addressInput[2]) && isxdigit(addressInput[3]) && isxdigit(addressInput[4])
             && isxdigit(addressInput[5]) && isxdigit(addressInput[6]) && isxdigit(addressInput[7]))) {
            cerr << "Error: invalid address." << endl;
            return "";
        }

        int address = (addressInput[0] - '0') * 16 + (addressInput[1] - '0') * 16 + (addressInput[2] - '0')
                     * 16 + (addressInput[3] - '0') * 16 + (addressInput[4] - '0') * 16
                     + (addressInput[5] - '0') * 16 + (addressInput[6] - '0') * 16 + (addressInput[7] - '0');

        char value;
        cout << "Enter new value for memory at address " << hex << address << ": ";
        cin >> value;

        // write the modified value back to the file
        while (bytesRead = file.read(buffer, sizeof(buffer))) {
            if (!file.eof()) {
                for (int i = 0; i < bytesRead; ++i) {
                    cout.write(buffer + i, 1);
                }
            } else {
                break;
            }

            cout << "Enter address to edit (in hex): ";
            getline(cin, addressInput);

            if (!(addressInput.length() == 8 && isxdigit(addressInput[0]) && isxdigit(addressInput[1])
                 && isxdigit(addressInput[2]) && isxdigit(addressInput[3]) && isxdigit(addressInput[4])
                 && isxdigit(addressInput[5]) && isxdigit(addressInput[6]) && isxdigit(addressInput[7]))) {
                cerr << "Error: invalid address." << endl;
                return "";
            }

            int addr = (addressInput[0] - '0') * 16 + (addressInput[1] - '0') * 16 + (addressInput[2] - '0')
                        * 16 + (addressInput[3] - '0') * 16 + (addressInput[4] - '0') * 16
                        + (addressInput[5] - '0') * 16 + (addressInput[6] - '0') * 16 + (addressInput[7] - '0');

            cout << "Enter new value for memory at address " << hex << addr << ": ";
            cin >> value;

            // write the modified value back to the file
            if ((addr & 0xFF) == (char)value) {
                char* ptr = (char*)malloc(sizeof(char));
                *ptr = value;
                memcpy(buffer + (addr & 0xFFFF), ptr, sizeof(char));

                free(ptr);
            }
        }

        // write any remaining data to the file
        if (!file.eof()) {
            for (int i = bytesRead; i < file.gcount(); ++i) {
                cout.write(&buffer[i], 1);
            }
        }

        // output a message to indicate that editing has finished
        cout << "Editing process... done." << endl;

        return "";
    }
};
