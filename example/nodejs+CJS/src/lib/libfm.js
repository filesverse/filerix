const libfm = require("../../build/Release/fm.node");

module.exports = libfm;

module.exports.getFiles = libfm.getFiles;
module.exports.searchFiles = libfm.searchFiles;
module.exports.copyFile = libfm.copyFile;
module.exports.cutFile = libfm.cutFile;
module.exports.renameFile = libfm.renameFile;

module.exports.getDrives = libfm.getDrives;
module.exports.getDriveUsage = libfm.getDriveUsage;
module.exports.mountDrive = libfm.mountDrive;
module.exports.unmountDrive = libfm.unmountDrive;
module.exports.getDeviceLabelOrUUID = libfm.getDeviceLabelOrUUID;

module.exports.getUserName = libfm.getUserName;
module.exports.changePermissions = libfm.changePermissions;

module.exports.compressFile = libfm.compressFile;
module.exports.decompressFile = libfm.decompressFile;
