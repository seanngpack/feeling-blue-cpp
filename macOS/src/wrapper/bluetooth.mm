#include "peripheral.h"
#import "bluetooth.h"
#import "wrapper.h"

#include <vector>
#include <string>

/*-------------------------------------------------------
                 C++ Wrapper implementation here

---------------------------------------------------------*/
namespace wrapper {
    struct WrapperImpl {
        CBluetooth *wrapped;
    };

    Wrapper::Wrapper() :
            impl(new WrapperImpl()) {
        impl->wrapped = [[CBluetooth alloc] init];
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
        [impl->wrapped findPeripheralUUID:(arr)];
        [arr release]; // WARNING: this might cause an issue
    }

    void Wrapper::find_peripheral(std::string name) {
        NSString *n = [NSString stringWithUTF8String:name.c_str()];
        [impl->wrapped findPeripheralName:(n)];
    }

    bluetooth::Peripheral *Wrapper::get_peripheral() {
        return [impl->wrapped getPeripheral];
    }

    void Wrapper::set_handler(void *central_event_handler) {
        auto *a = static_cast<handler::CentralEventHandler *>(central_event_handler);
        [impl->wrapped setHandler:a];
    }

    Wrapper::~Wrapper() {
        if (impl) {
            [impl->wrapped release];
        }
        delete impl;
    }
}


/*-------------------------------------------------------
              Objective-C implementation here

---------------------------------------------------------*/

@implementation CBluetooth

- (void)findPeripheralName:(NSString *)name {
    _nameSearch = true;
    [_centralManager scanForPeripheralsWithServices:nil
                                            options:nil];
}

- (void)findPeripheralUUID:(NSArray<CBUUID *> *)uuids {
    _nameSearch = false;
    [_centralManager scanForPeripheralsWithServices:uuids
                                            options:nil];
}

- (void)rotateTable:(int)degrees {
    _centralEventHandler->set_is_peripheral_found(true);
    NSData *bytes = [NSData dataWithBytes:&degrees length:sizeof(degrees)];
    [_peripheral
            writeValue:bytes
     forCharacteristic:_rotateTableChar
                  type:CBCharacteristicWriteWithResponse];
}

- (bluetooth::Peripheral *)getPeripheral {
    std::string name = std::string([_peripheral.name UTF8String]);
    auto *p = new bluetooth::Peripheral();
    p->set_name(name);
    return p;
}


- (void)setHandler:(handler::CentralEventHandler *)CentralEventHandler {
    _centralEventHandler = CentralEventHandler;
}


- (id)init {
    self = [super init];
    if (self) {
        return self;
    }
    return self;
}

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
            std::unique_lock<std::mutex> ul(_centralEventHandler->power_mutex);
            _centralEventHandler->set_is_powered_on(true);
            ul.unlock();
            _centralEventHandler->power_cv.notify_one();
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
    NSLog(@"NEXT PERIPHERAL: %@ (%@)", pName, peripheral.identifier.UUIDString);
    NSLog(@"NAME: %@ ", peripheral.name);

    // search by name
    if (_nameSearch) {
        if (pName) {
            if ([pName isEqualToString:_peripheralName]) {
                _peripheral = peripheral;
                _peripheral.delegate = self;
                [_centralManager connectPeripheral:_peripheral options:nil];
            }
        }
    } else {
        _peripheral = peripheral;
        _peripheral.delegate = self;
        [_centralManager connectPeripheral:_peripheral options:nil];
    }
}

// called after peripheral_mac is connected
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    [_centralManager stopScan];
    NSLog(@"Scanning stopped");
    NSLog(@"**** SUCCESSFULLY CONNECTED TO PERIPHERAL");
    NSLog(@"PERIPHERAL NAME: %@", peripheral.name);
    std::unique_lock<std::mutex> ul(_centralEventHandler->peripheral_mutex);
    _centralEventHandler->set_is_peripheral_found(true);
    ul.unlock();
    _centralEventHandler->peripheral_cv.notify_one();
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
    // Core Bluetooth creates an array of CBService objects —- one for each service that is discovered on the peripheral_mac.
    for (CBService *service in peripheral.services) {
        NSLog(@"Discovered service: %@", service);
        if (([service.UUID isEqual:[CBUUID UUIDWithString:UART_SERVICE_UUID]])) {
            [peripheral discoverCharacteristics:nil forService:service];
        }
    }
}

// peripheral_mac's response to discoverCharacteristics
// use this to turn on notifications
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {
    for (CBCharacteristic *characteristic in service.characteristics) {
        uint8_t enableValue = 0;
        NSData *enableBytes = [NSData dataWithBytes:&enableValue length:sizeof(uint8_t)];

        // rotate table
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:ROTATE_TABLE_CHAR_UUID]]) {
            NSLog(@"Enabled table rotation characteristic: %@", characteristic);
            _rotateTableChar = characteristic;
            [self.peripheral writeValue:enableBytes forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
        }

        // table position
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:TABLE_POSITION_CHAR_UUID]]) {
            NSLog(@"Enabled table position characteristic with notifications: %@", characteristic);
            _tablePosChar = characteristic;
            [self.peripheral setNotifyValue:YES forCharacteristic:characteristic];
        }

        // table rotation
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:IS_TABLE_ROTATING_CHAR_UUID]]) {
            NSLog(@"Enabled is table rotation? characteristic with notifications: %@", characteristic);
            [self.peripheral setNotifyValue:YES forCharacteristic:characteristic];
        }
    }


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
//    std::unique_lock<std::mutex> ul(_centralEventHandler->peripheral_mutex);
//    _centralEventHandler->set_is_peripheral_found(theInteger == 1);
//    ul.unlock();
//    _centralEventHandler->peripheral_cv.notify_one();
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