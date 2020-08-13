#include "peripheral.h"
#import "bluetooth.h"
#import "wrapper.h"

#include <iostream>
#include <vector>
#include <string>

/*-------------------------------------------------------
                 C++ Wrapper implementation here

---------------------------------------------------------*/
namespace bluetooth {
    namespace wrapper {
        struct WrapperImpl {
            CBluetooth *wrapped;
        };

        Wrapper::Wrapper() :
                impl(new WrapperImpl()) {
            impl->wrapped = [[CBluetooth alloc] init];
        }

        Wrapper::~Wrapper() {
            if (impl) {
                [impl->wrapped release];
            }
            delete impl;
        }

        void Wrapper::start_bluetooth() {
            NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
            [impl->wrapped performSelectorInBackground:@selector(startBluetooth) withObject:nil];
            [pool release];

            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
        }

        bool Wrapper::find_peripheral(std::vector<std::string> uuids) {
            NSMutableArray *arr = [[NSMutableArray alloc] init];
            for (int i = 0; i < uuids.size(); i++) {
                NSString *uuid = [NSString stringWithUTF8String:uuids[i].c_str()];
                [arr addObject:[CBUUID UUIDWithString:uuid]];
            }
            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];

            [impl->wrapped findAndConnectPeripheralByUUID:(arr) completion:^{
                dispatch_semaphore_signal(sem);
            }];

            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

            if ([impl->wrapped getPeripheral] == nil) {
                [arr release];
                return false;
            }
            [arr release];
            return true;
        }

        bool Wrapper::find_peripheral(std::string name) {
            NSString *n = [NSString stringWithUTF8String:name.c_str()];
            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];

            [impl->wrapped findAndConnectPeripheralByName:(n) completion:^{
                dispatch_semaphore_signal(sem);
            }];

            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

            if ([impl->wrapped getPeripheral] == nil) {
                return false;
            }
            return true;
        }

        bool Wrapper::find_service(std::string service_uuid) {
            NSString *service_string = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *service_cbuuid = [CBUUID UUIDWithString:service_string];

            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            std::cout << "1" << std::endl;
            [impl->wrapped findAndConnectServiceByUUID:service_cbuuid completion:^{
                dispatch_semaphore_signal(sem);
            }];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

            std::cout << "5" << std::endl;
            CBPeripheral *peripheral = [impl->wrapped getPeripheral];

            for (CBService *service in peripheral.services) {
                NSLog(@"service.uuid: %@", service);
                NSLog(@"cbuuid: %@", service);
                if (([service.UUID isEqual:service_cbuuid])) {
                    NSLog(@"**** SUCCESSFULLY CONNECTED TO SERVICE: %@", service);
                    return true;
                }
            }

            NSLog(@"Warning, service: %@ not found!", service_string);
            return false;
        }


        bool Wrapper::find_characteristic(std::string char_uuid, std::string service_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            std::cout << "we still food" << std::endl;
            [impl->wrapped findAndConnectCharacteristicByUUID:CBChar belongingToService:CBService completion:^{
                dispatch_semaphore_signal(sem);
            }];
            if ([impl->wrapped getCharFromService:CBChar belongingToService:CBService] == nil) {
                return false;
            }
            return true;
        }

        std::string Wrapper::get_peripheral_name() {
            auto *n = new std::string([[impl->wrapped getPeripheralName] UTF8String]);
            std::string temp = *n;
            delete n;
            return temp;
        }

        uint8_t *Wrapper::read(const std::string &service_uuid, const std::string &char_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            return [impl->wrapped read:CBChar belongingToService:CBService];
        }

        void Wrapper::set_handler(std::shared_ptr<handler::EventHandler> event_handler) {
            [impl->wrapped setHandler:event_handler];
        }


    }
}


/*-------------------------------------------------------
              Objective-C implementation here

---------------------------------------------------------*/

@implementation CBluetooth

- (void)startBluetooth {

    @autoreleasepool {
        dispatch_async(_centralQueue, ^{
            self.centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:_centralQueue options:nil];
            NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
            while (([runLoop runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]])) {
            }
            [[NSRunLoop currentRunLoop] run];
        });
    };
}

