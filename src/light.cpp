// #include <Arduino.h>
// #include "driver/twai.h"
// #include <unordered_map>
// #include <vector>

// using namespace std;

// class Device{
// public:
//     virtual ~Device() = default;
//     virtual twai_message_t toMessage() = 0;
// };

// class Light : public Device{
//     bool _state;
//     int _brightness;

// public:
//     void setOn(bool state){
//         if (state){
//             _state = 1;
//         }
//         else{
//             _state = 0;
//         }
//     }

//     void setBrightness(int brightness){
//         if (brightness > 0 && brightness <= 100){
//             _brightness = brightness;
//         };
//     }

//     twai_message_t toMessage(int id) {
//         twai_message_t msg = {};
//         msg.extd = 1;
//         msg.identifier = 0x100;
//         msg.data_length_code = 3;
//         msg.data[0] = id;
//         msg.data[1] = _state;
//         msg.data[2] = _brightness;

//         return msg;
//     }
// };