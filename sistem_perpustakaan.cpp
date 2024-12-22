#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

// Struct untuk data buku
struct Buku {
    string judul;
    string penulis;
    string noBuku;
    bool dipinjam;
    string peminjam;
};

// Array untuk menyimpan buku
const int MAX_BUKU = 100;
Buku daftarBuku[MAX_BUKU];
int jumlahBuku = 0;




// Fungsi untuk menghasilkan nomor acak dalam range tertentu
int generateRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// Fungsi untuk mengubah integer menjadi string dengan padding nol
string generatePaddedNumber(int num, int width) {
    string result = to_string(num);
    while (result.length() < width) {
        result = "0" + result;
    }
    return result;
}

// Fungsi untuk menghasilkan nomor seri acak
string generateNOMORSERI() {
    // Format: YYYY-DEPT-XXXXX
    // YYYY: Tahun
    // DEPT: Kode departemen (2 digit)
    // XXXXX: Nomor urut acak (5 digit)
    
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    
    // Generate departemen acak (01-05)
    int dept = generateRandomNumber(1, 5);
    // Generate nomor urut acak (00000-99999)
    int sequence = generateRandomNumber(0, 99999);
    
    string NOMORSERI = to_string(year) + "-" + 
                         generatePaddedNumber(dept, 2) + "-" + 
                         generatePaddedNumber(sequence, 5);
    
    return NOMORSERI;
}

// Fungsi untuk memeriksa apakah nomor seri sudah ada
bool isNOMORSERIExists(const string& NOMORSERI) {
    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].noBuku == NOMORSERI) {
            return true;
        }
    }
    return false;
}

// Fungsi menyimpan data buku ke file
void simpanKeFile() {
    ofstream file("buku.txt");
    if (file.is_open()) {
        file << jumlahBuku << endl; // Simpan jumlah buku
        for (int i = 0; i < jumlahBuku; i++) {
            file << daftarBuku[i].judul << endl
                 << daftarBuku[i].penulis << endl
                 << daftarBuku[i].noBuku << endl
                 << daftarBuku[i].dipinjam << endl
                 << daftarBuku[i].peminjam << endl;
        }
        file.close();
    } else {
        cout << "Gagal menyimpan ke file.\n";
    }
}

// Fungsi memuat data buku dari file
void muatDariFile() {
    ifstream file("buku.txt");
    if (file.is_open()) {
        file >> jumlahBuku;
        file.ignore(); // Mengabaikan karakter newline
        for (int i = 0; i < jumlahBuku; i++) {
            getline(file, daftarBuku[i].judul);
            getline(file, daftarBuku[i].penulis);
            getline(file, daftarBuku[i].noBuku);
            file >> daftarBuku[i].dipinjam;
            file.ignore();
            getline(file, daftarBuku[i].peminjam);
        }
        file.close();
    } else {
        cout << "File data tidak ditemukan, mulai dengan data kosong.\n";
    }
}

// Fungsi untuk otentikasi pengguna
bool login(string &role) {
    string username, password;
    cout << "Masukkan Username: ";
    cin >> username;
    cout << "Masukkan Password: ";
    cin >> password;

    if (username == "admin" && password == "1234") {
        role = "Pegawai";
        return true;
    } else if (username == "user" && password == "0000") {
        role = "Pengguna";
        return true;
    } else {
        cout << "Login gagal! Coba lagi.\n";
        return false;
    }
}

//fungsi tambah buku pegawai
void tambahBuku() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));  // Inisialisasi seed untuk random number generator
        seeded = true;
    }

    if (jumlahBuku >= MAX_BUKU) {
        cout << "Perpustakaan penuh!\n";
        return;
    }

    cout << "Masukkan Judul Buku: ";
    cin.ignore();
    getline(cin, daftarBuku[jumlahBuku].judul);
    
    cout << "Masukkan Penulis Buku: ";
    getline(cin, daftarBuku[jumlahBuku].penulis);
    
    // Generate unique serial number
    string NOMORSERI;
    do {
        NOMORSERI = generateNOMORSERI();
    } while (isNOMORSERIExists(NOMORSERI));
    
    daftarBuku[jumlahBuku].noBuku = NOMORSERI;
    cout << "Nomor Seri : " << NOMORSERI << endl;
    
    daftarBuku[jumlahBuku].dipinjam = false;
    daftarBuku[jumlahBuku].peminjam = "";
    
    jumlahBuku++;
    simpanKeFile();
    cout << "Buku berhasil ditambahkan!\n";
}

// Fungsi untuk menampilkan semua buku
void tampilkanBuku() {
    cout << "\nDaftar Buku:\n";
    for (int i = 0; i < jumlahBuku; i++) {
        cout << i + 1 << ". Judul: " << daftarBuku[i].judul
             << ", Penulis: " << daftarBuku[i].penulis
             << ", Nomor Seri: "<< daftarBuku[i].noBuku
             << ", Status: " << (daftarBuku[i].dipinjam ? "Dipinjam oleh " + daftarBuku[i].peminjam : "Tersedia") << endl;
    }
}

// Fungsi untuk mengubah string menjadi huruf kecil
string toLowercase(string str) {
    for (char &c : str) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
    return str;
}

