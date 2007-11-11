#ifndef __ENUM_H__
#define __ENUM_H__

class EnumValue {
public:
    EnumValue(int value, std::string name, std::string desc, std::string help):
        value(value),
        name (name ),
        desc (desc ),
        help (help )
    {};
    std::string fmt();

private:
    std::string name;
    std::string desc;
    std::string help;
    int value;
};

class Enum {
public:
    Enum(EnumValue values[]);

private:
    std::map<int,EnumValue *> values;
};

#endif
