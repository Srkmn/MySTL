#pragma once

#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;
enum reserve_type
{
	// Expansion mechanism, constant, 1.5 times, 2 times.
	CONSTAND,
	NORMAL,
	HIGH
};

template<typename T, reserve_type retype = reserve_type::NORMAL>
class myvector
{
	// Define basic member variables
public:
	// Define data types, iterators, and references
	typedef T value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef value_type& reference;
	typedef const value_type& const_reference;


private:
	// Declaring Member Variables
	iterator _start;
	iterator _end;
	iterator _endstroage;

public:

	myvector() // Default constructor
		:_start(nullptr)
		, _end(nullptr)
		, _endstroage(nullptr)
	{}

	myvector(const myvector<value_type>& v) // copy constructor
	{ 
		size_t v_sz = v.size();
		_start = new value_type[v_sz];
		copy(v.const_begin(), v.const_end(), std::begin(*this));
		_end = _start + v_sz;
		_endstroage = v_sz + _start;
	}

	myvector(const std::initializer_list<value_type>& v) // Overload initialization list constructor  
	{
		cout << "using initialization list constructor" << endl;

		for (auto it = v.begin(); it!=v.end(); it++)
		{
			push_back(*it);
		}

	}

	myvector(myvector<value_type>&& v)  // move-constructor
		:_start(v._start)
		, _end(v._end)
		, _endstroage(v._endstroage)
	{
		cout << "now, using move-constructor" << endl;
		
		v._start = nullptr;
		v._end = nullptr;
		v._endstroage = nullptr;
	}

	myvector(iterator first, iterator last)  // Sequence initialization construction
	{
		iterator inter_it = first;
		first = min(first, last);
		last = max(inter_it, last);

		size_t v_sz = last - first + 1;
		_start = new value_type[v_sz];
		iterator it = first;
		iterator element = _start;
		while (it <= last) 
		{
			*element = *it;
			element++;
			it++;
		}
		_end = element;
		_endstroage = _start + v_sz - 1;
	}

	myvector(size_t n, const value_type& value = value_type()) // Initial constructor with default values
	{  
		_start = new value_type[n];
		for (int i = 0; i < n; i++) 
		{
			*(_start + i) = value;
		}
		_end = _start + n;
		_endstroage = _start + n - 1;
	}

	~myvector()  // Destructor function
	{ 
		if (_start) 
		{
			delete[] _start;
			_start = _end = _endstroage = nullptr;
		}
	}

	// Obtaining member variables
	inline iterator begin() 
	{
		return _start;
	}
	inline iterator end() 
	{
		return _end;
	}
	inline const_iterator const_begin()const 
	{
		return _start;
	}
	inline const_iterator const_end()const 
	{
		return _end;
	}
	inline size_t size()const 
	{
		//return _end - _start;
		return std::distance(_start, _end);
	}
	inline size_t capacity()const 
	{
		//return _endstroage - _start + 1;
		return std::distance(_start, _endstroage + 1);
	}

	// crud
	void push_back(const value_type& value) 
	{
		if (_end == _endstroage) 
		{
			reserve();
		}
		*end() = value;
		++_end;
	}

	void pop_back()
	{
		assert(_start != nullptr);
		iterator ret = rel_erase(_end - 1);
	}

	iterator erase(iterator pos1, iterator pos2 = pos1)  // Delete two iterators and their intermediate elements
	{
		return rel_erase(pos1, pos2);
	}

	iterator erase(iterator pos1, size_t n)  // Delete n elements starting from pos1
	{
		return rel_erase(pos1, pos1 + n - 1);
	}

	void erase(size_t, size_t) = delete;  // Prevent the first parameter from being converted to iterator from 0, C++history issue, where 0 and NULL are equivalent

	iterator insert(iterator pos1, const value_type& val) 
	{
		assert(_start && pos1 && _start <= pos1 && pos1 < _end);

		/*
		*	It is worth noting that if the capacity is insufficient during insertion, it needs to be expanded
		*	After expansion, the original iterator will become a wild pointer
		*	So we need to record the subscripts
		*/
		size_t pos = pos1 - begin();
		if (end() == _endstroage)
		{
			reserve();
		}

		pos1 = begin() + pos;
		iterator end = _end - 1;

		while (pos1 < end) 
		{
			*(end + 1) = *end;
			--end;
		}
		*pos1 = *end;
		*(pos1 + 1) = val;
		++_end;

		return pos1;
	}