// Fungsi untuk mencari buku berdasarkan berbagai kriteria
void cariBuku() {
    string keyword;
    cout << "Masukkan Nomor Seri/Judul/Penulis yang dicari: ";
    cin.ignore();
    getline(cin, keyword);

    // Ubah keyword menjadi huruf kecil
    string lowercaseKeyword = toLowercase(keyword);

    bool ditemukan = false;
    cout << "\nHasil Pencarian:\n";
    for (int i = 0; i < jumlahBuku; i++) {
        // Ubah judul, penulis, dan nomor seri menjadi huruf kecil untuk pencarian
        string lowercaseJudul = toLowercase(daftarBuku[i].judul);
        string lowercasePenulis = toLowercase(daftarBuku[i].penulis);
        string lowercaseNoBuku = toLowercase(daftarBuku[i].noBuku);

        // Cek apakah keyword ada di nomor seri, judul, atau penulis
        if (lowercaseNoBuku.find(lowercaseKeyword) != string::npos || 
            lowercaseJudul.find(lowercaseKeyword) != string::npos || 
            lowercasePenulis.find(lowercaseKeyword) != string::npos) {
            // Tampilkan detail buku yang ditemukan
            cout << "Ditemukan: \n"
                 << "  Judul: " << daftarBuku[i].judul << "\n"
                 << "  Penulis: " << daftarBuku[i].penulis << "\n"
                 << "  Nomor Seri: " << daftarBuku[i].noBuku << "\n"
                 << "  Status: " << (daftarBuku[i].dipinjam ? "Dipinjam oleh " + daftarBuku[i].peminjam : "Tersedia") << endl;
            ditemukan = true;
        }
    }
    
    // Tampilkan pesan jika tidak ada buku yang ditemukan
    if (!ditemukan) {
        cout << "Buku tidak ditemukan.\n";
    }
}

// Fungsi untuk meminjam atau mengembalikan buku
void pinjamDanKembalikanBuku(string pengguna) {
    cout << "\nMenu Pinjam/Kembalikan Buku:\n";
    cout << "1. Pinjam Buku\n";
    cout << "2. Kembalikan Buku\n";
    cout << "Pilih opsi: ";
    int opsi;
    cin >> opsi;

    if (opsi == 1) { // Pinjam Buku
        tampilkanBuku();
        int pilihan;
        cout << "Masukkan nomor buku yang ingin dipinjam: ";
        cin >> pilihan;

        if (pilihan >= 1 && pilihan <= jumlahBuku) {
            if (daftarBuku[pilihan - 1].dipinjam) {
                cout << "Buku sudah dipinjam.\n";
            } else {
                daftarBuku[pilihan - 1].dipinjam = true;
                daftarBuku[pilihan - 1].peminjam = pengguna;
                simpanKeFile();
                cout << "Buku berhasil dipinjam.\n";
            }
        } else {
            cout << "Nomor buku tidak valid!\n";
        }
    } else if (opsi == 2) { // Kembalikan Buku
        cout << "\nDaftar Buku yang Anda Pinjam:\n";
        bool adaBukuDipinjam = false;
        for (int i = 0; i < jumlahBuku; i++) {
            if (daftarBuku[i].dipinjam && daftarBuku[i].peminjam == pengguna) {
                cout << i + 1 << ". " << daftarBuku[i].judul << endl;
                adaBukuDipinjam = true;
            }
        }

        if (!adaBukuDipinjam) {
            cout << "Anda tidak meminjam buku apapun.\n";
            return;
        }

        cout << "Masukkan nomor buku yang ingin dikembalikan (pisahkan dengan spasi, akhiri dengan -1): ";
        int pilihan;
        while (true) {
            cin >> pilihan;
            if (pilihan == -1) break; // Selesai
            if (pilihan >= 1 && pilihan <= jumlahBuku &&
                daftarBuku[pilihan - 1].dipinjam && daftarBuku[pilihan - 1].peminjam == pengguna) {
                cout << "Mengembalikan buku: " << daftarBuku[pilihan - 1].judul << endl;
                daftarBuku[pilihan - 1].dipinjam = false;
                daftarBuku[pilihan - 1].peminjam = "";
            } else {
                cout << "Nomor buku tidak valid atau bukan milik Anda!\n";
            }
        }
        simpanKeFile();
        cout << "Buku berhasil dikembalikan.\n";
    } else {
        cout << "Opsi tidak valid!\n";
    }
}

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// [Seluruh kode struct dan fungsi sebelumnya tetap sama]
// ... [semua kode sebelum main() tidak berubah]

// Fungsi utama
int main() {
    string role;
    muatDariFile(); // Muat data buku dari file

    while (true) { // Loop utama program
        cout << "=== Sistem Perpustakaan ===\n";
        
        // Proses login
        while (!login(role)); // Loop sampai login berhasil

        int pilihan;
        do {
            cout << "\nMenu:\n";
            if (role == "Pegawai") {
                cout << "1. Tambah Buku\n";
            }
            cout << "2. Tampilkan Daftar Buku\n";
            cout << "3. Cari Buku\n";
            if (role == "Pengguna") {
                cout << "4. Pinjam/Kembalikan Buku\n";
            }
            cout << "5. Logout\n";
            cout << "0. Keluar\n";
            cout << "Pilih menu: ";
            cin >> pilihan;

            switch (pilihan) {
                case 1:
                    if (role == "Pegawai") tambahBuku();
                    else cout << "Menu tidak tersedia.\n";
                    break;
                case 2:
                    tampilkanBuku();
                    break;
                case 3:
                    cariBuku();
                    break;
                case 4:
                    if (role == "Pengguna") pinjamDanKembalikanBuku(role);
                    else cout << "Menu tidak tersedia.\n";
                    break;
                case 5: // Logout
                    cout << "Anda telah logout.\n";
                    break; // Keluar dari switch, kembali ke login
                case 0:
                    cout << "Terima kasih telah menggunakan sistem perpustakaan.\n";
                    return 0; // Keluar dari program sepenuhnya
                default:
                    cout << "Pilihan tidak valid!\n";
            }

            // Jika logout, keluar dari inner loop dan kembali ke login
            if (pilihan == 5) break;
        } while (pilihan != 0);
    }

    return 0;
}