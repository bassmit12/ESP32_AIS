# ESP32 Sender - Monitor Only
# Run this to monitor the sender output (must be already uploaded)

$PIO = "C:\Users\bassm\.platformio\penv\Scripts\platformio.exe"

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "  ESP32 AIS Sender - Serial Monitor" -ForegroundColor Cyan
Write-Host "  COM Port: COM7" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Press Ctrl+C to exit monitoring" -ForegroundColor Yellow
Write-Host ""

Set-Location "D:\Projects\Neptargos\ESP32_Sender"

& $PIO device monitor --port COM7
