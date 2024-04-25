#include <fstream>
#include <sstream>

#include "Modules/HTTPParser.h"

using namespace HTTP_SERVICE;

std::string HTTPParser::parseHttpResponse(const char* httpResponse){

    char responseBody[MAX_BODY_SIZE] = {'\0',};
    // Find end of headers
    const char *headers_end = strstr(httpResponse, "\r\n\r\n");
    if (headers_end == NULL) {
        printf("Invalid HTTP response: no headers found\n");
        return std::string(responseBody);
    }

    // Print headers
    printf("HTTP Headers:\n");
    fwrite(httpResponse, 1, headers_end - httpResponse, stdout);
    printf("\n");

    // Print body, if any
    if (*(headers_end + 4) != '\0') {
        printf("HTTP Body:\n%s\n", headers_end + 4);
        strcpy(responseBody, headers_end + 4);
    } else {
        printf("No HTTP Body\n");
    }

    return std::string(responseBody);
}

std::string HTTPParser::readIndexHtml(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Output the contents of the file
    std::cout << "Contents of index.html:\n" << buffer.str() << std::endl;

    return buffer.str();
}

std::string HTTPParser::generateHttpResponse(const std::string& content) {
    std::cout << "CONTENT LENGTH***************" << content.length() << std::endl;
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << content.size() << "\r\n\r\n";
    response << content;
    return response.str();
}