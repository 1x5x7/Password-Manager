#include <time.h>
#include <fstream>
#include <shlobj.h>
#include <tchar.h>
#include "Classes.h"
////////////////////

Element::Element(char* pw, char* web): password(pw), website(web) {}
Element::Element(Element* cop): password(cop->getPassword()), website(cop->getWebsite()) {}

char* Element::getPassword() {
    return this->password;
}

char* Element::getWebsite() {
    return this->website;
}

void Element::setPassword(char* s) {
    this->password = s;
}

void Element::setWebsite(char* s) {
    this->website = s;
}

Password_Setup::Password_Setup(): style(0), num(0), code(NULL), let(NULL), pw("") {}
Password_Setup::Password_Setup(string website): style(0), num(0), code(NULL), let(NULL), pw(NULL) {

    // Password-Triplet Erstellung
    char* web = "";
    char* website_Upper = this->toUpperCase(website);

    for (int i = 0; i < 3; i++) {
        web = this->appendChar(web, website_Upper[i]);
    }

    // Style-Generierung
    srand(time(NULL)); // Bei Programmstart jedes Mal eine neue Generierung; random seed

    this->style = rand() % 3 + 1;

    // Zahl-Generierung
    for (int j = 0; j < 3; j++) {
        int n = rand() % 9 + 1;

        if (j == 0) {
            this->num += n * 100;
        } else if (j == 1) {
            this->num += n * 10;
        } else {
            this->num += n;
        }
    }

    // Code-Generierung
    this->code = "";

    for (int k = 0; k < 7; k++) {
        int num = 0;
        int area = rand() % 2;

        if (area == 0) {
            num = rand() % 26 + 65;
        } else {
            num = rand() % 26 + 97;
        }

        this->code = this->appendChar(this->code, num);
    }

    // Generierung des Zeichens
    int select = rand() % 2;

    if (select == 0) {
        this->let = '$';
    } else {
        this->let = '%';
    }

    // Zusammensetzung zum Password
    this->pw = this->attach(web);
}

char* Password_Setup::getPassword() {
    return this->pw;
}

void Password_Setup::setPassword(char* s) {
    this->pw = s;
}

void Password_Setup::setPassword(string s) {
    for (int i = 0; i < s.size(); i++) {
        this->pw = this->appendChar(this->pw, s[i]);
    }
}

char* Password_Setup::appendChar(char* arr, char a) {
    size_t len = strlen(arr);

    char* ret = new char[len + 2];

    strcpy(ret, arr);
    ret[len] = a;
    ret[len + 1] = '\0';

    return ret;
}

char* Password_Setup::toUpperCase(string website) {
    char* website_Upper = "";

    for (int i = 0; i < website.size(); i++) {
        int check_n = website[i];
        char check_c;

        if (check_n >= 97 && check_n <= 122) {
            check_n -= 32;
        }
        check_c = check_n;

        website_Upper = this->appendChar(website_Upper, check_c);
    }

    return website_Upper;
}

char* Password_Setup::attach(char* web) {
    char* output = "";

    ostringstream ostr; // Konvertierung von int zu string
    ostr << this->num;
    string num = ostr.str();

    if (this->style == 1) { // code + let + num + WEB
        for (int i = 0; i < 10; i++) {
            if (i < 7) {
                output = this->appendChar(output, this->code[i]);
            } else if (i == 7) {
                output = this->appendChar(output, this->let);
            } else if (i == 8) {
                for (int j = 0; j < 3; j++) {
                    output = this->appendChar(output, num[j]);
                }
            } else {
                for (int j = 0; j < 3; j++) {
                    output = this->appendChar(output, web[j]);
                }
            }
        }

        return output;
    } else if (this->style == 2) { // code + num + let + WEB
        for (int i = 0; i < 10; i++) {
            if (i < 7) {
                output = this->appendChar(output, this->code[i]);
            } else if (i == 7) {
                for (int j = 0; j < 3; j++) {
                    output = this->appendChar(output, num[j]);
                }
            } else if (i == 8) {
                output = this->appendChar(output, this->let);
            } else {
                for (int j = 0; j < 3; j++) {
                    output = this->appendChar(output, web[j]);
                }
            }
        }

        return output;
    } else { // num + let + code + WEB
        for (int i = 0; i < 10; i++) {
            if (i == 0) {
                for (int j = 0; j < 3; j++) {
                    output = this->appendChar(output, num[j]);
                }
            } else if (i == 1) {
                output = this->appendChar(output, this->let);
            } else if (i < 9) {
                output = this->appendChar(output, this->code[i - 2]);
            } else {
                for (int j = 0; j < 3; j++) {
                    output = this->appendChar(output, web[j]);
                }
            }
        }

        return output;
    }
}

