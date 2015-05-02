#include <Vorb/types.h>

// An enum where each value corresponds to a bit
enum class Channel {
    RED = 0x01,
    BLUE = 0x02,
    GREEN = 0x04,
};

// Define the bitwise OR and AND operators for this enum class
ENUM_CLASS_OP_INL(Channel, ui8, |)
ENUM_CLASS_OP_INL(Channel, ui8, &)

enum class MyBoolean {
    BFALSE = 0x00,
    BTRUE = 0x01
};

// Define OR, AND, XOR, ADD, SUB, and SHIFT operators for this enum class
ENUM_CLASS_OPS_INL(Channel, bool)

int main() {
    // Create variables that hold channels
    Channel red = Channel::RED;
    Channel green = Channel::GREEN;
    
    // Create a variable that symbolizes all the channels
    Channel all = red | green | Channel::BLUE;
    
    // This produces a compile error because no addition operator was defined
    // Channel redGreen = red + green; 
    
    bool truth = (MyBoolean::BFALSE ^ (MyBoolean::BFALSE & MyBoolean::BTRUE)) == MyBoolean::BFALSE;
    return truth ? 0 : -1;
}
