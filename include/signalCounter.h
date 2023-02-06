// class SignalCounter : public Signal {
//  public:
//   SignalCounter(const char* name, uint8_t startBit, uint8_t len, float scale, float offset, Endianess endianess, Sign sign, const char* unit, uint32_t counter = 0)
//       : Signal(name, startBit, len, scale, offset, endianess, sign, unit), counter_(counter) {}

//   bool CheckCounter(CanMessage& message, uint32_t value) {
//     if (value == counter_ + 1) {
//       counter_ = value;
//       return true;
//     }
//     counter_ = value;
//     return false;
//   }

//  private:
//   uint32_t counter_;
// };

//     bool MsgCheckCounter(CanMessage& message, uint8_t count) {
//         if (count == message.counter + 1) {
//             message.counter = count;
//             return true;
//         }
//         message.counter = count;
//         return false;
