#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string xorEncryptDecrypt(const string& input, const string& key) {
    string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key[i % key.size()];
    }
    return output;
}

string readFile(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        MessageBox(NULL, "Could not open the file.", "Error", MB_OK | MB_ICONERROR);
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

void writeFile(const string& filename, const string& content) {
    ofstream file(filename, ios::out | ios::binary);
    if (!file) {
        MessageBox(NULL, "Could not open the file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    file << content;
    file.close();
}

string openFileDialog() {
    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return string(ofn.lpstrFile);
    }
    return "";
}

string saveFileDialog() {
    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        return string(ofn.lpstrFile);
    }
    return "";
}

string getKeyFromUser() {
    string key;
    cout << "Enter the decryption key: ";
    getline(cin, key);
    return key;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    string key = "mysecretkey123";
    string filename;
    int choice = MessageBox(NULL, "Choose Yes to Encrypt or No to Decrypt.", "Select Operation", MB_YESNO | MB_ICONQUESTION);

    if (choice == IDYES) {
        filename = openFileDialog();
        if (!filename.empty()) {
            string fileContent = readFile(filename);
            if (!fileContent.empty()) {
                string encryptedContent = xorEncryptDecrypt(fileContent, key);
                string encryptedFileName = saveFileDialog();
                if (!encryptedFileName.empty()) {
                    writeFile(encryptedFileName, encryptedContent);
                    MessageBox(NULL, "File encrypted successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                }
            }
        }
    } else if (choice == IDNO) {
        filename = openFileDialog();
        if (!filename.empty()) {
            string encryptedContent = readFile(filename);
            if (!encryptedContent.empty()) {
                string userKey = getKeyFromUser();
                if (!userKey.empty()) {
                    string decryptedContent = xorEncryptDecrypt(encryptedContent, userKey);
                    string decryptedFileName = saveFileDialog();
                    if (!decryptedFileName.empty()) {
                        writeFile(decryptedFileName, decryptedContent);
                        MessageBox(NULL, "File decrypted successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                    }
                }
            }
        }
    }

    return 0;
}