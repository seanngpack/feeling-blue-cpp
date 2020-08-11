#include "peripheral.h"
#include "event_handler.h"
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
        }

        void Wrapper::find_peripheral(std::vector<std::string> uuids) {
            NSMutableArray *arr = [[NSMutableArray alloc] init];
            for (int i = 0; i < uuids.size(); i++) {
                NSString *uuid = [NSString stringWithUTF8String:uuids[i].c_str()];
                [arr addObject:[CBUUID UUIDWithString:uuid]];
            }
            [impl->wrapped findAndConnectPeripheralByUUID:(arr)];
            [arr release];
        }

        void Wrapper::find_peripheral(std::string name) {
            NSString *n = [NSString stringWithUTF8String:name.c_str()];
            [impl->wrapped findAndConnectPeripheralByName:(n)];
        }

        void Wrapper::find_service(std::string uuid) {
            NSString *s = [NSString stringWithUTF8String:uuid.c_str()];
            CBUUID *c = [CBUUID UUIDWithString:s];
            [impl->wrapped findAndConnectServiceByUUID:(c)];
        }

        void Wrapper::find_characteristic(std::string char_uuid, std::string service_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBchar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            [impl->wrapped findAndConnectCharacteristicByUUID:CBchar belongingToService:CBService];
        }

        std::string Wrapper::get_peripheral_name() {
            auto *n = new std::string([[impl->wrapped getPeripheralName] UTF8String]);
            std::string temp = *n;
            delete n;
            return temp;
        }

        void Wrapper::set_handler(void *central_event_handler) {
            auto *a = static_cast<handler::EventHandler *>(central_event_handler);
            [impl->wrapped setHandler:a];
        }

    }
}


/*-------------------------------------------------------
              Objective-C implementation here

---------------------------------------------------------*/

@implementation CBluetooth

- (void)startBluetooth {
    _centralQueue = dispatch_queue_create("centralManagerQueue", DISPATCH_QUEUE_SERIAL);

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

- (void)findAndConnectPeripheralByName:(NSString *)name {
    _nameSearch = true;
    _peripheralName = name;
    [_centralManager scanForPeripheralsWithServices:nil
                                            options:nil];
}

- (void)findAndConnectPeripheralByUUID:(NSArray<CBUUID *> *)uuids {
    _nameSearch = false;
    [_centralManager scanForPeripheralsWithServices:uuids
                                            options:nil];
}

- (void)findAndConnectServiceByUUID:(CBUUID *)uuid {
    _currentServiceSearchUUID = uuid;
    [_peripheral discoverServices:@[uuid]];
}

- (void)findAndConnectCharacteristicByUUID:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID {
    _currentCharSearchUUID = charUUID;
    for (CBService *service in _peripheral.services) {
        if (([service.UUID isEqual:serviceUUID])) {
            [_peripheral discoverCharacteristics:@[charUUID] forService:service];
        }
    }
}


- (void)rotateTable:(int)degrees {
    _eventHandler->set_proceed(true);
    NSData *bytes = [NSData dataWithBytes:&degrees length:sizeof(degrees)];
    [_peripheral
            writeValue:bytes
     forCharacteristic:_rotateTableChar
                  type:CBCharacteristicWriteWithResponse];
}


- (NSString *)getPeripheralName {
    return _peripheralName;
}


- (void)setHandler:(bluetooth::handler::EventHandler *)eventHandler {
    _eventHandler = eventHandler;
}


- (id)init {
    self = [super init];
    if (self) {
        return self;
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
            std::unique_lock<std::mutex> ul(_eventHandler->mut);
            _eventHandler->set_proceed(true);
            ul.unlock();
            _eventHandler->cv.notify_one();
            ul.lock();
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
    std::unique_lock<std::mutex> ul(_eventHandler->mut);
    _eventHandler->set_proceed(true);
    ul.unlock();
    _eventHandler->cv.notify_one();
    ul.lock();
//    NSLog(@"Now looking for services...");
//    [peripheral discoverServices:nil];
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
    bool found = false;
    for (CBService *service in peripheral.services) {
        if (([service.UUID isEqual:_currentServiceSearchUUID])) {
            NSLog(@"**** SUCCESSFULLY CONNECTED TO SERVICE: %@", service);
            found = true;
        }
    }
    if (!found) {
        NSLog(@"Warning, service: %@ not found!", _currentServiceSearchUUID);
    }

    // don't forget to reset
    _currentServiceSearchUUID = nil;

    _eventHandler->service_found = found;

    std::unique_lock<std::mutex> ul(_eventHandler->mut);
    _eventHandler->set_proceed(true);
    ul.unlock();
    _eventHandler->cv.notify_one();
    ul.lock();
}

// peripheral_mac's response to discoverCharacteristics
// use this to turn on notifications
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {
    bool found = false;
    for (CBCharacteristic *characteristic in service.characteristics) {
//        NSLog(@"we have this charactersitic in service: %@", characteristic);
        if ([characteristic.UUID isEqual:_currentCharSearchUUID]) {
            NSLog(@"**** SUCCESSFULLY CONNECTED TO CHARACTERISTIC: %@", characteristic);
            found = true;
        }
    }
    if (!found) {
        NSLog(@"WARNING, char: %@ not found!", _currentCharSearchUUID);
    }

    // don't forget to reset
    _currentCharSearchUUID = nil;

    _eventHandler->char_found = found;

    std::unique_lock<std::mutex> ul(_eventHandler->mut);
    _eventHandler->set_proceed(true);
    ul.unlock();
    _eventHandler->cv.notify_one();
    ul.lock();

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
        // extract the data from the characteristic's value property and display the value based on the characteristic type
        NSData *dataBytes = characteristic.value;
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:TABLE_POSITION_CHAR_UUID]]) {
            [self displayTablePosInfo:dataBytes];
        } else if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:IS_TABLE_ROTATING_CHAR_UUID]]) {
            [self displayRotInfo:dataBytes];
            [self setIsRotating:dataBytes];
        }
    }
}

- (void)setIsRotating:(NSData *)dataBytes {
    int theInteger;
    [dataBytes getBytes:&theInteger length:sizeof(theInteger)];
//    std::unique_lock<std::mutex> ul(_eventHandler->peripheral_mutex);
//    _eventHandler->set_is_peripheral_found(theInteger == 1);
//    ul.unlock();
//    _eventHandler->peripheral_cv.notify_one();
//    ul.lock();
}


- (void)displayRotInfo:(NSData *)dataBytes {
    int theInteger;
    [dataBytes getBytes:&theInteger length:sizeof(theInteger)];
//    if (theInteger == 1) {
//        std::cout << "table is rotating" << std::endl;
//    }
//    else {
//        std::cout << "table is not rotating" << std::endl;
//    }
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