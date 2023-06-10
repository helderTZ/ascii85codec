#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <cmath>
#include <cstring>

bool is_ascii85(std::string_view contents) {
    return (contents.compare(0,2,"<~") == 0)
        && (contents.compare(contents.size()-2,2,"~>") == 0);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Expected argument: filename" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string_view filename(argv[1]);
    std::string contents;
    std::getline(std::ifstream(filename.data()), contents, '\0');

    if (!is_ascii85(contents)) {
        std::cerr << "The file is not encoded in ascii85" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string_view encoded_payload = std::string_view(contents).substr(2, contents.size()-4);
    std::string decoded_payload;

    size_t i = 0;
    for (; i < encoded_payload.size(); i+=5) {
        char digit1 = contents[i+0] - 33;
        char digit2 = contents[i+1] - 33;
        char digit3 = contents[i+2] - 33;
        char digit4 = contents[i+3] - 33;
        char digit5 = contents[i+4] - 33;

        int value =   digit1*std::pow(85, 4)
                    + digit2*std::pow(85, 3)
                    + digit3*std::pow(85, 2)
                    + digit4*85
                    + digit5;

        // char ascii_char1 = (char) ((value && 0xFFFF'0000'0000'0000L) >> 24);
        // char ascii_char2 = (char) ((value && 0x0000'FFFF'0000'0000L) >> 16);
        // char ascii_char3 = (char) ((value && 0x0000'0000'FFFF'0000L) >>  8);
        // char ascii_char4 = (char) ((value && 0x0000'0000'0000'FFFFL) >>  0);

        typedef union {
            int value;
            char chars[4];
        } magic;

        magic m;
        memcpy(&m, &value, sizeof(int));

        char ascii_char1 = m.chars[0];
        char ascii_char2 = m.chars[1];
        char ascii_char3 = m.chars[2];
        char ascii_char4 = m.chars[3];

        decoded_payload.push_back(ascii_char1);
        decoded_payload.push_back(ascii_char2);
        decoded_payload.push_back(ascii_char3);
        decoded_payload.push_back(ascii_char4);
    }

    std::cout << decoded_payload << std::endl;

    return EXIT_SUCCESS;
}