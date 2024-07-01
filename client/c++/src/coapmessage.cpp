#include "coapmessage.h"
#include <iomanip>

CoapMessage::CoapMessage(CoapCode code, uint16_t messageId):
        code(code), messageId(messageId)
{}

std::ostream& operator<<(std::ostream& os, const CoapType& rhs) 
{
    switch(rhs) {
        case Confirmable: 
            os << "Confirmable";
            break;
        case NonConfirmable: 
            os << "Non Confirmable";
            break;
        case Acknowledgement: 
            os << "Acknowledgement";
            break;
        case Reset: 
            os << "Reset";
            break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const CoapCode& rhs) 
{
    switch(rhs) {
        case EMPTY:
            os << "EMPTY";
            break;
        case GET: 
            os << "GET";
            break;
        case POST: 
            os << "POST";
            break;
        case PUT: 
            os << "PUT";
            break;
        case DELETE: 
            os << "DELETE";
            break;
        case Created:
            os << "Created";
            break;
        case Deleted:
            os << "Deleted";
            break;
        case Valid:
            os << "Valid";
            break;
        case Changed:
            os << "Changed";
            break;
        case Content:
            os << "Content";
            break;
        case Continue:
            os << "Continue";
            break;
        case BadRequest:
            os << "Bad Request";
            break;
        case Unauthorized:
            os << "Unauthorized";
            break;
        case BadOption:
            os << "Bad Option";
            break;
        case Forbidden:
            os << "Forbidden";
            break;
        case NotFound:
            os << "Not Found";
            break;
        case MethodNotAllowed:
            os << "Method Not Allowed";
            break;
        case NotAcceptable:
            os << "Not Acceptable";
            break;
        case RequestEntityIncomplete:
            os << "Request Entity Incomplete";
            break;
        case PreconditionFailed:
            os << "Precondition Failed";
            break;
        case RequestEntityTooLarge:
            os << "Request Entity Too Large";
            break;
        case UnsupportedContentFormat:
            os << "Unsupported Content Format";
            break;
        case InternalServerError:
            os << "Internal Server Error";
            break;
        case NotImplemented:
            os << "Not Implemented";
            break;
        case BadGateway:
            os << "Bad Gateway";
            break;
        case ServiceUnavailable:
            os << "Service Unavailable";
            break;
        case GatewayTimeout:
            os << "Gateway Timeout";
            break;
        case ProxyingNotSupported:
            os << "Proxying Not Supported";
            break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const CoapOptionDelta& rhs) 
{
    os << int(rhs) << " (";
    
    switch(rhs) {
        case IfMatch:
            os << "If-Match";
            break;
        case UriHost:
            os << "Uri-Host";
            break;
        case ETag:
            os << "ETag";
            break;
        case IfNoneMatch:
            os << "If-None-Match";
            break;
        case UriPort:
            os << "Uri-Port";
            break;
        case LocationPath:
            os << "Location-Path";
            break;
        case UriPath:
            os << "Uri-Path";
            break;
        case ContentFormat:
            os << "Content-Format";
            break;
        case MaxAge:
            os << "Max-Age";
            break;
        case UriQuery:
            os << "Uri-Query";
            break;
        case Accept:
            os << "Accept";
            break;
        case LocationQuery:
            os << "Location-Query";
            break;
        case Size2:
            os << "Size2";
            break;
        case ProxyUri:
            os << "Proxy-Uri";
            break;
        case ProxyScheme:
            os << "Proxy-Scheme";
            break;
        case Size1:
            os << "Size1";
            break;
    }

    os << ")";

    return os;
}

std::ostream& operator<<(std::ostream& os, const CoapContentFormat& rhs)
{
    switch(rhs) {
        case text:
            os << "text/plain; charset=utf-8";
            break;
        case linkFormat:
            os << "application/link-format";
            break;
        case xml:
            os << "application/xml";
            break;
        case octetStream:
            os << "application/octet-stream";
            break;
        case exi:
            os << "application/exi";
            break;
        case json:
            os << "application/json";
            break;
        case cbor:
            os << "application/cbor";
            break;
    }

    return os;
} 


std::ostream& operator<<(std::ostream& os, const CoapMessage::Option& rhs)
{
    std::string delimiter = ", ";

    os << "Delta: " << int(rhs.delta) << delimiter;
    os << "Length: " << int(rhs.length) << delimiter;
    os << "Value: ";
    
    switch(rhs.delta)
    {  
        case ContentFormat:
            if(rhs.value.empty()) os << CoapContentFormat::text;
            else {
                uint16_t val = rhs.value[0];

                if(rhs.value.size() > 1) val = val << 8 | rhs.value[1];

                os << static_cast<CoapContentFormat>(int(val));
            }
            break;
        case LocationPath:
        case UriPath:
            for(auto c: rhs.value) os << c;
            break;
        default:
            for(auto c: rhs.value) os << std::hex << (0xff & static_cast<int>(c)) << " ";
            break;
    }

    return os;
} 


std::ostream& operator<<(std::ostream& os, const CoapMessage& rhs) 
{    
    std::string delimiter = ", ";

    os << "\n----Coap Message----\n"; 
    os << "Version: " << int(rhs.version) << delimiter;
    os << "Type: " << rhs.type << delimiter;
    os << "Token Length: " << int(rhs.tokenLength) << delimiter; 
    os << "Code: " << rhs.code << delimiter;
    os << "Message Id: " << rhs.messageId;

    for(int i = 0; i < rhs.options.size(); i++) {
        const auto& option = rhs.options[i];
        os << "\nOption #" << i+1 << ": " << option;
    } 
    os << "\nPayload: " << rhs.payload << "\n";
    os << "--------------------\n";

    return os;
}


void CoapMessage::setOptionUriPath(const std::string& path) 
{
    Option option;
    option.delta = UriPath;
    option.length = path.size();

    for(const auto&c: path) option.value.push_back(c);

    options.push_back(option);
}

void CoapMessage::setPayload(const std::string& payload)
{
    this->payload = payload;
}

int CoapMessage::deserialize(const std::vector<char>& msg) 
{
    int byteIt = 0;

    version = (msg[byteIt] >> 6);
    type = static_cast<CoapType>((msg[byteIt] & 0b00110000) >> 4);
    tokenLength = msg[byteIt++] & 0x0f;
    code = static_cast<CoapCode>(msg[byteIt++]);
    messageId = (msg[byteIt] << 8) | (msg[byteIt+1] & 0x00ff);

    byteIt += 2 + tokenLength; //Increment 2 for message Id, skip tokens and go to option
    
    int prevDelta = 0;

    for(;byteIt != msg.size()-1; byteIt++){

        //Found payload, read payload and then we are done
        if((msg[byteIt] & 0xff) == 0xff) { 
            payload = "";
            while(byteIt++ != msg.size()-1){
                payload += char(msg[byteIt]);
            }            
            break;
        } else {
            Option option;
            option.delta = static_cast<CoapOptionDelta>(prevDelta + ((msg[byteIt] & 0xf0) >> 4));
            prevDelta = option.delta;
            option.length = msg[byteIt] & 0x0f;
            byteIt++;
            for(int i = 0; i < option.length; i++){
                option.value.push_back(msg[byteIt++]);
            }
            byteIt--;
            options.push_back(option);
        }
    }

    return 1;
}

std::vector<char> CoapMessage::serialize()
{
    std::vector<char> msg = {
        //Bitshift version and type to place them at the correct places in the byte, use the or-operator to append them together with the token length
        char((version << 6) | (type << 4) | tokenLength),
        char(code),
        //The message Id requires two bytes, add the 8 most significant bits to one byte and the 8 least significant to another
        char(messageId >> 8),  
        char(messageId & 0xff),
    };

    //Loop through all the options and append them to the message
    for(int i = 0; i < options.size(); i++) {
        const Option& option = options[i];
        //Combine the delta and length values to one byte
        msg.push_back(char(option.delta << 4 | option.length));
        //Go through every character in the value string and add them to the byte array
        for(auto c: option.value) msg.push_back(c);
    }

    //If there is a payload add the 8 1s
    if(!payload.empty()) msg.push_back(0xff);

    //Loop through the payload and add each character to the byte array
    for(int i = 0; i < payload.size(); i++) msg.push_back(payload[i]);

    return msg;
}

