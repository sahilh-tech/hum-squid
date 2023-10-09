/*!
  * @file  ActuatorController.h
  * @brief This is a header file that contains all functionality to control Actuators
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef __ACTUATOR_CONTROLLER_H__
#define __ACTUATOR_CONTROLLER_H__

#include "Arduino.h"
#include "config.h"


class ActuatorController {
public:
    ActuatorController();  // Constructor
    void init();  // Initialization function to be called in setup()

    // Set functions
    void setHVACRequest();
    void setDrumRotation(); 
    void setGreenLED();
    void setYellowLED();

    // Clear functions
    void clearHVACRequest();
    void clearDrumRotation(); 
    void clearGreenLED();
    void clearYellowLED();
};

#endif //  __ACTUATOR_CONTROLLER_H__