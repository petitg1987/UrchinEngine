#include <iostream>
#include <limits>
#include <cmath>

#include <util/TypeConverter.h>
#include <util/StringUtil.h>
#include <logger/Logger.h>

namespace urchin {

    //static
    const char TypeConverter::FLOAT_DELIMITER = ' ';
    const float TypeConverter::FLOAT_INT_SCALE = 8192.0;

    bool TypeConverter::isInt(const std::string& str) {
        std::istringstream iss(str);
        int value = 0;
        iss >> std::noskipws >> value;
        return iss.eof() && !iss.fail();
    }

    int TypeConverter::toInt(const std::string& str) {
        std::istringstream iss(str);
        int value = 0;
        iss >> value;
        return value;
    }

    bool TypeConverter::isUnsignedInt(const std::string& str) {
        std::istringstream iss(str);
        unsigned int value = 0;
        iss >> std::noskipws >> value;
        return iss.eof() && !iss.fail();
    }

    unsigned int TypeConverter::toUnsignedInt(const std::string& str) {
        std::istringstream iss(str);
        unsigned int value = 0;
        iss >> value;
        return value;
    }

    bool TypeConverter::isFloat(const std::string& str) {
        std::locale::global(std::locale("C")); //for float

        std::istringstream iss(str);
        float value = 0.0f;
        iss >> std::noskipws >> value;
        return iss.eof() && !iss.fail();
    }

    float TypeConverter::toFloat(const std::string& str) {
        std::locale::global(std::locale("C")); //for float

        std::istringstream iss(str);
        float value = 0.0f;
        iss >> value;
        return value;
    }

    bool TypeConverter::isChar(const std::string& str) {
        return str.size() == 1;
    }

    char TypeConverter::toChar(const std::string& str) {
        return str[0];
    }

    bool TypeConverter::isBool(const std::string& str) {
        return str.size() == 1 && isInt(str);
    }

    bool TypeConverter::toBool(const std::string& str) {
        return str[0] != '0';
    }

    long long TypeConverter::toLongLong(float value, float scale) {
        const float minValue = (float)std::numeric_limits<long long>::min() / scale;
        const float maxValue = (float)std::numeric_limits<long long>::max() / scale;

        if (value < 0.0) {
            if (value < minValue) {
                Logger::instance()->logError("Impossible to convert float " + std::to_string(value) + " in long long type.");
            }
            return std::llround(value * scale - 0.5);
        }
        else {
            if (value > maxValue) {
                Logger::instance()->logError("Impossible to convert float " + std::to_string(value) + " in long long type.");
            }
            return std::llround(value * scale + 0.5);
        }
    }

    float TypeConverter::toFloat(long long value, float scale) {
        return (float)value / scale;
    }

    Point2<float> TypeConverter::toPoint2(const std::string& str) {
        std::vector<float> floatValues = floatSplit(str, 2);
        return Point2<float>(floatValues[0], floatValues[1]);
    }

    Point3<float> TypeConverter::toPoint3(const std::string& str) {
        std::vector<float> floatValues = floatSplit(str, 3);
        return Point3<float>(floatValues[0], floatValues[1], floatValues[2]);
    }

    Point4<float> TypeConverter::toPoint4(const std::string& str) {
        std::vector<float> floatValues = floatSplit(str, 4);
        return Point4<float>(floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
    }

    Vector2<float> TypeConverter::toVector2(const std::string& str) {
        std::vector<float> floatValues = floatSplit(str, 2);
        return Vector2<float>(floatValues[0], floatValues[1]);
    }

    Vector3<float> TypeConverter::toVector3(const std::string& str) {
        std::vector<float> floatValues = floatSplit(str, 3);
        return Vector3<float>(floatValues[0], floatValues[1], floatValues[2]);
    }

    Vector4<float> TypeConverter::toVector4(const std::string& str) {
        std::vector<float> floatValues = floatSplit(str, 4);
        return Vector4<float>(floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
    }

    /**
     * Split a string into float
     * @param str String to split
     * @param expectedSplit Number of expected split
     * @return Split string in float
     */
    std::vector<float> TypeConverter::floatSplit(const std::string& str, unsigned int expectedSplit) {
        std::vector<std::string> stringValues;
        stringValues.reserve(expectedSplit);
        StringUtil::split(str, FLOAT_DELIMITER, stringValues);

        if (stringValues.size() != expectedSplit) {
            throw std::invalid_argument("Number of float expected: " + std::to_string(expectedSplit) + ", found: "
                    + std::to_string((unsigned int)stringValues.size()) + ". String value: " + str + ".");
        }

        std::vector<float> floatValues;
        floatValues.reserve(expectedSplit);
        for (const auto& stringValue : stringValues) {
            floatValues.push_back(toFloat(stringValue));
        }

        return floatValues;
    }

}