	iterator emplace(iterator pos1, const value_type& val)
	{
		return insert(pos1 - 1, val);
	}

	iterator find(const value_type& val)  // Found output iterator, unable to find output    _end
	{
		iterator it = begin();
		while (it != end())
		{
			if (*it == val) return it;
			++it;
		}

		return end();
	}

	inline bool IsValidIndex(size_t index) const // Check if the index exists
	{
		return size() > index;
	}

	inline bool empty() const 
	{
		return !size();
	}

	void clear() 
	{
		if (end() == begin()) return;

		while (--end() != begin())
		{
			*end() = value_type();
		}
	}

	void shrink_to_fit()   // Clean memory
	{
		const myvector<value_type>& temp(*this);
		swap(temp);
	}

	void swap(myvector<value_type> v) noexcept // Replace vector
	{  
		std::swap(_start, v._start);
		std::swap(_end, v._end);
		std::swap(_endstroage, v._endstroage);
	} 
	
	void reverse(iterator first, iterator last)  // Reverse elements between two iterators
	{
		if (size() <= 1) return;

		myvector<value_type> temp(first, last);
		iterator vic_it = first;
		iterator vio_it = temp.end() - 1;

		while(vic_it <= last && vio_it >= temp.begin())
		{
			*vic_it = *vio_it;
			++vic_it;
			--vio_it;
		}

	}


	/*
	*  override
	*/

	bool operator == (const myvector<value_type>& v)
	{
		return v._start == _start;
	}
	
	myvector<value_type>& operator = (const myvector<value_type>& v) 
	{ 
		//To overload "=" here, it is necessary to first determine whether it is a self assignment, 
		// and then perform copy self swapping, so as to release excess memory in parallel.

		if (*this == v)
		{
			return *this;
		}

		myvector temp(v);  
		swap(temp);
		return *this;
	}

	myvector<value_type>& operator = (myvector<value_type>&& v) noexcept
	{
		// move-assignment

		if (*this == v)
		{
			return *this;
		}

		_start = v._start;
		_end = v._end;
		_endstroage = v._endstroage;

		v._start = nullptr;
		v._end = nullptr;
		v._endstroage = nullptr;

		return *this;
	}

	value_type& operator[] (size_t pos) 
	{ 
		assert(pos <= size());
		return begin()[pos];
	}

	const value_type& operator[] (size_t pos) const
	{
		assert(pos <= size());
		return begin()[pos];
	}

	friend ostream& operator << (ostream& out, myvector& A) 
	{
		iterator it = A.begin();
		while (it < A.end()) 
		{
			out << *it << " ";
			++it;
		}
		//out << endl;
		return out;
	}


private:
	// Expansion
	void reserve() 
	{ 
		size_t sz = size();
		size_t capacity_new = capacity() == 0 ? 5 : capacity();
		switch (retype) 
		{
			case reserve_type::CONSTAND:
				capacity_new += 20;
				break;
			case reserve_type::NORMAL:
				capacity_new = capacity_new + capacity_new / 2;
				break;
			case reserve_type::HIGH:
				capacity_new *= 2;
				break;
		}

		value_type* temp = new value_type[capacity_new];
		if (begin() != nullptr)
		{
			memcpy(temp, begin(), sizeof(value_type) * sz);
			delete[] begin();
		}
		_start = temp;
		_end = temp + sz;
		_endstroage = begin() + capacity_new;
	}
	
	// Delete specified interval
	iterator rel_erase(iterator pos1, iterator pos2) 
	{
		iterator p1 = min(pos1, pos2);
		iterator p2 = max(pos1, pos2);

		assert(begin() <= p1 && p2 < end());
		iterator it = p2 + 1;
		while (it != end())
		{
			*(it - (p2 - p1) - 1) = *it;
			++it;
		}
		_end -= (p2 - p1) + 1;
		
		return p1;
	}
	iterator rel_erase(const iterator pos1) {
		return rel_erase(pos1, pos1);
	}
};

#include "Vector.tpp"
