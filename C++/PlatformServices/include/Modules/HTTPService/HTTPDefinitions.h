#ifndef __HTTP_DEFINITIONS_H__
#define __HTTP_DEFINITIONS_H__

namespace HTTP_SERVICE{
    enum eHTTP_METHODS
    {
        HTTP_GET        = 0,
        HTTP_POST       = 1,
        HTTP_PUT        = 2,
        HTTP_PATCH      = 3,
        HTTP_UNKNOWN    = 4
    };

    enum eCONTENT_TYPE
    {
        APPLICATION_UNKNOWN = 0,
        APPLICATION_JSON    = 1,
        APPLICATION_XML     = 2,
        APPLICATION_MIME    = 3
    };
};

#endif