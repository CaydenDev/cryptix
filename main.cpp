#include <iostream>
#include <string>
#include <cryptlib.h>
#include <aes.h>
#include <modes.h>
#include <filters.h>
#include <osrng.h>
#include <hex.h>

using namespace CryptoPP;

void generateKey(byte key[AES::DEFAULT_KEYLENGTH]) {
    AutoSeededRandomPool prng;
    prng.GenerateBlock(key, AES::DEFAULT_KEYLENGTH);
    std::cout << "Schlüssel generiert: ";
    StringSource(key, AES::DEFAULT_KEYLENGTH, true,
        new HexEncoder(new FileSink(std::cout))); 
    std::cout << std::endl;
}

std::string encrypt(const std::string& plaintext, const byte key[AES::DEFAULT_KEYLENGTH]) {
    std::string ciphertext;
    try {
        
        byte iv[AES::BLOCKSIZE];
        AutoSeededRandomPool prng;
        prng.GenerateBlock(iv, sizeof(iv));

        
        CFB_Mode<AES>::Encryption encryption(key, AES::DEFAULT_KEYLENGTH, iv);
        StringSource(plaintext, true,
            new StreamTransformationFilter(encryption,
                new StringSink(ciphertext)
            )
        );

        
        ciphertext.insert(0, (char*)iv, AES::BLOCKSIZE);
    }
    catch (const Exception& e) {
        std::cerr << "Fehler bei der Verschlüsselung: " << e.what() << std::endl;
    }
    return ciphertext;
}

std::string decrypt(const std::string& ciphertext, const byte key[AES::DEFAULT_KEYLENGTH]) {
    std::string decryptedtext;
    try {
        
        byte iv[AES::BLOCKSIZE];
        memcpy(iv, ciphertext.data(), AES::BLOCKSIZE);
        std::string actualCiphertext = ciphertext.substr(AES::BLOCKSIZE);

        
        CFB_Mode<AES>::Decryption decryption(key, AES::DEFAULT_KEYLENGTH, iv);
        StringSource(actualCiphertext, true,
            new StreamTransformationFilter(decryption,
                new StringSink(decryptedtext)
            )
        );
    }
    catch (const Exception& e) {
        std::cerr << "Fehler bei der Entschlüsselung: " << e.what() << std::endl;
    }
    return decryptedtext;
}

int main() {
    byte key[AES::DEFAULT_KEYLENGTH];
    generateKey(key);

    std::string plaintext = "Das ist eine geheime Nachricht.";
    std::string encrypted = encrypt(plaintext, key);
    std::cout << "Verschlüsselte Nachricht: ";
    StringSource(encrypted, true,
        new HexEncoder(new FileSink(std::cout)));
    std::cout << std::endl;

    std::string decrypted = decrypt(encrypted, key);
    std::cout << "Entschlüsselte Nachricht: " << decrypted << std::endl;

    return 0;
}
