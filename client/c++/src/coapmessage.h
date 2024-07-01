#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

enum CoapType {
  Confirmable = 0,
  NonConfirmable = 1,
  Acknowledgement = 2,
  Reset = 3
};

enum CoapCode {
  // Method Codes
  EMPTY = 0x00,
  GET = 0x01,
  POST = 0x02,
  PUT = 0x03,
  DELETE = 0x04,

  // Response Codes
  // Success
  Created = 0x41,
  Deleted = 0x42,
  Valid = 0x43,
  Changed = 0x44,
  Content = 0x45,
  Continue = 0x5f,

  // Client Error
  BadRequest = 0x80,
  Unauthorized = 0x81,
  BadOption = 0x82,
  Forbidden = 0x83,
  NotFound = 0x84,
  MethodNotAllowed = 0x85,
  NotAcceptable = 0x86,
  RequestEntityIncomplete = 0x88,
  PreconditionFailed = 0x8c,
  RequestEntityTooLarge = 0x8d,
  UnsupportedContentFormat = 0x8f,

  // Server Error
  InternalServerError = 0xa0,
  NotImplemented = 0xa1,
  BadGateway = 0xa2,
  ServiceUnavailable = 0xa3,
  GatewayTimeout = 0xa4,
  ProxyingNotSupported = 0xa5
};

enum CoapOptionDelta {
  IfMatch = 1,
  UriHost = 3,
  ETag = 4,
  IfNoneMatch = 5,
  UriPort = 7,
  LocationPath = 8,
  UriPath = 11,
  ContentFormat = 12,
  MaxAge = 14,
  UriQuery = 15,
  Accept = 17,
  LocationQuery = 20,
  Size2 = 28,
  ProxyUri = 35,
  ProxyScheme = 39,
  Size1 = 60
};

enum CoapContentFormat {
  text = 0,
  linkFormat = 40,
  xml = 41,
  octetStream = 42,
  exi = 47,
  json = 50,
  cbor = 60
};

class CoapMessage {
private:
  uint8_t version : 2 = 1;
  CoapType type = CoapType::Confirmable;
  uint8_t tokenLength : 4 = 0;

  CoapCode code = CoapCode::EMPTY;
  uint16_t messageId = 0;

  struct Option {
    CoapOptionDelta delta;
    uint8_t length : 4;
    std::vector<char> value;
  };

  std::vector<Option> options;

  std::string payload;

public:
  CoapMessage() = default;

  CoapMessage(CoapCode code, uint16_t messageId);

  void setOptionUriPath(const std::string& path);

  void setPayload(const std::string& payload);

  std::vector<char> serialize();

  int deserialize(const std::vector<char>& msg);

  friend std::ostream& operator<<(std::ostream& os, const CoapMessage& rhs);
  friend std::ostream& operator<<(std::ostream& os, const CoapType& rhs);
  friend std::ostream& operator<<(std::ostream& os, const CoapCode& rhs);
  friend std::ostream& operator<<(std::ostream& os, const CoapOptionDelta& rhs);
  friend std::ostream& operator<<(std::ostream& os,
                                  const CoapContentFormat& rhs);
  friend std::ostream& operator<<(std::ostream& os, const Option& rhs);
};