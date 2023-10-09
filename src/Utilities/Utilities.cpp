#include "Utilities.h"

String versionToString(const versionNumber& version) {
    return String(version.majorVersion) + "." + String(version.minorVersion) + "." + String(version.patchVersion);
}

bool isPositiveInteger(const String& str) {
  if (str.length() == 0) {
    return false;
  }
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!isDigit(str.charAt(i))) {
      return false;
    }
  }
  return true;
}

bool isInteger(String str) {
    if (str.length() == 0) {
        return false;
    }
    
    for (int i = 0; i < str.length(); i++) {
        // Check for a negative sign at the first position
        if (i == 0 && str[i] == '-') {
            continue;
        }
        
        // Check if each character is a digit
        if (!isDigit(str[i])) {
            return false;
        }
    }
    
    return true;
}