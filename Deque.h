#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include <deque>
#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty, typename _Alloc = _STD allocator<_Ty>>
class deque : _STD deque<_Ty, _Alloc> {};



// This is a class optimized specifically for use in huffman encoding
// See: Huff.h & Huff.cpp
template <typename _Alloc>
class deque<bool, _Alloc> {
	typedef _NSTD uint  uint;
	typedef _NSTD uchar uchar;
public:
	class _Bool_linkage {
		uint relative_pos_;
		uchar* char_;
	public:
		_Bool_linkage(uchar* c, uint rp) : relative_pos_(rp), char_(c)  {}
		_Bool_linkage(uchar& c, uint rp) : relative_pos_(rp), char_(&c) {}
		_Bool_linkage(_Bool_linkage& other) : relative_pos_(other.relative_pos_), char_(other.char_) {}
		// funcs
		_Bool_linkage& set(bool b) {
			b ? *char_ |=  _BITX(relative_pos_)
			  : *char_ &= ~_BITX(relative_pos_);
			return *this;
		}
		// ops
		operator bool() const { 
			return *char_ & _BITX(relative_pos_);
		}
		bool operator* () {
			return static_cast<bool>(*this);
		}
		// One cannot access internal bools via classic pointers
		// This class should in itself be used as a pointer
		bool* operator& () = delete;
		_Bool_linkage& operator= (bool b) {
			return set(b);
		}
		_Bool_linkage& operator= (const _Bool_linkage& other) {
			relative_pos_ = other.relative_pos_;
			char_         = other.char_;
		}
		// Unchecked
		_Bool_linkage& operator+ (uint size_jump) {
			if (relative_pos_ += size_jump >= CHAR_BIT) {
				char_ += (relative_pos_ / CHAR_BIT);
				relative_pos_ %= CHAR_BIT;
			}
			return *this;
		}
		// Unchecked
		_Bool_linkage& operator- (uint size_jump) {
			if ((relative_pos_ - size_jump) & (uint(1) << (sizeof(uint) * CHAR_BIT - 1))) /* check for overflow */ {
				char_ -= (size_jump - relative_pos_) / CHAR_BIT;
				relative_pos_ = (size_jump - relative_pos_) % CHAR_BIT;
			} else {
				relative_pos_ -= size_jump;
			}
			return *this;
		}
		_Bool_linkage& operator++() {
			return *this + 1;
		}
		_Bool_linkage& operator--() {
			return *this - 1;
		}
	};
private:
	uchar* carr_;
	uint carr_sz_, Hpos_, Tpos_;

// ripped
	using _Alty           = _STD _Rebind_alloc_t<_Alloc, bool>;
	using _Alty_traits    = _STD allocator_traits<_Alty>;
	using _Alpty          = _STD _Rebind_alloc_t<_Alloc, typename _Alty_traits::pointer>;
	using _Alpty_traits   = _STD allocator_traits<_Alpty>;
	using _Mapptr         = typename _Alpty_traits::pointer;
	using _Alproxy_ty     = _STD _Rebind_alloc_t<_Alty, _STD _Container_proxy>;
	using _Alproxy_traits = _STD allocator_traits<_Alproxy_ty>;

	using _Scary_val = _STD _Deque_val<_STD conditional_t<_STD _Is_simple_alloc_v<_Alty>, _STD _Deque_simple_types<bool>,
		_STD _Deque_iter_types<bool, typename _Alty_traits::size_type, typename _Alty_traits::difference_type,
			typename _Alty_traits::pointer, typename _Alty_traits::const_pointer, _Bool_linkage&, const _Bool_linkage&, _Mapptr>>>;

	static constexpr int _Block_size = _Scary_val::_Block_size;

public:
	using allocator_type  = _Alloc;
	using value_type      = bool;
	using size_type       = typename _Alty_traits::size_type;
	using difference_type = typename _Alty_traits::difference_type;
	using pointer         = typename _Alty_traits::pointer;
	using const_pointer   = typename _Alty_traits::const_pointer;
	using reference       = _Bool_linkage;
	using const_reference = const _Bool_linkage;

	using iterator                  = _STD _Deque_iterator<_Scary_val>;
	using const_iterator            = _STD _Deque_const_iterator<_Scary_val>;
	using _Unchecked_iterator       = _STD _Deque_unchecked_iterator<_Scary_val>;
	using _Unchecked_const_iterator = _STD _Deque_unchecked_const_iterator<_Scary_val>;

	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

// end_ripped

public:
	deque(uint size = 0xff)   : carr_(new uchar[size / CHAR_BIT + 1]), carr_sz_(size / CHAR_BIT + 1), Hpos_(carr_sz_ / 2), Tpos_(Hpos_ - 1) {}
	deque(const deque& other) : carr_(new uchar[other.carr_sz_]),      carr_sz_(other.carr_sz_),      Hpos_(other.Hpos_),  Tpos_(other.Tpos_) {
		_NSTD_FOR_I(carr_sz_)
			carr_[i] = other.carr_[i];
	}


	// This is a utility.
	// Note: the caller must call delete[] on generated array themselves
	_NODISCARD static uchar* gen_bitX() {
		uchar* nbitX(new uchar[CHAR_BIT]);
		_NSTD_FOR_I(CHAR_BIT)
			nbitX[i] = _BITX(i);
		return nbitX;
	}
	
/*  // Debugging
	uchar*& get_carr_() {
		return carr_;
	}
	uchar*& get_bitX_() {
		return bitX_;
	}
	const uint& get_Hpos_() {
		return Hpos_;
	}
	const uint& get_Tpos_() {
		return Tpos_;
	}
*/


