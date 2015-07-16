/**
 * @brief: ettd.cc
 * @date : Jun 21, 2015
 * @author: Peizun Liu
 */

#include "ettd.hh"

namespace sura {

/// Global ID: used to defined the transitions' id across all scc
id_tran Transition::ID = 0;

matrix ETTD::real_R;
matrix ETTD::spaw_R;
matrix ETTD::expd_R;

ETTD::ETTD(const adj_list& TTD, const vector<inout>& s_in_out) :
		V(mapping_TS.size()), expanded_TTD(TTD) {
	this->build_real_R();
	this->build_ETTD(TTD, s_in_out);
}

ETTD::~ETTD() {
}

/**
 * @brief return expanded TTD
 * @return
 */
const adj_list& ETTD::get_expanded_TTD() const {
	return expanded_TTD;
}

/**
 * @brief return the number of vertices
 * @return
 */
size_V ETTD::get_V() const {
	return V;
}
/**
 * @TODO do this at the beginning
 * @brief mark real transitions
 */
void ETTD::build_real_R() {
	real_R = vector<vector<bool>>(V, vector<bool>(V, false));
	for (auto iu = original_TTD.begin(); iu != original_TTD.end(); ++iu) {
		for (auto iv = iu->second.begin(); iv != iu->second.end(); ++iv) {
			real_R[iu->first][*iv] = true;
		}
	}

	spaw_R = vector<vector<bool>>(V, vector<bool>(V, false));
	for (auto iu = spawntra_TTD.begin(); iu != spawntra_TTD.end(); ++iu) {
		for (auto iv = iu->second.begin(); iv != iu->second.end(); ++iv) {
			real_R[iu->first][*iv] = true;
		}
	}
}

/**
 * @brief build expanded TTD
 * @param TTD: original TTD, represented by adjacency list
 */
void ETTD::build_ETTD(const adj_list& TTD, const vector<inout>& s_in_out) {
	expd_R = vector<vector<bool>>(V, vector<bool>(V, false));
	for (auto s = 0; s < s_in_out.size(); ++s) {
		const auto& in_out = s_in_out[s];
		for (auto iv = in_out.first.begin(); iv != in_out.first.end(); ++iv)
			for (auto iw = in_out.second.begin(); iw != in_out.second.end();
					++iw)
				if (*iv != *iw)
					if (!this->is_real_trans(*iv, *iw)) {
						expanded_TTD[*iv].push_back(*iw); /// add expansion edge
						expd_R[*iv][*iw] = true;
					}
	}
}

/**
 * @brief determine if (u,v) corresponds to a real transition
 * @param u
 * @param v
 * @return bool
 * 			true : if (u, v) is a real edge
 * 			false: otherwise
 */
bool ETTD::is_real_trans(const vertex& u, const vertex& v) {
	return real_R[u][v];
}

/**
 * @brief determine if u.shared = v.shared
 * @param u
 * @param v
 * @return bool
 * 			true : if u.shared = v.shared
 * 			false: otherwise
 */
bool ETTD::is_horizontal(const vertex& u, const vertex& v) {
	return mapping_TS[u].share == mapping_TS[v].share;
}

/////////////////////////////////for test//////////////////////////////////////

void ETTD::print_expanded_TTD() {
	cout << "The expanded TTD:\n";
	for (auto isrc = expanded_TTD.begin(); isrc != expanded_TTD.end(); ++isrc)
		for (auto idst = isrc->second.begin(); idst != isrc->second.end();
				++idst)
			cout << mapping_TS[isrc->first] << " -> " << mapping_TS[*idst]
					<< "\n";
	cout << endl;
}

void ETTD::print_transitions() {
	for (auto i = 0; i < V; ++i)
		for (auto j = 0; j < V; ++j)
			if (real_R[i][j])
				cout << mapping_TS[i] << " -> " << mapping_TS[j] << "\n";
			else if (expd_R[i][j])
				cout << mapping_TS[i] << " :> " << mapping_TS[j] << "\n";
			else if (spaw_R[i][j])
				cout << mapping_TS[i] << " +> " << mapping_TS[j] << "\n";
	cout << endl;
}

}
/* namespace SURA */
