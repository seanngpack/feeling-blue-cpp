#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#include <memory>
#include <functional>
#include <map>

namespace bluetooth {
    class Peripheral;

}

typedef void (^semaphoreCompletionBlock)(void);

/**
 * Represents a class that manages corebluetooth functionality.
 */
@interface CBluetooth : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

@property(strong, nonatomic) CBCentralManager *centralManager;
@property(strong, nonatomic) CBPeripheral *peripheral;
@property(nonatomic, strong) NSString *peripheralName;
@property(strong, nonatomic) CBCharacteristic *rotateTableChar;
@property(nonatomic, strong) dispatch_queue_t centralQueue;
@property(nonatomic, strong) dispatch_semaphore_t semaphore;
@property(nonatomic, assign) BOOL nameSearch;
@property(nonatomic, assign) BOOL readCommand;
@property std::map<std::string, std::function<void(uint8_t *)>> callbackMap;


#define SWAG_SCANNER_NAME @"SwagScanner"
#define UART_SERVICE_UUID @"5ffba521-2363-41da-92f5-46adc56b2d37"
#define ROTATE_TABLE_CHAR_UUID @"5ffba522-2363-41da-92f5-46adc56b2d37"
#define TABLE_POSITION_CHAR_UUID @"5ffba523-2363-41da-92f5-46adc56b2d37"
#define IS_TABLE_ROTATING_CHAR_UUID @"5ffba524-2363-41da-92f5-46adc56b2d37"


// constructor override
- (id)init;

// destructor override
- (void)dealloc;

/**
 * Create a new thread. On this thread, allocates a new instance of CBCentralManager to run on that queue.
 * Also creates a runloop.
 */
- (void)startBluetooth;

/**
 * Scan for peripheral that matches given name, store it, and connect to it.
 * @param name name of peripheral.
 */
- (void)findAndConnectPeripheralByName:(NSString *)name completion:(semaphoreCompletionBlock)completionBlock;

/**
 * Scan for peripheral that advertises given uuids, store it, then connect to it.
 * @param uuids array of UUIDs
 */
- (void)findAndConnectPeripheralByUUID:(NSArray<CBUUID *> *)uuids completion:(semaphoreCompletionBlock)completionBlock;

/**
 * Use the peripheral to scan for a service given its uuid.
 * Uses a semaphore to wait until didDiscoverServices() is called.
 * @param uuid uuid of the service.
 * @param completionBlock block contains signal to semaphore to unblock calling thread.
 */
- (void)findAndConnectServiceByUUID:(CBUUID *)uuid completion:(semaphoreCompletionBlock)completionBlock;

/**
 * Find and connect to characteristics given its uuid and the service it belongs to.
 * @param uuid uuid of the characteristic.
 * @param service service that the characteristic belongs to.
 */
- (void)findAndConnectCharacteristicByUUID:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID completion:(semaphoreCompletionBlock)completionBlock;

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

/**
 * Get the dispatch semaphore.
 * @return the semaphore.
 */
- (dispatch_semaphore_t)getSemaphore;

/**
 * Read value from characteristic. Blocking method.
 * @param charUUID the uuid of the characteristic you want to read from.
 * @param serviceUUID the uuid the characteristic belongs to.
 */
- (void)read:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID completion:(semaphoreCompletionBlock)completionBlock;

/**
 * Write value to the characteristic.
 * @param data the data you want to write.
 * @param length the number of bytes in the data.
 * @param charUUID characteristic to write to.
 * @param serviceUUID service the characteristic belongs to.
 */
- (void)writeWithoutResponse:(NSData *)data
           forCharacteristic:(CBUUID *)charUUID
          belongingToService:(CBUUID *)serviceUUID;

/**
 * Write value to the characteristic. Waits for the didWriteValueFor callback before returning.
 * @param data the data you want to write.
 * @param length the number of bytes in the data.
 * @param charUUID characteristic to write to.
 * @param serviceUUID service the characteristic belongs to.
 */
- (void)writeWithResponse:(NSData *)data
        forCharacteristic:(CBUUID *)charUUID
       belongingToService:(CBUUID *)serviceUUID
               completion:(semaphoreCompletionBlock)completionBlock;


- (void) setNotify:(CBUUID *)charUUID
belongingToService:(CBUUID *)serviceUUID
      callbackFunc:(std::function<void(uint8_t *)>)callback
        completion:(semaphoreCompletionBlock)completionBlock;

/**
 * After discovering and connecting characteristics, you may want to fetch one.
 * Use this helper method to extract a characteristics from the given service.
 * @param service uuid of the service containing the characteristic.
 * @return the characteristic. Or nil if not found.
 */
- (CBCharacteristic *)getCharFromService:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID;

/**
 * Convert NSData * to uint8_t *.
 * @param data the data to convert.
 * @return uint8_t array.
 */
- (uint8_t *)NSDataTouint8:(NSData *)data;

@end

