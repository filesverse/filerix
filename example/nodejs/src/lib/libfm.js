import libfm from "../../build/Release/fm.node";

export default libfm;
export const getFiles = libfm.getFiles;
export const searchFiles = libfm.searchFiles;
export const copyFile = libfm.copyFile;
export const cutFile = libfm.cutFile;
export const renameFile = libfm.renameFile;

export const getDrives = libfm.getDrives;
export const getDriveUsage = libfm.getDriveUsage;
export const mountDrive = libfm.mountDrive;
export const unmountDrive = libfm.unmountDrive;
export const getDeviceLabelOrUUID = libfm.getDeviceLabelOrUUID;

export const getUserName = libfm.getUserName;
export const changePermissions = libfm.changePermissions;

export const compressFile = libfm.compressFile;
export const decompressFile = libfm.decompressFile;
