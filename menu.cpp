#include "menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>

// Hilfsfunktion: String in Großbuchstaben umwandeln
std::string toUpper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// <---------------------- ADD ---------------------->
void handleAdd(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Aktie a;
    std::cout << "Aktienname: ";
    std::getline(std::cin, a.name);
    a.name = toUpper(a.name);

    std::cout << "WKN: ";
    std::getline(std::cin, a.wkn);

    std::cout << "Ticker: ";
    std::getline(std::cin, a.ticker);
    a.ticker = toUpper(a.ticker);

    if (ht.add(a, a.wkn))
        std::cout << "Aktie hinzugefügt!\n";
    else
        std::cout << "Fehler: Aktie existiert bereits!\n";
}

// <---------------------- DEL ---------------------->
void handleDel(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string key;
    std::cout << "WKN der zu löschenden Aktie: ";
    std::getline(std::cin, key);

    if (ht.remove(key))
        std::cout << "Aktie gelöscht!\n";
    else
        std::cout << "Fehler: Aktie nicht gefunden!\n";
}

// <---------------------- IMPORT ---------------------->
void handleImport(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Aktienname für Import: ";
    std::string name;
    std::getline(std::cin, name);
    name = toUpper(name);

    Aktie* a = ht.searchByName(name);
    if (!a) {
        std::cout << "Aktie nicht gefunden.\n";
        return;
    }

    std::cout << "CSV-Datei eingeben: ";
    std::string filename;
    std::getline(std::cin, filename);
    
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Datei konnte nicht geöffnet werden.\n";
        return;
    }

    std::string line;
    std::getline(file, line);  // Header überspringen
    a->kursdaten.clear();

    while (std::getline(file, line) && a->kursdaten.size() < 30) {
        std::stringstream ss(line);
        Kurs k;
        std::getline(ss, k.date, ',');
        ss >> k.close; ss.ignore();
        ss >> k.volume; ss.ignore();
        ss >> k.open; ss.ignore();
        ss >> k.high; ss.ignore();
        ss >> k.low;
        a->kursdaten.push_back(k);
    }

    file.close();
    std::cout << "Daten importiert!\n";
}

// <---------------------- SEARCH ---------------------->
void handleSearch(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "WKN eingeben: ";
    std::string key;
    std::getline(std::cin, key);

    Aktie* a = ht.search(key);
    if (!a)
        std::cout << "Aktie nicht gefunden!\n";
    else
        std::cout << "Gefunden: " << a->name << " (" << a->wkn << ", " << a->ticker << ")\n";
}

// <---------------------- SAVE ---------------------->
void handleSave(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string filename;
    std::cout << "Dateiname zum Speichern: ";
    std::getline(std::cin, filename);

    std::ofstream ofs(filename);
    if (!ofs) {
        std::cout << "Fehler beim Öffnen der Datei.\n";
        return;
    }

    for (size_t i = 0; i < ht.getTableSize(); i++) {
        if (ht.getStatus(i) == SlotStatus::OCCUPIED) {
            Aktie* a = ht.getTable()[i];
            ofs << a->name << "|" << a->wkn << "|" << a->ticker << "\n";
        }
    }

    std::cout << "Daten gespeichert!\n";
}

// <---------------------- LOAD ---------------------->
void handleLoad(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string filename;
    std::cout << "Dateiname zum Laden: ";
    std::getline(std::cin, filename);

    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "Fehler beim Öffnen der Datei.\n";
        return;
    }

    ht.clear();
    std::string line;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        Aktie a;
        std::getline(ss, a.name, '|');
        std::getline(ss, a.wkn, '|');
        std::getline(ss, a.ticker);
        ht.add(a, a.wkn);
    }

    std::cout << "Daten geladen!\n";
}
