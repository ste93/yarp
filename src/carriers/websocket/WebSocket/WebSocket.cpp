/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

// WebSocket, v1.00 2012-09-13
//
// Description: WebSocket RFC6544 codec, written in C++.
// Homepage: http://katzarsky.github.com/WebSocket
// Author: katzarsky@gmail.com

#include "WebSocket.h"

#include <yarp/os/NetInt32.h>

#include "base64/base64.h"
#include "sha1/sha1.h"
#include <string>
#include <vector>

using namespace std;

WebSocket::WebSocket()
{
}

WebSocketFrameType WebSocket::parseHandshake(unsigned char* input_frame, int input_len)
{
    // 1. copy char*/len into string
    // 2. try to parse headers until \r\n occurs
    string headers((char*)input_frame, input_len);
    int header_end = headers.find("\r\n\r\n");

    if (header_end == string::npos) { // end-of-headers not found - do not parse
        return INCOMPLETE_FRAME;
    }

    headers.resize(header_end); // trim off any data we don't need after the headers
    vector<string> headers_rows = explode(headers, string("\r\n"));
    for (int i = 0; i < headers_rows.size(); i++) {
        string& header = headers_rows[i];
        if (header.find("GET") == 0) {
            vector<string> get_tokens = explode(header, string(" "));
            if (get_tokens.size() >= 2) {
                this->resource = get_tokens[1];
            }
        } else {
            int pos = header.find(":");
            if (pos != string::npos) {
                string header_key(header, 0, pos);
                string header_value(header, pos + 1);
                header_value = trim(header_value);
                if (header_key == "Host")
                    this->host = header_value;
                else if (header_key == "Origin")
                    this->origin = header_value;
                else if (header_key == "Sec-WebSocket-Key")
                    this->key = header_value;
                else if (header_key == "Sec-WebSocket-Protocol")
                    this->protocol = header_value;
            }
        }
    }
    return OPENING_FRAME;
}

string WebSocket::trim(string str)
{
    //printf("TRIM\n");
    char* whitespace = " \t\r\n";
    string::size_type pos = str.find_last_not_of(whitespace);
    if (pos != string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(whitespace);
        if (pos != string::npos)
            str.erase(0, pos);
    } else {
        return string();
    }
    return str;
}

vector<string> WebSocket::explode(
    string theString,
    string theDelimiter,
    bool theIncludeEmptyStrings)
{
    vector<string> theStringVector;
    int start = 0, end = 0, length = 0;

    while (end != string::npos) {
        end = theString.find(theDelimiter, start);

        // If at end, use length=maxLength.  Else use length=end-start.
        length = (end == string::npos) ? string::npos : end - start;

        if (theIncludeEmptyStrings
            || ((length > 0) /* At end, end == length == string::npos */
                && (start < theString.size())))
            theStringVector.push_back(theString.substr(start, length));

        // If at end, use start=maxSize.  Else use start=end+delimiter.
        start = ((end > (string::npos - theDelimiter.size()))
                     ? string::npos
                     : end + theDelimiter.size());
    }
    return theStringVector;
}

string WebSocket::answerHandshake()
{
    unsigned char digest[20]; // 160 bit sha1 digest
    string answer;
    answer += "HTTP/1.1 101 Switching Protocols\r\n";
    answer += "Upgrade: WebSocket\r\n";
    answer += "Connection: Upgrade\r\n";
    if (this->key.length() > 0) {
        string accept_key;
        accept_key += this->key;
        accept_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"; //RFC6544_MAGIC_KEY

        SHA1 sha;
        sha.Input(accept_key.data(), accept_key.size());
        sha.Result((unsigned*)digest);

        // little endian to big endian
        for (int i = 0; i < 20; i += 4) {
            unsigned char c;

            c = digest[i];
            digest[i] = digest[i + 3];
            digest[i + 3] = c;

            c = digest[i + 1];
            digest[i + 1] = digest[i + 2];
            digest[i + 2] = c;
        }
        accept_key = base64_encode((const unsigned char*)digest, 20); //160bit = 20 bytes/chars
        answer += "Sec-WebSocket-Accept: " + (accept_key) + "\r\n";
    }
    if (this->protocol.length() > 0) {
        answer += "Sec-WebSocket-Protocol: " + (this->protocol) + "\r\n";
    }
    answer += "\r\n";
    return answer;
}
