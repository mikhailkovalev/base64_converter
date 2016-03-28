#include "base64_converter.hpp"

bool Base64_converter::isCharValid(char c)
{
    return c >= 'A' && c <= 'Z' ||
           c >= 'a' && c <= 'z' ||
           c >= '0' && c <= '9' ||
           c == '+' || c == '/' ||
           c == '=';
}

bool Base64_converter::toBase64(const char* inputFileName, const char* outputFileName)
{
    std::ifstream input(inputFileName, std::ios::in | std::ios::binary);
    if (!input) return false;

    std::ofstream output(outputFileName, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!output) return false;

    const int inputByteCount = 3;
    const int outputByteCount = 4;

    int j;
    char *inputBuf = new char[inputByteCount];
    char *outputBuf = new char[outputByteCount];

    int breakPlace = -1;

    while (true)
    {
        for (j = 0; j < inputByteCount; ++j)
        {
            input.get(inputBuf[inputByteCount - j - 1]);
            if (!input)
            {
                breakPlace = j;
                break;
            }
        }

        if (breakPlace == 0)
        {
            break;
        }

        if (breakPlace != -1)
        {
            for (j = breakPlace; j < inputByteCount; ++j)
            {
                inputBuf[inputByteCount - j - 1] = 0;
            }
        }

        outputBuf[3] = inputBuf[0] & 0x3F;
        outputBuf[2] = ((inputBuf[0] >> 6) & 0x3) | ((inputBuf[1] & 0xF) << 2);
        outputBuf[1] = ((inputBuf[1] >> 4) & 0xF) | ((inputBuf[2] & 0x3) << 4);
        outputBuf[0] = (inputBuf[2] >> 2) & 0x3F;

        for (j = 0; j < outputByteCount; ++j)
        {
            if (outputBuf[j] < 26)       outputBuf[j] += 'A';
            else if (outputBuf[j] < 52)  outputBuf[j] += 'a' - 26;
            else if (outputBuf[j] < 62)  outputBuf[j] += '0' - 52;
            else if (outputBuf[j] == 62) outputBuf[j] =  '+';
            else                         outputBuf[j] =  '/';
        }

        if (breakPlace == 1)
        {
            outputBuf[3] = outputBuf[2] = '=';
        }
        else if (breakPlace == 2)
        {
            outputBuf[3] = '=';
        }

        for (j = 0; j < outputByteCount; ++j)
        {
            output.put(outputBuf[j]);
        }

        if (!input) break;
    }

    delete[] inputBuf;
    delete[] outputBuf;

    input.close();
    output.close();
}

bool Base64_converter::fromBase64(const char* inputFileName, const char* outputFileName)
{
    std::ifstream input(inputFileName, std::ios::in | std::ios::binary);
    if (!input) return false;

    std::ofstream output(outputFileName, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!output) return false;

    const int inputByteCount = 4;
    const int outputByteCount = 3;

    int j;
    char* inputBuf = new char[inputByteCount];
    char* outputBuf = new char[outputByteCount];
    int equalCount;
    int breakPlace = -1;

    while (true)
    {
        j = 0;
        while(j < inputByteCount)
        {
            input.get(inputBuf[inputByteCount - j - 1]);

            if (!input)
            {
                breakPlace = j;
                break;
            }

            if (isCharValid(inputBuf[inputByteCount - j - 1]))
            {
                ++j;
            }
        }

        if (breakPlace == 0)
        {
            break;
        }

        if (breakPlace != -1)
        {
            for (j = breakPlace; j < inputByteCount; ++j)
            {
                inputBuf[inputByteCount - j - 1] = '=';
            }
        }

        equalCount = 0;
        for (j = 0; j < inputByteCount; ++j)
        {
            if (inputBuf[j] == '=')
            {
                ++equalCount;
            }
        }

        if (equalCount == inputByteCount)
        {
            break;
        }

        for (j = 0; j < inputByteCount; ++j)
        {
            if (inputBuf[j] >= 'A' && inputBuf[j] <= 'Z') inputBuf[j] -= 'A';
            else if (inputBuf[j] >= 'a' && inputBuf[j] <= 'z') inputBuf[j] -= 'a' - 26;
            else if (inputBuf[j] >= '0' && inputBuf[j] <= '9') inputBuf[j] -= '0' - 52;
            else if (inputBuf[j] == '+') inputBuf[j] = 62;
            else if (inputBuf[j] == '/') inputBuf[j] = 63;
            else inputBuf[j] = '\0';
        }

        outputBuf[0] = inputBuf[0] | ((inputBuf[1] & 0x3) << 6);
        outputBuf[1] = ((inputBuf[1] >> 2) & 0xF) | ((inputBuf[2] & 0xF) << 4);
        outputBuf[2] = ((inputBuf[2] >> 4) & 0x3) | ((inputBuf[3] & 0x3F) << 2);

        for (j = 0; j < outputByteCount - equalCount; ++j)
        {
            output.put(outputBuf[outputByteCount - j - 1]);
        }

        if (!input) break;

    }

    delete[] inputBuf;
    delete[] outputBuf;

    input.close();
    output.close();
}
