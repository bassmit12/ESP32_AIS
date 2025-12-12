# ESP32 Sender - Upload Script
# Run this to upload the sender code to your ESP32

$PIO = "C:\Users\bassm\.platformio\penv\Scripts\platformio.exe"

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "  ESP32 AIS Sender - Upload & Monitor" -ForegroundColor Cyan
Write-Host "  COM Port: COM7" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

Set-Location "D:\Projects\Neptargos\ESP32_Sender"

Write-Host "Building and uploading..." -ForegroundColor Yellow
& $PIO run --target upload --target monitor --upload-port COM7

# Exit code will be displayed
if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Upload completed successfully!" -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "Upload failed with exit code: $LASTEXITCODE" -ForegroundColor Red
}
