#ifndef EXIT_SERVER_COMMAND
#define EXIT_SERVER_COMMAND

#include "BaseServerCommand.h"

class ExitServerCommand : public BaseServerCommand
{
public:
  ExitServerCommand();
private:
  virtual bool handleImpl(const std::string & args) override;
};

#endif
