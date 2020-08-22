************************
How to use and examples!
************************

.. contents:: Table of Contents
    :local:

Read this before starting
=========================

When you create a new ``Central`` object, feeling-blue creates it on another thread.
So **you must keep your project's main thread alive** with some mechanism or else
your program will exit immediately before you get to make bluetooth calls. If you want a quick
and dirty way to demo the library, then just use a simple ``while`` loop to keep your main thread
busy.

feeling-blue uses NSLog to display messages to your console. These messages may display out of order
relative to your cout statements. But the code still runs in the order that you write it.

Connecting to your device
=========================

Connecting to your device only takes a couple steps!

Central initialization
----------------------

To connect to your device, you must first initialize a ``Central``. The central represents your
computer and it manages ``Peripheral``. Then you start up bluetooth.

.. code:: c++

    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();

Search for peripheral
----------------------
The peripheral represents the device you want to connect to. There is a limit to one
peripheral per central. You can spawn more instances of central to connect to more devices.

To connect to a peripheral, you call the ``find_peripheral()`` method. If you know your device is advertising its localName,
you can pass its name to find it:

.. code:: c++

    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral("SwagScanner");

You can also connect to your peripheral if you know the 128-bit UUIDs of the **services** that it advertises.

.. code:: c++

    std::vector<std:string> uuids = {service_uuid1, service_uuid2, etc}
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral(uuids);

Search for services
-------------------

Peripherals can have many services. A service holds a collection of ``Characteristics``, so a a serivce is
a logical grouping of characteristics. A service may contain a service, but that is currently not officially supported,
however...it may work.

To connect to a service, you call the ``find_service`` method and pass in its UUID as a std::string:

.. code:: c++

    std::shared_ptr<bluetooth::Service> service = peripheral->find_service(service_uuid);

Search for characteristics
--------------------------

A Characteristic holds a value. feeling-blue stores this as a byte array. It is up to you
to know how to convert this data into something human-readable, a string representation for instance.
Future updates will include built-in functionality with the option to make it human-readable, but for now just byte the bullet.

Characteristic values can be read or written. A characteristic may also have **notifications** enabled
meaning when the value is updated, it will notify your peripheral and send data to it.

To connect to a characteristic, call the ``find_characteristic`` method on the service that the characteristic belongs to
and pass in the characteristic's UUID as a std::string:

.. code:: c++

    std::shared_ptr<bluetooth::Characteristic> rotate_char = service->find_characteristic(characteristic_uuid);

Code
----

.. code:: c++

    main.cpp

    int main() {

        std::string service_uuid = "5ffba521-2363-41da-92f5-46adc56b2d37";
        std::string char1_uuid = "5ffba522-2363-41da-92f5-46adc56b2d37";
        std::string char1_uuid = "5ffba523-2363-41da-92f5-46adc56b2d37";

        std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
        central->start_bluetooth();
        std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral("SwagScanner");
        std::shared_ptr<bluetooth::Service> service = peripheral->find_service(service_uuid);
        std::shared_ptr<bluetooth::Characteristic> characteristic1 = service->find_characteristic(char1_uuid);
        std::shared_ptr<bluetooth::Characteristic> characteristic2 = service->find_characteristic(char2_uuid);

        while (true) {
            ...blah blah
        }

        return 0;
    }


Connecting to multiple devices
==============================

Okay, so you have multiple devices you want to connect to--no problem! As mentioned, a central can only manage one
peripheral at a time. So to connect more, just instantiate more centrals and run through the connection and discovery
steps again.

Below is some code creating two centrals and connecting them to their respective devices(I'll leave connecting to services
and characteristics to you):

.. code:: c++

    main.cpp

    int main() {

        std::unique_ptr<bluetooth::Central> smart_watch_central = std::make_unique<bluetooth::Central>();
        smart_watch_central->start_bluetooth();
        std::shared_ptr<bluetooth::Peripheral> smart_watch = smart_watch_central->find_peripheral("SmartWatch");
        ...find services and characteristics

        std::unique_ptr<bluetooth::Central> smart_clock_central = std::make_unique<bluetooth::Central>();
        smart_clock_central->start_bluetooth();
        std::shared_ptr<bluetooth::Peripheral> smart_clock = smart_clock_central->find_peripheral("SmartClock");
        ...find services and characteristics

        while (true) {
            ...blah blah
        }

        return 0;
    }


Reading characteristic value
============================

Let's get some data! To read the value of your characteristic, call the ``read()`` method. This method blocks
the calling thread until the data has been read from your characteristic and assigned to your variable.

.. code:: c++

    std::vector<std::byte> data = characteristic->read();

There are additional read methods available to convert the payload into human-readable datatypes. If your device is sending
four bytes representing an integer, you can it directly as an integer instead of doing the conversion yourself:

.. code:: c++

    int data = characteristic->read_int();




Writing to characteristic
=========================

There are two main options to write to your device. First we can ``write_without_response()`` which writes to your
devices asynchronously and does not block your calling thread. If your write fails, you will not get a message
telling you that it failed. You must provide this method the data as a ``std::vector<std::byte>``

.. code:: c++

    characteristic->write_without_response(data);


And if you write with a response, then the method will block your calling thread and wait until your data has been
successfully written to the device.

.. code:: c++

    rotate_char->write_with_response(data);

There are convenience overloaded methods to write to your device if you'd rather send a human-readable datatype rather
than a vector of bytes. If you use these convenience methods, just make on the other end, your device is configured to handle
this information. These convenience methods assume little-endian ordering of bytes.

.. code:: c++

    std::string data = "Celcius";
    switch_units_char->write_with_response(data);


Notifying characteristic
========================


If your device and characteristic supports notifications, then let's use it. First, just double check that your characteristic
has notification support and that it's enabled. So now when your device sends your computer notifications with a data payload you can capture
that payload and write your own function to do something with it!

Non-member function event handler
---------------------------------

Let's write a callback event handler that takes in a ``std::vector<std::byte>`` and enable notifications, passing the function as a parameter.

IMPORTANT! All event handlers must follow this signature: ``void (std::vector<std::byte>)``

.. code:: c++

    void print_data(std::vector<std::byte> data) {
        for (auto const &b : data) {
            std::cout << (int)b << std::endl;
        }
    }

    characteristic->set_notify(print_data);


Member function event handler
-----------------------------

member functions are a little trickier to write, but you just have to bind their class to std::function
and add a placeholder parameter, then pass it like normal.

.. code:: c++

    class A {
    public:
        void print_data(std::vector<std::byte> data) {
            for (auto const &b : data) {
                std::cout << (int)b << std::endl;
            }
        }

        void set_notify(std::shared_ptr<bluetooth::Characteristic> c) {
            using namespace std::placeholders;
            std::function<void(std::vector<std::byte>)> binded_print_data = std::bind(&A::print_data, this, std::placeholders::_1);
            characteristic->notify(binded_print_data);
        }

    private:
        std::shared_ptr<bluetooth::Characteristic> characteristic;
    };

Passing member functions is really powerful because you can do things such as update an instance variable when notified.

If you're passing the same function to multiple characteristic notifications, then just make sure your function contents are
thread-safe, this applies to both member and non-member functions.