bool Password_Setup::identification() {
    int check = this->pw[0]; //erste Stelle
    int check_n;

    if ((check >= 65 && check <= 90) || (check >= 97 && check <= 122)) { // Buchstaben zuerst -> Style 1,2
        for (int i = 1; i < 7; i++) {
            check_n = this->pw[i];

            if (!(check_n >= 65 && check_n <= 90) && !(check_n >= 97 && check_n <= 122)) {
                return false;
            }
        } // alle Buchstaben gefunden, pruefe nun ob Zeichen oder Zahl

        check_n = this->pw[7];
        if (check_n == 36 || check_n == 37) { // Zeichen? -> Nach Zahlen schauen
            for (int i = 8; i < 11; i++) {
                check_n = this->pw[i];

                if (!(check_n >= 48 && check_n <= 57)) {
                    return false;
                }
            }

            this->style = 1;
        } else if (check_n >= 48 && check_n <= 57) { // Zahlen? -> Nach Zeichen schauen
            for (int i = 8; i < 11; i++) {
                check_n = this->pw[i];

                if (i < 10) { // zwei weitere Zahlen
                    if (!(check_n >= 48 && check_n <= 57)) {
                        return false;
                    }
                } else { // Zeichen
                    if (!(check_n == 36 || check_n == 37)) {
                        return false;
                    }
                }
            }

            this->style = 2;
        } else {
            return false;
        }
    } else if (check >= 48 && check <= 57) { // Zahlen zuerst -> Style 3
        for (int i = 1; i < 11; i++) {
            check_n = this->pw[i];

            if (i < 3) { // zwei Zahlen
                if (!(check_n >= 48 && check_n <= 57)) {
                    return false;
                }
            } else if (i == 3) { // Zeichen
                if (!(check_n == 36 || check_n == 37)) {
                    return false;
                }
            } else { // 7 Buchstaben
                if (!(check_n >= 65 && check_n <= 90) && !(check_n >= 97 && check_n <= 122)) {
                    return false;
                }
            }
        }

        this->style = 3;
    } else {
        return false;
    }

    return true; // = true, wenn keine Fehler
}

char* Password_Setup::encryption(string KEY) {
    int i, key_part;
    char* encrypted = "";

    if (this->style == 1) {
        for (i = 0; i < 7; i++) {
            if (i < 2) {
                key_part = KEY[2] - 64;
            } else if (i < 5) {
                key_part = KEY[1] - 64;
            } else {
                key_part = KEY[0] - 64;
            }
            encrypted = this->appendChar(encrypted, loop_Chars(this->pw[i], key_part, true));
        }

        encrypted = this->appendChar(encrypted, this->pw[i]);

        for (i += 1; i < 11; i++) {
            if (i == 9) {
                key_part = KEY[0] - 64;
            } else {
                key_part = KEY[2] - 64;
            }
            encrypted = this->appendChar(encrypted, loop_Nums(this->pw[i] - 48, key_part, true));
        }

        for (i = 11; i < 14; i++) {
            encrypted = this->appendChar(encrypted, this->pw[i]);
        }
    } else if (this->style == 2) {
        for (i = 0; i < 7; i++) {
            if (i < 2) {
                key_part = KEY[1] - 64;
            } else if (i < 5) {
                key_part = KEY[2] - 64;
            } else {
                key_part = KEY[0] - 64;
            }
            encrypted = this->appendChar(encrypted, loop_Chars(this->pw[i], key_part, true));
        }

        for (i += 0; i < 10; i++) {
            if (i == 9) {
                key_part = KEY[1] - 64;
            } else {
                key_part = KEY[2] - 64;
            }
            encrypted = this->appendChar(encrypted, loop_Nums(this->pw[i] - 48, key_part, true));
        }

        encrypted = this->appendChar(encrypted, this->pw[i]);

        for (i = 11; i < 14; i++) {
            encrypted = this->appendChar(encrypted, this->pw[i]);
        }
    } else {
        for (i= 0; i < 3; i++) {
            if (i == 2) {
                key_part = KEY[2] - 64;
            } else {
                key_part = KEY[0] - 64;
            }
            encrypted = this->appendChar(encrypted, loop_Nums(this->pw[i] - 48, key_part, true));
        }

        encrypted = this->appendChar(encrypted, this->pw[i]);

        for (i += 1; i < 11; i++) {
            if (i < 6) {
                key_part = KEY[0] - 64;
            } else if (i < 9) {
                key_part = KEY[1] - 64;
            } else {
                key_part = KEY[2] - 64;
            }
            encrypted = this->appendChar(encrypted, loop_Chars(this->pw[i], key_part, true));
        }
        for (i = 11; i < 14; i++) {
            encrypted = this->appendChar(encrypted, this->pw[i]);
        }
    }

    cout << endl;
    cout << "Passwort erfolgreich gespeichert und verschluesselt!" << endl;
    return encrypted;
}

