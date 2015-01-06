#include "brick.h"
#include <cstdlib>
#include <ctime>

static const Uint32 colors[] = {
  0x0000FF00, // red
  0x00FFFF00, // yellow
  0x00FF0000, // green
  0x0000FFFF, // cyan
  0x000000FF, // blue
  0x00FF00FF  // magenta
};

static int seed = 0;
static int count = 0;
//static bool initialized = false;

brick::brick() {
  //if (!initialized) {
  //  seed = (int)time(0) % 10000; // random enough =)
  //  srand(seed);
  //}
	hit = false;
	//color = colors[rand() % (sizeof colors / sizeof colors[0])];

	color = colors[seed];
  seed++;
  if (seed > 5) {
    seed = 0;
  }

	/*color = colors[seed];
	count++;
	if (count >= 9) {
    count = 0;
    seed++;
	}*/
}

brick::~brick() {

}

