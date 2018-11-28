#include "pch.h"

#include "Poco/JSON/Parser.h"

#include "NetworkData.h"


namespace PocoChatGeneral
{
  void NetworkData::parse(const std::string& data)
  {
    parseImpl(PartialParse(data));
  }

  Poco::Dynamic::Var NetworkData::PartialParse(const std::string& data)
  {
    Poco::JSON::Parser parser;
    return parser.parse(data);
  }

  void NetworkData::generate(const std::string& data, std::ostream& out) const
  {
    generateImpl(data, out);
  }
}
