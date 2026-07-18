@echo off
setlocal

echo STEP 1: CLEANING BUILD DIRECTORY
:: Kiem tra va xoa thu muc build neu da ton tai
if exist build (
    echo Deleting existing build folder...
    rmdir /s /q build
)

echo.
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
:: Cau hinh du an bang CMake, dung Ninja generator, luu vao thu muc build
cmake -B build -G Ninja
if %ERRORLEVEL% neq 0 (
    echo Configuration failed!
    exit /b 1
)

echo.
echo STEP 3: COMPILING FIRMWARE WITH NINJA
:: Chay Ninja de bien dich ma nguon ben trong thu muc build
ninja -C build
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
:: Nap file app_firmware.bin xuong Flash tai dia chi 0x08000000 qua giao thuc SWD va tu dong reset
STM32_Programmer_CLI -c port=SWD -w build/app_firmware.bin 0x08000000 -v -rst

echo.
echo Application running ...
echo MCU Reset.

endlocal
pause