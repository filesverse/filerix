### Resolving "Error: libfilerix.so: cannot open shared object file: No such file or directory"  

If you encounter the following error when running an application that depends on `libfilerix.so`:  

```
Error: libfilerix.so: cannot open shared object file: No such file or directory
```

This typically indicates that the system is unable to locate the shared library. To resolve this issue, update the `LD_LIBRARY_PATH` environment variable to include the directory where `libfilerix.so` is installed:  

```bash
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
```

This ensures that the application can properly locate `libfilerix.so` in `/usr/local/lib64/`.  

To make this change persistent across sessions, add the export command to your shell configuration file (e.g., `.bashrc`, `.zshrc`, or `/etc/environment` for system-wide configuration).