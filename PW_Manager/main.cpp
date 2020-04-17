#include "Classes.h"

void toClipboard(HWND hwnd, const string &s);

int main()
{
    HWND hwnd = GetDesktopWindow();

    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    GetUserName(username, &username_len);

    string path = "";
    int Username_Length = 0;

    for (int i = 0; i < UNLEN+1; i++) {
        int check = username[i];

        if ((check >= 48 && check <= 57) || (check >= 65 && check <= 90) || (check >= 97 && check <= 122)) {
            Username_Length++;
        } else {
            break;
        }
    }

    for (int i = 0; i < Username_Length; i++) {
        path.append(1, username[i]);
    }

    Password_Setup* pw;
    Files* files = new Files(path.append("PW.dat"));

    char* output = "";
    int input = 0, confirm = 0;
    string website, KEY;

    cout << " __________________________________________________ " << endl;
    cout << "| Version 1.2 @ Robert Schmidt                     |" << endl;

    while (true) {
        List* liste = files->file_Load();
        liste->quickSort(0, liste->getMax());

        cout << " __________________________________________________ " << endl;
        cout << "|                   HAUPTMENUE:                    |" << endl;
        cout << "| (1) Vorhandenes Passwort entschluesseln          |" << endl;
        cout << "| (2) Neues Passwort generieren und verschluesseln |" << endl;
        cout << "| (3) Eigenes Passwort hinzufuegen                 |" << endl;
        cout << "| (4) Alle verschluesselten Passwoerter ausgeben   |" << endl;
        cout << "| (5) Vorhandenes Passwort loeschen                |" << endl;
        cout << "| (6) Beenden                                      |" << endl;
        cout << "|__________________________________________________|" << endl;

        cout << "=> ";
        cin >> input;

        if (!cin.fail()) {
            while (!(1 <= input <= 6)) {
                cin >> input;
            }
        } else {
            cout << "Fehler; Bitte bedienen Sie das Menue mit entsprechenden Zahleingaben!" << endl;
            cin.clear();
            cin.sync();
        }

        switch(input) {
        case 1:
            {
                cin.ignore();
                cout << "Geben Sie den Namen der Website ein, dessen Passwort Sie benoetigen." << endl;

                do {
                    cout << "=> ";
                    getline(cin, website);

                    if (website.size() < 3) {
                        cout << "Fehler; Name der Website ist zu kurz. Versuchen Sie es erneut." << endl;
                    }
                } while(website.size() < 3);

                if (liste->suche(website) == -1) {
                    cout << "Fehler; Website konnte nicht gefunden werden! Kehre zum Menue zurueck..." << endl;
                } else {
                    bool loop = true;
                    cout << "Geben Sie Ihren 3-stelligen Key aus Grossbuchstaben zur Entschluesselung ein." << endl;
                        do {
                            cout << "=> ";
                            getline(cin, KEY);

                            if (KEY.size() != 3) {
                                cout << "Fehler; Ungueltige Schluessellaenge! Bitte versuchen Sie es erneut." << endl;
                            } else {
                                loop = false;

                                for (int i = 0; i < 3; i++) {
                                    int check_n = KEY[i];

                                    if (check_n < 65 || check_n > 90) {
                                        cout << "Fehler; Ungueltiges Schluesselformat! Bitte versuchen Sie es erneut." << endl;
                                        loop = true;
                                        break;
                                    }
                                }
                            }
                        } while(loop);

                    output = pw->decryption(liste->getIndexPassword(liste->suche(website)), liste->getIndexWebsite(liste->suche(website)), KEY);
                    cout << "Ihr entschluesseltes Passwort (Key: " << KEY << ") lautet: " << output << endl;

                    toClipboard(hwnd, (string) output);
                }

                website.clear();
                KEY.clear();
                break;
            }
        case 2:
            {
                cin.ignore();
                cout << "Geben Sie den Namen der Website ein, fuer die Sie ein Passwort generieren moechten." << endl;

                do {
                    cout << "=> ";
                    getline(cin, website);

                    if (website.size() < 3) {
                        cout << "Fehler; Name der Website ist zu kurz. Versuchen Sie es erneut." << endl;
                    }
                } while(website.size() < 3);

                if (liste->suche(website) != -1) {
                    cout << "Fehler; Website existiert bereits! Kehre zum Menue zurueck..." << endl;
                } else {
                    do {
                        pw = new Password_Setup(website);

                        cout << endl;
                        cout << pw->getPassword() << endl;
                        cout << "Sind Sie zufrieden mit diesem generierten Passwort? (Akzeptieren mit '1'; Ablehnen mit Sonstigem)" << endl;

                        cin >> confirm;

                        if (cin.fail()) { // neben sonstiger Zahleneingabe duerfen auch Buchstaben verwendet werden
                            cin.clear();
                            cin.sync();
                        }
                    } while (confirm != 1);

                    cin.ignore();
                    cout << "Geben Sie einen 3-stelligen Key aus Grossbuchstaben zur Verschluesselung ein." << endl;

                    do {
                        cout << "=> ";
                        getline(cin, KEY);

                        if (KEY.size() != 3) {
                            cout << "Fehler; Ungueltige Schluessellaenge! Bitte versuchen Sie es erneut." << endl;
                        } else {
                            int i;

                            for (i = 0; i < 3; i++) {
                                int check_n = KEY[i];

                                if (check_n < 65 || check_n > 90) {
                                    cout << "Fehler; Ungueltiges Schluesselformat! Bitte versuchen Sie es erneut." << endl;
                                    break;
                                }
                            }

                            if (i == 3) {
                                break;
                            }
                        }
                    } while(true);

                    char* encrypted = pw->encryption(KEY);

                    cout << "Ihr verschluesseltes Passwort (KEY: " << KEY << ") lautet: " << encrypted << endl;

                    files->file_Save(website, encrypted, liste);
                }

                website.clear();
                KEY.clear();
                break;
            }
        case 3:
            {
                Password_Setup* custom_pw = new Password_Setup();
                string passw = "";

                cin.ignore();
                cout << "Geben Sie den Namen der Website ein, fuer die Sie ein Passwort hinzufuegen moechten." << endl;

                do {
                    cout << "=> ";
                    getline(cin, website);

                    if (website.size() < 3) {
                        cout << "Fehler; Name der Website ist zu kurz. Versuchen Sie es erneut." << endl;
                    }
                } while(website.size() < 3);

                if (liste->suche(website) != -1) {
                        cout << "Fehler; Website existiert bereits! Kehre zum Menue zurueck..." << endl;
                } else {
                    cout << "Geben Sie ein standardmaessiges Passwort ein, um es zu verschluesseln und zu speichern." << endl;
                    cout << "Formate: (AAAAAAA$111), (111$AAAAAAA), (AAAAAAA111$)" << endl;

                    cout << "=> ";
                    getline(cin, passw);

                    if (passw.size() != 11) {
                        cout << "Fehler; Passwort muss 11 Zeichen lang sein. Beachten Sie das Format! Kehre zum Menue zurueck..." << endl;
                        break;
                    } else {
                        char* website_Upper = pw->toUpperCase(website);

                        for (int i = 0; i < 3; i++) {
                            passw.append(1, website_Upper[i]);
                        }

                        custom_pw->setPassword(passw);

                        if (custom_pw->identification()) {

                            cout << "Geben Sie einen 3-stelligen Key aus Grossbuchstaben zur Verschluesselung ein." << endl;

                            do {
                                cout << "=> ";
                                getline(cin, KEY);

                                if (KEY.size() != 3) {
                                    cout << "Fehler; Ungueltige Schluessellaenge! Bitte versuchen Sie es erneut." << endl;
                                } else {
                                    int i;

                                    for (i = 0; i < 3; i++) {
                                        int check_n = KEY[i];

                                        if (check_n < 65 || check_n > 90) {
                                            cout << "Fehler; Ungueltiges Schluesselformat! Bitte versuchen Sie es erneut." << endl;
                                            break;
                                        }
                                    }

                                    if (i == 3) {
                                        break;
                                    }
                                }
                            } while(true);

                        char* encrypted = custom_pw->encryption(KEY);

                        cout << "Ihr verschluesseltes Passwort (KEY: " << KEY << ") lautet: " << encrypted << endl;

                        files->file_Save(website, encrypted, liste);
                    } else {
                        cout << "Fehler; Format wurde nicht korrekt eingegeben. Kehre zum Menue zurueck..." << endl;
                    }
                }
            }
                break;
            }
        case 4:
            {
                liste->display();
                break;
            }
        case 5:
            {
                char* ad_pin = "";
                bool break_bool = false;

                if (liste->suche(".W") == -1) {
                    int pin = 0;
                    ostringstream ostr;

                    cout << "Bitte erstellen Sie eine Administrator-PIN aus vier Ziffern." << endl;

                    do {
                        cout << "=> ";
                        cin >> pin;

                        if (cin.fail()) {
                            cout << "Fehler; Bitte bedienen Sie das Menue mit entsprechenden Zahleingaben!" << endl;
                            cout << endl;
                            cin.clear();
                            cin.sync();
                        } else if (pin > 9999 || pin < 1000) {
                            cout << "Fehler; PIN muss aus vier Ziffern bestehen. Versuchen Sie es erneut." << endl;
                        }
                    } while(pin > 9999 || pin < 1000);

                    ostr << pin;
                    string pinn = ostr.str();

                    for (int i = 0; i < 4; i++) {
                        ad_pin = pw->appendChar(ad_pin, pinn[i]);
                    }

                    files->file_Save(".W", ad_pin, liste);

                    cout << "Administrator-PIN erfolgreich gesetzt! Kehre zum Menue zurueck..." << endl;
                    break;
                } else {
                    string pin = "";
                    int stelle = liste->suche(".W");
                    ad_pin = liste->getIndexPassword(stelle);

                    cin.ignore();
                    cout << "Bitte geben Sie Ihre Administrator-PIN ein." << endl;

                    do {
                        cout << "=> ";
                        cin >> pin;

                        if (pin.size() != 4) {
                            cout << "Fehler; PIN muss aus vier Ziffern bestehen. Versuchen Sie es erneut." << endl;
                        } else if (pin != ad_pin) {
                            cout << "Fehler; Inkorrekte PIN wurde eingegeben. Kehre zum Menue zurueck..." << endl;
                            break_bool = true;
                        }
                    } while(pin.size() != 4);

                    if (break_bool) {
                        break;
                    } else {
                        cin.ignore();
                        cout << "Geben Sie den Namen der Website ein, dessen Passwort Sie loeschen moechten." << endl;

                        do {
                            cout << "=> ";
                            getline(cin, website);

                            if (website.size() < 3) {
                                cout << "Fehler; Name der Website ist zu kurz. Versuchen Sie es erneut." << endl;
                            }
                        } while(website.size() < 3);

                        if (liste->suche(website) == -1) {
                            cout << "Fehler; Website konnte nicht gefunden werden! Kehre zum Menue zurueck..." << endl;
                        } else {
                            liste->loeschen(liste->suche(website), files->getFILE_PATH());
                        }

                        website.clear();
                        break;
                    }
                }
            }
        case 6:
            {
                //return 0;
                exit(0);
            }
        }
    }
    return 1;
}

void toClipboard(HWND hwnd, const string &s) {
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);

	if (!hg) {
		CloseClipboard();
		return;
	}

	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}
