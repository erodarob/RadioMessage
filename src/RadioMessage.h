#ifndef RADIO_MESSAGE_H
#define RADIO_MESSAGE_H

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__unix__) || defined(__APPLE__) // Windows, Linux, or OSX
#include <cstdint>
#endif

#include "Message.h"

#include "Message/GSMessage.h"

#include "Data.h"
// pull APRS classes
#include "APRS/APRSData.h"
#include "APRS/APRSTelem.h"
#include "APRS/APRSCmd.h"
#include "APRS/APRSText.h"
// pull Astra classes
// #include "Astra/AstraData.h"
// pull GroundStation classes
#include "GroundStation/GSControl.h"
#include "GroundStation/Metrics.h"
// pull Raw classes
#include "Raw/GenericData.h"
#include "Raw/VideoData.h"
// pull Types
#include "Types/PackedNum.h"

#endif