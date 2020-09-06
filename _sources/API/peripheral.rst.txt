**********
Peripheral
**********

Peripheral represents the device you want to connect to. Peripherals can be found through
either searching for a local name or searching for a vector of uuids it contains. A peripheral
can have many services.

Currently, if your peripheral has a local name and you search for a peripheral by its uuids,
then it will not automatically find its name. To fix this, call the ``set_name()`` method after
successfully connecting to your peripheral. If you search by name, you're good.



.. doxygenclass:: bluetooth::Peripheral
   :project: feeling-blue
   :members: