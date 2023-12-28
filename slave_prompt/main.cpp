#include "slave_prompt.h"

int main(int argc, char** argv)
{
	SlavePromptApp app = SlavePromptApp("SlavePromptApp");
	app.init(argc, argv);
	app.run();
	return 0;
}