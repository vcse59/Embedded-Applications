#ifndef __HTTP_DEFINITIONS_H__
#define __HTTP_DEFINITIONS_H__

namespace HTTP_SERVICE{
    enum eHTTP_METHODS
    {
        HTTP_GET        = 0,
        HTTP_POST       = 1,
        HTTP_PATCH      = 2,
        HTTP_PUT        = 3,
        HTTP_OPTIONS    = 4,
        HTTP_HEAD       = 5,
        HTTP_DELETE     = 6,
        HTTP_CONNECT    = 7,
        HTTP_TRACE      = 8,
        HTTP_UNKNOWN    = 9
    };

    static std::unordered_map<std::string, eHTTP_METHODS> HTTP_METHOD = {  
                                                            {"GET",     eHTTP_METHODS::HTTP_GET},
                                                            {"POST",    eHTTP_METHODS::HTTP_POST},
                                                            {"PATCH",   eHTTP_METHODS::HTTP_PATCH},
                                                            {"PUT",     eHTTP_METHODS::HTTP_PUT},
                                                            {"OPTIONS", eHTTP_METHODS::HTTP_OPTIONS},
                                                            {"HEAD",    eHTTP_METHODS::HTTP_HEAD},
                                                            {"DELETE",  eHTTP_METHODS::HTTP_DELETE},
                                                            {"CONNECT", eHTTP_METHODS::HTTP_CONNECT},
                                                            {"TRACE",   eHTTP_METHODS::HTTP_TRACE},
                                                            {"UNKNOWN", eHTTP_METHODS::HTTP_UNKNOWN}
                                                            };

    enum eCONTENT_TYPE
    {
        APPLICATION_UNKNOWN = 0,
        APPLICATION_JSON    = 1,
        APPLICATION_XML     = 2,
        APPLICATION_MIME    = 3
    };

    enum eHEADER_FIELD{
        HEADER_ACCEPT                       = 0,
        HEADER_ACCEPT_ENCODING              = 1,
        HEADER_ACCEPT_LANGUAGE              = 2,
        HEADER_CACHE_CONTORL                = 3,
        HEADER_CONNECTION                   = 4,
        HEADER_COOKIE                       = 5,
        HEADER_HOST                         = 6,
        HEADER_UPGRADE_INSECURE_REQUESTS    = 7,
        HEADER_USER_AGENT                   = 8,
        HEADER_X_REAL_IP                    = 9,
        HEADER_X_FORWARDED_FOR              = 10,
        HEADER_X_FORWARDED_PROTO            = 11,
        HEADER_REFERER                      = 12,
        HEADER_METHOD                       = 13,
        HEADER_RESOURCE_URL                 = 14,
        HEADER_HTTP_ERROR                   = 15,
        HEADER_AUTHORIZATION                = 16
    };

    static std::unordered_map<std::string, eHEADER_FIELD> HEADER_STRING  = { 
        {"Accept", eHEADER_FIELD::HEADER_ACCEPT},
        {"Accept-Encoding", eHEADER_FIELD::HEADER_ACCEPT_ENCODING},
        {"Accept-Language", eHEADER_FIELD::HEADER_ACCEPT_LANGUAGE},
        {"Cache-Control", eHEADER_FIELD::HEADER_CACHE_CONTORL},
        {"Connection", eHEADER_FIELD::HEADER_CONNECTION},
        {"Cookie", eHEADER_FIELD::HEADER_COOKIE},
        {"Host", eHEADER_FIELD::HEADER_HOST},
        {"Upgrade-Insecure-Requests", eHEADER_FIELD::HEADER_UPGRADE_INSECURE_REQUESTS},
        {"User-Agent", eHEADER_FIELD::HEADER_USER_AGENT},
        {"X-Real-IP", eHEADER_FIELD::HEADER_X_REAL_IP},
        {"X-Forwarded-For", eHEADER_FIELD::HEADER_X_FORWARDED_FOR},
        {"X-Forwarded-Proto", eHEADER_FIELD::HEADER_X_FORWARDED_PROTO},
        {"Authorization", eHEADER_FIELD::HEADER_AUTHORIZATION}
        };

    enum eHTTP_ERROR
    {
        INVALID_HTTP_METHOD     = 0,
        INVALID_RESOURCE_URL    = 1,
        INVALID_HOST            = 2,
        INVALID_UNKNOWN         = 3,
        NO_ERROR                = 4
    };

    static std::unordered_map<HTTP_SERVICE::eHTTP_ERROR, std::string> HTTP_ERROR= {
        {HTTP_SERVICE::eHTTP_ERROR::INVALID_HOST, "Invalid Host in Header....Redirect to login page"},                                                                        
        {HTTP_SERVICE::eHTTP_ERROR::INVALID_HTTP_METHOD, "Invalid HTTP Method in Header....Redirect to login page"},                                                                        
        {HTTP_SERVICE::eHTTP_ERROR::INVALID_RESOURCE_URL, "Invalid Resource path in Header....Redirect to login page"},                                                                        
        {HTTP_SERVICE::eHTTP_ERROR::INVALID_UNKNOWN, "Unknown Request....Redirect to login page"},
        {HTTP_SERVICE::eHTTP_ERROR::NO_ERROR, "No Error Found"}                                                                        
        };
};

#endif