#pragma once

#pragma comment(lib, "windowsapp")

#include <winrt/Windows.Foundation.Collections.h>
#include <wrl\client.h>
#include <stdio.h>
#include <stdio.h>
#include <tchar.h>

#include <sstream>
#include <iomanip>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h>
#include <winrt/Windows.Storage.Streams.h>

using namespace winrt;
using namespace ABI::Windows::Foundation;
using namespace winrt::Windows::Devices::Bluetooth;
using namespace winrt::Windows::Devices::Enumeration;
using namespace winrt::Windows::Devices::Bluetooth::Advertisement;
using namespace winrt::Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace winrt::Windows::Storage::Streams;

// TODO: reference additional headers your program requires he