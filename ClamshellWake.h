#include <IOKit/IOService.h>

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>

class com_bebekoubou_driver_ClamshellWake : public IOService
{
    OSDeclareDefaultStructors(com_bebekoubou_driver_ClamshellWake)
public:
    //virtual bool init(OSDictionary *dictionary = 0);
    //virtual void free(void);
    //virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
    
	virtual IOReturn message(UInt32 type, IOService *provider, void *argument = 0);
    
private:
	static bool send_event(UInt32 msg);
	bool lastLidState;
};