char* Password_Setup::decryption(char* encrypted, char* web, string KEY) {
    int i, key_part;
    int char_check = encrypted[0], num_check = encrypted[10];
    char* decrypted = "";

    if ((char_check >= 65 && char_check <= 122) && (num_check >= 48 && num_check <= 57)) {
        for (i = 0; i < 7; i++) {
            if (i < 2) {
                key_part = KEY[2] - 64;
            } else if (i < 5) {
                key_part = KEY[1] - 64;
            } else {
                key_part = KEY[0] - 64;
            }
            decrypted = this->appendChar(decrypted, loop_Chars(encrypted[i], key_part, false));
        }

        decrypted = this->appendChar(decrypted, encrypted[i]);

        for (i += 1; i < 11; i++) {
            if (i == 9) {
                key_part = KEY[0] - 64;
            } else {
                key_part = KEY[2] - 64;
            }
            decrypted = this->appendChar(decrypted, loop_Nums(encrypted[i] - 48, key_part, false));
        }

        for (i = 0; i < 3; i++) {
            decrypted = this->appendChar(decrypted, web[i]);
        }
    } else if ((char_check >= 65 && char_check <= 122) && (num_check < 48)) {
            for (i = 0; i < 7; i++) {
				if (i < 2) {
					key_part = KEY[1] - 64;
				} else if (i < 5) {
					key_part = KEY[2] - 64;
				} else {
					key_part = KEY[0] - 64;
				}

				decrypted = this->appendChar(decrypted, loop_Chars(encrypted[i], key_part, false));
			}

			for (i += 0; i < 10; i++) {
				if (i == 9) {
					key_part = KEY[1] - 64;
				} else {
					key_part = KEY[2] - 64;
				}

				decrypted = this->appendChar(decrypted, loop_Nums(encrypted[i] - 48, key_part, false));
			}
			decrypted = this->appendChar(decrypted, encrypted[i]);

        for (i = 0; i < 3; i++) {
            decrypted = this->appendChar(decrypted, web[i]);
        }
    } else {
            for (i = 0; i < 3; i++) {
				if (i == 2) {
					key_part = KEY[2] - 64;
				} else {
					key_part = KEY[0] - 64;
				}

				decrypted = this->appendChar(decrypted, loop_Nums(encrypted[i] - 48, key_part, false));
			}

			decrypted = this->appendChar(decrypted, encrypted[i]);

			for (i += 1; i < 11; i++) {
				if (i < 6) {
					key_part = KEY[0] - 64;
				} else if (i < 9) {
					key_part = KEY[1] - 64;
				} else {
					key_part = KEY[2] - 64;
				}

				decrypted = this->appendChar(decrypted, loop_Chars(encrypted[i], key_part, false));
			}

            for (i = 0; i < 3; i++) {
                decrypted = this->appendChar(decrypted, web[i]);
            }
    }

    return decrypted;
}

char Password_Setup::loop_Chars(char c, int key, bool encr_decr) { // true = encryption / false = decryption
    if (encr_decr) {
        int character = c;
        character += key;

        if (character > 122) {
			int checkpoint = character - 122;
			character = 64 + checkpoint;
		}

		return character;
    } else {
        int character = c;
		character -= key;

        if (character < 65) {
			int checkpoint = character + 122;
			character = checkpoint - 64;
		}

		return character;
    }
}

