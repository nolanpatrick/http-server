#pragma once
// HTTP GET REQUEST:
//   GET /absolute%20path%20to%20resource/ HTTP/1.1
//   Host: 127.0.0.1:8080
//   User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0
//   Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
//   Accept-Language: en-US,en;q=0.5
//   Accept-Encoding: gzip, deflate, br
//   DNT: 1
//   Connection: keep-alive
//   Upgrade-Insecure-Requests: 1
//   Sec-Fetch-Dest: document
//   Sec-Fetch-Mode: navigate
//   Sec-Fetch-Site: none
//   Sec-Fetch-User: ?1

/*
https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods

HTTP Request Methods:
 - GET - retreive a specified resource
 - HEAD - response identical to GET minus response body
 - POST - submit entry to resource
 - PUT - replace representation of resource
 - DELETE - delete a specified resource
 - CONNECT - establish a tunnel to server
 - OPTIONS - describe communication options
 - TRACE - loopback test
 - PATCH - partial modification to resource

https://en.wikipedia.org/wiki/List_of_HTTP_header_fields
Standard Request Fields:
 - A-IM
 - Accept
 - Accept-Charset
 - Accept-Datetime
 - Accept-Encoding
 - Accept-Language
 - Access-Control-Request-Method
 - Access-Control-Request-Headers
 - Authorization
 - Cache-Control
 - Connection
 - Content-Encoding
 - Content-Length
 - Content-MD5
 - Content-Type
 - Cookie
 - Date
 - Expect
 - Forwarded
 - From
 - Host
 - HTTP2-Settings
 - If-Match
 - If-Modified-Since
 - If-None-Match
 - If-Range
 - If-Unmodified-Since
 - Max-Forwards
 - Origin
 - Pragma
 - Prefer
 - Proxy-Authorization
 - Range
 - Referer
 - TE
 - Trailer
 - Transfer-Encoding
 - User-Agent
 - Upgrade
 - Via
 - Warning
*/

enum http_request_type {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

// https://httpwg.org/specs/rfc9110.html#header.fields
// struct http_request_fields { 
    // A-IM
    // Accept
    // Accept-Charset
    // Accept-Datetime
    // Accept-Encoding
    // Accept-Language
    // Access-Control-Request-Method
    // Access-Control-Request-Headers
    // Authorization
    // Cache-Control
    // Connection
    // Content-Encoding
    // Content-Length
    // Content-MD5
    // Content-Type
    // Cookie
    // Date
    // Expect
    // Forwarded
    // From
    // Host
    // HTTP2-Settings
    // If-Match
    // If-Modified-Since
    // If-None-Match
    // If-Range
    // If-Unmodified-Since
    // Max-Forwards
    // Origin
    // Pragma
    // Prefer
    // Proxy-Authorization
    // Range
    // Referer
    // TE
    // Trailer
    // Transfer-Encoding
    // User-Agent
    // Upgrade
    // Via
    // Warning
// };

enum http_request_type get_request_type(unsigned char *buf) {
    return 0;
}

