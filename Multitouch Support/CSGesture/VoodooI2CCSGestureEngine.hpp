//
//  VoodooI2CCSGestureEngine.hpp
//  VoodooI2C
//
//  Created by Alexandre on 22/09/2017.
//  Copyright © 2017 Alexandre Daoud. All rights reserved.
//

#ifndef VoodooI2CCSGestureEngine_hpp
#define VoodooI2CCSGestureEngine_hpp

#include <libkern/OSBase.h>

#include <IOKit/IOLib.h>
#include <IOKit/IOKitKeys.h>
#include <IOKit/IOService.h>
#include <IOKit/IOTimerEventSource.h>

#include "../VoodooI2CMultitouchEngine.hpp"
#include "../MultitouchHelpers.hpp"
#include "../VoodooI2CDigitiserStylus.hpp"

#include <stdint.h>
#include "VoodooCSGestureHIDWrapper.h"
#include "VoodooCSGestureHIPointingWrapper.hpp"
#include "csgesture-softc.h"
#include "csgesturescroll.h"

#define MAX_FINGERS 15

class VoodooI2CMultitouchInterface;

class VoodooI2CCSGestureEngine : VoodooI2CMultitouchEngine {
    OSDeclareDefaultStructors(VoodooI2CCSGestureEngine);
private:
    VoodooCSGestureHIDWrapper *_wrapper;
    VoodooCSGestureHIPointingWrapper *_pointingWrapper;
    CSGestureScroll *_scrollHandler;
    
    struct {
        UInt8 x;
        UInt8 y;
        UInt8 buttonMask;
    } lastmouse;
	
	enum PinchSpread { Null, GesturePinch, GestureSpread};
	
    IOWorkLoop* work_loop;
    IOTimerEventSource* timer_event_source;
    
    int distancesq(int delta_x, int delta_y);
	bool isSameSign(int n1, int n2);
	bool moveRightUp(int dx, int dy);
	bool moveLeftDown(int dx, int dy);
	bool isThreeFingerPinch(int deltay1, int deltay2, int deltay3);
	bool isThreeFingerSpread(int deltay1, int deltay2, int deltay3);
	bool isFourFingerPinch(int delta1, int delta2, int delta3, int delta4, int deltax1, int deltax2, int deltax3, int deltax4);
	bool isFourFingerSpread(int delta1, int delta2, int delta3, int delta4, int deltax1, int deltax2, int deltax3, int deltax4);
    
    //os callbacks
    void update_relative_mouse(char button,
                               char x, char y, char wheelPosition, char wheelHPosition);
    void update_keyboard(uint8_t shiftKeys, uint8_t *keyCodes);
public:
    csgesture_softc softc;
    
    //public csgesture functions
    bool ProcessMove(csgesture_softc *sc, int abovethreshold, int iToUse[4]);
    bool ProcessScroll(csgesture_softc *sc, int abovethreshold, int iToUse[4]);
    bool ProcessThreeFingerSwipe(csgesture_softc *sc, int abovethreshold, int iToUse[4]);
    bool ProcessFourFingerSwipe(csgesture_softc *sc, int abovethreshold, int iToUse[4]);
    
    void TapToClickOrDrag(csgesture_softc *sc, int button);
    void ClearTapDrag(csgesture_softc *sc, int i);
    void ProcessGesture(csgesture_softc *sc);
    void timedProcessGesture();
    
    //os specific functions
    void prepareToSleep();
    void wakeFromSleep();
    
    void initialize_wrapper(IOService *service);
    void destroy_wrapper(void);
    
    int vendorID;
    int productID;
    
    int reportDescriptorLength();
    void write_report_to_buffer(IOMemoryDescriptor *buffer);
    void write_report_descriptor_to_buffer(IOMemoryDescriptor *buffer);

    MultitouchReturn handleInterruptReport(VoodooI2CMultitouchEvent event, AbsoluteTime timestamp);
    bool start(IOService* service);
    void stop(IOService* provider);
};


#endif /* VoodooI2CCSGestureEngine_hpp */
