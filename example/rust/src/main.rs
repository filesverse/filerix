use filerix::FileSystem::DriveUtils;

fn main() {
  let drive_path = "/dev/sda1";

  match DriveUtils::get_drive_usage(drive_path) {
    Some(usage) => {
      println!("Used space: {} bytes", usage.used);
      println!("Total space: {} bytes", usage.total);
    }
    None => {
      println!("Failed to retrieve drive usage for {}", drive_path);
    }
  }
}
