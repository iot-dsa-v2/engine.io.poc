#pragma once
#include <string>
#include <map>

using namespace std;

struct PayloadGenerator 
{
	char size = 128;
	string ant_1byte = "I";
	string bee_2bytes = "HI";
	string bug_4bytes = "yllo";

	string first = "working?";
	string data = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567";

	string payload_1K = data + data + data + data + data + data + data + data;
	string payload_8K = payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K;
	string payload_64K = payload_8K + payload_8K + payload_8K + payload_8K + payload_8K + payload_8K + payload_8K + payload_8K;
	string payload_128K = payload_64K + payload_64K;

	map<string, string> payloadMap;

	PayloadGenerator() {
		payloadMap["1b"] = ant_1byte;
		payloadMap["2b"] = bee_2bytes;
		payloadMap["4b"] = bug_4bytes;
		payloadMap["8b"] = first;
		payloadMap["64b"] = ant_1byte;
		payloadMap["128b"] = data;
		payloadMap["1024"] = payload_1K;
		payloadMap["8k"] = payload_8K;
		payloadMap["64k"] = payload_64K;
		payloadMap["128k"] = payload_128K;
	}

	const char* getPayload(string& key) {
		return payloadMap[key].c_str();
	}
};
