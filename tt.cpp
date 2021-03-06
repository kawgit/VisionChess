#include "pos.h"
#include "bits.h"
#include "tt.h"
#include "search.h"

TTEntry* TT::probe(BB hashkey, bool& found) {
	TTEntry* entry = &table[hashkey & HASHMASK];
	found = entry->matches(hashkey);
	return entry;
}

void TT::clear() {
	gen = 0;
	for (int i = 0; i < TABLESIZE; i++) {
		table[i].set_zero();
	}
}

vector<Move> TT::getPV(Pos p) {
	vector<Move> pv;
	addPV(p, pv);
	return pv;
}

int TT::hashfull() {
	int count = 0;
	for (int i = 0; i < 1000; i++) {
		bool found = false;
		if ((probe(rand_BB(), found)->get_hashkey32()) != 0) count++;
	}
	return count;
}

void TT::addPV(Pos& p, vector<Move>& pv) {
	bool found = false;
	TTEntry* entry = probe(p.hashkey, found);
	if (found && (entry->get_bound() != LB || entry->get_eval() >= MINMATE) && entry->get_move() != MOVE_NONE) {
		p.do_move(entry->get_move());
		pv.push_back(entry->get_move());
		if (!p.is_over()) addPV(p, pv);
		p.undo_move();
	}
}