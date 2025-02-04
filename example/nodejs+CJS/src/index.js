const { init, getDriveUsage } = require('@kingmaj0r/filerix/lib');

init(__dirname + "/../build/Release/filerix.node");

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
