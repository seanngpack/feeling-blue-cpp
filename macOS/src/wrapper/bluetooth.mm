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

        bool Wrapper::find_peripheral(const std::vector<std::string> &uuids) {
            NSMutableArray *arr = [[NSMutableArray alloc] init];
            for (auto const &i : uuids) {
                NSString *uuid = [NSString stringWithUTF8String:i.c_str()];
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

        bool Wrapper::find_peripheral(const std::string &name) {
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

        bool Wrapper::find_service(const std::string &service_uuid) {
            NSString *service_string = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *service_cbuuid = [CBUUID UUIDWithString:service_string];

            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            [impl->wrapped findAndConnectServiceByUUID:service_cbuuid completion:^{
                dispatch_semaphore_signal(sem);
            }];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

            CBPeripheral *peripheral = [impl->wrapped getPeripheral];

            for (CBService *service in peripheral.services) {
                if (([service.UUID isEqual:service_cbuuid])) {
                    NSLog(@"**** SUCCESSFULLY CONNECTED TO SERVICE: %@", service);
                    return true;
                }
            }

            NSLog(@"Warning, service: %@ not found!", service_string);
            return false;
        }

        bool Wrapper::find_characteristic(const std::string &char_uuid, const std::string &service_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            [impl->wrapped findAndConnectCharacteristicByUUID:CBChar belongingToService:CBService completion:^{
                dispatch_semaphore_signal(sem);
            }];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
            if ([impl->wrapped getCharFromService:CBChar belongingToService:CBService] == nil) {
                NSLog(@"**** WARNING COULD NOT CONNECT TO CHARACTERISTIC: %@", char_s);
                return false;
            }
            NSLog(@"**** SUCCESSFULLY CONNECTED TO CHARACTERISTIC: %@", char_s);
            return true;
        }

        std::string Wrapper::get_peripheral_name() {
            auto *n = new std::string([[impl->wrapped getPeripheralName] UTF8String]);
            std::string temp = *n;
            delete n;
            return temp;
        }

        std::vector<std::byte> Wrapper::read(const std::string &service_uuid, const std::string &char_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];

            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            [impl->wrapped read:CBChar belongingToService:CBService completion:^{
                dispatch_semaphore_signal(sem);
            }];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

            CBCharacteristic *c = [impl->wrapped getCharFromService:CBChar belongingToService:CBService];
            return [impl->wrapped NSDataToVector:c.value];
        }

        void Wrapper::write_without_response(const std::vector<std::byte> &data,
                                             const std::string &service_uuid,
                                             const std::string &char_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            [impl->wrapped writeWithoutResponse:[NSData dataWithBytes:data.data() length:data.size() *
                                                                                         sizeof(std::byte)]
                              forCharacteristic:CBChar
                             belongingToService:CBService];
        }

        void Wrapper::write_with_response(const std::vector<std::byte> &data,
                                          const std::string &service_uuid,
                                          const std::string &char_uuid) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            [impl->wrapped writeWithResponse:[NSData dataWithBytes:data.data() length:data.size() * sizeof(std::byte)]
                           forCharacteristic:CBChar
                          belongingToService:CBService
                                  completion:^{
                                      dispatch_semaphore_signal(sem);
                                  }];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
        }

        void Wrapper::notify(const std::string &service_uuid,
                             const std::string &char_uuid,
                             const std::function<void(std::vector<std::byte>)> &callback) {
            NSString *char_s = [NSString stringWithUTF8String:char_uuid.c_str()];
            NSString *service_s = [NSString stringWithUTF8String:service_uuid.c_str()];
            CBUUID *CBChar = [CBUUID UUIDWithString:char_s];
            CBUUID *CBService = [CBUUID UUIDWithString:service_s];
            dispatch_semaphore_t sem = [impl->wrapped getSemaphore];
            [impl->wrapped setNotify:CBChar
                  belongingToService:CBService
                        callbackFunc:callback
                          completion:^{
                              dispatch_semaphore_signal(sem);
                          }];
            dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
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
    NSLog(@"SCANNING FOR: %@", name);
    [_centralManager scanForPeripheralsWithServices:nil
                                            options:nil];

    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void)findAndConnectPeripheralByUUID:(NSArray<CBUUID *> *)uuids completion:(semaphoreCompletionBlock)completionBlock {
    _nameSearch = false;
    NSLog(@"SCANNING FOR PERIPHERAL WITH UUIDS");
    [_centralManager scanForPeripheralsWithServices:uuids
                                            options:nil];
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void)findAndConnectServiceByUUID:(CBUUID *)uuid completion:(semaphoreCompletionBlock)completionBlock {
    [_peripheral discoverServices:@[uuid]];
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void)findAndConnectCharacteristicByUUID:(CBUUID *)charUUID
                        belongingToService:(CBUUID *)serviceUUID
                                completion:(semaphoreCompletionBlock)completionBlock; {
    for (CBService *service in _peripheral.services) {
        if (([service.UUID isEqual:serviceUUID])) {
            [_peripheral discoverCharacteristics:@[charUUID] forService:service];

            dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
            completionBlock();
        }
    }
}


- (void)writeWithoutResponse:(NSData *)data
           forCharacteristic:(CBUUID *)charUUID
          belongingToService:(CBUUID *)serviceUUID {
    [_peripheral
            writeValue:data
     forCharacteristic:[self getCharFromService:charUUID belongingToService:serviceUUID]
                  type:CBCharacteristicWriteWithoutResponse];
}


- (void)writeWithResponse:(NSData *)data
        forCharacteristic:(CBUUID *)charUUID
       belongingToService:(CBUUID *)serviceUUID
               completion:(semaphoreCompletionBlock)completionBlock {
    [_peripheral
            writeValue:data
     forCharacteristic:[self getCharFromService:charUUID belongingToService:serviceUUID]
                  type:CBCharacteristicWriteWithResponse];

    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void) setNotify:(CBUUID *)charUUID
belongingToService:(CBUUID *)serviceUUID
      callbackFunc:(std::function<void(std::vector<std::byte> data)>)callback
        completion:(semaphoreCompletionBlock)completionBlock {

    [self.peripheral setNotifyValue:YES
                  forCharacteristic:[self getCharFromService:charUUID belongingToService:serviceUUID]];
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);

    CBCharacteristic *c = [self getCharFromService:charUUID belongingToService:serviceUUID];
    DLog(@"set call handler to notification: %@ ", c);
    std::string char_string = std::string([charUUID.UUIDString UTF8String]);
    _callbackMap.insert_or_assign(char_string, callback);
    completionBlock();
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

- (void)read:(CBUUID *)charUUID belongingToService:(CBUUID *)serviceUUID completion:(semaphoreCompletionBlock)completionBlock {
    CBCharacteristic *c = [self getCharFromService:charUUID belongingToService:serviceUUID];
    _readCommand = true;

    [_peripheral readValueForCharacteristic:c];
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
    _readCommand = false;
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
    DLog(@"NEXT PERIPHERAL: %@ (%@)", pName,
         peripheral.identifier.UUIDString); // cannot predict UUIDSTRING, it is seeded
    DLog(@"NAME: %@ ", peripheral.name);

    if (_nameSearch) {
        if (pName) {
            if ([pName isEqualToString:_peripheralName]) {
                self.peripheral = peripheral;
                self.peripheral.delegate = self;
                [_centralManager stopScan];
                NSLog(@"SCANNING STOPPED");
                [self.centralManager connectPeripheral:self.peripheral options:nil];
            }
        }
    } else {
        _peripheral = peripheral;
        _peripheralName = peripheral.name;
        _peripheral.delegate = self;
        [_centralManager stopScan];
        NSLog(@"SCANNING STOPPED");
        [_centralManager connectPeripheral:_peripheral options:nil];
    }
}

// called after peripheral_mac is connected
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    [_centralManager stopScan];
    NSLog(@"**** SUCCESSFULLY CONNECTED TO PERIPHERAL: %@", peripheral);

    dispatch_semaphore_signal(_semaphore);
}

