import { app, BrowserWindow } from "electron";
import { electronApp, optimizer } from "@electron-toolkit/utils";
import { init as initFilerix } from "@kingmaj0r/filerix/lib";
import { createWindow } from "./window/index";

app.whenReady().then(() => {
  electronApp.setAppUserModelId("com.kingmaj0r");

  app.on("browser-window-created", (_, window) => {
    optimizer.watchWindowShortcuts(window);
  });

  initFilerix(__dirname + "/../../build/Release/filerix.node");

  createWindow();

  app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") app.quit();
});
