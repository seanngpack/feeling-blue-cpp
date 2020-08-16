**************
Characteristic
**************

A characteristic holds a value. This value is represented by a ``std::vector<std::byte>`` byte vector.
A characteristic can also be set with properties to read, write, and notify. feeling-blue provides
the tools to read, write, and be notified from characteristics.

.. doxygenclass:: bluetooth::Characteristic
   :project: feeling-blue
   :members: