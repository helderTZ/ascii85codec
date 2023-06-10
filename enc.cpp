#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <cmath>

char get_next_char_or_padding(std::string_view sv, const size_t index) {
    if (index < sv.size()) {
        return sv[index];
    } else {
        return 'u';
    }
}

void push_back_if_not_u(std::string& str, const char character) {
    if ((int)character != 84) {
        str.push_back(character);
    }
}

size_t get_nearest_multiple5(const size_t size) {
    return size + 5*ceil((float)size/5);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Expected argument: filename" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string contents;
    std::getline(std::ifstream(argv[1]), contents, '\0');
    std::string encoded_payload;
    size_t padded_size = get_nearest_multiple5(contents.size());

    encoded_payload.append("<~");

    for (size_t i = 0; i < padded_size; i+=4) {
        char digit1 = get_next_char_or_padding(contents, i+0);
        char digit2 = get_next_char_or_padding(contents, i+1);
        char digit3 = get_next_char_or_padding(contents, i+2);
        char digit4 = get_next_char_or_padding(contents, i+3);

        int value = (digit1 << 24) | (digit2 << 16) | (digit3 << 8) | digit4;

        char rem1 = (char) (value % 85);
        value /= 85;
        char rem2 = (char) (value % 85);
        value /= 85;
        char rem3 = (char) (value % 85);
        value /= 85;
        char rem4 = (char) (value % 85);
        value /= 85;
        char rem5 = (char) (value % 85);
        value /= 85;

        push_back_if_not_u(encoded_payload, rem5+33);
        push_back_if_not_u(encoded_payload, rem4+33);
        push_back_if_not_u(encoded_payload, rem3+33);
        push_back_if_not_u(encoded_payload, rem2+33);
        push_back_if_not_u(encoded_payload, rem1+33);
    }

    encoded_payload.append("~>");

    std::cout << encoded_payload << std::endl;

    return EXIT_SUCCESS;
}