	_NODISCARD uint size() {
		// inclusive
		return Tpos_ - Hpos_ + 1;
	}
	_NODISCARD uint real_size() {
		return carr_sz_;
	}
	_NODISCARD uint potential_size() {
		return carr_sz_ * CHAR_BIT - 1;
	}
	_NODISCARD bool empty() {
		return size() == 0;
	}
	// Maintains size
	void clear() {
		Tpos_ = 0;
		Hpos_ = 1;
	}
	// Copies contents of old deque into new deque (maintains position)
	// Note: expands backwards
	void resize(uint num_bools, bool init = false) {
		uint real_sz = static_cast<uint> (num_bools / CHAR_BIT) + 1;
		uchar* cashe(carr_);
		carr_ = init ? new uchar[real_sz](0xff) : new uchar[real_sz](0x00);
		if (real_sz >= carr_sz_) {
			_NSTD_FOR_I(carr_sz_)
				carr_[i] = cashe[i];
		} else /* truncation */ {
			_NSTD_FOR_I(real_sz)
				carr_[i] = cashe[i];
			Tpos_ = num_bools + 1;
		}
		delete[] cashe;
	}
	void resize_NOCOPY(uint num_bools, bool init = false) {
		delete[] carr_;
		carr_ = init ? new uchar[num_bools](0xff) : new uchar[num_bools](0x00);
		clear();
	}
	_NODISCARD reference front() {
		_NSTD_ASSERT(!empty(), "called front() on empty deque");
		return _Bool_linkage(carr_[static_cast<uint> (Hpos_ / CHAR_BIT)], Hpos_ % CHAR_BIT);
	}
	_NODISCARD const_reference front() const {
		return front();
	}
	reference pop_front() {
		_Bool_linkage out(front());
		++Hpos_;
		return out;
	}
	_NODISCARD reference back() {
		_NSTD_ASSERT(!empty(), "called back() on empty deque");
		return _Bool_linkage(carr_[static_cast<uint> (Tpos_ / CHAR_BIT)], Tpos_ % CHAR_BIT);
	}
	_NODISCARD const_reference back() const {
		return back();
	}
	reference pop_back() {
		_Bool_linkage out(back());
		--Tpos_;
		return out;
	}
	// Sum of shrink_front & shrink_back
	deque& shrink_fit() {
		return shrink_front().shrink_back();
	}
	deque& shrink_front() {
		if (static_cast<uint> (Hpos_ / CHAR_BIT)) {
			uint high_pos(static_cast<uint> (Hpos_ / CHAR_BIT));
			carr_sz_ -= high_pos;
			uchar* narr(new uchar[carr_sz_]);
			_NSTD_FOR_I(carr_sz_)
				narr[i] = carr_[high_pos + i];
			delete[] carr_;
			carr_ = narr;
		}
		return *this;
	}
	deque& shrink_back() {
		if (static_cast<uint> (Tpos_ / CHAR_BIT + 1) < carr_sz_) {
			carr_sz_ = static_cast<uint> (Tpos_ / CHAR_BIT + 1);
			uchar* narr(new uchar[carr_sz_]);
			_NSTD_FOR_I(carr_sz_)
				narr[i] = carr_[i];
			delete[] carr_;
			carr_ = narr;
		}
		return *this;
	}
	deque& push_back(bool b) {
		if (static_cast<uint> (++Tpos_ / CHAR_BIT) >= carr_sz_) {
			uchar* cashe(carr_);
			carr_ = new uchar[carr_sz_ *= 2](0);
			_NSTD_FOR_I(carr_sz_ / 2)
				carr_[i] = cashe[i];
			delete[] cashe;
		}
		set(Tpos_ - Hpos_, b);
		return *this;
	}
	// TODO: debug -- set(0,...) fails after several push fronts
	deque& push_front(bool b) {
		if (!--Hpos_) {
			uchar* cashe(carr_);
			carr_ = new uchar[carr_sz_ * 2](0);
			_NSTD_FOR_I(carr_sz_)
				carr_[i + carr_sz_] = cashe[i];
			delete[] cashe;
			Hpos_ += carr_sz_;
			Tpos_ += carr_sz_;
			carr_sz_ *= 2;
		}
		set(0, b);
		return *this;
	}

	// at(pos)
	_Bool_linkage operator[] (uint pos) {
		return at(pos);
	}
	_Bool_linkage at(uint pos) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		return _Bool_linkage(carr_[static_cast<uint> (real_pos / CHAR_BIT)], real_pos % CHAR_BIT);
	}
	// set(pos, !at(pos))
	deque& flip(uint pos) {
		return set(pos, !at(pos));
	}
	deque& set(uint pos, bool b = true) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		b ? carr_[static_cast<uint> (real_pos / CHAR_BIT)] |=  _BITX(real_pos % CHAR_BIT) 
		  : carr_[static_cast<uint> (real_pos / CHAR_BIT)] &= ~_BITX(real_pos % CHAR_BIT);
		return *this;
	}

	// vvv Methods to match std::deque vvv
	deque& operator=(const deque& other) {

	}
	deque& operator=(deque&& other) noexcept {

	}
	deque& operator=(std::initializer_list<bool> ilist) {

	}







public:
	~deque() { delete[] carr_; }
};

_NSTD_END
#endif // !_NSTD_DEQUE_