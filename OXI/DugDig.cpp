#include "GL.h"
#include "Util.h"
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "DugDig.h"

//std::unordered_map<s

struct SpriteData {
	std::vector<Rect> player_dig, player_move, player_inflate, player_dead, player_extend, player_squash_h, player_squash_v,
		tether_attached, tether_extend;

	//  there are many other sprites but im lazy

} data;

static void loadSprites() {
	FILE *f = fopen("sprites.csv", "r");

	if (!f)
		throw std::runtime_error("Unable to open sprites.csv");

	char entity[16] = "";
	char state[16] = "";
	int x = 0, y = 0, w = 0, h = 0;

	int lineNum = 0;
	char line[128] = "";
	while (fgets(line, sizeof(line), f)) {
		lineNum++;

		int res;
		if ((res = sscanf(line, "%15s %15s %d %d %d %d\n",
			&entity,
			&state,
			&x,
			&y,
			&w,
			&h)) < 6)
			continue;
			//throw std::runtime_error(std::string("Failed to parse sprites.csv at line ") + std::to_string(lineNum));

		Rect rect{ x, y, w, h };
		// add data to game
		if (!strcmp(entity, "player")) {
			if (!strcmp(state, "dig")) {
				data.player_dig.emplace_back(rect);
			}
			else if (!strcmp(state, "move")) {
				data.player_move.emplace_back(rect);
			}
			else if (!strcmp(state, "inflate")) {
				data.player_inflate.emplace_back(rect);
			}
			else if (!strcmp(state, "dead")) {
				data.player_dead.emplace_back(rect);
			}
			else if (!strcmp(state, "squash_h")) {
				data.player_squash_h.emplace_back(rect);
			}
			else if (!strcmp(state, "squash_v")) {
				data.player_squash_v.emplace_back(rect);
			}
			else if (!strcmp(state, "extend")) {
				data.player_extend.emplace_back(rect);
			}
		}
		else if (!strcmp(entity, "tether")) {
			//data.te.push_back({ x, y, w, h });
		}
	}

	//throw std::runtime_error("Unable to read sprites.csv (fscanf_s EOF)")

	fclose(f);
}

void run() {
	DD_glInit();

	loadSprites();

	DD_glUnInit();
}