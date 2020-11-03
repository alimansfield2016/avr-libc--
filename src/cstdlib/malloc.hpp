#pragma once

#include <cstdlib>
#include <compare>

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Malloc Header

The "malloc.hpp" header includes:
- freelist struct
- 

*/

class freelist
{
	public:
	class freelist_node
	{
		public:
		std::size_t m_size;
		freelist_node *next;
		freelist_node *prev;

		freelist_node *split(std::size_t size) 
		{
			if(size < (sizeof(this) - sizeof(this->m_size))) return nullptr;
			if(this->m_size - size < sizeof(this)) return nullptr;

			freelist_node *other = this + sizeof(this->m_size) + size;
			other->m_size = this->m_size - sizeof(other->m_size) - size;

			this->m_size = size;
			other->next = this->next;
			other->prev = this;
			this->next = other;


			return other;
		}

		bool join(const freelist_node *other)
		{
			// check that the two nodes are adjacent
			// actually that other is just after this

			// if( other + other->size + sizeof(other->size) == this) return other->join(this);
			if( this + this->m_size + sizeof(this->m_size) != other) return false;

			this->next = other->next;
			this->next->prev = this;
			this->m_size += other->m_size + sizeof(other->m_size);
			
			return true;
		}
	};

	freelist_node *head;
	freelist_node *wilderness;

	explicit freelist(void *ptr) : head(static_cast<freelist_node*>(ptr)), 
									wilderness(static_cast<freelist_node*>(ptr)) {}

	class iterator
	{
		freelist_node *node;
		public:
		constexpr iterator(freelist_node *ptr = nullptr) : node(ptr) {}
		constexpr iterator(const iterator&) = default;

		operator freelist_node*() const { return node; }

		iterator &operator=(const iterator& copy) { node = copy.node; return *this; }

		freelist_node *operator->() const { return node; }
		freelist_node &operator*() const { return *node; }
		operator bool() const {return node; }

		iterator &operator++() {node = node->next; return *this; }
		[[nodiscard]] iterator operator++(int) {iterator tmp(node); node = node->next; return tmp; }
		iterator &operator--() {node = node->prev; return *this; }
		[[nodiscard]] iterator operator--(int) {iterator tmp(node); node = node->prev; return tmp; }

		std::strong_ordering operator<=>(const iterator &other) const = default;
	};

	void remove(const iterator &elem) 
	{ 
		if(elem == iterator(head)) head = elem->next;
		if(elem == iterator(wilderness)) wilderness = elem->prev;
		elem->prev->next = elem->next; 
		elem->next->prev = elem->prev;
	}
	void insert(const iterator &elem)
	{
		auto it = begin();
		//find the point at which to insert the new node

		//check if we could join to the tail element. 
		//If we can, then we know we deallocated at the end of the heap and can retract it
		if(&*elem + elem->m_size + sizeof(elem->m_size) == wilderness){
			it = wilderness;
			wilderness = elem;

		}
		if( std::is_lt(elem <=> (iterator(head))) ){
			// elem->join(head); //attempt to join to the head element
			it = head;
			head = elem;
		}
		for(; it; ++it){
			if(std::is_gt(it <=> elem)) break;
		}
		if(!it) return;
		//the element we are now on is the node after where we want ours to go
		//so we insert between this and the previous

		elem->next = it;
		//check not null while we're at it
		if((elem->prev = it->prev)){
			elem->prev->next = elem;
			elem->prev->join(elem);
		}
		it->prev = elem;
		elem->join(it);

		

		return;

	}
	iterator begin() const { return head; }

	[[nodiscard]] void* alloc(std::size_t size)
	{
		iterator best(wilderness);
		for(iterator it = begin(); it; ++it){
			if(it->m_size == size){
				remove(it);
				return &(*it) + sizeof(freelist_node::m_size);
			}
			if(it->m_size >= best->m_size) continue;
			if(it->m_size < (size + sizeof(freelist_node)) ) continue;

			best = it;

		}

		//check that the stack pointer and heap do not overlap

		best->split(size);
		remove(best);

		return &(*best) + sizeof(freelist_node::m_size);
	}

	void dealloc(void *ptr)
	{
		freelist_node *block = reinterpret_cast<freelist_node*>(reinterpret_cast<char*>(ptr)-sizeof(freelist_node::m_size));
		insert(block);
	}


};