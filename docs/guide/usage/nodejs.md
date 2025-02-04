# **Node.js Usage**

## **Prerequisites**

Before proceeding, ensure the following are installed and set up:

1. **Node.js**: Install the latest LTS version from [Node.js official site](https://nodejs.org/).
2. **Python**: Required by `node-gyp`. Install Python 3.x.
3. **Build Tools**:
   - **Linux**: Install `build-essential` (e.g., `sudo apt install build-essential` on Ubuntu).
4. **filerix**: Ensure the `filerix` library and its development files are installed. If you built `filerix` manually, ensure the `.so` files are in your library path.

## **Building the Node.js Wrapper with `node-gyp`**

1. Navigate to the `example/nodejs` directory:
   ```bash
   cd example/nodejs
   ```

2. Run the following command to configure and build the wrapper:
   ```bash
   node-gyp configure
   node-gyp build
   ```

3. After building, the compiled Node.js addon (`filerix.node`) will be available in the `build/Release` directory.

## **Installation**

1. Ensure you are in the `example/nodejs` directory:
   ```bash
   cd example/nodejs
   ```

2. Install the required dependencies:
   ```bash
   npm install
   ```

## **Running the Example**

1. The example is set up in `example/nodejs/src/index.js`. 
   - The `getDriveUsage` function fetches and logs the disk usage information for a given path.

2. To run the example:
   ```bash
   node src/index.js
   ```

## **Common issues**

- If you encounter issues with library loading (e.g., `filerix.so` not found), ensure the library path is correctly set:
  ```bash
  export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
  ```