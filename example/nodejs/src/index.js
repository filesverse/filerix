const libfm = require('../build/Release/fm.node');

async function getDriveUsage(drivePath) {
  try {
    const usage = await libfm.getDriveUsage(drivePath);

    const usedBytes = usage.used_space;
    const totalBytes = usage.total_space;

    console.log(`Used space: ${usedBytes} bytes`);
    console.log(`Total space: ${totalBytes} bytes`);
  } catch (error) {
    console.error('Error fetching drive usage:', error);
  }
}

const drivePath = '/';
getDriveUsage(drivePath);
