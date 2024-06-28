#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <unordered_map>

#include "Modules/HTTPService/HTTPUtility.h"

using namespace HTTP_SERVICE;

std::string HTTPUtility::readIndexHtml(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();


    return buffer.str();
}

std::string HTTPUtility::generateSessionID() {
    // Generate a random seed using current time
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());

    // Use a random device and a random number generator to generate random bytes
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(seed);

    // Generate a random nonce of 16 bytes
    std::vector<unsigned char> nonce(16);
    for (size_t i = 0; i < nonce.size(); ++i) {
        nonce[i] = static_cast<unsigned char>(gen());
    }

    // Convert the nonce to a hexadecimal string
    std::stringstream ss;
    for (const auto& byte : nonce) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

std::string HTTPUtility::base64_encode(const std::string &input) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, input.c_str(), input.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);
    return result;
}

std::string HTTPUtility::base64_decode(const std::string &input) {
    BIO *bio, *b64;
    int decodeLen = input.size();
    char *decodedData = new char[decodeLen + 1];
    memcpy(decodedData, input.c_str(), input.size());
    decodedData[input.size()] = '\0';
    bio = BIO_new_mem_buf(decodedData, -1);
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);
    BIO_set_close(bio, BIO_NOCLOSE);
    int len = BIO_read(bio, decodedData, input.size());
    decodedData[len] = '\0';
    std::string result(decodedData);
    BIO_free_all(bio);
    delete[] decodedData;
    return result;
}