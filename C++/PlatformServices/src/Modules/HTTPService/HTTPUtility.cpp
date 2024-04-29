#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <openssl/sha.h>
#include <unordered_map>

#include "Modules/HTTPService/HTTPUtility.h"

using namespace HTTP_SERVICE;

std::string HTTPUtility::parseHttpResponse(const char* httpResponse){

    char responseBody[MAX_BODY_SIZE] = {'\0',};

    // Find end of headers
    const char *headers_end = strstr(httpResponse, "\r\n\r\n");
    if (headers_end == NULL) {
        printf("Invalid HTTP response: no headers found\n");
        return std::string(responseBody);
    }

    // Print headers
    /*printf("HTTP Headers:\n");
    fwrite(httpResponse, 1, headers_end - httpResponse, stdout);
    printf("\n");*/

    // Print body, if any
    if (*(headers_end + 4) != '\0') {
        //printf("HTTP Body:\n%s\n", headers_end + 4);
        strcpy(responseBody, headers_end + 4);
    } else {
        printf("No HTTP Body\n");
    }

    return std::string(responseBody);
}

std::string HTTPUtility::readIndexHtml(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Output the contents of the file
    //std::cout << "Contents of index.html:\n" << buffer.str() << std::endl;

    return buffer.str();
}

std::string HTTPUtility::generateHttpResponse(const std::string& content, const std::string& sessionId) {
    //std::cout << "CONTENT LENGTH***************" << sessionId << std::endl;
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Set-Cookie: sessionID=" << sessionId << "\r\n"; // Embed session ID in cookie
    response << "Content-Length: " << content.size() << "\r\n\r\n";
    response << content;

    //std::cout << "RESPONSE : " << response.str() << std::endl;
    return response.str();
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

// Function to parse HTTP headers
std::unordered_map<std::string, std::string> HTTPUtility::parseHttpHeaders(const std::string& headers, std::string& sessionId) {

    std::unordered_map<std::string, std::string> headerMap;

    // Create a string stream to process the headers line by line
    std::istringstream iss(headers);
    std::string line;
    while (std::getline(iss, line)) {
        // Check for empty line, which indicates end of headers
        if (line.empty()) {
            break;
        }

        // Find the position of the colon (:) to split the header name and value
        auto pos = line.find(':');
        if (pos != std::string::npos) {
            // Extract header name and value
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);

            // Remove leading and trailing whitespaces from header value
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);

            // Store header name and value in the map
            headerMap[headerName] = headerValue;

            // Check if the header is Set-Cookie and extract session ID
            if (headerName == "Set-Cookie") {
                // Find the position of the equals sign (=) to split the cookie name and value
                auto equalPos = headerValue.find('=');
                if (equalPos != std::string::npos) {
                    // Extract cookie name and value
                    std::string cookieName = headerValue.substr(0, equalPos);
                    std::string cookieValue = headerValue.substr(equalPos + 1);

                    // Check if the cookie name is sessionID
                    if (cookieName == "sessionID") {
                        // Set the session ID
                        sessionId = cookieValue;
                    }
                }
            }
        }
    }

    return headerMap;
}