- (void)findAndConnectPeripheralByName:(NSString *)name completion:(semaphoreCompletionBlock)completionBlock {
    _nameSearch = true;
    _peripheralName = name;
    [_centralManager scanForPeripheralsWithServices:nil
                                            options:nil];

    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void)findAndConnectPeripheralByUUID:(NSArray<CBUUID *> *)uuids completion:(semaphoreCompletionBlock)completionBlock {
    _nameSearch = false;
    [_centralManager scanForPeripheralsWithServices:uuids
                                            options:nil];
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void)findAndConnectServiceByUUID:(CBUUID *)uuid completion:(semaphoreCompletionBlock)completionBlock {
    std::cout << "2" << std::endl;

    [_peripheral discoverServices:@[uuid]];
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);

    std::cout << "4" << std::endl;
    completionBlock();
}

- (void)findAndConnectCharacteristicByUUID:(CBUUID *)charUUID
                        belongingToService:(CBUUID *)serviceUUID
                                completion:(semaphoreCompletionBlock)completionBlock; {
    _currentCharSearchUUID = charUUID;
    for (CBService *service in _peripheral.services) {
        if (([service.UUID isEqual:serviceUUID])) {
            [_peripheral discoverCharacteristics:@[charUUID] forService:service];

            dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
            completionBlock();
        }
    }
}


- (void)rotateTable:(int)degrees {
    NSData *bytes = [NSData dataWithBytes:&degrees length:sizeof(degrees)];
    [_peripheral
            writeValue:bytes
     forCharacteristic:_rotateTableChar
                  type:CBCharacteristicWriteWithResponse];
}


- (NSString *)getPeripheralName {
    return _peripheralName;
}

- (CBPeripheral *)getPeripheral {
    return _peripheral;
}

- (dispatch_semaphore_t)getSemaphore {
    return _semaphore;
}

- (CBCharacteristic *)getCharFromService:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID {
    for (CBService *service in _peripheral.services) {
        if (([service.UUID isEqual:serviceUUID])) {
            for (CBCharacteristic *characteristic in service.characteristics) {
                if (([characteristic.UUID isEqual:charUUID])) {
                    return characteristic;
                }
            }
        }
    }
    return nil;
}

- (uint8_t *)read:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID; {
    CBCharacteristic *c = nil;
    for (CBService *service in _peripheral.services) {
        if (([service.UUID isEqual:serviceUUID])) {
            for (CBCharacteristic *characteristic in service.characteristics) {
                if (([characteristic.UUID isEqual:charUUID])) {
                    c = characteristic;
                }
            }
        }
    }
    dispatch_sync(dispatch_get_main_queue(), ^{
        [_peripheral readValueForCharacteristic:c];
    });

//    NSUInteger size = [charData length] / sizeof(unsigned char);
//    auto* byte_array = (unsigned char*) [charData bytes];

//    std::vector<std::byte> bytes(byte_array, byte_array + [charData length]);
    auto *bytes = (uint8_t *) c.value.bytes;
    return bytes;
}


- (void)setHandler:(std::shared_ptr<bluetooth::handler::EventHandler>)eventHandler {
    _eventHandler = eventHandler;
}


- (id)init {
    if (self = [super init]) {
        _centralQueue = dispatch_queue_create("centralManagerQueue", DISPATCH_QUEUE_SERIAL);
        _semaphore = dispatch_semaphore_create(0);
    }
    return self;
}


- (void)dealloc {
    std::cout << "destructing this bluetooth_object object";
    [super dealloc];
}


// called immediately after initializing central manager with initWithDelegate.
- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    NSString *state = @"";
    NSDictionary *options = @{CBCentralManagerScanOptionAllowDuplicatesKey: @YES};
    switch ([central state]) {
        case CBManagerStateUnsupported:
            state = @"This device does not support Bluetooth Low Energy.";
            break;
        case CBManagerStateUnauthorized:
            state = @"This app is not authorized to use Bluetooth Low Energy.";
            break;
        case CBManagerStatePoweredOff:
            state = @"Bluetooth on this device is currently powered off.";
            break;
        case CBManagerStateResetting:
            state = @"The BLE Manager is resetting; a state update is pending.";
            break;
        case CBManagerStatePoweredOn: {
            state = @"Bluetooth LE is turned on and ready for communication.";
            dispatch_semaphore_signal(_semaphore);
            break;
        }
        case CBManagerStateUnknown:
            state = @"The state of the BLE Manager is unknown.";
            break;
        default:
            state = @"The state of the BLE Manager is unknown.";
    }
}

