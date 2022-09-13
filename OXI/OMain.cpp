#ifdef _TINSPIRE
#include <libndls.h>
#endif

#include <stdexcept>

#include "DugDig.h"

// so this starts --> 
// load sprites, (must fix texture drawing/make sure works correctly)
// draw some sprites to test
int main(void)
{

	// throw when required textures arent found
	// or 

	// exceptions end up not getting caught gracefully when not using try/catch,
	// which wont allow allocated memory to be freed (like in smart ptrs)
	try {
		run();
	}
	catch (const std::exception& e) {
		printf("Error: %s\n", e.what());
#ifdef _TINSPIRE
		show_msgbox("OXI Error", e.what());
#endif
	}

	return 0;
}