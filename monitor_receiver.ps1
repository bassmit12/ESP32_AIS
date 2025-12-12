# ESP32 Receiver - Monitor Only
# Run this to monitor the receiver output (must be already uploaded)

$PIO = "C:\Users\bassm\.platformio\penv\Scripts\platformio.exe"

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "  ESP32 AIS Receiver - Serial Monitor" -ForegroundColor Cyan
Write-Host "  COM Port: COM8" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Press Ctrl+C to exit monitoring" -ForegroundColor Yellow
Write-Host ""

Set-Location "D:\Projects\Neptargos\ESP32_Receiver"

& $PIO device monitor --port COM8
