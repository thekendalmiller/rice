#include <rice/rice.hpp>
#include <ruby/version.h>

void embed_ruby()
{
  static bool initialized__ = false;

  if (!initialized__)
  {
    int argc = 0;
    char* argv = nullptr;
    char** pArgv = &argv;

    ruby_sysinit(&argc, &pArgv);
    ruby_init();
    ruby_init_loadpath();

#if RUBY_API_VERSION_MAJOR == 3 && RUBY_API_VERSION_MINOR >= 3
    // Force the prelude / builtins
    char *opts[] = { "ruby", "-e;" };
    ruby_options(2, opts);
#endif

    initialized__ = true;
  }
}
