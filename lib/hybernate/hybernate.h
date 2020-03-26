/**
 * hybernate.h
 * 
 * Hybernate functions.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#ifndef HYBERNATE_H
#define HYBERNATE_H

#include <Arduino.h>

/**
 * Sleep for config.delayUS in microseconds
 * config.delayUS is calculated according config.delay and config.delayUnit
 */
void sleep();

/**
 * Wake WiFi after sleep.
 */
void wake();

#endif
