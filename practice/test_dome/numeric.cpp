#include <iostream>
#include <string>

class TextInput
{
public:
    virtual void add(char c) { value += c; }

    std::string getValue() { return value; }
    virtual ~TextInput() {}
private:
    std::string value;
};

class NumericInput : public TextInput { 
    void add(char c) {
        if (std::isdigit(c)) {
            TextInput::add(c);
        }
    }
    
};

#ifndef RunTests
int main()
{
    TextInput* input = new NumericInput();
    input->add('1');
    input->add('a');
    input->add('0');
    std::cout << input->getValue();
}
#endif