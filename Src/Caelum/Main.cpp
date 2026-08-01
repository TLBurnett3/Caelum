#include <HipparcosLoader.h>

int main(void)
{
std::string     filepath = "E:\\Stars\\Hipparcos\\main";
HipparcosLoader hipparcosLoader(filepath);
StarCatalog     starCatalog;
bool            success(false);

  if (hipparcosLoader.loadCatalog(starCatalog) == 0)
  {
    success = true;
  }

  return success ? 0 : 1;
}


