#include "../hardware.h"
#include "../hw_config.cpp"
#include "hw_config.h"
#include "ff.h"

FATFS fs;

ExternalSD::ExternalSD() {}

bool ExternalSD::is_ready()
{
    if (!this->mounted)
        return false;

    FIL file;
    if (f_open(&file, ".sprig", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        return false;

    f_close(&file);

    return true;
}

FRESULT ExternalSD::mount()
{
    FRESULT fr = f_mount(&fs, "", 1);
    if (fr == FR_OK)
        this->mounted = true;

    return fr;
}

FRESULT ExternalSD::unmount()
{
    FRESULT fr = f_unmount("");
    if (fr == FR_OK)
        this->mounted = false;

    return fr;
}

FRESULT ExternalSD::write(const char *const path, char *text)
{
    if (!is_ready())
        return FR_NOT_READY;

    FIL file;
    FRESULT fr = f_open(&file, path, FA_CREATE_ALWAYS | FA_WRITE);
    if (fr != FR_OK)
        return fr;

    UINT bytes_written;
    f_write(&file, text, strlen(text), &bytes_written);
    f_close(&file);

    return FR_OK;
}

FRESULT ExternalSD::read(const char *const path, void *buffer, uint64_t offset)
{
    if (!is_ready())
        return FR_NOT_READY;

    FIL file;
    FRESULT fr = f_open(&file, path, FA_READ);
    if (fr != FR_OK)
        return fr;

    if (offset > 0)
    {
        fr = f_lseek(&file, offset);
        if (fr != FR_OK)
        {
            f_close(&file);
            return fr;
        }
    }

    uint64_t file_size = f_size(&file);
    if (offset >= file_size)
    {
        f_close(&file);
        return FR_OK;
    }

    uint64_t remaining = file_size - offset;

    uint8_t temp_buf[2];
    UINT bytes_read = 0;
    uint64_t total_read = 0;
    uint8_t *out_ptr = static_cast<uint8_t *>(buffer);

    while (remaining > 0)
    {
        UINT to_read = (remaining >= 2) ? 2 : (UINT)remaining;
        fr = f_read(&file, temp_buf, to_read, &bytes_read);

        if (fr != FR_OK || bytes_read == 0)
            break;

        memcpy(out_ptr + total_read, temp_buf, bytes_read);

        total_read += bytes_read;
        remaining -= bytes_read;
    }

    f_close(&file);
    return fr;
}
