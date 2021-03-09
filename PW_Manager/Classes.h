#include <iostream>
#include <complex>
#include <Windows.h>
#include <winuser.h>
#include <Lmcons.h>
using namespace std;
////////////////////

class Element {
private:
    char* password;
    char* website;
public:
    Element(char*, char*);
    Element(Element*);

    char* getPassword();
    char* getWebsite();

    void setPassword(char*);
    void setWebsite(char*);
};

class Password_Setup {
private:
    int style;
    int num;
    char* code;
    char let;

    char* pw;
public:
    Password_Setup();
    Password_Setup(string);

    char* getPassword();

    void setPassword(char*);
    void setPassword(string);

    char* appendChar(char*, char);
    char* toUpperCase(string);
    char* attach(char*);

    bool identification();
    char* encryption(string);
    char* decryption(char*, char*, string);
    char loop_Chars(char, int, bool);
    char loop_Nums(int, int, bool);

    void txtOutput(HANDLE, int, bool, string);
};

class List {
private:
    int anzahl;
    int Max;

    Element** Index;
public:
    List(int);

    int getMax();
    char* getIndexPassword(int);
    char* getIndexWebsite(int);

    void anhang(Element*);
    int suche(string);
    void display();
    void loeschen(int, string);

    void vertausche(int, int);
    int part(int, int);
    void quickSort(int, int);
};

class Files {
private:
    string FILE_PATH;
public:
    Files(string);

    string getFILE_PATH();
    void setFILE_PATH(string);

    int file_Count();
    List* file_Load();
    void file_Save(string, char*, List*);
};
