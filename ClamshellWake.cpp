#include <IOKit/IOLib.h>
#include "ClamshellWake.h"
#include <IOKit/pwr_mgt/RootDomain.h>

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(com_bebekoubou_driver_ClamshellWake, IOService)

// Define the driver's superclass.
#define super IOService
bool com_bebekoubou_driver_ClamshellWake::start(IOService *provider)
{
    IOLog("MyDriver: Starting\n");
    com_bebekoubou_driver_ClamshellWake::send_event(kIOPMDisableClamshell); 
    return super::start(provider);
}

void com_bebekoubou_driver_ClamshellWake::stop(IOService *provider)
{
    IOLog("MyDriver: Stopping\n");
    com_bebekoubou_driver_ClamshellWake::send_event(kIOPMEnableClamshell | kIOPMAllowSleep);
    super::stop(provider);
}

/* kIOPMMessageClamshallStateChange Notification */
IOReturn com_bebekoubou_driver_ClamshellWake::message(UInt32 type, IOService * provider, void * argument)
{	
	if (type == kIOPMMessageClamshellStateChange) {
		IOLog("========================\n");
		IOLog("MyDriver: Clamshell State Changed\n");
		
		// If lid was opened
		if ((argument && kClamshellStateBit) & (!lastLidState)) {
			IOLog("MyDriver: kClamshellStateBit set - lid was opened\n");
			lastLidState = true;
			com_bebekoubou_driver_ClamshellWake::send_event(kIOPMClamshellOpened);
			
            // If lastLidState is true - lid closed
		} else if (lastLidState) {
			IOLog("MyDriver: kClamshellStateBit not set - lid was closed \n");
			lastLidState = false;
			com_bebekoubou_driver_ClamshellWake::send_event(kIOPMDisableClamshell); 
		}
	}
	
	return super::message(type, provider, argument);
}



bool com_bebekoubou_driver_ClamshellWake::send_event(UInt32 msg)
{
    IOPMrootDomain *root = NULL;
	IOReturn		ret=kIOReturnSuccess;
	
    // warning is no problem.(format '%X' expects type 'unsigned int', but argument 2 has type 'UInt32')
    IOLog("MyDriver: Sending event 0x%X\n", msg);
	
	root = getPMRootDomain();
    if (!root) {
        IOLog("MyDriver: Fatal error could not get RootDomain.\n");
        return false;
    }
	
	ret = root->receivePowerNotification(msg);
	
	IOLog("MyDriver: root returns %d\n", ret);
	
	if(ret!=kIOReturnSuccess)
		IOLog("MyDriver: Error sending event\n");
	else
		IOLog("MyDriver: Message sent to root\n");
	
	return true;
}