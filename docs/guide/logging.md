# **Logging**

---

The logging system is automatically initialized by checking the `filerix_log_level` environment variable during the application's startup. This allows you to configure the logging level without needing to explicitly call any initialization functions.

## **Log Level Configuration**

You can control the log level by setting the `filerix_log_level` environment variable. The available log levels are:

- **debug**: Logs all messages (including debug, warning, and error).
- **warning**: Logs warning and error messages.
- **error**: Logs only error messages.

To set the log level, use the following command in your terminal before running your application:

```bash
export filerix_log_level=debug  # Or warning | error
```

Alternatively, you can set the environment variable in your application's code (for example, in your main function or before starting the application).

## **Log Files Location**

Logs are stored in the following directory:
```
~/.local/filerix/{error_log | warning_log | info_log}.txt
```
- **error_log.txt:** Stores error-level messages.
- **warning_log.txt:** Stores warning-level messages.
- **info_log.txt:** Stores debug and info-level messages.
