/*!
  * @file  Utilities.h
  * @brief This is a header file that contains all utility functions used in the software
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-10-09 
*/
#ifndef UTILITIES_H
#define UTILITIES_H

#include "config.h"

String versionToString(const versionNumber& version);
bool isPositiveInteger(const String& str);
bool isInteger(String str);  

 
#endif