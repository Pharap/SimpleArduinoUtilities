#pragma once

/*
   Copyright (C) 2018 Pharap (@Pharap)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdint.h>

//
// Declarations
//

template< typename Type, uint8_t Capacity >
class Deque;

template< typename Type, uint8_t CapacityValue >
class Deque
{
public:

	//
	// Constraints
	//

	static_assert(CapacityValue > 0, "Attempt to create Deque with a capacity less than 1");
	static_assert(CapacityValue < 128, "Attempt to create Deque with a capacity greater than 127");

	//
	// Type Aliases
	//

	using ItemType = Type;
	using SizeType = uint8_t;
	using IndexType = uint8_t;
	using IndexOfType = int8_t;
	
	//
	// Constants
	//
	
	constexpr static const SizeType Capacity = CapacityValue;
	constexpr static const IndexOfType InvalidIndex = -1;
	constexpr static const IndexType FirstIndex = 0;
	constexpr static const IndexType LastIndex = Capacity - 1;

private:
	
	//
	// Member Variables
	//
	
	ItemType items[Capacity] = {};
	IndexType next = 0;
	
public:

	//
	// Common Member Functions
	//
	
	// O(1)
	bool isEmpty(void) const noexcept
	{
		return (this->next == FirstIndex);
	}
	
	// O(1)
	bool isFull(void) const noexcept
	{
		return (this->next > LastIndex);
	}
	
	// O(1)
	SizeType getCount(void) const noexcept
	{
		return this->next;
	}
	
	// O(1)
	constexpr SizeType getCapacity(void) const noexcept
	{
		return Capacity;
	}
	
	// O(1)
	ItemType * getData(void) noexcept
	{
		return &this->items[FirstIndex];
	}
	
	// O(1)
	const ItemType * getData(void) const noexcept
	{
		return &this->items[FirstIndex];
	}
	
	// O(1)
	ItemType & operator [](const IndexType & index)
	{
		return this->items[index];
	}
	
	// O(1)
	const ItemType & operator [](const IndexType & index) const
	{
		return this->items[index];
	}
	
	// O(N)
	void clear(void);
	
	// O(N)
	bool contains(const ItemType & item) const;
	
	// O(N)
	IndexOfType indexOf(const ItemType & item) const;
	
public:

	//
	// Specific Member Functions
	//

	// O(1)
	ItemType & getFirst(void)
	{
		return this->items[FirstIndex];
	}

	// O(1)
	const ItemType & getFirst(void) const
	{
		return this->items[FirstIndex];
	}

	// O(1)
	ItemType & getLast(void)
	{
		return this->items[this->next];
	}

	// O(1)
	const ItemType & getLast(void) const
	{
		return this->items[this->next];
	}

	// O(1)
	bool append(const ItemType & item);

	// O(1)
	bool prepend(const ItemType & item);

	// O(1)
	void unappend(void);

	// O(1)
	void unprepend(void);
	
	// O(N)
	bool removeFirst(const ItemType & item);
	
	// O(N)
	bool removeLast(const ItemType & item);
	
	// O(N)
	bool removeAt(const IndexType & index);

	// O(N)
	bool insert(const IndexType & index, const ItemType & item);
};

//
// Definition
//

// O(N)
template< typename Type, uint8_t Capacity >
void Deque<Type, Capacity>::clear(void)
{
	for (IndexType i = 0; i < this->next; ++i)
		this->items[i].~ItemType();
	this->next = 0;
}

// O(N)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::contains(const ItemType & item) const
{
	for (IndexType i = 0; i < this->next; ++i)
		if (this->items[i] == item)
			return true;
	return false;
}

// O(N)
template< typename Type, uint8_t Capacity >
auto Deque<Type, Capacity>::indexOf(const ItemType & item) const -> IndexOfType
{
	for (IndexType i = 0; i < this->next; ++i)
		if (this->items[i] == item)
			return i;
	return InvalidIndex;
}

// O(1)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::append(const ItemType & item)
{
	if (this->isFull())
		return false;

	this->items[this->next] = item;
	++this->next;
	return true;
}

// O(1)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::prepend(const ItemType & item)
{
	if (this->isFull())
		return false;

	++this->next;
	for (IndexType i = this->next; i > 0; --i)
			this->items[i] = this->items[i - 1]; // should be std::move
	this->items[0] = item;
	return true;
}

// O(1)
template< typename Type, uint8_t Capacity >
void Deque<Type, Capacity>::unappend(void)
{
	if (this->isEmpty())
		return;

	--this->next;
	this->items[this->next].~ItemType();
	return true;
}

// O(1)
template< typename Type, uint8_t Capacity >
void Deque<Type, Capacity>::unprepend(void)
{
	if (this->isEmpty())
		return;

	--this->next;
	for (IndexType i = 0; i < this->next; ++i)
			this->items[i] = this->items[i + 1]; // should be std::move
	this->items[this->next].~ItemType();
	return true;
}

// O(N)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::removeFirst(const ItemType & item)
{
	for(IndexType i = 0; i < this->next; ++i)
		if (this->items[i] == item)
		{
			--this->next;
			while (i < this->next)
			{
				this->items[i] = this->items[i + 1]; // should be std::move
				++i;
			}
			this->items[this->next].~ItemType();
			return true;
		}
	return false;
}

// O(N)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::removeLast(const ItemType & item)
{
	for(IndexType i = this->next; i > 0; --i)
		if (this->items[i] == item)
		{
			--this->next;
			while (i < this->next)
			{
				this->items[i] = this->items[i + 1]; // should be std::move
				++i;
			}
			this->items[this->next].~ItemType();
			return true;
		}
	return false;
}

// O(N)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::removeAt(const IndexType & index)
{
	if(index >= this->next)
		return false;

	--this->next;
	for (IndexType i = index; i < this->next; ++i)
			this->items[i] = this->items[i + 1]; // should be std::move
	this->items[this->next].~ItemType();
	return true;
}

// O(N)
template< typename Type, uint8_t Capacity >
bool Deque<Type, Capacity>::insert(const IndexType & index, const ItemType & item)
{
	if(index >= this->next)
		return false;

	if (this->isFull())
		return false;

	++this->next;
	for (IndexType i = index + 1; i < this->next; ++i)
		this->items[i] = this->items[i - 1]; // should be std::move
	this->items[index] = item;
	return true;
}

//
// Empty Deque
//

template< typename Type >
class Deque<Type, 0>
{
public:

	//
	// Type Aliases
	//

	using ItemType = Type;
	using SizeType = uint8_t;
	using IndexType = uint8_t;
	using IndexOfType = int8_t;
	
	//
	// Constants
	//
	
	constexpr static const SizeType Capacity = 0;
	constexpr static const IndexOfType InvalidIndex = -1;
	constexpr static const IndexType FirstIndex = InvalidIndex;
	constexpr static const IndexType LastIndex = InvalidIndex;
	
public:

	//
	// Common Member Functions
	//
	
	// O(1)
	constexpr bool isEmpty(void) const
	{
		return true;
	}
	
	// O(1)
	constexpr bool isFull(void) const noexcept
	{
		return true;
	}
	
	// O(1)
	constexpr SizeType getCount(void) const noexcept
	{
		return 0;
	}
	
	// O(1)
	constexpr SizeType getCapacity(void) const noexcept
	{
		return 0;
	}
	
	// O(1)
	/*constexpr*/ ItemType * getData(void) noexcept
	{
		return nullptr;
	}
	
	// O(1)
	constexpr const ItemType * getData(void) const noexcept
	{
		return nullptr;
	}
	
	// O(1)
	/*constexpr*/ ItemType & operator [](const IndexType & index) = delete;
	
	// O(1)
	constexpr const ItemType & operator [](const IndexType & index) const = delete;
	
	// O(1)
	constexpr void clear(void) noexcept
	{
	}
	
	// O(1)
	constexpr bool contains(const ItemType & item) const noexcept
	{
		return false;
	}
	
	// O(1)
	constexpr IndexOfType indexOf(const ItemType & item) const noexcept
	{
		return InvalidIndex;
	}
	
public:

	//
	// Specific Member Functions
	//

	// O(1)
	/*constexpr*/ ItemType & getFirst(void) = delete;

	// O(1)
	constexpr const ItemType & getFirst(void) const = delete;

	// O(1)
	/*constexpr*/ ItemType & getLast(void) = delete;
	
	// O(1)
	constexpr const ItemType & getLast(void) const = delete;

	// O(1)
	constexpr bool append(const ItemType & item)
	{
		return false;
	}

	// O(1)
	constexpr bool prepend(const ItemType & item)
	{
		return false;
	}

	// O(1)
	constexpr void unappend(void)
	{
	}

	// O(1)
	constexpr void unprepend(void)
	{
	}
	
	// O(N)
	constexpr bool removeFirst(const ItemType & item)
	{
		return false;
	}
	
	// O(N)
	constexpr bool removeLast(const ItemType & item)
	{
		return false;
	}
	
	// O(N)
	constexpr bool removeAt(const IndexType & index)
	{
		return false;
	}

	// O(N)
	constexpr bool insert(const IndexType & index, const ItemType & item)
	{
		return false;
	}
};