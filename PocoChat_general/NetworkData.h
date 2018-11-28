#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include <iosfwd>
#include <string>

namespace Poco
{
  namespace Dynamic
  {
    class Var;
  }
}

namespace PocoChatGeneral
{
  class NetworkData
  {
  public:
    NetworkData() = default;
    NetworkData(const NetworkData&) = default;
    NetworkData(NetworkData&&) = default;
    NetworkData& operator=(const NetworkData&) = default;
    NetworkData& operator=(NetworkData&&) = default;
    virtual ~NetworkData() = default;

    void parse(const std::string& data);
    void generate(const std::string& data, std::ostream& out) const;
  protected:
    static Poco::Dynamic::Var PartialParse(const std::string& data);
  private:
    virtual void parseImpl(const Poco::Dynamic::Var& data) = 0;
    virtual void generateImpl(const std::string& data, std::ostream& out) const = 0;
  };
}

#endif
