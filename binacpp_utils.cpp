
#include "binacpp_utils.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
//--------------------------------
void split_string( string &s, char delim, vector <string> &result) {

    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    
}



//--------------------------------
int replace_string_once( string& str, const char *from, const char *to, int offset) {

    size_t start_pos = str.find(from, offset);
    if( start_pos == std::string::npos ) {
        return 0;
    }
    str.replace(start_pos, strlen(from), to);
    return start_pos + strlen(to);
}


//--------------------------------
bool replace_string( string& str, const char *from, const char *to) {

    bool found = false;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, strlen( from ), to);
        found = true;
        start_pos += strlen(to);
    }
    return found;
}


//-----------------------
void string_toupper( string &src) {
    for ( int i = 0 ; i < src.size() ; i++ ) {
        src[i] = toupper(src[i]);
    }
}

//------------------
string string_toupper( const char *cstr ) {
    string ret;
    for ( int i = 0 ; i < strlen( cstr ) ; i++ ) {
        ret.push_back( toupper(cstr[i]) );
    }
    return ret;
}


//--------------------------------------
string b2a_hex( char *byte_arr, int n ) {

    const static std::string HexCodes = "0123456789abcdef";
    string HexString;
    for ( int i = 0; i < n ; ++i ) {
        unsigned char BinValue = byte_arr[i];
        HexString += HexCodes[( BinValue >> 4 ) & 0x0F];
        HexString += HexCodes[BinValue & 0x0F];
    }
    return HexString;
}



//---------------------------------
time_t get_current_epoch( ) {

    //struct timeval tv;
    //gettimeofday(&tv, NULL); 

    //return tv.tv_sec ;
    __time64_t Time;
    _time64(&Time);
    return Time;
}

//---------------------------------
time_t get_current_ms_epoch( ) {

    //struct timeval tv;
    //gettimeofday(&tv, NULL); 

    //return tv.tv_sec * 1000 + tv.tv_usec / 1000 ;
    __time64_t Time;
    _time64(&Time);
    return Time * 1000;

}

//---------------------------
string hmac_sha256( const char *key, const char *data) {

    unsigned char* digest;
    digest = HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)data, strlen(data), NULL, NULL);    
    return b2a_hex( (char *)digest, 32 );
}   

//------------------------------
string sha256( const char *data ) {

    unsigned char digest[32];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, strlen(data) );
    SHA256_Final(digest, &sha256);
    return b2a_hex( (char *)digest, 32 );
    
}
