use filerix::FileSystem::{DriveUtils, UserUtils, FileUtils};
use std::fs;
use std::io::Write;
use std::path::Path;

fn create_file(file_path: &str, content: &str) {
  let mut file = fs::File::create(file_path).expect("Failed to create file");
  file.write_all(content.as_bytes()).expect("Failed to write to file");
  println!("File created: {}", file_path);
}

fn create_directory(dir_path: &str) {
  fs::create_dir_all(dir_path).expect("Failed to create directory");
  println!("Directory created: {}", dir_path);
}

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

  let drives = DriveUtils::get_drives();
  if drives.is_empty() {
    println!("No drives found.");
  } else {
    println!("Available Drives:");
    for drive in &drives {
      println!("Device: {}", unsafe { std::ffi::CStr::from_ptr(drive.device.as_ptr()) }.to_string_lossy());
    }
  }

  match UserUtils::get_user_name() {
    Some(username) => println!("Current user: {}", username),
    None => println!("Failed to get username"),
  }

  let home_dir = format!("/home/{}/Downloads/TestFolder", UserUtils::get_user_name().unwrap_or("user".to_string()));
  create_directory(&home_dir);

  let file_path = format!("{}/example.txt", home_dir);
  create_file(&file_path, "Hello, Filerix!");

  let files = FileUtils::get_files(&home_dir);
  if files.is_empty() {
    println!("No files found in {}", home_dir);
  } else {
    println!("Files in {}:", home_dir);
    for file in files {
      println!("- {}", unsafe { std::ffi::CStr::from_ptr(file.name) }.to_string_lossy());
    }
  }

  let search_query = "example";
  let found_files = FileUtils::search_files(&home_dir, search_query);
  if found_files.is_empty() {
    println!("No files found matching query '{}'", search_query);
  } else {
    println!("Search results for '{}':", search_query);
    for file in found_files {
      println!("- {}", unsafe { std::ffi::CStr::from_ptr(file.name) }.to_string_lossy());
    }
  }

  let new_file_path = format!("{}/renamed_example.txt", home_dir);
  if FileUtils::rename(&file_path, &new_file_path) {
    println!("File renamed successfully");
  } else {
    println!("Failed to rename file");
  }

  let copy_path = format!("{}/copy_example.txt", home_dir);
  if FileUtils::copy(&new_file_path, &copy_path) {
    println!("File copied successfully");
  } else {
    println!("Failed to copy file");
  }

  let move_path = format!("{}/Moved/example_moved.txt", home_dir);
  create_directory(&format!("{}/Moved", home_dir));
  if FileUtils::move_to(&copy_path, &move_path) {
    println!("File moved successfully");
  } else {
    println!("Failed to move file");
  }

  if fs::remove_file(&new_file_path).is_ok() {
    println!("File deleted: {}", new_file_path);
  } else {
    println!("Failed to delete file");
  }

  let permissions_changed = UserUtils::change_permissions(&move_path, 0o644);
  println!("Permissions changed: {}", permissions_changed);
}