// call this during scanning when it finds a peripheral_mac
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI {
    NSString *pName = advertisementData[@"kCBAdvDataLocalName"];
    NSLog(@"NEXT PERIPHERAL: %@ (%@)", pName,
          peripheral.identifier.UUIDString); // cannot predict UUIDSTRING, it is seeded
    NSLog(@"NAME: %@ ", peripheral.name);

    if (_nameSearch) {
        if (pName) {
            if ([pName isEqualToString:_peripheralName]) {
                self.peripheral = peripheral;
                self.peripheral.delegate = self;
                [_centralManager stopScan];
                NSLog(@"Scanning stopped");
                [self.centralManager connectPeripheral:self.peripheral options:nil];
            }
        }
    } else {
        _peripheral = peripheral;
        _peripheralName = peripheral.name;
        _peripheral.delegate = self;
        [_centralManager stopScan];
        NSLog(@"Scanning stopped");
        [_centralManager connectPeripheral:_peripheral options:nil];
    }
}

// called after peripheral_mac is connected
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    [_centralManager stopScan];
    NSLog(@"**** SUCCESSFULLY CONNECTED TO PERIPHERAL");

    dispatch_semaphore_signal(_semaphore);
}

// called if didDiscoverPeripheral fails to connect
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"**** CONNECTION FAILED!!!");
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"**** DISCONNECTED FROM SWAG SCANNER");
}

#pragma mark - CBPeripheralDelegate methods

// When the specified services are discovered, this is called.
// Can access the services throup peripheral.services
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
//    bool found = false;
    NSLog(@"discovered a new service");
    dispatch_semaphore_signal(_semaphore);

}

// peripheral_mac's response to discoverCharacteristics
// use this to turn on notifications
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {

    dispatch_semaphore_signal(_semaphore);

    // don't forget to reset
//    _currentCharSearchUUID = nil;
//
//    _eventHandler->char_found = found;

//    std::unique_lock<std::mutex> ul(_eventHandler->mut);
//    _eventHandler->set_proceed(true);
//    ul.unlock();
//    _eventHandler->cv.notify_one();
//    ul.lock();

//        uint8_t enableValue = 0;
//        NSData *enableBytes = [NSData dataWithBytes:&enableValue length:sizeof(uint8_t)];
//
//        //TODO: as I'm writing the find_characteristics, these write value stuff don't need to be here
//
//        // rotate table
//        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:ROTATE_TABLE_CHAR_UUID]]) {
//            NSLog(@"Enabled table rotation characteristic: %@", characteristic);
//            _rotateTableChar = characteristic;
//            [self.peripheral writeValue:enableBytes forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
//        }
//
//        // table position
//        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:TABLE_POSITION_CHAR_UUID]]) {
//            NSLog(@"Enabled table position characteristic with notifications: %@", characteristic);
////            _tablePosChar = characteristic;
//            [self.peripheral setNotifyValue:YES forCharacteristic:characteristic];
//        }
//
//        // table rotation
//        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:IS_TABLE_ROTATING_CHAR_UUID]]) {
//            NSLog(@"Enabled is table rotation? characteristic with notifications: %@", characteristic);
//            [self.peripheral setNotifyValue:YES forCharacteristic:characteristic];
//        }



}

// start receiving data from this method once we set up notifications. Also can be manually
// called with readValueForCharacteristic
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (error) {
        NSLog(@"Error changing notification state: %@", [error localizedDescription]);
    } else {

    }
}

- (void)setIsRotating:(NSData *)dataBytes {
    int theInteger;
    [dataBytes getBytes:&theInteger length:sizeof(theInteger)];
}


- (void)displayRotInfo:(NSData *)dataBytes {
    int theInteger;
    [dataBytes getBytes:&theInteger length:sizeof(theInteger)];
}

- (void)displayTablePosInfo:(NSData *)dataBytes {
    int theInteger;
    [dataBytes getBytes:&theInteger length:sizeof(theInteger)];
//    std::cout << "Table is at position: " + std::to_string(theInteger) << std::endl;
}

- (int)bytesToInt:(NSData *)dataBytes {
    int theInteger;
    [dataBytes getBytes:&theInteger length:sizeof(theInteger)];
    return theInteger;
}


@end