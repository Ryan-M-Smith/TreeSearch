//
// FILENAME: successstates.h | Shifting Stones Search
// DESCRIPTION: Determine if a given board is a success state for a given target card
// CREATED: 2024-03-30 @ 7:11 PM
//

#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "decl.h"

namespace success_states {

	const static std::string& getID(board_t board) {
		static std::string id = "";
		id.reserve(9);

		for (int i = 24; i >= 0; i -= 3) {
			id.push_back(((board & 0b111 << i) >> i) + '0');
		}

		return id;
	}
	
	bool isSuccessState(board_t board, const std::string& target);
	bool matchesSuccessState(board_t board, const std::string& state);

	const std::unordered_map<std::string, std::vector<std::string>> SUCCESS_STATES = {
		{"888548888", {"888548888", "888854888", "548888888", "854888888", "888888548", "888888854"}},
		{"888854888", {"888548888", "888854888", "548888888", "854888888", "888888548", "888888854"}},
		{"548888888", {"888548888", "888854888", "548888888", "854888888", "888888548", "888888854"}},
		{"854888888", {"888548888", "888854888", "548888888", "854888888", "888888548", "888888854"}},
		{"888888548", {"888548888", "888854888", "548888888", "854888888", "888888548", "888888854"}},
		{"888888854", {"888548888", "888854888", "548888888", "854888888", "888888548", "888888854"}},
		{"498928888", {"498928888", "849892888", "888498928", "888894892"}},
		{"849892888", {"498928888", "849892888", "888498928", "888894892"}},
		{"888498928", {"498928888", "849892888", "888498928", "888894892"}},
		{"888894892", {"498928888", "849892888", "888498928", "888894892"}},
		{"858828888", {"858828888", "588288888", "888885882", "888858828", "888588288", "885882888"}},
		{"588288888", {"858828888", "588288888", "888885882", "888858828", "888588288", "885882888"}},
		{"888885882", {"858828888", "588288888", "888885882", "888858828", "888588288", "885882888"}},
		{"888858828", {"858828888", "588288888", "888885882", "888858828", "888588288", "885882888"}},
		{"888588288", {"858828888", "588288888", "888885882", "888858828", "888588288", "885882888"}},
		{"885882888", {"858828888", "588288888", "888885882", "888858828", "888588288", "885882888"}},
		{"999999991", {"999999991"}},
		{"969494919", {"969494919"}},
		{"799999992", {"799999992"}},
		{"999939999", {"999939999"}},
		{"969999696", {"969999696"}},
		{"909999393", {"909999393"}},
		{"828878848", {"828878848", "288788488", "882887884"}},
		{"288788488", {"828878848", "288788488", "882887884"}},
		{"882887884", {"828878848", "288788488", "882887884"}},
		{"878838888", {"878838888", "788388888", "887883888", "888878838", "888788388", "888887883"}},
		{"788388888", {"878838888", "788388888", "887883888", "888878838", "888788388", "888887883"}},
		{"887883888", {"878838888", "788388888", "887883888", "888878838", "888788388", "888887883"}},
		{"888878838", {"878838888", "788388888", "887883888", "888878838", "888788388", "888887883"}},
		{"888788388", {"878838888", "788388888", "887883888", "888878838", "888788388", "888887883"}},
		{"888887883", {"878838888", "788388888", "887883888", "888878838", "888788388", "888887883"}},
		{"858838888", {"858838888", "588388888", "885883888", "888858838", "888588388", "888885883"}},
		{"588388888", {"858838888", "588388888", "885883888", "888858838", "888588388", "888885883"}},
		{"885883888", {"858838888", "588388888", "885883888", "888858838", "888588388", "888885883"}},
		{"888858838", {"858838888", "588388888", "885883888", "888858838", "888588388", "888885883"}},
		{"888588388", {"858838888", "588388888", "885883888", "888858838", "888588388", "888885883"}},
		{"888885883", {"858838888", "588388888", "885883888", "888858838", "888588388", "888885883"}},
		{"888595888", {"888595888", "595888888", "888888595"}},
		{"595888888", {"888595888", "595888888", "888888595"}},
		{"888888595", {"888595888", "595888888", "888888595"}},
		{"978398888", {"978398888", "897839888", "888897839", "888978398"}},
		{"897839888", {"978398888", "897839888", "888897839", "888978398"}},
		{"888897839", {"978398888", "897839888", "888897839", "888978398"}},
		{"888978398", {"978398888", "897839888", "888897839", "888978398"}},
		{"968598888", {"968598888", "896859888", "888968598", "888896859"}},
		{"896859888", {"968598888", "896859888", "888968598", "888896859"}},
		{"888968598", {"968598888", "896859888", "888968598", "888896859"}},
		{"888896859", {"968598888", "896859888", "888968598", "888896859"}},
		{"798078888", {"798078888", "879807888", "888798078", "888879807"}},
		{"879807888", {"798078888", "879807888", "888798078", "888879807"}},
		{"888798078", {"798078888", "879807888", "888798078", "888879807"}},
		{"888879807", {"798078888", "879807888", "888798078", "888879807"}},
		{"888628888", {"888628888", "628888888", "888888628", "888888862", "888862888", "862888888"}},
		{"628888888", {"888628888", "628888888", "888888628", "888888862", "888862888", "862888888"}},
		{"888888628", {"888628888", "628888888", "888888628", "888888862", "888862888", "862888888"}},
		{"888888862", {"888628888", "628888888", "888888628", "888888862", "888862888", "862888888"}},
		{"888862888", {"888628888", "628888888", "888888628", "888888862", "888862888", "862888888"}},
		{"862888888", {"888628888", "628888888", "888888628", "888888862", "888862888", "862888888"}},
		{"888494888", {"888494888", "494888888", "888888494"}},
		{"494888888", {"888494888", "494888888", "888888494"}},
		{"888888494", {"888494888", "494888888", "888888494"}},
		{"838838888", {"838838888", "388388888", "883883888", "888838838", "888883883", "888388388"}},
		{"388388888", {"838838888", "388388888", "883883888", "888838838", "888883883", "888388388"}},
		{"883883888", {"838838888", "388388888", "883883888", "888838838", "888883883", "888388388"}},
		{"888838838", {"838838888", "388388888", "883883888", "888838838", "888883883", "888388388"}},
		{"888883883", {"838838888", "388388888", "883883888", "888838838", "888883883", "888388388"}},
		{"888388388", {"838838888", "388388888", "883883888", "888838838", "888883883", "888388388"}},
		{"818868888", {"818868888", "188688888", "881886888", "888818868", "888188688", "888881886"}},
		{"188688888", {"818868888", "188688888", "881886888", "888818868", "888188688", "888881886"}},
		{"881886888", {"818868888", "188688888", "881886888", "888818868", "888188688", "888881886"}},
		{"888818868", {"818868888", "188688888", "881886888", "888818868", "888188688", "888881886"}},
		{"888188688", {"818868888", "188688888", "881886888", "888818868", "888188688", "888881886"}},
		{"888881886", {"818868888", "188688888", "881886888", "888818868", "888188688", "888881886"}},
		{"888725888", {"888725888", "888888725", "725888888"}},
		{"888888725", {"888725888", "888888725", "725888888"}},
		{"725888888", {"888725888", "888888725", "725888888"}},
		{"878848888", {"878848888"}},
		{"878868888", {"878868888"}},
		{"848858888", {"848858888"}},
		{"888796888", {"888796888"}},
		{"888697888", {"888697888"}},
		{"798948888", {"798948888"}},
		{"888768888", {"888768888"}},
		{"888278888", {"888278888"}},
		{"878878878", {"878878878"}},
		{"858858888", {"858858888"}},
		{"888518888", {"888518888"}},
		{"888048888", {"888048888"}},
		{"898898818", {"898898818"}},
		{"888990888", {"888990888"}},
		{"979292959", {"979292959"}},
		{"888658888", {"888658888"}},
		{"888258888", {"888258888"}},
		{"888228888", {"888228888"}},
		{"838868888", {"838868888"}},
		{"868878888", {"868878888"}},
		{"868868888", {"868868888"}},
		{"858848888", {"858848888"}},
		{"828898878", {"828898878"}},
		{"828848888", {"828848888"}},
		{"888448888", {"888448888"}},
		{"878808888", {"878808888"}},
		{"888348888", {"888348888"}},
		{"888748888", {"888748888"}},
		{"888943888", {"888943888"}},
		{"888778888", {"888778888"}},
		{"828898828", {"828898828"}},
		{"888666888", {"888666888"}},
		{"199929992", {"199929992"}},
		{"888678888", {"888678888"}},
		{"868898838", {"868898838"}},
		{"888777888", {"888777888"}},
		{"858868888", {"858868888"}},
		{"888393888", {"888393888"}},
		{"888436888", {"888436888"}},
		{"958518888", {"958518888"}},
		{"996999699", {"996999699"}},
		{"858858858", {"858858858"}},
		{"497999695", {"497999695"}},
		{"499999994", {"499999994"}},
		{"808898898", {"808898898"}},
		{"993999599", {"993999599"}},
		{"999929999", {"999929999"}},
		{"888199888", {"888199888"}},
		{"099999999", {"099999999"}},
		{"888458888", {"888458888"}},
	};
}