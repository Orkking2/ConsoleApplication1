#include "Huff.h"
#ifdef _NSTD_HUFF_


_NSTD_BEGIN

// HuffTree::Node:: methods
_STD vector<void*> HuffTree::Node::vpArr() const {
	_STD vector<void*> l(l_->vpArr());
	_STD vector<void*> r(r_->vpArr());
	l.insert(l.end(), r.begin(), r.end());
	return l;
}

// HuffTree:: methods
void HuffTree::release_ptrs_NODELETE() {
	ptrs_.~vector();
	ptrs_set_ = false;
}

void HuffTree::create_tree(_STD vector<_STD pair<void*, uint>> vals) {
	create_tree_NOREVERSE(vals);
	for (void* p : ptrs_)
		map_[p] = _STD vector<bool>(map_[p].rbegin(), map_[p].rend());
}

void HuffTree::create_tree_NOREVERSE(_STD vector<_STD pair<void*, uint>> vals) {
	_NSTD_ASSERT(!ptrs_set_, "HuffTree pointers must be released before another tree is constructed using the same HuffTree object");
	// Initialize val set
	ptrs_.resize(vals.size(), NULL);
	// Disallow recreation of HuffTree object while void*s have not been released
	ptrs_set_ = true;
	_STD vector<IElement*> elements;
	_NSTD_FOR_I(vals.size()) {
		if (vals[i].second == 0) {
			// No void* should have freq 0 -> any with freq 0 are pruned
			vals.erase(vals.begin() + i);
			ptrs_.erase(ptrs_.begin() + i);
			i--;
		} else {
			ptrs_[i] = vals[i].first;
			elements.push_back(new vPtr(vals[i]));
		}
	}

	// If size is one, you should just send the object and a count
	// This is to handle exceptions, it is always suboptimal
	if (elements.size() == 1) {
		elements[0] = new Node(elements[0], NULL);
		map_[vals[0].first].push_back(false);
	}

	while (elements.size() > 1) {
		IElement* low_cashe, * high_cashe;
		// Lowest freq
		uint index = 0, freq = elements[index]->GetFreq();
		_NSTD_FOR_I(elements.size()) {
			if (elements[i]->GetFreq() < freq) 
				freq = elements[index = i]->GetFreq();
		}
		low_cashe = elements[index];
		elements.erase(elements.begin() + index);
		// Note that the map_[index] vectors are reversed
		for (void* p : low_cashe->vpArr())
			map_[p].push_back(false);


		// Second lowest freq
		index = 0, freq = elements[index]->GetFreq();
		_NSTD_FOR_I(elements.size()) {
			if (elements[i]->GetFreq() < freq)
				freq = elements[index = i]->GetFreq();
		}
		high_cashe = elements[index];
		elements.erase(elements.begin() + index);
		// Note that the map_[index] vectors are reversed
		for (void* p : high_cashe->vpArr())
			map_[p].push_back(true);


		elements.push_back(new Node(low_cashe, high_cashe));
	}

	Fin_node_ = elements[0];
}

//void HuffTree::vPtr::operator= (const IElement*& r) {
//	vPtr* p = reinterpret_cast<vPtr*>(const_cast<IElement*>(r));
//	ptr_ = p->ptr_;
//	freq_ = p->freq_;
//}

_NSTD_END
#else // ^^^^ _NSTD_HUFF_ / !_NSTD_HUFF_ vvvv
#pragma once(message, "Huff.h not found")
#endif 