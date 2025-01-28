import { init, getDriveUsage } from "@kingmaj0r/libfm/lib";
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

init(__dirname + "/../build/Release/fm.node");

async function getDriveUsageFunc(drivePath) {
  try {
    const usage = await getDriveUsage(drivePath);

    const usedBytes = usage.used_space;
    const totalBytes = usage.total_space;

    console.log(`Used space: ${usedBytes} bytes`);
    console.log(`Total space: ${totalBytes} bytes`);
  } catch (error) {
    console.error('Error fetching drive usage:', error);
  }
}

const drivePath = '/dev/sdX';
getDriveUsageFunc(drivePath);
