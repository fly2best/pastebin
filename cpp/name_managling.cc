
#include <string>


//g++ -c name_manageling.cc
//nm name_namageling.o
//nm name_namageling.o | c++filt

namespace wikipedia
{
    class article
    {
        public:
            std::string format (void);
            /* = _ZN9wikipedia7article6formatEv */

            bool print_to (std::ostream&);
            /* = _ZN9wikipedia7article8print_toERSo */

            class wikilink
            {
                public:
                    wikilink (std::string const& name);
                    /* = _ZN9wikipedia7article8wikilinkC1ERKSs */
            };
    };
}


void test()
{
   using namespace wikipedia;

   article a;

   a.format();

   article::wikilink aw("abc");
}


