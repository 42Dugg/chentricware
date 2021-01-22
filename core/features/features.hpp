#pragma once
#include "../../dependencies/utilities/csgo.hpp"


namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void edge_jump(c_usercmd* cmd);
		void skate(c_usercmd* cmd);
		void strafer(c_usercmd* user_cmd);
		void movement_related(c_usercmd* cmd);
	};

	namespace spam {
		void clan_tag();
		void chat_spam();
		void spam_related();
	};
}

namespace c_backtracking
{
	void RegisterTick(c_usercmd* cmd);
	void Begin(c_usercmd* cmd);
	void End();

}
