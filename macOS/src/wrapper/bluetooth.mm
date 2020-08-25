#import "bluetooth.h"
#import "wrapper.h"
#include <vector>
#include <string>
#include "spdlog/spdlog.h"

/*-------------------------------------------------------
                 C++ Wrapper implementation here

---------------------------------------------------------*/
namespace bluetooth {
    namespace detail {
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
                        SPDLOG_INFO("**** SUCCESSFULLY CONNECTED TO SERVICE: {}", service_uuid);
                        return true;
                    }
                }

                SPDLOG_WARN("Service: {} not found!", service_uuid);
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

                    SPDLOG_WARN("**** COULD NOT CONNECT TO CHARACTERISTIC: {}", char_uuid);
                    return false;
                }
                SPDLOG_INFO("**** SUCCESSFULLY CONNECTED TO CHARACTERISTIC: {}", char_uuid);
                return true;
            }

            std::string Wrapper::get_peripheral_name() {
                return std::string([[impl->wrapped getPeripheralName] UTF8String]);
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
                [impl->wrapped writeWithResponse:[NSData dataWithBytes:data.data() length:data.size() *
                                                                                          sizeof(std::byte)]
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

            void Wrapper::disconnect() {
                [impl->wrapped disconnect];
            }
        }
    }
}

/*-------------------------------------------------------
              Trampoline methods

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
    SPDLOG_INFO("SCANNING FOR: {}", std::string([name UTF8String]));
    NSDictionary *dictionary = @{CBCentralManagerScanOptionAllowDuplicatesKey: @1};
    [_centralManager scanForPeripheralsWithServices:nil
                                            options:dictionary];

    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    completionBlock();
}

- (void)findAndConnectPeripheralByUUID:(NSArray<CBUUID *> *)uuids completion:(semaphoreCompletionBlock)completionBlock {
    _nameSearch = false;
    SPDLOG_INFO("SCANNING FOR PERIPHERAL WITH UUIDS");
    NSDictionary *dictionary = @{CBCentralManagerScanOptionAllowDuplicatesKey: @1};
    [_centralManager scanForPeripheralsWithServices:uuids
                                            options:dictionary];
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

    std::string char_string = std::string([charUUID.UUIDString UTF8String]);
    _callbackMap.insert_or_assign(char_string, callback);
    completionBlock();
}

- (void)disconnect {
    [_centralManager cancelPeripheralConnection:_peripheral];
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

/*-------------------------------------------------------
              CoreBluetooth stuff below

---------------------------------------------------------*/

- (id)init {
    if (self = [super init]) {
        _centralQueue = dispatch_queue_create("centralManagerQueue", DISPATCH_QUEUE_SERIAL);
        _semaphore = dispatch_semaphore_create(0);
    }
    return self;
}


- (void)dealloc {
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

/*-------------------------------------------------------
              Central Methods

---------------------------------------------------------*/

// call this during scanning when it finds a peripheral_mac
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI {
    NSString *pName = advertisementData[@"kCBAdvDataLocalName"];
    SPDLOG_DEBUG("sd%@", advertisementData);
    SPDLOG_DEBUG("NEXT PERIPHERAL: {}", pName,
         peripheral.identifier.UUIDString); // cannot predict UUIDSTRING, it is seeded
    SPDLOG_DEBUG("NAME: {} ", peripheral.name);

    if (_nameSearch) {
        if (pName) {
            if ([pName isEqualToString:_peripheralName]) {
                self.peripheral = peripheral;
                self.peripheral.delegate = self;
                [_centralManager stopScan];
                SPDLOG_INFO("FOUND DEVICE, SCANNING STOPPED");
                [self.centralManager connectPeripheral:self.peripheral options:nil];
            }
        }
    } else {
        self.peripheral = peripheral;
        self.peripheral.delegate = self;
        self.peripheralName = @"None"; // seems like a corebluetooth bug where if you scan for peripherals
        // using UUIDs, it removes the localName advertisement key.
        [_centralManager stopScan];
        SPDLOG_INFO("FOUND DEVICE, SCANNING STOPPED");
        [self.centralManager connectPeripheral:self.peripheral options:nil];
    }
}

// called after peripheral_mac is connected
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    std::string peripheral_tostring = std::string([peripheral.description UTF8String]);
    SPDLOG_INFO("**** SUCCESSFULLY CONNECTED TO PERIPHERAL: {}", peripheral_tostring);

    dispatch_semaphore_signal(_semaphore);
}

- (void)peripheralDidUpdateName:(CBPeripheral *)peripheral {
    std::string peripheral_tostring = std::string([peripheral.description UTF8String]);
    SPDLOG_INFO("**** PERIPHERAL UPDATED NAME: {}", peripheral_tostring);
}


// called if didDiscoverPeripheral fails to connect
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    SPDLOG_ERROR("**** CONNECTION FAILED!!!");
}

- (void) centralManager:(CBCentralManager *)central
didDisconnectPeripheral:(CBPeripheral *)peripheral
                  error:(NSError *)error {
    SPDLOG_INFO("**** DISCONNECTED PERIPHERAL");
}

/*-------------------------------------------------------
              Peripheral methods

---------------------------------------------------------*/

// When the specified services are discovered, this is called.
// Can access the services throup peripheral.services
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
//    bool found = false;
    SPDLOG_DEBUG("discovered a new service");
    dispatch_semaphore_signal(_semaphore);
}

// called when you call [setNotify for:]
- (void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    std::string char_tostring = std::string([characteristic.description UTF8String]);
    if (error) {
        SPDLOG_ERROR("**** ERROR SETTING UP NOTIFICATIONS FOR CHARACTERISTIC: {}", char_tostring);
    } else {
        SPDLOG_INFO("**** SUCCESSFULLY SET UP NOTIFICATIONS FOR CHARACTERISTIC: {}", char_tostring);
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
        std::string error_string = std::string([[error localizedDescription] UTF8String]);
        SPDLOG_INFO("**** ERROR WRITING TO CHARACTERISTIC: {}", error_string);
    }
    dispatch_semaphore_signal(_semaphore);
}

// start receiving data from this method once we set up notifications. Also can be manually
// called with readValueForCharacteristic
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (error) {
        std::string error_string = std::string([[error localizedDescription] UTF8String]);
        SPDLOG_ERROR("**** ERROR CHANGING NOTIFICATION STATE: {}", error_string);
    }
    if (_readCommand) {
        SPDLOG_DEBUG("read triggered by read()");
    } else {
        SPDLOG_DEBUG("read triggered by notification");
        for (auto const&[key, val] : _callbackMap) {
            std::string char_string = std::string([characteristic.UUID.UUIDString UTF8String]);
            if (key == char_string) {
                std::vector<std::byte> data_received = [self NSDataToVector:characteristic.value];
                std::string char_tostring = std::string([characteristic.description UTF8String]);
                SPDLOG_INFO("NOTIFICATION RECEIVED FROM CHARACTERISTIC: {}", char_tostring);
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