char Password_Setup::loop_Nums(int i, int key, bool encr_decr) {
    char character;

    if (encr_decr) {
		if (key >= 20) {
			i += 8;
		} else if (key >= 14) {
			i += 4;
		} else if (key >= 5) {
			i += 6;
		} else {
			i += 1;
		}

		if (i > 9) {
			i -= 10;
		}

		character = i + 48;

		return character;
    } else {
		if (key >= 20) {
			if (i >= 8) {
				i -= 8;
			} else {
				i += 2;
			}
		} else if (key >= 14) {
			if (i >= 4) {
				i -= 4;
			} else {
				i += 6;
			}
		} else if (key >= 5) {
			if (i >= 6) {
				i -= 6;
			} else {
				i += 4;
			}
		} else {
			if (i >= 1) {
				i -= 1;
			} else {
				i += 9;
			}
		}

		character = i + 48;

		return character;
    }
}

void Password_Setup::txtOutput(HANDLE console, int color, bool line, string text) {
    SetConsoleTextAttribute(console, color); //1 blau //2 green //3 hellblau //4 rot //5 lila //6 gold //7 silber  // 15 white
    if (line) {
        cout << text << endl;
    } else {
        cout << text;
    }
    SetConsoleTextAttribute(console, 15); //back to default (white)
}

List::List(int anzahl): anzahl(0), Max(anzahl - 2), Index(NULL) {
    this->Index = (Element**) malloc(anzahl * sizeof(Element*));
}

int List::getMax() {
    return this->Max;
}

char* List::getIndexPassword(int stelle) {
    return this->Index[stelle]->getPassword();
}

char* List::getIndexWebsite(int stelle) {
    return this->Index[stelle]->getWebsite();
}

void List::anhang(Element* neu) {
    this->Index[anzahl] = neu;
    this->anzahl++;
}

int List::suche(string website) {
    Password_Setup* methods;
    char* website_toUpper = "";
    website_toUpper = methods->toUpperCase(website);

    int read = 0;
    int length = 1;
    int i;

    for (int j = 0; j <= this->Max; j++) {
        while (Index[j]->getWebsite()[read] != NULL) {
            read++;
        }

        length = read;
        read = 0;

        for (i = 0; i < length; i++) {
            if (Index[j]->getWebsite()[i] == website_toUpper[i]) {
                read++;
            }
        }

        if (read == length) {
            return j;
        } else {
            read = 0;
        }
    }

    return -1;
}

void List::display() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << endl;
    if (this->anzahl-1 > 0) {
        if (this->anzahl-1 == 1 && this->suche(".W") != -1) {
            cout << "Fehler; Es existieren keine Passwoerter! Kehre zum Menue zurueck..." << endl;
        } else {
            Password_Setup* methods;
            char* existing_letters = "";
            int existing_letters_amount = 0;

            for (int i = 0; i <= this->Max; i++) {
                char* web = "";
                char* website = this->Index[i]->getWebsite();
                char* password = this->Index[i]->getPassword();
                bool existing_letter = false;
                string str_web = "";

                for (int j = 0; j < 3; j++) {
                    web = methods->appendChar(web, website[j]);
                }

                if (web[0] != '.') {
                    for (int i = 0; i < existing_letters_amount + 1; i++) {
                        if (website[0] == existing_letters[i]) {
                            existing_letter = true;
                        }
                    }

                    if (!existing_letter) {
                        str_web.append(website, 1);
                        methods->txtOutput(hConsole, 6, true, str_web);
                        str_web = "";
                    }

                    cout << "   " << website << ": " << password << web << endl;

                    existing_letters = methods->appendChar(existing_letters, website[0]);
                    existing_letters_amount++;
                }
            }
        }
    } else {
        cout << "Fehler; Es existieren keine Passwoerter! Kehre zum Menue zurueck..." << endl;
    }
}

void List::loeschen(int stelle, string path) {
    ofstream fileWrite;
    int more = this->anzahl - stelle;
    string deleted_website = Index[stelle]->getWebsite();

    for (int i = 0; i < more; i++) {
        Index[stelle + i] = Index[stelle + 1 + i];
    }

    this->anzahl--;

    DWORD attributes = GetFileAttributes(path.c_str());
    SetFileAttributes(path.c_str(), attributes & FILE_ATTRIBUTE_NORMAL);
    fileWrite.open(path.c_str());

    for (int i = 0; i < this->anzahl - 1; i++) {
            fileWrite << this->getIndexPassword(i) << ";" << this->getIndexWebsite(i) << endl;
    }

    cout << "Passwort der Website '" << deleted_website <<"' erfolgreich geloescht!" << endl;
}

void List::vertausche(int first, int second) {
    Element* temp = new Element(this->Index[first]);
    this->Index[first] = this->Index[second];
    this->Index[second] = temp;
}

