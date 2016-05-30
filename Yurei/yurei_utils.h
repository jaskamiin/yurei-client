#ifndef YUREI_UTILS_H
#define YUREI_UTILS_H

#include <string>

typedef int EXCEPTION;

#define ERROR_LOG_FN "../ErrorLog.txt"

//return value definitions -- using 10* format for uniqueness...
#define BOUNDS_ERROR			100
#define BENCODE_SUCCESS			101
#define BENCODE_FAILURE			102
#define BENCODE_INT_NONE_FOUND	103
#define BENCODE_INT_BAD_FORMAT	104

//exception messages -- kind of long names but whatever
#define BOUNDS_ERROR_MSG(loc)			"Out of bounds error: index ran out of bounds in " loc
#define BENCODE_FAILURE_MSG				"Undefined bencoding failure"
#define BENCODE_INT_NONE_FOUND_MSG		"Bencoding failure: torrent file contains invalid format for integer: no integer found"
#define BENCODE_INT_BAD_FORMAT_MSG		"Bencoding failure: torrent file contains invalid format `ie`"


//various other utility function declarations
int count_digits(long long);

#endif  /*YUREI_UTILS_H*/