#include "test_app.h"

int main(int argc, char** argv)
{
	TestApp app = TestApp("ComNucleoApp");
	app.init(argc, argv);
	app.run();
	return 0;
}