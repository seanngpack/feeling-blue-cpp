#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#include <memory>

namespace bluetooth {
    class Peripheral;

    namespace handler {
        class EventHandler;
    }
}

typedef void (^findServiceCompletionBlock) (void);

/**
 * Represents a class that manages corebluetooth functionality.
 */
@interface CBluetooth : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

@property(nonatomic) std::shared_ptr<bluetooth::handler::EventHandler> eventHandler;
@property(strong, nonatomic) CBCentralManager *centralManager;
@property(strong, nonatomic) CBPeripheral *peripheral;
@property(nonatomic, strong) NSString *peripheralName;
@property(strong, nonatomic) CBCharacteristic *rotateTableChar;
@property(strong, nonatomic) NSMutableData *data;
@property(nonatomic, strong) dispatch_queue_t centralQueue;
@property(nonatomic, strong) dispatch_semaphore_t semaphore;
@property(nonatomic, assign) BOOL nameSearch;
@property(nonatomic, strong) CBUUID *currentCharSearchUUID;




#define SWAG_SCANNER_NAME @"SwagScanner"
#define UART_SERVICE_UUID @"5ffba521-2363-41da-92f5-46adc56b2d37"
#define ROTATE_TABLE_CHAR_UUID @"5ffba522-2363-41da-92f5-46adc56b2d37"
#define TABLE_POSITION_CHAR_UUID @"5ffba523-2363-41da-92f5-46adc56b2d37"
#define IS_TABLE_ROTATING_CHAR_UUID @"5ffba524-2363-41da-92f5-46adc56b2d37"


// constructor override
- (id)init;

// destructor override
- (void)dealloc;


- (void)setHandler:(std::shared_ptr<bluetooth::handler::EventHandler>)eventHandler;

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
 * Use the peripheral to scan for a service given its uuid.
 * Uses a semaphore to wait until didDiscoverServices() is called.
 * @param uuid uuid of the service.
 * @param completionBlock block contains signal to semaphore to unblock calling thread.
 */
- (void)findAndConnectServiceByUUID:(CBUUID *)uuid completion:(findServiceCompletionBlock)completionBlock;

/**
 * Find and connect to characteristics given its uuid and the service it belongs to.
 * @param uuid uuid of the characteristic.
 * @param service service that the characteristic belongs to.
 */
- (void)findAndConnectCharacteristicByUUID:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID;

/**
 * Get the peripheral name. Call this after findAndConnectPeripheral.
 * @return the name of the peripheral.
 */
- (NSString *)getPeripheralName;

/**
 * Get the peripheral.
 * @return peripheral.
 */
- (CBPeripheral *)getPeripheral;

- (dispatch_semaphore_t)getSemaphore;

/**
 * Read value from characteristic.
 * @param charUUID the uuid of the characteristic you want to read from.
 * @param serviceUUID the uuid the characteristic belongs to.
 * @return byte array.
 */
- (uint8_t *)read:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID;

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

