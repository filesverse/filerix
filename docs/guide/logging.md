# **Logging**

---

The logging system is initialized during the application's startup. The following examples show how to initialize the logging system in both C++ and JavaScript (Node.js).

## **Logging Initialization**

To initialize the logging system, use the `Logger::init()` or `libfm.initLogger()` function. This function sets up the logging mechanism for the application, enabling it to log messages at different levels.

## **Log Files Location**

Logs are stored in the following directory:
```
~/.local/libfm/{error_log | warning_log | info_log}.txt
```
- **error_log.txt:** Stores error-level messages.  
- **warning_log.txt:** Stores warning-level messages.  
- **info_log.txt:** Stores debug and info-level messages.  

## **Example**

:::code-group

```cpp [C++]
#include "include/utils/Logger.hpp"

int main() {
  Logger::init("debug"); // AVAILABLE OPTIONS: debug/info | warning | error
}
```

```javascript [Node.js]
import libfm from "/path/to/fm.node";

libfm.initLogger("debug");  // AVAILABLE OPTIONS: debug/info | warning | error
```

:::

---

The `Logger::init()` or `libfm.initLogger()` function should only be called once, during the startup of your application. It ensures the logger is ready to capture and display logs.