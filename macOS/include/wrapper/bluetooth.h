#include "central_event_handler.h"
#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#include "peripheral_impl.h"

//class PeripheralImpl;

// An Objective-C class that needs to be accessed from C++
@interface CBluetooth : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

@property(nonatomic) handler::CentralEventHandler *centralEventHandler;
@property(strong, nonatomic) CBCentralManager *centralManager;
@property(strong, nonatomic) CBPeripheral *peripheral;
@property(nonatomic, strong) NSString *peripheralName;
@property(strong, nonatomic) CBCharacteristic *rotateTableChar;
@property(strong, nonatomic) CBCharacteristic *tablePosChar;
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


- (void)setHandler:(handler::CentralEventHandler *)arduinoEventHandler;

/**
 * Start the bluetooth_object discovery and initialization process. Will create a CBCentralManager and
 * actively scan for Swag Scanner's bluetooth_object service. Then it will subscribe to notifications.
 */
- (void)startBluetooth;

/**
 * Scan for peripheral that matches given name. Store the peripheral into the
 * class field and return a PeripheralImpl.
 * @param name name of peripheral.
 */
- (void)findPeripheralName:(NSString *)name;

/**
 * Scan for peripheral that advertises given uuids.
 * @param uuids array of UUIDs
 */
- (void)findPeripheralUUID:(NSArray<CBUUID *> *)uuids;

/**
 * Call this method after findPeripheral. This returns the found peripheral converted
 * to the C++ object PeripheralImpl.
 * This method exists because we need to wait for the peripheral to be connected which
 * occurs in the didConnectPeripheral method.
 * @return the peripheral.
 */
- (bluetooth::PeripheralMac *)getPeripheral;

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