// called if didDiscoverPeripheral fails to connect
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"**** CONNECTION FAILED!!!");
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"**** DISCONNECTED");
}

// When the specified services are discovered, this is called.
// Can access the services throup peripheral.services
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
//    bool found = false;
    DLog(@"discovered a new service");
    dispatch_semaphore_signal(_semaphore);
}

// called when you call [setNotify for:]
- (void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (error) {
        NSLog(@"**** ERROR SETTING UP NOTIFICATIONS FOR CHARACTERISTIC: %@", characteristic);
    } else {
        NSLog(@"**** SUCCESSFULLY SET UP NOTIFICATIONS FOR CHARACTERISTIC: %@", characteristic);
    }
    dispatch_semaphore_signal(_semaphore);
}

// peripheral_mac's response to discoverCharacteristics
// use this to turn on notifications
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {
    dispatch_semaphore_signal(_semaphore);
}

- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (error) {
        NSLog(@"**** ERROR WRITING TO CHARACTERISTIC: %@", [error localizedDescription]);
    }
    dispatch_semaphore_signal(_semaphore);
}

// start receiving data from this method once we set up notifications. Also can be manually
// called with readValueForCharacteristic
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (error) {
        NSLog(@"**** ERROR CHANGING NOTIFICATION STATE: %@", [error localizedDescription]);
    }
    if (_readCommand) {
        DLog(@"read triggered by read()");
    } else {
        DLog(@"read triggered by notification");
        for (auto const&[key, val] : _callbackMap) {
            std::string char_string = std::string([characteristic.UUID.UUIDString UTF8String]);
            if (key == char_string) {
                std::vector<std::byte> data_received = [self NSDataToVector:characteristic.value];
                val(data_received);
            }
        }
    }

    dispatch_semaphore_signal(_semaphore); // this is dangerous. actually wait maybe not
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

- (uint8_t *)NSDataTouint8:(NSData *)data {
    auto size = static_cast<uint8_t>(data.length);
    auto *uArr = new uint8_t[size];
    memcpy(uArr, [data bytes], size);
    return uArr;
}

- (std::vector<std::byte>)NSDataToVector:(NSData *)data {
    std::vector<std::byte> byte_vector;
    auto length = (uint8_t) [data length];
    auto *uArr = new uint8_t[length];
    memcpy(uArr, [data bytes], length);

    for (int i = 0; i < length; i++) {
        uint8_t x = uArr[i];
        byte_vector.push_back((std::byte) x);
    }
    return byte_vector;
}


@end