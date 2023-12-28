#include "evaluation_board_manager.h"

int main(int argc, char** argv)
{
	EvaMan app = EvaMan("EvaMan");
	app.init(argc, argv);
	app.run();
	return 0;
}