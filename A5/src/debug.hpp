
//#define MY_DEBUG
#ifdef MY_DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define NDEBUG
#define DEBUG_MSG(str) do { } while ( false )
#endif

// less verbose
#define PRINT_STATUS
#ifdef PRINT_STATUS
#define STATUS_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define STATUS_MSG(str) do { } while ( false )
#endif


#include <assert.h>
