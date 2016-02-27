/**
 * @file util.h
 * @author Thanasis Georgiou
 * @brief Various utilities used by the library
 */

#include "`$INSTANCE_NAME`_util.h"

char NibbleToAscii(char nibble) {
    if (nibble < 0xA) {
        return nibble + 48;
    } else {
        return nibble + 55; // +65 to reach ABCDEF, -9 to remove 0-9
    }
}