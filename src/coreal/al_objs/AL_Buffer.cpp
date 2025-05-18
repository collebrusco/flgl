#include "AL_Buffer.h"
#include <flgl/logger.h>
#include <flgl/config.h>
LOG_MODULE(albuffer);

AL_Buffer::AL_Buffer() : DeviceObject(ALBUFFER) {
    handle = 0xFFFFFFFF;
}

AL_Buffer::AL_Buffer(uint32_t h) : AL_Buffer() {
    handle = h;
}

void AL_Buffer::create() {
    alGenBuffers(1, &handle);
    LOG_DBG("created %d", handle);
    this->enlist(handle);
}

void AL_Buffer::destroy() {
    this->delist(handle);
    LOG_DBG("destroying %d", handle);
    alDeleteBuffers(1, &handle);
}

void AL_Buffer::buffer_mono(const int8_t *const dat, size_t size, size_t freq)
{
    alBufferData(handle, AL_FORMAT_MONO8, dat, size * sizeof(int8_t), freq);
}

void AL_Buffer::buffer_mono(const int16_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_MONO16, dat, size * sizeof(int16_t), freq);
}

void AL_Buffer::buffer_stereo(const int8_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_STEREO8, dat, size * sizeof(int8_t), freq);
}

void AL_Buffer::buffer_stereo(const int16_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_STEREO16, dat, size * sizeof(int16_t), freq);
}


void AL_Buffer::buffer_mono(std::vector<int8_t> const& v, size_t freq) {
    this->buffer_mono(v.data(), v.size(), freq);
}

void AL_Buffer::buffer_mono(std::vector<int16_t> const& v, size_t freq) {
    this->buffer_mono(v.data(), v.size(), freq);
}

void AL_Buffer::buffer_stereo(std::vector<int8_t> const& v, size_t freq) {
    this->buffer_stereo(v.data(), v.size(), freq);
}

void AL_Buffer::buffer_stereo(std::vector<int16_t> const& v, size_t freq) {
    this->buffer_stereo(v.data(), v.size(), freq);
}



#include <fstream>

static uint16_t read_u16(std::ifstream& f) {
    uint8_t b1 = f.get();
    uint8_t b2 = f.get();
    return static_cast<uint16_t>(b1 | (b2 << 8));
}

static uint32_t read_u32(std::ifstream& f) {
    uint8_t b1 = f.get();
    uint8_t b2 = f.get();
    uint8_t b3 = f.get();
    uint8_t b4 = f.get();
    return static_cast<uint32_t>(b1 | (b2 << 8) | (b3 << 16) | (b4 << 24));
}

AL_Buffer AL_Buffer::from_wav(std::string const& name) {
    std::string path = flglPathConfig::audio_path() + name + ".wav";
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        LOG_ERR("Failed to open wav file: %s", path.c_str());
        return AL_Buffer();
    }

    /* validate header */
    char riff[4]; file.read(riff, 4);
    if (std::strncmp(riff, "RIFF", 4) != 0) {
        LOG_ERR("Invalid RIFF header in %s", path.c_str());
        return AL_Buffer();
    }

    read_u32(file); /* file size */

    char wave[4]; file.read(wave, 4);
    if (std::strncmp(wave, "WAVE", 4) != 0) {
        LOG_ERR("Invalid WAVE format in %s", path.c_str());
        return AL_Buffer();
    }

    /* find 'fmt' */
    uint16_t audioFormat = 0, channels = 0, bitsPerSample = 0;
    uint32_t sampleRate = 0;

    while (file) {
        char chunkId[4]; file.read(chunkId, 4);
        uint32_t chunkSize = read_u32(file);

        if (std::strncmp(chunkId, "fmt ", 4) == 0) {
            audioFormat   = read_u16(file);
            channels      = read_u16(file);
            sampleRate    = read_u32(file);
            read_u32(file);  /* skip byte rate */
            read_u16(file);  /* skip block align */
            bitsPerSample = read_u16(file);

            if (chunkSize > 16) {
                file.seekg(chunkSize - 16, std::ios::cur);  /* skip extra fmt data */
            }
        } else if (std::strncmp(chunkId, "data", 4) == 0) {
            /* read pcm dat */
            std::vector<char> pcm(chunkSize);
            file.read(pcm.data(), chunkSize);

            ALenum format = AL_NONE;
            if (channels == 1 && bitsPerSample == 8)  format = AL_FORMAT_MONO8;
            if (channels == 1 && bitsPerSample == 16) format = AL_FORMAT_MONO16;
            if (channels == 2 && bitsPerSample == 8)  format = AL_FORMAT_STEREO8;
            if (channels == 2 && bitsPerSample == 16) format = AL_FORMAT_STEREO16;

            if (format == AL_NONE) {
                LOG_ERR("Unsupported WAV format in %s", path.c_str());
                return AL_Buffer();
            }

            AL_Buffer buffer;
            buffer.create();
            alBufferData(buffer.id(), format, pcm.data(), chunkSize, sampleRate);
            LOG_DBG("Loaded wav %s as %dHz %dbit %s", path.c_str(), sampleRate,
                    bitsPerSample, (channels == 1 ? "mono" : "stereo"));
            return buffer;
        } else {
            file.seekg(chunkSize, std::ios::cur);
        }
    }

    LOG_ERR("Failed to find data chunk in %s", path.c_str());
    return AL_Buffer();
}

AL_Buffer AL_Buffer::from_ogg(std::string const& name) {
    LOG_ERR("LOADING FROM OGG UNIMPLEMENTED! TODO fix it! but .wav works");
    return AL_Buffer();
}
