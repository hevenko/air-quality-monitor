/**
 * misc.h
 * 
 * Misc functions.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#ifndef MISC_H
#define MISC_H

#include <Arduino.h>
#include <map>

// general function type for callbacks (no returning type and w/o parameters)
typedef void (*GeneralFunction) ();

// calculates CRC32
uint32_t calculateCRC32(const uint8_t *data, size_t length);

// non-blocking delay
void delayNonBlocking(unsigned long pause, GeneralFunction cb = NULL);

// translates int into String
String translate(std::map<int,String> m, int value);

// print out (Serial monitor) of ESP info
void showESPInfo();

/**
 * Ensure leading in string.
 * 
 * "abc", "/" => "/abc"
 * "/abc", "/" => "/abc"
 */
String ensureLeft(String s, String leading);

/**
 * Ensure trailing in string.
 * 
 * "abc", "/" => "abc/"
 * "abc/", "/" => "abc/"
 */
String ensureRight(String s, String trailing);

/**
 * Ensure both leading and trailing in string.
 * Same as ensureBoth(s, both, both)
 * 
 * "abc", "/" => "/abc/"
 * "/abc", "/" => "/abc/"
 */
String ensureBoth(String s, String both);

/**
 * Ensure both leading and trailing in string.
 * "abc", "/", "\" => "/abc\"
 * "/abc", "/", "\" => "/abc\"
 */
String ensureBoth(String s, String leading, String trailing);

/**
 * Ensure leading and trim trailing.
 * Same as ensure ensureLeftTrimRight(s, both, both)
 * 
 * "abc", "/" => "/abc"
 * "/abc/", "/" => "/abc"
 * "abc/", "/" => "/abc"
 */
String ensureLeftTrimRight(String s, String both);

/**
 * Ensure leading and trim trailing.
 * 
 * "abc  ", "/", " " => "/abc"
 * "/abc  ", "/", " " => "/abc"
 * "abc  ", "/", " " => "/abc"
 */
String ensureLeftTrimRight(String s, String leading, String trailing);

/**
 * Trim leading and ensure trailing.
 * Same as trimLeftEnsureRight(s, both, both)
 * 
 * "abc", "/" => "abc/"
 * "/abc/", "/" => "abc/"
 * "/abc", "/" => "abc/"
 */
String trimLeftEnsureRight(String s, String both);

/**
 * Trim leading and ensure trailing.
 * 
 * "  abc", " ", "/" => "abc/"
 * "  abc/", " ", "/" => "abc/"
 * "  abc", " ", "/" => "abc/"
 */
String trimLeftEnsureRight(String s, String leading, String trailing);

/**
 * Trim leading string.
 * "   a", " " => "a"
 * "  a ", " " => "a "
 */
String trimLeft(String s, String leading);

/**
 * Trim trailing string.
 * 
 * "a  ", " " => "a"
 * " a ", " " => " a"
 */
String trimRight(String s, String trailing);

/**
 * Trim leading and trailing.
 * 
 * "a  ", " " => "a"
 * " a ", " " => "a"
 * "  a", " " => "a"
 */
String trimBoth(String s, String both);

/**
 * Trim leading and trailing strings.
 * 
 * "*a  ", " ", "*" => "*a  "
 * "  a*", " ", "*" => "a"
 * "  *a* ", " ", "*" => "*a* "
 */
String trimBoth(String s, String leading, String trailing);

/**
 * Format double value with decimals and returns String.
 * To convert String into dobule value use String.toDouble()
 */
String formatAsFloat(double value, byte decimals);

#endif
