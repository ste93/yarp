// WebSocket, v1.00 2012-09-13
//
// Description: WebSocket RFC6544 codec, written in C++.
// Homepage: http://katzarsky.github.com/WebSocket
// Author: katzarsky@gmail.com

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <yarp/os/ConnectionState.h>
#include <yarp/os/ManagedBytes.h>

#include <assert.h>
#include <cstring>
#include <ctype.h>  /* isdigit */
#include <stddef.h> /* int */
#include <stdint.h> /* uint8_t */
#include <stdio.h>  /* sscanf */
#include <string>
#include <vector>

using namespace std;


enum WebSocketFrameType
{
    ERROR_FRAME = 0xFF00,
    INCOMPLETE_FRAME = 0xFE00,

    OPENING_FRAME = 0x3300,
    CLOSING_FRAME = 0x3400,

    // this means at least another message need to be received
    INCOMPLETE_TEXT_FRAME = 0x01,
    INCOMPLETE_BINARY_FRAME = 0x02,

    //this means that is the last frame sended (no other splits)
    TEXT_FRAME = 0x81,
    BINARY_FRAME = 0x82,

    PING_FRAME = 0x19,
    PONG_FRAME = 0x1A
};

class WebSocket
{

public:
    WebSocket();

    /**
	 * @param input_frame .in. pointer to input frame
	 * @param input_len .in. length of input frame
	 * @return [WS_INCOMPLETE_FRAME, WS_ERROR_FRAME, WS_OPENING_FRAME]
	 */
    WebSocketFrameType parseHandshake(unsigned char* input_frame, int input_len);
    string answerHandshake();

private:
    string resource;
    string host;
    string origin;
    string protocol;
    string key;

    string trim(string str);
    vector<string> explode(string theString, string theDelimiter, bool theIncludeEmptyStrings = false);
};

#endif /* WEBSOCKET_H */
