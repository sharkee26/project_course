#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>
#include <chrono>
#include <thread>

struct WavHeader {
    char chunkID[4];
    unsigned int chunkSize;
    char format[4];
    char subchunk1ID[4];
    unsigned int subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2ID[4];
    unsigned int subchunk2Size;
};

void loadWAV(const char* filename, ALuint& buffer) {
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }

    WavHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

    // Проверка формата WAV
    if (std::string(header.chunkID, 4) != "RIFF" || std::string(header.format, 4) != "WAVE") {
        std::cerr << "Неверный формат WAV файла." << std::endl;
        return;
    }

    // Загрузка данных в буфер OpenAL
    ALenum format = (header.numChannels == 1) ? 
                    (header.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16) :
                    (header.bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);
    
    char* data = new char[header.subchunk2Size];
    
    if (!file.read(data, header.subchunk2Size)) {
        std::cerr << "Ошибка при чтении данных из файла." << std::endl;
        delete[] data; // Освобождение памяти
        return;
    }

    alGenBuffers(1, &buffer);
    
    // Проверка на ошибки
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Ошибка при создании буфера: " << error << std::endl;
        delete[] data; // Освобождение памяти
        return;
    }

    alBufferData(buffer, format, data, header.subchunk2Size, header.sampleRate);

    delete[] data; // Освобождение памяти

    // Вывод информации о текущей мелодии
    std::cout << "Загружена мелодия: " << filename << std::endl; // Выводим имя файла
}

void setVolume(ALuint source, float volume) {
    // Установка громкости источника
    alSourcef(source, AL_GAIN, volume);
}

int main() {
    ALCdevice* device = alcOpenDevice(nullptr); // Открыть устройство вывода
    if (!device) {
        std::cerr << "Не удалось открыть устройство вывода." << std::endl;
        return -1;
    }

    ALCcontext* context = alcCreateContext(device, nullptr); // Создать контекст
    if (!context) {
        std::cerr << "Не удалось создать контекст." << std::endl;
        alcCloseDevice(device);
        return -1;
    }
    
    alcMakeContextCurrent(context); // Активировать контекст

    ALuint buffer;

   loadWAV("C:/Codes/Course_project/project_course/sample2.wav", buffer); // Укажите путь к вашему WAV файлу

   // Создание источника звука
   ALuint source;

   alGenSources(1, &source);
   
   // Привязка буфера к источнику
   alSourcei(source, AL_BUFFER, buffer);
   
   // Установка начальной громкости
   setVolume(source, 0.5f); // Установите начальную громкость на 50%

   // Воспроизведение звука
   alSourcePlay(source);

   // Проверка состояния источника
   ALint state;
   alGetSourcei(source, AL_SOURCE_STATE, &state);
   if (state != AL_PLAYING) {
       std::cerr << "Ошибка воспроизведения: источник не воспроизводится." << std::endl;
       return -1; // Завершение программы при ошибке воспроизведения
   }

   // Ожидание завершения воспроизведения с возможностью регулировки громкости
   float volume = 0.5f; // Начальная громкость
    std::cout << "Введите '+' для увеличения громкости или '-' для уменьшения (q для выхода): ";
   while (true) {
       char command;
       std::cin >> command;

       if (command == '+') {
           volume += 0.1f; // Увеличиваем громкость на 10%
           if (volume > 1.0f) volume = 1.0f; // Ограничиваем максимум
           setVolume(source, volume);
       } else if (command == '-') {
           volume -= 0.1f; // Уменьшаем громкость на 10%
           if (volume < 0.0f) volume = 0.0f; // Ограничиваем минимум
           setVolume(source, volume);
       } else if (command == 'q') {
           break; // Выход из цикла при вводе 'q'
       }

       // Обновление уровня громкости в одной строке
       std::cout << "\rГромкость: " << volume * 100 << "%     "; // Обновляем строку с уровнем громкости

       alGetSourcei(source, AL_SOURCE_STATE, &state);
       if (state != AL_PLAYING) break; // Выход из цикла если звук закончил играть
   }

   // Освобождение ресурсов
   alDeleteSources(1, &source); // Удаление источника
   alDeleteBuffers(1, &buffer); // Удаление буфера
   alcDestroyContext(context); // Уничтожение контекста
   alcCloseDevice(device); // Закрытие устройства

   return 0;
}
