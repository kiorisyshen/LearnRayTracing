#pragma once
#include <vector>

namespace LearnRT {

template <typename T>
class Frame {
   public:
    Frame(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) {
        m_Data.resize(width * height);
    }

    T &at(uint32_t row, uint32_t col) {
        return m_Data[row * m_Width + m_Height];
    }

    const T &at(uint32_t row, uint32_t col) const {
        return m_Data[row * m_Width + m_Height];
    }

    uint32_t getWidth() const {
        return m_Width;
    }

    uint32_t getHeight() const {
        return m_Height;
    }

   private:
    uint32_t m_Width;
    uint32_t m_Height;
    std::vector<T> m_Data;
};

}  // namespace LearnRT