void List::quickSort(int low, int high) {
    if (low < high) {
        int pivot = this->part(low, high);
        this->quickSort(low, pivot - 1);
        this->quickSort(pivot + 1, high);
    }
}

int List::part(int low, int high) {
    char* compare = this->Index[high]->getWebsite();
    char* compare_object = "";

    long i = low - 1;
    long j;

	 for (j = low; j <= high; j++) {
	     compare_object = this->Index[j]->getWebsite();

		 if (strcmp(compare, compare_object) > 0) {
            i++;

            vertausche(i, j);
		 }
	 }

    vertausche(i + 1, high);

    return (i + 1);
}

Files::Files(string path): FILE_PATH(path) {}

string Files::getFILE_PATH() {
    return this->FILE_PATH;
}

void Files::setFILE_PATH(string s) {
    this->FILE_PATH = s;
}

int Files::file_Count() {
    ifstream fileRead(this->getFILE_PATH().c_str());

    if (fileRead.good()) {
        DWORD attributes = GetFileAttributes(this->getFILE_PATH().c_str());
        SetFileAttributes(this->getFILE_PATH().c_str(), attributes | FILE_ATTRIBUTE_HIDDEN);

        int counter = 0;
        string pw = "";

        while (fileRead.good()) {
            getline(fileRead, pw, ';');
            counter++;
        }

        return counter - 1;
    } else {
        ofstream fileCreate(this->getFILE_PATH().c_str());
        fileCreate.open(this->getFILE_PATH().c_str());

        DWORD attributes = GetFileAttributes(this->getFILE_PATH().c_str());
        SetFileAttributes(this->getFILE_PATH().c_str(), attributes | FILE_ATTRIBUTE_HIDDEN);

        return 0;
    }
}

List* Files::file_Load() {
    int amount = this->file_Count();

    ifstream fileRead(this->getFILE_PATH().c_str());

    string pw = "", web = "";
    char* conv_PW, *conv_WEB;

    Element* neu = NULL;
    List* ret = new List(amount + 1);

    if (amount > 0) {
        while (fileRead.good()) {
            getline(fileRead, pw, ';');
            getline(fileRead, web, '\n');

            conv_PW = new char[pw.length()];
            conv_WEB = new char[web.length()];

            for(int i = 0; i <= pw.length(); i++) {
                    conv_PW[i] = pw[i];
                }

            for(int i = 0; i <= web.length(); i++) {
                    conv_WEB[i] = web[i];
                }

            neu = new Element(conv_PW, conv_WEB);
            ret->anhang(neu);
        }
    }

    fileRead.close();
    return ret;
}

void Files::file_Save(string website, char* encrypted, List* liste) {
    ofstream fileWrite;
    string line = "";

    Password_Setup* methods;

    char* website_toUpper = "";
    website_toUpper = methods->toUpperCase(website);

    line.append(encrypted, 11);
    line.append(";");
    line.append(website_toUpper);

    DWORD attributes = GetFileAttributes(this->getFILE_PATH().c_str());
    SetFileAttributes(this->getFILE_PATH().c_str(), attributes & FILE_ATTRIBUTE_NORMAL);

    fileWrite.open(this->getFILE_PATH().c_str());

    for (int i = 0; i < liste->getMax() + 2; i++) {
        if (i <= liste->getMax()) {
            fileWrite << liste->getIndexPassword(i) << ";" << liste->getIndexWebsite(i) << endl;
        } else {
            fileWrite << line << endl;
        }
    }

    fileWrite.close();
}

/*void Files::file_Backup(List* liste) {
    ofstream fileWrite;
    Password_Setup* methods;
    TCHAR path[_MAX_PATH] = _T(""); // must be _MAX_PATH in size

    HRESULT hr = SHGetFolderPath(NULL, CSIDL_DESKTOP, 0, NULL, path);

    cout << path << endl;

//    fileWrite.open("Backup.txt", path); // path + "Backup.txt"

    for (int i = 0; i < liste->getMax() + 2; i++) {
        if (i <= liste->getMax() && liste->getIndexWebsite(i)[0] != '.') {
            char* web = "";

            for (int j = 0; j < 3; j++) {
                web = methods->appendChar(web, liste->getIndexWebsite(i)[j]);
            }

            fileWrite << liste->getIndexWebsite(i) << ": " << liste->getIndexPassword(i) << web << endl;
        }
    }

    fileWrite.close();
}*/
