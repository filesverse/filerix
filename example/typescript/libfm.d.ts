declare module "@libfm" {
  // Compression
  export function compressFile(source: string, destination: string): boolean;
  export function decompressFile(source: string, destination: string): boolean;

  // Drives
  export interface DriveInfo {
    device: string;
    status: string;
    unmountable: boolean;
    mountPoint: string;
    partition: string;
  }

  export function getDrives(): DriveInfo[];
  export interface DriveUsage {
    used_space: number;
    total_space: number;
  }
  export function getDriveUsage(drivePath: string): DriveUsage;
  export function mountDrive(source: string, target: string): boolean;
  export function unmountDrive(target: string): boolean;
  export function getDeviceLabelOrUUID(devicePath: string): string;

  // Files
  export interface FileInfo {
    name: string;
    path: string;
    isDirectory: boolean;
    size: number;
    modifiedDate: Date;
  }

  export function getFiles(path: string): FileInfo[];
  export function searchFiles(path: string, query: string): FileInfo[];
  export function copyFile(source: string, destination: string): boolean;
  export function cutFile(source: string, destination: string): boolean;
  export function renameFile(oldPath: string, newPath: string): boolean;

  // User
  export function getUserName(): string;
  export function changePermissions(path: string, mode: number): boolean;
}
