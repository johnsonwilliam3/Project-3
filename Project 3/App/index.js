const { app, BrowserWindow } = require('electron')

const createWindow = () => {
  const win = new BrowserWindow({
    width: 1024,
    height: 768
  })

  win.loadFile('../output/footprint.html')
}

app.whenReady().then(() => {
  createWindow()
})