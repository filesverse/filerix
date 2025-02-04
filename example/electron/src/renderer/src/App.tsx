import { useState, useEffect } from "react";
import { getDrives, getDriveUsage } from "@kingmaj0r/filerix";

function App(): JSX.Element {
  const [drives, setDrives] = useState<string[]>([]);
  const [selectedDrive, setSelectedDrive] = useState<string | null>(null);
  const [driveUsage, setDriveUsage] = useState<any | null>(null);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    // Call getDrives when the component mounts to fetch the list of available drives
    const fetchDrives = async () => {
      try {
        const driveList = await getDrives();
        setDrives(driveList);
      } catch (err) {
        console.error("Error fetching drives:", err);
        setError("Failed to fetch drives.");
      }
    };

    fetchDrives();
  }, []);

  const handleDriveClick = async (drive: string) => {
    setSelectedDrive(drive);
    setDriveUsage(null);
    setError(null);

    try {
      const usage = await getDriveUsage(drive.partition); // Get the usage for the selected drive
      setDriveUsage(usage);
    } catch (err) {
      console.error("Error fetching drive usage:", err);
      setError("Failed to get drive usage. Please check the drive.");
      setDriveUsage(null);
    }
  };

  return (
    <div style={{ padding: "20px", fontFamily: "Arial, sans-serif" }}>
      <h1>Example</h1>

      <h3>Available Drives:</h3>
      {drives.length === 0 ? (
        <p>No drives found.</p>
      ) : (
        <ul>
          {drives.map((drive, index) => (
            <li
              key={index}
              onClick={() => handleDriveClick(drive)}
              style={{
                cursor: "pointer",
                padding: "5px",
                margin: "5px",
                backgroundColor: selectedDrive === drive ? "#e0e0e0" : "transparent"
              }}
            >
              <div>
                <p>
                  <strong>Device:</strong> {drive.device}
                </p>
              </div>
            </li>
          ))}
        </ul>
      )}

      {error && <p style={{ color: "red", marginTop: "20px" }}>{error}</p>}

      {driveUsage && selectedDrive && (
        <div style={{ marginTop: "20px" }}>
          <h3>Drive Usage Details:</h3>
          <table style={{ width: "100%", borderCollapse: "collapse" }}>
            <thead>
              <tr>
                <th style={{ padding: "8px", border: "1px solid #ddd" }}>Property</th>
                <th style={{ padding: "8px", border: "1px solid #ddd" }}>Value</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Device</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>{selectedDrive.device}</td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Status</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>{selectedDrive.status}</td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Unmountable</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  {selectedDrive.unmountable ? "Yes" : "No"}
                </td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Mount Point</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  {selectedDrive.mountPoint}
                </td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Partition</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  {selectedDrive.partition}
                </td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>File System Type</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>{selectedDrive.fsType}</td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Used Space (bytes)</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  {driveUsage.used_space}
                </td>
              </tr>
              <tr>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  <strong>Total Space (bytes)</strong>
                </td>
                <td style={{ padding: "8px", border: "1px solid #ddd" }}>
                  {driveUsage.total_space}
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      )}
    </div>
  );
}

export default App;
