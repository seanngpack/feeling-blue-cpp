#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

namespace bluetooth {
    class Peripheral;

    namespace handler {
        class EventHandler;
    }
}

// An Objective-C class that needs to be accessed from C++
@interface CBluetooth : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

@property(nonatomic) bluetooth::handler::EventHandler *eventHandler;
@property(strong, nonatomic) CBCentralManager *centralManager;
@property(strong, nonatomic) CBPeripheral *peripheral;
@property(nonatomic, strong) NSString *peripheralName;
@property(strong, nonatomic) CBCharacteristic *rotateTableChar;
@property(strong, nonatomic) NSMutableData *data;
@property(nonatomic, strong) dispatch_queue_t centralQueue;
@property(nonatomic, assign) BOOL nameSearch;


#define SWAG_SCANNER_NAME @"SwagScanner"
#define UART_SERVICE_UUID @"5ffba521-2363-41da-92f5-46adc56b2d37"
#define ROTATE_TABLE_CHAR_UUID @"5ffba522-2363-41da-92f5-46adc56b2d37"
#define TABLE_POSITION_CHAR_UUID @"5ffba523-2363-41da-92f5-46adc56b2d37"
#define IS_TABLE_ROTATING_CHAR_UUID @"5ffba524-2363-41da-92f5-46adc56b2d37"


// constructor override
- (id)init;

// destructor override
- (void)dealloc;


- (void)setHandler:(bluetooth::handler::EventHandler *)eventHandler;

/**
 * Create a new thread. On this thread, allocates a new instance of CBCentralManager to run on that queue.
 * Also creates a runloop.
 */
- (void)startBluetooth;

/**
 * Scan for peripheral that matches given name, store it, and connect to it.
 * @param name name of peripheral.
 */
- (void)findAndConnectPeripheralByName:(NSString *)name;

/**
 * Scan for peripheral that advertises given uuids, store it, then connect to it.
 * @param uuids array of UUIDs
 */
- (void)findAndConnectPeripheralByUUID:(NSArray<CBUUID *> *)uuids;

/**
 * Get the peripheral name. Call this after findAndConnectPeripheral.
 * @return the name of the peripheral.
 */
- (NSString *)getPeripheralName;

/**
 * Rotate the table with the given angle in degrees.
 * @param deg the angle you want to rotate the table in degrees.
 */
- (void)rotateTable:(int)degrees;

/**
 * Helper method to call setIsRotating on the Arduino.
 * @param dataBytes the bytes received from isRotating characteristic.
 */
- (void)setIsRotating:(NSData *)dataBytes;


/**
 * Display whether the table is rotating or not.
 * @param dataBytes bytes from notification.
 */
- (void)displayRotInfo:(NSData *)dataBytes;


/**
 * Convert bytes to int.
 * @param dataBytes data from characteristic in bytes.
 * @return int.
 */
- (int)bytesToInt:(NSData *)dataBytes;

@end

