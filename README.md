# Password_Manager
Console application written in C++.
Allows the storage of custom and generated passwords, protected with a simple three-letter-key.

User can direct through a menu:
1) Decrypt existing password: asks user for website and key to decrypt password (if existant) with. Password will be copied to clipboard automatically.
2) Generate and encrypt new password: asks user for website, generates a 14-symbol long password until user accepts, then asks for a key to encrypt new password with.
3) Add custom password: asks user for website to add and password in a specific format, lastly asks for key to encrypt with.
4) Put all encrypted passwords out: displays in alphabetical order all websites and their encrypted passwords.
5) Delete existing password: asks user to create admin pin. If user enters admin pin, asks him for website and allows him to delete existant password.
6) Exit: quit program

Currently supported languages: German

Version 1.5 by Robert Schmidt